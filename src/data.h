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

#ifndef TIMENCY_DATA_H
#define TIMENCY_DATA_H

#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <stdio.h>

struct TyDataRow {
    struct tm begin_tm;
    struct tm end_tm;
    int total_time_min;
    char project_name[255];
    char task_name[255];
};

struct TyData {
    char firm_name[255];
    double rate_per_hour;
    int row_count;
    struct TyDataRow rows[];
};

long ty_get_filesize(FILE* fp);

/**
 * Load timency data file and populate ty_data struct
 * Returns 0 for OK, 1 for error
 *
 * @param filename
 * @param ty_data
 * @return int
 */
int ty_load_from_file(const char* filename, struct TyData* ty_data);
char* ty_get_header_value(char* header_line, const char* key);
char* ty_trim_str(char* str);

#endif //TIMENCY_DATA_H
