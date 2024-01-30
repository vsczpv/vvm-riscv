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
#include <stdlib.h>

#include "rv32i-emu.h"
#include "rv32i-tui.h"
#include "rv32i-error.h"

#define curses_emerg_leave() endwin(); exit(EXIT_FAILURE)

void rv32i_debbuger_tui_refresh_dimensions(rv32i_debugger_tui_s* tui)
{
	getmaxyx(tui->win, tui->max_height, tui->max_width);
}

rv32i_debugger_tui_s rv32i_init_ncurses(void)
{

	rv32i_debugger_tui_s tui = { 0 };

	if (debug)
	{

		use_env(true);

		tui.win = initscr();

		if (start_color() == ERR) { rv32i_error_nocolor(); curses_emerg_leave(); }

		use_default_colors();

		short colors[] = { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_CYAN };

		for (unsigned int i = 0; i < sizeof(colors)/sizeof(short); i++) init_pair(colors[i], colors[i], -1);

		rv32i_debbuger_tui_refresh_dimensions(&tui);

		if (tui.max_height < RV32I_TUI_MINHEIGHT || tui.max_width < RV32I_TUI_MINWIDTH)
		{
			rv32i_error_toosmallwindow(tui); curses_emerg_leave();
		}

		curs_set(false);
	}
	return tui;
}

void rv32i_destroy_ncurses(rv32i_debugger_tui_s tui)
{

	(void) tui;

	curs_set(true);

	endwin();

	return;
}
