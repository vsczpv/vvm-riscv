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

#ifndef RV32I_ERROR_H_
#define RV32I_ERROR_H_

#include "rv32i-emu.h"

#define rv32i_error_inval(inst_name, subinst, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Invalid instruction %s%x: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, subinst, inst, pc \
)

#define rv32i_error_inval2(inst_name, subinst7, subinst, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Invalid instruction %s%x:%x: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, subinst7, subinst, inst, pc \
)

#define rv32i_error_inval_nosubtype(inst_name, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Invalid instruction %s: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, inst, pc \
)

#define rv32i_error_unsupported_inst_fmt(inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Unsupported instruction format. Instruction \033[1m0x%08x\033[0m, program counter \033[1m0x%08x\033[0m.\n",\
	__FILE__, __LINE__, __func__, inst, pc \
)

#define rv32i_error_inst_noimpl(inst_name, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Opcode %snot implemented: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, inst, pc \
)

#define rv32i_error_mis_jump(inst_name, immd, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Misaligned %sjump: \033[1m0x%08x\033[0m. Instruction: \033[1m0x%08x\033[0m, program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, immd, inst, pc \
)

#define rv32i_error_oob(op_name, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Out of bounds %s: \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, op_name, pc \
)

#define rv32i_error_nofile(filename) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"No such file '%s'.\n", \
	__FILE__, __LINE__, __func__, filename \
)

#define rv32i_error_aintfile(filename) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"'%s' is not a file.\n", \
	__FILE__, __LINE__, __func__, filename \
)

/* """"Malformed"""" instructions means I have no idea what bug causes them falling through everything else */
#define rv32i_error_malinst(inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Malformed instruction: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst, pc \
)

#define rv32i_error_oom(need, have) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Not enough memory to run program: Need %lu bytes, have %u.\n", \
	__FILE__, __LINE__, __func__, need, have \
)

#define rv32i_too_many_maps()  fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Too many memory maps, cannot exceed %i.\n", \
	__FILE__, __LINE__, __func__, IOMAP_HARDCAP \
)


#define rv32i_overlapping_iomaps(of)  fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Overlapping IOMAPs.\n" \
	"Offender 1: 0x%08x -> 0x%08lx\n" \
	"Offender 2: 0x%08x -> 0x%08lx\n", \
	__FILE__, __LINE__, __func__, \
	of.a.addr, of.a.addr + of.a.size - 1, \
	of.b.addr, of.b.addr + of.b.size - 1 \
)

#define rv32i_nonaligned_iomap(addr)  fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Non-aligned IOMAP: 0x%08x\n", \
	__FILE__, __LINE__, __func__, \
	addr \
)

#define rv32i_nomap_atexec() fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"No allocated memory region at execution vector. Maybe you forgot --load-at?\n", \
	__FILE__, __LINE__, __func__ \
)

#endif // RV32I_ERROR_H_
