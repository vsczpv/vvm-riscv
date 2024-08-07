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
#include "rv32i-string.h"

typedef struct rv32i_debugger_tui_s
{
	WINDOW* win;
	int max_width;
	int max_height;

	WINDOW* register_dump_win;
	WINDOW* disassemble_win;
	WINDOW* memory_dump_win;

	WINDOW* stdout_win;
	WINDOW* stdout_inner_win;
	rv32i_counted_string_s stdout_buf;
} rv32i_debugger_tui_s;

rv32i_debugger_tui_s rv32i_init_ncurses    (void);
void rv32i_destroy_ncurses(rv32i_debugger_tui_s* tui);

void rv32i_debugger_tui_update_screen_dimensions(rv32i_debugger_tui_s* tui);

#define RV32I_TUI_MINWIDTH  93
#define RV32I_TUI_MINHEIGHT 43

#define RV32I_TUI_COMMAND_NEXT 'n'
#define RV32I_TUI_COMMAND_QUIT 'q'

#define RV32I_REGISTERDISPLAY_WIDTH  54
#define RV32I_REGISTERDISPLAY_HEIGHT 10
#define RV32I_DISASSEMBLY_WIDTH      54
#define RV32I_DISASSEMBLY_HEIGHT     19
#define RV32I_MEMORYDUMP_WIDTH       40
#define RV32I_MEMORYDUMP_HEIGHT      26

#define RV32I_STDOUT_WIDTH  54
#define RV32I_STDOUT_HEIGHT 14

#define RV32I_IOBUF_WIDTH   (RV32I_STDOUT_WIDTH  - 2)
#define RV32I_IOBUF_HEIGHT  (RV32I_STDOUT_HEIGHT - 2)

#endif // RV32I_TUI_H_
