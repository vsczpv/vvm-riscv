/*
 * Copyright © 2022, 2023 Vinícius Schütz Piva
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

#include <curses.h>

#include "rv32i-emu.h"
#include "rv32i-tui.h"

rv32i_debugger_tui_s rv32i_init_ncurses(void)
{

	rv32i_debugger_tui_s tui = { 0 };

	if (debug) tui.win = initscr();

	curs_set(false);

	/* TODO: error handling */

	start_color();
	use_default_colors();

	short colors[] = { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_CYAN };

	for (unsigned int i = 0; i < sizeof(colors)/sizeof(short); i++) init_pair(colors[i], colors[i], -1);

	return tui;
}

void rv32i_destroy_ncurses(rv32i_debugger_tui_s tui)
{

	(void) tui;

	curs_set(true);

	if (debug) endwin();

	return;
}

