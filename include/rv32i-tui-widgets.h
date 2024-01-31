/*
 * Copyright © 2024 Vinícius Schütz Piva
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

#ifndef RV32I_TUI_WIDGETS_H_
#define RV32I_TUI_WIDGETS_H_

#include "rv32i-emu.h"
#include "rv32i-tui.h"

void rv32i_debugger_tui_clear  (rv32i_hart_s* cpu);
void rv32i_debugger_tui_refresh(rv32i_hart_s* cpu);

void rv32i_debugger_tui_draw_cheatsheet(rv32i_hart_s* cpu);
void rv32i_debugger_tui_draw_titlebar  (rv32i_hart_s* cpu);

void rv32i_debugger_tui_draw_registers  (rv32i_hart_s* cpu);
void rv32i_debugger_tui_draw_disassembly(rv32i_hart_s* cpu);
void rv32i_debugger_tui_draw_memorydump (rv32i_hart_s* cpu);

#endif // RV32I_TUI_WIDGETS_H_
