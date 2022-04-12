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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"
#include "tui.h"

int main(int argc, char** argv) {

    if ( argc < 2 ) {
        printf(
                "Usage:\n"
                "\ttimency project.json\n"
            );
        return EXIT_SUCCESS;
    }

    struct TyData ty_data;
    ty_data.rate_per_hour = 0;
    ty_data.row_count = 0;

    assert( ty_load_from_file( argv[1], &ty_data ) == 0 );

    // printf("Firm: %s\nRate: %.2f\n", ty_data.firm_name, ty_data.rate_per_hour);
    // printf("Count: %d\n", ty_data.row_count);

    // return true;
    // char rate_per_hour[6] = "Rate: ";
    // char buffer[10];
    // sprintf(buffer, "%f", ty_data.rate_per_hour);
    // strcat(rate_per_hour, buffer);

    // ty_init_ui();
    // ty_set_titlebar(ty_data.firm_name, "Begin: 2021-12-01", "End: 2021-12-24", rate_per_hour, "Hours: 42", "Profit: 6.300 EUR");
    // //ty_set_tasklist(ty_data.rows);
    // ty_refresh();

    // while( getch() != 'q' ) {

    // }

    // ty_deinit_ui();
    return EXIT_SUCCESS;
}

