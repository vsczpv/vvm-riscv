/*
 * Copyright © 2022 Vinícius Schütz Piva
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

#ifndef R32I_MISC_H_
#define R32I_MISC_H_

#define rv32i_getinst(cpu, index)       \
	((cpu)->ram.buf[index+0] << 0)  |  \
	((cpu)->ram.buf[index+1] << 8)  |  \
	((cpu)->ram.buf[index+2] << 16) |  \
	((cpu)->ram.buf[index+3] << 24)

// TODO: Check later; I changed some stuff with instruction loading and this macro is probably borked
#define rv32i_setinst(cpu, index, val)             \
	(cpu)->ram.buf[index+0] = (val >> 0)  & 0xff; \
	(cpu)->ram.buf[index+1] = (val >> 8)  & 0xff; \
	(cpu)->ram.buf[index+2] = (val >> 16) & 0xff; \
	(cpu)->ram.buf[index+3] = (val >> 24) & 0xff;

#define rv32i_getword(cpu, index)      \
	((cpu)->ram.buf[index+0] << 24) | \
	((cpu)->ram.buf[index+1] << 16) | \
	((cpu)->ram.buf[index+2] << 8)  | \
	((cpu)->ram.buf[index+3] << 0)

#define rv32i_gethalf(cpu, index)     \
	((cpu)->ram.buf[index+0] << 8) | \
	((cpu)->ram.buf[index+1] << 0)

#define rv32i_getbyte(cpu, index) (cpu)->ram.buf[index]

#define rv32i_setword(cpu, index, val)             \
	(cpu)->ram.buf[index+0] = (val >> 24) & 0xff; \
	(cpu)->ram.buf[index+1] = (val >> 16) & 0xff; \
	(cpu)->ram.buf[index+2] = (val >> 8)  & 0xff; \
	(cpu)->ram.buf[index+3] = (val >> 0)  & 0xff;

#define rv32i_sethalf(cpu, index, val)             \
	(cpu)->ram.buf[index+0] = (val >> 8)  & 0xff; \
	(cpu)->ram.buf[index+1] = (val >> 0)  & 0xff;

#define rv32i_setbyte(cpu, index, val) (cpu)->ram.buf[index] = val & 0xff;

#endif /* R32I_EMU_H_ */
