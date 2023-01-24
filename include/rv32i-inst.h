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

#ifndef RV32I_INST_H_
#define RV32I_INST_H_

#include <stdint.h>
#include <stdbool.h>

#include "rv32i-emu.h"

typedef struct rv32i_inst_param
{
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	uint8_t funct3;
	uint8_t funct7;
	uint32_t immd;
} rv32i_inst_param;

bool rv32i_invalassert(uint32_t inst, uint32_t pc);

rv32i_inst_param rv32i_parse_rtype(uint32_t inst);
rv32i_inst_param rv32i_parse_itype(uint32_t inst);
rv32i_inst_param rv32i_parse_stype(uint32_t inst);
rv32i_inst_param rv32i_parse_utype(uint32_t inst);
rv32i_inst_param rv32i_parse_btype(uint32_t inst);
rv32i_inst_param rv32i_parse_jtype(uint32_t inst);

#define RV32I_OPCODE_LOAD      0b0000011
#define RV32I_OPCODE_LOADFP    0b0000111
#define RV32I_OPCODE_CUSTOM0   0b0001011
#define RV32I_OPCODE_MISCMEM   0b0001111
#define RV32I_OPCODE_OPIMM     0b0010011
#define RV32I_OPCODE_AUIPC     0b0010111
#define RV32I_OPCODE_OPIMM32   0b0011011
#define RV32I_OPCODE_STORE     0b0100011
#define RV32I_OPCODE_STOREFP   0b0100111
#define RV32I_OPCODE_CUSTOM1   0b0101011
#define RV32I_OPCODE_AMO       0b0101111
#define RV32I_OPCODE_OP        0b0110011
#define RV32I_OPCODE_LUI       0b0110111
#define RV32I_OPCODE_OP32      0b0111011
#define RV32I_OPCODE_MADD      0b1000011
#define RV32I_OPCODE_MSUB      0b1000111
#define RV32I_OPCODE_NMSUB     0b1001011
#define RV32I_OPCODE_NMADD     0b1001111
#define RV32I_OPCODE_OPFP      0b1010011
#define RV32I_OPCODE_RESERVED0 0b1010111
#define RV32I_OPCODE_CUSTOM2   0b1011011
#define RV32I_OPCODE_BRANCH    0b1100011
#define RV32I_OPCODE_JALR      0b1100111
#define RV32I_OPCODE_RESERVED1 0b1101011
#define RV32I_OPCODE_JAL       0b1101111
#define RV32I_OPCODE_SYSTEM    0b1110011
#define RV32I_OPCODE_RESERVED2 0b1110111
#define RV32I_OPCODE_CUSTOM3   0b1111011

#define rv32i_inst_getopcode(inst) ((inst) & 0b1111111)

extern bool (*rv32i_inst_instructions[RV32I_OPCODE_CUSTOM3+1])(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_LOAD_BYTE 0b000
#define RV32I_OPCODE_LOAD_HALF 0b001
#define RV32I_OPCODE_LOAD_WORD 0b010
#define RV32I_OPCODE_LOAD_BYTU 0b100
#define RV32I_OPCODE_LOAD_HLFU 0b101

bool rv32i_inst_load(int inst, rv32i_hart_s* cpu);

bool rv32i_inst_loadpf(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_custom0(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_MISCMEM_FENCE 0b000

bool rv32i_inst_miscmem(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_OPIMM_ADDI      0b000
#define RV32I_OPCODE_OPIMM_SLLI      0b001
#define RV32I_OPCODE_OPIMM_SLTI      0b010
#define RV32I_OPCODE_OPIMM_SLTIU     0b011
#define RV32I_OPCODE_OPIMM_XORI      0b100
#define RV32I_OPCODE_OPIMM_SRLISRAI  0b101
#define RV32I_OPCODE_OPIMM_ORI       0b110
#define RV32I_OPCODE_OPIMM_ANDI      0b111

bool rv32i_inst_opimm(int inst, rv32i_hart_s* cpu);

bool rv32i_inst_auipc(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_opimm32(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_STORE_BYTE 0b000
#define RV32I_OPCODE_STORE_HALF 0b001
#define RV32I_OPCODE_STORE_WORD 0b010

bool rv32i_inst_store(int inst, rv32i_hart_s* cpu);

bool rv32i_inst_storefp(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_custom1(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_amo(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_OP_ADDSUB 0b000
#define RV32I_OPCODE_OP_SLL    0b001
#define RV32I_OPCODE_OP_SLT    0b010
#define RV32I_OPCODE_OP_SLTU   0b011
#define RV32I_OPCODE_OP_XOR    0b100
#define RV32I_OPCODE_OP_SRLSRA 0b101
#define RV32I_OPCODE_OP_OR     0b110
#define RV32I_OPCODE_OP_AND    0b111

bool rv32i_inst_op(int inst, rv32i_hart_s* cpu);

bool rv32i_inst_lui(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_op32(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_madd(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_msub(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_nmsub(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_nmadd(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_opfp(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_reserved0(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_custom2(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_BRANCH_BEQ  0b000
#define RV32I_OPCODE_BRANCH_BNE  0b001
#define RV32I_OPCODE_BRANCH_BLT  0b100
#define RV32I_OPCODE_BRANCH_BGE  0b101
#define RV32I_OPCODE_BRANCH_BLTU 0b110
#define RV32I_OPCODE_BRANCH_BGEU 0b111

bool rv32i_inst_branch(int inst, rv32i_hart_s* cpu);

bool rv32i_inst_jalr(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_reserved1(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_jal(int inst, rv32i_hart_s* cpu);

#define RV32I_OPCODE_SYSTEM_PRIV 0b000

#define RV32I_OPCODE_SYSTEM_PRIV_ECALL  0b000000000000
#define RV32I_OPCODE_SYSTEM_PRIV_EBREAK 0b000000000001

bool rv32i_inst_system(int inst, rv32i_hart_s* cpu);

bool rv32i_inst_reserved2(int inst, rv32i_hart_s* cpu);
bool rv32i_inst_custom3(int inst, rv32i_hart_s* cpu);

#define RV32I_RTYPE 0
#define RV32I_ITYPE 1
#define RV32I_STYPE 2
#define RV32I_UTYPE 3
#define RV32I_BTYPE 4
#define RV32I_JTYPE 5

#endif /* RV32I_INST_H_ */
