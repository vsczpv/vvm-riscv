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

#ifndef RV32I_ERROR_H_
#define RV32I_ERROR_H_

#include "rv32i-emu.h"
#include "rv32i-tui.h"
#include "rv32i-vt-colors.h"

void rv32i_error_fatal_fprintf(FILE* stream, const char* restrict fmt, ...);

#define RV32I_TEMPLATE_ERROR_TRIPLET VT_BOLD VT_FORE "%s:%i: " VT_RED "error: " VT_END "In function " VT_BOLD "'%s'" VT_END ": "

#define rv32i_error_inval(inst_name, subinst, inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Invalid instruction %s%x: " VT_BOLD "0x%08x" VT_END ". Program counter " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, inst_name, subinst, inst, pc \
)

#define rv32i_error_inval2(inst_name, subinst7, subinst, inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Invalid instruction %s%x:%x: " VT_BOLD "0x%08x" VT_END ". Program counter " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, inst_name, subinst7, subinst, inst, pc \
)

#define rv32i_error_inval_nosubtype(inst_name, inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Invalid instruction %s: " VT_BOLD "0x%08x" VT_END ". Program counter " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, inst_name, inst, pc \
)

#define rv32i_error_unsupported_inst_fmt(inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Unsupported instruction format. Instruction " VT_BOLD "0x%08x" VT_END ", program counter " VT_BOLD "0x%08x" VT_END ".\n",\
	__FILE__, __LINE__, __func__, inst, pc \
)

#define rv32i_error_inst_noimpl(inst_name, inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Opcode %snot implemented: " VT_BOLD "0x%08x" VT_END ". Program counter " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, inst_name, inst, pc \
)

#define rv32i_error_mis_jump(inst_name, immd, inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Misaligned %sjump: " VT_BOLD "0x%08x" VT_END ". Instruction: " VT_BOLD "0x%08x" VT_END ", program counter " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, inst_name, immd, inst, pc \
)

#define rv32i_error_oob(op_name, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Out of bounds %s: " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, op_name, pc \
)

#define rv32i_error_nofile(filename) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"No such file '%s'.\n", \
	__FILE__, __LINE__, __func__, filename \
)

#define rv32i_error_aintfile(filename) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"'%s' is not a file.\n", \
	__FILE__, __LINE__, __func__, filename \
)

/* """"Malformed"""" instructions means I have no idea what bug causes them falling through everything else */
#define rv32i_error_malinst(inst, pc) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Malformed instruction: " VT_BOLD "0x%08x" VT_END ". Program counter " VT_BOLD "0x%08x" VT_END ".\n", \
	__FILE__, __LINE__, __func__, inst, pc \
)

#define rv32i_error_oom(need, have) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Not enough memory to run program: Need %lu bytes, have %u.\n", \
	__FILE__, __LINE__, __func__, need, have \
)

#define rv32i_too_many_maps()  rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Too many memory maps, cannot exceed %i.\n", \
	__FILE__, __LINE__, __func__, IOMAP_HARDCAP \
)


#define rv32i_overlapping_iomaps(of)  rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Overlapping IOMAPs.\n" \
	"Offender 1: 0x%08x -> 0x%08lx\n" \
	"Offender 2: 0x%08x -> 0x%08lx\n", \
	__FILE__, __LINE__, __func__, \
	of.a.addr, of.a.addr + of.a.size - 1, \
	of.b.addr, of.b.addr + of.b.size - 1 \
)

#define rv32i_nonaligned_iomap(addr)  rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Non-aligned IOMAP: 0x%08x\n", \
	__FILE__, __LINE__, __func__, \
	addr \
)

#define rv32i_nomap_atexec() rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"No allocated memory region at execution vector. Maybe you forgot --load-at?\n", \
	__FILE__, __LINE__, __func__ \
)

#define rv32i_error_nocolor() rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Terminal must have color support.\n", \
	__FILE__, __LINE__, __func__ \
)

#define rv32i_error_toosmallwindow(tui) rv32i_error_fatal_fprintf \
( \
	stderr, \
	RV32I_TEMPLATE_ERROR_TRIPLET  \
	"Terminal must be atleast %ix%i in size, current is %ix%i.\n", \
	__FILE__, __LINE__, __func__, RV32I_TUI_MINWIDTH, RV32I_TUI_MINHEIGHT, tui.max_width, tui.max_height \
)

#endif // RV32I_ERROR_H_
