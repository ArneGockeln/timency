/*
 *  Copyright 2021 Arne Gockeln
 *
 *  timency is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  timency is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with timency.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tui.h"
#include <ncurses.h>
#include <string.h>

static WINDOW* win_title;
static WINDOW* win_main;
static int max_cols;
static int max_lines;

void ty_init_ui(void) {
    // NCURSES initialization:
    initscr();             // initialize curses
    cbreak();              // pass key presses to program, but not signals
    noecho();              // don't echo key presses to screen
    keypad(stdscr, TRUE);  // allow arrow keys
    timeout(0);            // no blocking on getch()
    curs_set(0);           // set the cursor to invisible

    max_cols = getmaxx(stdscr);
    max_lines = getmaxy(stdscr);
    win_title = newwin(1, max_cols, 0, 0);
    win_main = newwin(max_lines - 1, max_cols, 1, 0);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    wbkgd(win_title, COLOR_PAIR(1));
    wbkgd(win_main, COLOR_PAIR(2));
}

void ty_deinit_ui(void) {
    // Deinitialize NCurses
    delwin(win_main);
    delwin(win_title);
    wclear(stdscr);
    endwin();
}

void ty_set_titlebar(char* firm, char* begin_date, char* end_date, char* rate, char* hours, char* profit) {
    size_t next_x = 2;

    mvwaddstr( win_title, 0, next_x, "Timency" );
    next_x += ty_title_len( "Timency" );
    if ( next_x > max_cols ) return;

    mvwaddstr( win_title, 0, next_x, firm );
    next_x += ty_title_len( firm );
    if ( next_x > max_cols ) return;

    mvwaddstr( win_title, 0, next_x, begin_date );
    next_x += ty_title_len( begin_date );
    if ( next_x > max_cols ) return;

    mvwaddstr( win_title, 0, next_x, end_date );
    next_x += ty_title_len( end_date );
    if ( next_x > max_cols ) return;

    mvwaddstr( win_title, 0, next_x, rate );
    next_x += ty_title_len( rate );
    if ( next_x > max_cols ) return;

    mvwaddstr( win_title, 0, next_x, hours );
    next_x += ty_title_len( hours );
    if ( next_x > max_cols ) return;

    mvwaddstr( win_title, 0, next_x, profit );
}

size_t ty_title_len(char* str) {
    return strlen( str ) + 2;
}


void ty_refresh(void) {
    refresh();
    wrefresh(win_title);
    wrefresh(win_main);
}