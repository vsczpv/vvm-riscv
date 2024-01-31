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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>

#include "rv32i-inst.h"
#include "rv32i-backtrace.h"
#include "rv32i-mem.h"
#include "rv32i-tui-widgets.h"

char* rv32i_mnemonic_registers[32] =
{
	"zero",
	"ra", "sp", "gp",
	"tp", "t0", "t1", "t2",
	"fp", "s1", "a0", "a1",
	"a2", "a3", "a4", "a5",
	"a6", "a7", "s2", "s3",
	"s4", "s5", "s6", "s7",
	"s8", "s9","s10","s11",
	"t3", "t4", "t5", "t6"
};

rv32i_mnemonic_s rv32i_backtrace_mnemonictable[RV32I_OPCODE_CUSTOM3+1] =
{
	[RV32I_OPCODE_LOAD] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_LOAD_BYTE] = "lb    ",
			[RV32I_OPCODE_LOAD_HALF] = "lh    ",
			[RV32I_OPCODE_LOAD_WORD] = "lw    ",
			[RV32I_OPCODE_LOAD_BYTU] = "lbu   ",
			[RV32I_OPCODE_LOAD_HLFU] = "lhu   ",
		},
		.type = RV32I_ITYPE
	},
	[RV32I_OPCODE_MISCMEM] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_MISCMEM_FENCE] = "fence   "
		},
		.type = RV32I_ITYPE
	},
	[RV32I_OPCODE_OPIMM] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_OPIMM_ADDI]      = "addi  ",
			[RV32I_OPCODE_OPIMM_SLLI]      = "slli  ",
			[RV32I_OPCODE_OPIMM_SLTI]      = "slti  ",
			[RV32I_OPCODE_OPIMM_SLTIU]     = "sltiu ",
			[RV32I_OPCODE_OPIMM_XORI]      = "xori  ",
			[RV32I_OPCODE_OPIMM_SRLISRAI]  = "srli  ",
			[RV32I_OPCODE_OPIMM_ORI]       = "ori   ",
			[RV32I_OPCODE_OPIMM_ANDI]      = "andi  ",
		},
		.funct7 =
		{
			[RV32I_OPCODE_OPIMM_ADDI]      = "?",
			[RV32I_OPCODE_OPIMM_SLLI]      = "?",
			[RV32I_OPCODE_OPIMM_SLTI]      = "?",
			[RV32I_OPCODE_OPIMM_SLTIU]     = "?",
			[RV32I_OPCODE_OPIMM_XORI]      = "?",
			[RV32I_OPCODE_OPIMM_SRLISRAI]  = "srai  ",
			[RV32I_OPCODE_OPIMM_ORI]       = "?",
			[RV32I_OPCODE_OPIMM_ANDI]      = "?",
		},
		.type = RV32I_ITYPE
	},
	[RV32I_OPCODE_AUIPC] =
	{
		.nofunct_mnemonic = "auipc ",
		.type = RV32I_UTYPE
	},
	[RV32I_OPCODE_STORE] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_STORE_BYTE] = "sb    ",
			[RV32I_OPCODE_STORE_HALF] = "sh    ",
			[RV32I_OPCODE_STORE_WORD] = "sw    ",
		},
		.type = RV32I_STYPE
	},
	[RV32I_OPCODE_OP] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_OP_ADDSUB] = "add   ",
			[RV32I_OPCODE_OP_SLL]    = "sll   ",
			[RV32I_OPCODE_OP_SLT]    = "slt   ",
			[RV32I_OPCODE_OP_SLTU]   = "sltu  ",
			[RV32I_OPCODE_OP_XOR]    = "xor   ",
			[RV32I_OPCODE_OP_SRLSRA] = "srl   ",
			[RV32I_OPCODE_OP_OR]     = "or    ",
			[RV32I_OPCODE_OP_AND]    = "and   ",
		},
		.funct7 =
		{
			[RV32I_OPCODE_OP_ADDSUB] = "sub   ",
			[RV32I_OPCODE_OP_SLL]    = "?",
			[RV32I_OPCODE_OP_SLT]    = "?",
			[RV32I_OPCODE_OP_SLTU]   = "?",
			[RV32I_OPCODE_OP_XOR]    = "?",
			[RV32I_OPCODE_OP_SRLSRA] = "sra   ",
			[RV32I_OPCODE_OP_OR]     = "?",
			[RV32I_OPCODE_OP_AND]    = "?",
		},
		.type = RV32I_RTYPE
	},
	[RV32I_OPCODE_LUI] =
	{
		.nofunct_mnemonic = "lui   ",
		.type = RV32I_UTYPE
	},
	[RV32I_OPCODE_BRANCH] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_BRANCH_BEQ]  = "beq   ",
			[RV32I_OPCODE_BRANCH_BNE]  = "bne   ",
			[RV32I_OPCODE_BRANCH_BLT]  = "blt   ",
			[RV32I_OPCODE_BRANCH_BGE]  = "bge   ",
			[RV32I_OPCODE_BRANCH_BLTU] = "bltu  ",
			[RV32I_OPCODE_BRANCH_BGEU] = "bgeu  "
		},
		.type = RV32I_BTYPE
	},
	[RV32I_OPCODE_JALR] =
	{
		.nofunct_mnemonic = "jalr  ",
		.type = RV32I_ITYPE
	},
	[RV32I_OPCODE_JAL] =
	{
		.nofunct_mnemonic = "jal   ",
		.type = RV32I_JTYPE
	},
	[RV32I_OPCODE_SYSTEM] =
	{
		.nofunct_mnemonic = NULL,
		.funct3 =
		{
			[RV32I_OPCODE_SYSTEM_PRIV_ECALL]  = "ecall ",
			[RV32I_OPCODE_SYSTEM_PRIV_EBREAK] = "ebreak"
		},
		.type = RV32I_ITYPE
	}
};

