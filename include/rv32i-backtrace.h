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

#ifndef RV32I_BACKTRACE_H_
#define RV32I_BACKTRACE_H_

#include <stdbool.h>

#include "rv32i-emu.h"
#include "rv32i-inst.h"

typedef struct rv32i_mnemonic_s
{
	char* nofunct_mnemonic;
	char funct3[8][8];
	char funct7[8][8];
	char type;
} rv32i_mnemonic_s;

typedef struct rv32i_mnemonic_string_s
{
	char* inst;
	char* arg1;
	char* arg2;
	char* arg3;
	char immd[22];
} rv32i_mnemonic_string_s;

rv32i_mnemonic_string_s rv32i_backtrace_getmnemonic(uint32_t inst);
void rv32i_backtrace(rv32i_hart_s* cpu);

extern rv32i_mnemonic_s rv32i_backtrace_mnemonictable[RV32I_OPCODE_CUSTOM3+1];
extern char* rv32i_backtrace_registers[32];

extern char* rv32i_mnemonic_null;

#define RV32I_STACKDISPLAY_WIDTH 36

#endif // RV32I_BACKTRACE_H_
