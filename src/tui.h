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

#ifndef TIMENCY_TUI_H
#define TIMENCY_TUI_H

#include <ncurses.h>

void ty_init_ui(void);
void ty_deinit_ui(void);
void ty_set_titlebar(char* firm, char* begin_date, char* end_date, char* rate, char* hours, char* profit);
void ty_refresh(void);
size_t ty_title_len(char* str);

#endif //TIMENCY_TUI_H