rv32i_mnemonic_string_s rv32i_backtrace_getmnemonic(uint32_t inst)
{

	rv32i_mnemonic_string_s res = { "?", "", "", "", "" };

	if (!inst) { res.inst = "0"; return res; }

	uint32_t opcode = rv32i_inst_getopcode(inst);
	rv32i_mnemonic_s* details = &rv32i_backtrace_mnemonictable[opcode];
	rv32i_inst_param param;

	switch (details->type)
	{
		case RV32I_ITYPE:
		{
			param = rv32i_parse_itype(inst);
			bool srai;
			if (opcode == RV32I_OPCODE_JALR)
			{
				srai = false;
				res.inst = details->nofunct_mnemonic;
			} else if (opcode == RV32I_OPCODE_OPIMM) {
				srai = param.funct3 == RV32I_OPCODE_OPIMM_SRLISRAI ? param.immd & 0b10000000000 : false;
				res.inst = srai ? details->funct7[param.funct3] : details->funct3[param.funct3];
			} else if (opcode == RV32I_OPCODE_SYSTEM) {
				if (param.funct3 != RV32I_OPCODE_SYSTEM_PRIV) res.inst = "?";
				else res.inst = details->funct3[param.immd];
			} else {
				res.inst = details->funct3[param.funct3];
			}
			if (opcode != RV32I_OPCODE_MISCMEM && opcode != RV32I_OPCODE_SYSTEM)
			{
				res.arg1 = rv32i_mnemonic_registers[param.rd];
				res.arg2 = rv32i_mnemonic_registers[param.rs1];
				sprintf(res.immd, "%u", srai ? param.immd & 0x1f : param.immd);
			}
			break;
		}
		case RV32I_UTYPE:
		{
			param = rv32i_parse_utype(inst);
			res.inst = details->nofunct_mnemonic;
			res.arg1 = rv32i_mnemonic_registers[param.rd];
			sprintf(res.immd, "%u", param.immd);
			break;
		}
		case RV32I_RTYPE:
		{
			param = rv32i_parse_rtype(inst);
			bool subsra = param.immd & 0b10000000000;
			res.inst = subsra ? details->funct7[param.funct3] : details->funct3[param.funct3];
			res.arg1 = rv32i_mnemonic_registers[param.rd];
			res.arg2 = rv32i_mnemonic_registers[param.rs1];
			res.arg3 = rv32i_mnemonic_registers[param.rs2];
			break;
		}
		case RV32I_JTYPE:
		{
			param = rv32i_parse_jtype(inst);
			res.inst = details->nofunct_mnemonic;
			res.arg1 = rv32i_mnemonic_registers[param.rd];
			sprintf(res.immd, "%u", param.immd);
			break;
		}
		case RV32I_BTYPE:
		{
			param = rv32i_parse_btype(inst);
			res.inst = details->funct3[param.funct3];
			res.arg1 = rv32i_mnemonic_registers[param.rs1];
			res.arg2 = rv32i_mnemonic_registers[param.rs2];
			sprintf(res.immd, "%u", param.immd);
			break;
		}
		case RV32I_STYPE:
		{
			param = rv32i_parse_stype(inst);
			res.inst = details->funct3[param.funct3];
			res.arg1 = rv32i_mnemonic_registers[param.rs1];
			res.arg2 = rv32i_mnemonic_registers[param.rs2];
			sprintf(res.immd, "%u", param.immd);
			break;
		}
		default:
			puts("Fatal internal error at backtrace parser.");
			exit(EXIT_FAILURE);
	}

	if (res.inst[0] == '\0') res.inst = "?";

	return res;
}

void rv32i_backtrace(rv32i_hart_s* cpu)
{

	rv32i_debugger_tui_clear(cpu);

	/* Draw main window surroudings */
	rv32i_debugger_tui_draw_cheatsheet(cpu);
	rv32i_debugger_tui_draw_titlebar  (cpu);

	/* Prevent screen overflow */
	if (cpu->tui.max_height < RV32I_TUI_MINHEIGHT || cpu->tui.max_width < RV32I_TUI_MINWIDTH)
	{
		const char msg[]  = "Screen is too small.";
		const int  msglen = (sizeof(msg) - 1) / 2;
		mvprintw(cpu->tui.max_height / 2, cpu->tui.max_width / 2 - msglen, msg);
		return;
	}

	/* Draw widgets */
	rv32i_debugger_tui_draw_registers(cpu);
	rv32i_debugger_tui_draw_disassembly(cpu);
	rv32i_debugger_tui_draw_memorydump(cpu);

	rv32i_debugger_tui_refresh(cpu);

	return;
}
