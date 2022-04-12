/* Copyright 2021 Arne Gockeln
 *
 * timency is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * timency is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with timency.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <cjson/cJSON.h>

long ty_get_filesize(FILE* fp) {
    long size = 0;
    fpos_t pos;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

int ty_load_from_file(const char* filename, struct TyData* ty_data) {
    assert(filename != NULL);
    int status = 0;

    printf("The file: %s\n", filename);

    // open file
    FILE* file;
    file = fopen(filename, "rb");
    if ( file == NULL ) {
        perror("Problem reading the file");
        return 1;
    }

    // read file
    long lSize = ty_get_filesize(file);
    char *buffer;

    buffer = malloc(lSize + 1);
    if ( !buffer ) {
        fclose( file );
        fprintf(stderr, "memory alloc fails\n");
        return 1;
    }

    fread( buffer, 1, lSize, file );

    buffer[lSize] = '\0';

    // parse json
    cJSON *json = cJSON_Parse( buffer );
    if ( json == NULL ) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if ( error_ptr != NULL ) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }

        status = 1;
        goto end;
    }

    const cJSON* client = NULL;
    client = cJSON_GetObjectItemCaseSensitive(json, "client");
    if ( cJSON_IsString(client) && client->valuestring != NULL ) {
        printf("Client: %s\n", client->valuestring);
    }

    const cJSON* rate_per_hour = NULL;
    rate_per_hour = cJSON_GetObjectItemCaseSensitive(json, "rate_per_hour");
    if ( ! cJSON_IsNumber(rate_per_hour) ) {
        fprintf(stderr, "Rate per Hour is not double.");
        status = 1;
        goto end;
    }

    printf("Rate/Hour: %f\n", rate_per_hour->valuedouble);

    strcpy( ty_data->firm_name, client->valuestring );
    ty_data->rate_per_hour = rate_per_hour->valuedouble;

    const cJSON* tasks = NULL;
    const cJSON* task = NULL;
    tasks = cJSON_GetObjectItemCaseSensitive(json, "tasks");
    cJSON_ArrayForEach(task, tasks) {
        cJSON *date = cJSON_GetObjectItemCaseSensitive(task, "date");
        cJSON *start = cJSON_GetObjectItemCaseSensitive(task, "start");
        cJSON *end = cJSON_GetObjectItemCaseSensitive(task, "end");
        cJSON *project = cJSON_GetObjectItemCaseSensitive(task, "project");
        cJSON *notes = cJSON_GetObjectItemCaseSensitive(task, "notes");

        if ( ! cJSON_IsString( date ) || ! cJSON_IsString( start ) || 
        ! cJSON_IsString( end ) || ! cJSON_IsString( project ) || ! cJSON_IsString( notes ) ) {
            status = 0;
            goto end;
        }

        struct TyDataRow row;

        // ----------------------------------------------
        // PARSE DATE & TIME
        // ----------------------------------------------
        // clear the result structure
        memset (&row.begin_tm, '\0', sizeof (row.begin_tm));
        // Try ISO Format
        if ( strptime( start->valuestring, "%F %H:%M:%S", &row.begin_tm ) == NULL ) {
            perror("Begin time parsing failed.");
            return 1;
        }

        mktime( &row.begin_tm );

        // End Time
        memset (&row.end_tm, '\0', sizeof (row.end_tm));

        // Try ISO Format
        if ( strptime( end->valuestring, "&F %H:%M:%S", &row.end_tm ) == NULL ) {
            perror("End end time parsing failed.");
            return 1;
        }

        mktime( &row.end_tm );

        // ----------------------------------------------
        // ASSIGN ROW TO LIST
        // ----------------------------------------------
        struct TyDataRow *tmp = realloc(ty_data->rows, ++ty_data->row_count * sizeof(*tmp));
        if ( tmp ) {
            ty_data->rows[ ty_data->row_count - 1 ] = row;
        }

        printf("Task: %s %s-%s, %s, %s\n", 
            date->valuestring, 
            start->valuestring, 
            end->valuestring, 
            project->valuestring, 
            notes->valuestring);
    }
    
end:
    free( buffer );
    fclose( file );
    cJSON_Delete(json);
    return status;

    // open and read file
    // FILE* file;
    // file = fopen(filename, "r");
    // if ( file == NULL ) {
    //     printf("Couldn't open file %s\n", filename);
    //     return 1;
    // }

//     enum ParserState {
//         kParserStateHead,
//         kParserStateRows
//     };

//     enum ParserState state = kParserStateHead;

//     setlocale (LC_ALL, "");

//     char line[255];
//     while( fgets( line, 255, file ) != NULL ) {
//         if ( state == kParserStateHead ) {
//             // Parse Header
//             if ( strcmp( line, "---" ) == 0 ) continue;

//             // if line starts with Firm:
//             char* firm = ty_get_header_value(line, "Firm: ");
//             if ( firm != NULL ) {
//                 strcpy( ty_data->firm_name, firm );
//                 continue;
//             }

//             // if line starts with Rate:
//             char* rate = ty_get_header_value(line, "Rate: ");
//             if ( rate != NULL ) {
//                 // convert value to double
//                 ty_data->rate_per_hour = strtod( rate, NULL );
//                 continue;
//             }

//             // skip line starting with csv head
//             char* csvHead = strstr(line, "Date,");
//             if ( csvHead != NULL ) {
//                 state = kParserStateRows;
//                 continue;
//             }
//         }

//         // parse csv line
//         if ( state == kParserStateRows ) {
//             struct TyDataRow row;
//             row.begin_tm.tm_isdst = -1;
//             row.end_tm.tm_isdst = -1;

//             // Date
//             char* ptr = strtok( line, "," );
//             char date_begin[20];
//             char date_end[20];
//             if ( ptr != NULL ) {
//                 // copy date
//                 strcpy(date_begin, ptr);
//                 strcpy(date_end, ptr);

//                 // Begin Time
//                 ptr = strtok(NULL, ",");
//                 strcat(date_begin, " ");
//                 strcat(date_begin, ptr);

//                 // clear the result structure
//                 memset (&row.begin_tm, '\0', sizeof (row.begin_tm));
//                 // Try ISO Format
//                 if ( strptime( date_begin, "%F %H:%M:%S", &row.begin_tm ) == NULL ) {
//                     perror("Begin date parsing failed.");
//                     return 1;
//                 }

//                 mktime( &row.begin_tm );

//                 // End Time
//                 ptr = strtok(NULL, ",");
//                 strcat(date_end, " ");
//                 strcat(date_end, ptr);

//                 memset (&row.end_tm, '\0', sizeof (row.end_tm));

//                 // Try ISO Format
//                 if ( strptime( date_end, "%F %H:%M:%S", &row.end_tm ) == NULL ) {
//                     perror("End date parsing failed.");
//                     return 1;
//                 }

//                 mktime( &row.end_tm );

//                 // Project
//                 ptr = strtok(NULL, ",");
//                 strcpy(row.project_name, ptr);

//                 // Task
//                 ptr = strtok(NULL, ",");
//                 strcpy(row.task_name, ptr);

//                 // Assign row to list
//                 struct TyDataRow *tmp = realloc(ty_data->rows, ++ty_data->row_count * sizeof(*tmp));
//                 if ( tmp ) {
//                     ty_data->rows[ ty_data->row_count - 1 ] = row;
//                 }

// //                printf("%s\n", date_begin);
// //                printf("year: %d; month: %d; day: %d;\n",
// //                       row.begin_tm.tm_year, row.begin_tm.tm_mon, row.begin_tm.tm_mday);
// //                printf("hour: %d; minute: %d; second: %d\n",
// //                       row.begin_tm.tm_hour, row.begin_tm.tm_min, row.begin_tm.tm_sec);
// //                printf("week day: %d; year day: %d\n", row.begin_tm.tm_wday, row.begin_tm.tm_yday);
// //
// //                puts("-----------------------");
// //
// //                printf("%s\n", date_end);
// //                printf("year: %d; month: %d; day: %d;\n",
// //                       row.end_tm.tm_year, row.end_tm.tm_mon, row.end_tm.tm_mday);
// //                printf("hour: %d; minute: %d; second: %d\n",
// //                       row.end_tm.tm_hour, row.end_tm.tm_min, row.end_tm.tm_sec);
// //                printf("week day: %d; year day: %d\n", row.end_tm.tm_wday, row.end_tm.tm_yday);
// //
// //                puts("-----------------------");


//             }
//         }
//     }

    // fclose( file );

    // return 0;
}

char* ty_get_header_value(char* header_line, const char* key) {
    // get ptr to occourence of key in line
    char* linePtr = strstr( header_line, key );
    if ( linePtr != NULL ) {
        // split line by : into token
        char* keyPtr = strtok( header_line, ":" );
        if ( keyPtr != NULL ) {
            // get second token
            keyPtr = strtok(NULL, ":");
            if ( keyPtr != NULL ) {
                // trim spaces
                return ty_trim_str( keyPtr );
            }
        }
    }
    return NULL;
}

char* ty_trim_str(char* str) {
    while( isspace( (unsigned char) *str ) ) str++;

    // all spaces?
    if ( *str == 0 ) {
        return str;
    }

    // trim trailing space
    char* end = str + strlen( str ) - 1;
    while( end > str && isspace( (unsigned char) *end ) ) end--;

    // write new null terminator character
    end[1] = '\0';

    return str;
}