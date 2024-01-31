/*
 * Copyright © 2022-2024 Vinícius Schütz Piva
 *
 * This file is part of vvm-riscv
 *
 * vvm-riscv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 *
 */

#ifndef RV32I_TUI_H_
#define RV32I_TUI_H_

#include <curses.h>

typedef struct rv32i_debugger_tui_s
{
	WINDOW* win;
	int max_width;
	int max_height;
} rv32i_debugger_tui_s;

rv32i_debugger_tui_s rv32i_init_ncurses    (void);
void rv32i_destroy_ncurses(rv32i_debugger_tui_s* tui);

void rv32i_debbuger_tui_refresh_dimensions(rv32i_debugger_tui_s* tui);

#define RV32I_TUI_MINWIDTH  100
#define RV32I_TUI_MINHEIGHT 31

#define RV32I_TUI_COMMAND_NEXT 'n'
#define RV32I_TUI_COMMAND_QUIT 'q'

#endif // RV32I_TUI_H_
