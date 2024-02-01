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

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "rv32i-emu.h"
#include "rv32i-tui.h"
#include "rv32i-error.h"
#include "rv32i-mem.h"
#include "rv32i-backtrace.h"

#define curses_emerg_leave() endwin(); exit(EXIT_FAILURE)

void rv32i_debugger_tui_update_screen_dimensions(rv32i_debugger_tui_s* tui)
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

		rv32i_debugger_tui_update_screen_dimensions(&tui);

		if (tui.max_height < RV32I_TUI_MINHEIGHT || tui.max_width < RV32I_TUI_MINWIDTH)
		{
			rv32i_error_toosmallwindow(tui); curses_emerg_leave();
		}

		curs_set(false);
	}

	tui.register_dump_win = newwin(RV32I_REGISTERDISPLAY_HEIGHT, RV32I_REGISTERDISPLAY_WIDTH, 1, 0);
	tui.disassemble_win   = newwin(RV32I_DISASSEMBLY_HEIGHT,     RV32I_DISASSEMBLY_WIDTH,    10, 0);
	tui.memory_dump_win   = newwin(RV32I_MEMORYDUMP_HEIGHT,      RV32I_MEMORYDUMP_WIDTH,      1,53);

	tui.stdout_win       = newwin(RV32I_STDOUT_HEIGHT, RV32I_STDOUT_WIDTH, 9 + RV32I_DISASSEMBLY_HEIGHT, 0);
	tui.stdout_inner_win = newwin(RV32I_IOBUF_HEIGHT,  RV32I_IOBUF_WIDTH,  10+ RV32I_DISASSEMBLY_HEIGHT, 1);

	(void) idlok   (tui.stdout_inner_win, true);
	(void) scrollok(tui.stdout_inner_win, true);

	tui.stdout_buf = rv32i_cnstring_init();

	return tui;
}

void rv32i_destroy_ncurses(rv32i_debugger_tui_s* tui)
{

	if (tui)
	{
		rv32i_cnstring_delete(&tui->stdout_buf);
	}

	curs_set(true);
	endwin();

	return;
}

void rv32i_debugger_tui_clear(rv32i_hart_s* cpu)
{

	clear();
	wclear(cpu->tui.register_dump_win);
	wclear(cpu->tui.disassemble_win);
	wclear(cpu->tui.memory_dump_win);
	wclear(cpu->tui.stdout_win);
	wclear(cpu->tui.stdout_inner_win);

	return;
}

void rv32i_debugger_tui_refresh(rv32i_hart_s* cpu)
{

	refresh();
	wrefresh(cpu->tui.register_dump_win);
	wrefresh(cpu->tui.disassemble_win);
	wrefresh(cpu->tui.memory_dump_win);
	wrefresh(cpu->tui.stdout_win);
	wrefresh(cpu->tui.stdout_inner_win);

	return;
}

void rv32i_debugger_tui_draw_cheatsheet(rv32i_hart_s* cpu)
{

	/* This is all printed at the bottom of the main screen */

	attron (A_REVERSE | A_BOLD);
	mvprintw(cpu->tui.max_height - 1, 0, " %c", RV32I_TUI_COMMAND_NEXT);
	attroff(A_REVERSE | A_BOLD);
	printw(" Exec. Next Inst. ");

	attron (A_REVERSE | A_BOLD);
	printw(" %c", RV32I_TUI_COMMAND_QUIT);
	attroff(A_REVERSE | A_BOLD);
	printw(" Quit VVM ");

	return;
}

void rv32i_debugger_tui_draw_titlebar(rv32i_hart_s* cpu)
{

	/* This is drawn at the top of the main window */
	int msglen = strlen(cpu->running_program_filename) / 2;
	mvprintw(0, cpu->tui.max_width / 2 - msglen, "%.*s", cpu->tui.max_width, cpu->running_program_filename);
	mvwchgat(cpu->tui.win, 0, 0, cpu->tui.max_width, A_BOLD | A_REVERSE, 0, NULL);

	return;
}

void rv32i_debugger_tui_draw_registers(rv32i_hart_s* cpu)
{

	WINDOW* rw = cpu->tui.register_dump_win;

	mvwprintw
	(
		rw,
		1, 0,
		"  ra=%08x  sp=%08x  gp=%08x  tp=%08x\n"
		"  a0=%08x  a1=%08x  a2=%08x  a3=%08x\n"
		"  a4=%08x  a5=%08x  a6=%08x  a7=%08x\n"
		"  fp=%08x  s1=%08x  s2=%08x  s3=%08x\n"
		"  s4=%08x  s6=%08x  s6=%08x  s7=%08x\n"
		"  s8=%08x  s9=%08x s10=%08x s11=%08x\n"
		"  t0=%08x  t1=%08x  t2=%08x  t3=%08x\n"
		"  t4=%08x  t5=%08x  t6=%08x  pc=%08x",
		cpu->regs[1],  cpu->regs[2],  cpu->regs[3],  cpu->regs[4],
		cpu->regs[10], cpu->regs[11],
		cpu->regs[12], cpu->regs[13], cpu->regs[14], cpu->regs[15],
		cpu->regs[16], cpu->regs[17], cpu->regs[8],  cpu->regs[9],  cpu->regs[18], cpu->regs[19],
		cpu->regs[20], cpu->regs[21], cpu->regs[22], cpu->regs[23],
		cpu->regs[24], cpu->regs[25], cpu->regs[26], cpu->regs[27],
		cpu->regs[5],  cpu->regs[6],  cpu->regs[7],  cpu->regs[28],
		cpu->regs[29], cpu->regs[30], cpu->regs[31],
		cpu->pc
	);

	/* Paint the registers with fancy colors */
	mvwchgat(rw, 7, 2,  -1, A_BOLD, COLOR_CYAN,   NULL);
	mvwchgat(rw, 8, 2,  -1, A_BOLD, COLOR_CYAN,   NULL);

	mvwchgat(rw, 1, 2 , -1, A_BOLD, COLOR_YELLOW, NULL);
	mvwchgat(rw, 8, 40, -1, A_BOLD, COLOR_YELLOW, NULL);

	mvwchgat(rw, 4, 2,  -1, A_BOLD, COLOR_RED,    NULL);

	mvwchgat(rw, 4, 14, -1, A_BOLD, COLOR_BLUE,   NULL);
	mvwchgat(rw, 5, 2,  -1, A_BOLD, COLOR_BLUE,   NULL);
	mvwchgat(rw, 6, 2,  -1, A_BOLD, COLOR_BLUE,   NULL);

	wborder(rw, 0,0,0,0, 0,0,0,0);

	mvwprintw(rw, 0, 2, "Registers:");

	return;
}

