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

#ifndef R32I_MACRO_H_
#define R32I_MACRO_H_

#define rv32i_macro_rtype(opcode, rd, funct3, rs1, rs2, funct7) \
(	(opcode & 0b1111111)     | ((rd & 0b11111) << 7)   | \
	((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | \
	((rs2 & 0b11111) << 20)  | ((funct7 & 0b1111111) << 25) )

#define rv32i_macro_itype(opcode, rd, funct3, rs1, immd) \
(	(opcode & 0b1111111)     | ((rd & 0b11111) << 7)   | \
	((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | \
	((immd & 0b111111111111) << 20) )

#define rv32i_macro_stype(opcode, funct3, rs1, rs2, immd) \
(	(opcode & 0b1111111)     | ((immd & 0b11111) << 7) | \
	((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | \
	((rs2 & 0b11111) << 20)  | (((immd >> 5) & 0b1111111) << 25) )

#define rv32i_macro_utype(opcode, rd, immd) \
(	(opcode & 0b1111111)     | ((rd & 0b11111) << 7) | \
	(immd << 12) )

#define rv32i_macro_btype(opcode, funct3, rs1, rs2, immd) \
(	(opcode & 0b1111111)             | (((immd >> 10) & 0b1) << 7) | \
	((immd & 0b1111) << 8)           | ((funct3 & 0b111) << 12)    | \
	((rs1 & 0b11111) << 15)          | ((rs2 & 0b11111) << 20)     | \
	(((immd >> 4) & 0b111111) << 25) | (((immd >> 11) & 0b1) << 31) )

#define rv32i_macro_jtype(opcode, rd, immd) \
(	(opcode & 0b1111111)                  | ((rd & 0b11111) << 7)        | \
	(((immd >> 11) & 0b11111111) << 12)   | (((immd >> 10) & 0b1) << 20) | \
	((immd & 0b1111111111) << 21)         | (((immd >> 19) & 0b1) << 31) )

#endif /* R32I_MACRO_H_ */