void rv32i_debugger_tui_draw_disassembly(rv32i_hart_s* cpu)
{

	WINDOW* dw = cpu->tui.disassemble_win;

	wborder(dw, 0,0,0,0, ACS_LTEE,ACS_RTEE,0,0);

	mvwprintw(dw, 0, 2, "Disassembly:");

	int j = 0; for (unsigned int i = cpu->pc-32; i != cpu->pc+68; i+=4)
	{

		/* Skip nonexistent addresses */
		if ( rv32i_oob_addr(cpu, i) ) continue;

		/* Highlight cpu->pc */
		bool cur = i == cpu->pc ? true : false;

		/* Get the decoded instruction */
		rv32i_mnemonic_string_s ins = rv32i_backtrace_getmnemonic(rv32i_getinst(cpu, i));

		/* Handle invalid and null instructions */
		bool unknown = ins.inst[0] == '?';
		bool null    = ins.inst[0] == '0';
		char* noarg  = "\0";

		     if (unknown) ins.inst = "Unknown", ins.arg1 = noarg, ins.arg2 = noarg, ins.arg3 = noarg;
		else if (null)    ins.inst = "Null",    ins.arg1 = noarg, ins.arg2 = noarg, ins.arg3 = noarg;

		/* Print it */
		mvwprintw(dw, j + 1, 2, "0x%08x:", i);

		mvwprintw
		(
			dw,
			j + 1, 22,
			"%s %s%s%s%s%s",
			ins.inst,
			ins.arg1, ins.arg2[0] ? ", " : "",
			ins.arg2, ins.arg3[0] || ins.immd[0] ? ", " : "",
			ins.immd[0] ? ins.immd : ins.arg3
		);

		/* "" cpu->pc */
		mvwprintw(dw, 1 + j++, 50, "%s", cur ? "<-" : "");

		const int chgat_len = RV32I_DISASSEMBLY_WIDTH - 3;

		/* Colors */
		if (null)    mvwchgat(dw, j, 2, chgat_len, A_DIM,  COLOR_WHITE,  NULL);
		if (unknown) mvwchgat(dw, j, 2, chgat_len, A_BOLD, COLOR_YELLOW, NULL);
		if (cur)     mvwchgat(dw, j, 2, chgat_len, A_BOLD, COLOR_GREEN,  NULL);

	}

	return;
}

void rv32i_debugger_tui_draw_memorydump(rv32i_hart_s* cpu)
{

	WINDOW* mw = cpu->tui.memory_dump_win;

	int j = 0, k = 0;

	/* Buffer for the ASCII dump */
	char datastring[9] = { [8] = '\0' };

	for (unsigned int i = cpu->regs[8]-96; i != (unsigned) cpu->regs[8]+96; i++)
	{

		/* Check if the memory address currently being displayed actually exists */
		if ( rv32i_oob_addr(cpu, i) )
		{
			mvwprintw(mw, 1 + j, 2 + (k*3), ".. ");
			mvwchgat (mw, 1 + j, 2 + (k*3), 3, A_DIM, COLOR_WHITE, NULL);
			datastring[k] = '.';
			goto skip;
		};

		uint32_t value = rv32i_getbyte(cpu, i);

		datastring[k] = (char) value;

		/* Only display well behaved ASCII */
		if (datastring[k] < 0x20 || datastring[k] >= 0x7f) datastring[k] = '.';

		mvwprintw(mw, 1 + j, 2 + (k*3), "%02x", value);

		/* Handle coloring for byte */
		switch (value)
		{
			case 0x00:
			{
				mvwchgat(mw, 1 + j, 2 + (k*3), 3, A_DIM, COLOR_WHITE, NULL);
				break;
			}
			default: break;
		}

		skip:

		k++;

		/* After the eight byte, display the ASCII dump from the last octate */
		if (k == 8)
		{
			mvwprintw(mw, 1 + j, 2 + (k*3), "  |%s|", datastring);
			k = 0, j++;
		}

	}

	wborder(mw, 0,0,0,0, ACS_TTEE,0,ACS_LTEE,0);
	mvwprintw(mw, 0, 2, "Frame: At %08x", cpu->regs[8]);

	mvwaddch(mw, 9, 0, ACS_RTEE);

	return;
}

void rv32i_debugger_tui_draw_stdout(rv32i_hart_s* cpu)
{

	WINDOW* sw = cpu->tui.stdout_win;
	WINDOW* iw = cpu->tui.stdout_inner_win;

	wborder(sw, 0,0,0,0, ACS_LTEE,ACS_RTEE,0,0);

	mvwprintw(sw, 0, 2, "Program IO:");

	mvwprintw(iw, 0, 0, "%.*s", (int) cpu->tui.stdout_buf.count, cpu->tui.stdout_buf.string);

	return;
}
