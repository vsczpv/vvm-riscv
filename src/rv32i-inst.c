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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "rv32i-emu.h"
#include "rv32i-inst.h"
#include "rv32i-misc.h"
#include "rv32i-error.h"
#include "rv32i-backtrace.h"

bool rv32i_invalassert(uint32_t inst, uint32_t pc)
{

	if (!(inst & 0xffff) || !(inst ^ 0xffffffff))
	{
		rv32i_error_inval_nosubtype("", inst, pc);

		return true;
	}

	if ((inst & 0b11) != 0b11 && (inst & 0b11111) != 0b11111)
	{

		rv32i_error_unsupported_inst_fmt(inst, pc);

		return true;
	}

	return false;
}

rv32i_inst_param rv32i_parse_rtype(uint32_t inst)
{

	rv32i_inst_param param = { 0 };

	param.rd     = (inst >> 7)  & 0b11111;
	param.funct3 = (inst >> 12) & 0b111;
	param.rs1    = (inst >> 15) & 0b11111;
	param.rs2    = (inst >> 20) & 0b11111;
	param.funct7 = (inst >> 25) & 0b1111111;

	return param;
}

rv32i_inst_param rv32i_parse_itype(uint32_t inst)
{

	rv32i_inst_param param = { 0 };

	param.rd     = (inst >> 7)  & 0b11111;
	param.funct3 = (inst >> 12) & 0b111;
	param.rs1    = (inst >> 15) & 0b11111;
	param.immd   = (inst >> 20) & 0b111111111111;

	if (param.immd & 0b100000000000) param.immd = param.immd | 0xfffff000;

	return param;
}

rv32i_inst_param rv32i_parse_stype(uint32_t inst)
{

	rv32i_inst_param param = { 0 };

	param.immd   = (inst >> 7)  & 0b11111;
	param.funct3 = (inst >> 12) & 0b111;
	param.rs1    = (inst >> 15) & 0b11111;
	param.rs2    = (inst >> 20) & 0b11111;
	param.immd  |=((inst >> 25) & 0b1111111) << 5;

	if (param.immd & 0b100000000000) param.immd = param.immd | 0xfffff000;

	return param;
}

rv32i_inst_param rv32i_parse_utype(uint32_t inst)
{

	rv32i_inst_param param = { 0 };

	param.rd   = (inst >> 7) & 0b11111;
	param.immd = inst & (0xffffffff << 12);

	return param;
}

rv32i_inst_param rv32i_parse_btype(uint32_t inst)
{

	rv32i_inst_param param = { 0 };

	param.immd   =((inst >> 7) & 0b1) << 11;
	param.immd  |=((inst >> 8) & 0b1111) << 1;
	param.funct3 = (inst >> 12) & 0b111;
	param.rs1    = (inst >> 15) & 0b11111;
	param.rs2    = (inst >> 20) & 0b11111;
	param.immd  |=((inst >> 25) & 0b111111) << 5;
	param.immd  |=((inst >> 31) & 0b1) << 12;

	if (param.immd & 0b1000000000000) param.immd = param.immd | 0xffffe000;

	return param;
}

rv32i_inst_param rv32i_parse_jtype(uint32_t inst)
{

	rv32i_inst_param param = { 0 };

	param.rd    = (inst >> 7)  & 0b11111;
	param.immd  =((inst >> 12) & 0b11111111) << 12;
	param.immd |=((inst >> 20) & 0b1) << 11;
	param.immd |=((inst >> 21) & 0b1111111111) << 1;
	param.immd |=((inst >> 31) & 0b1) << 20;

	if (param.immd & 0b100000000000000000000) param.immd = param.immd | 0xffe00000;

	return param;
}

bool rv32i_inst_load(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_itype(inst);

	cpu->pc += 4;
	if (!param.rd) return false;

	uint32_t addr = cpu->regs[param.rs1] + (signed) param.immd;
	if (rv32i_oob_addr(cpu, addr)) { rv32i_error_oob("LOAD", addr); return true; }

	switch (param.funct3)
	{
		case RV32I_OPCODE_LOAD_BYTE:
		{
			uint32_t val = rv32i_getbyte(cpu, addr);
			if (val & 0x80) val |= 0xffffff80;
			cpu->regs[param.rd] = val;
			break;
		}
		case RV32I_OPCODE_LOAD_HALF:
		{
			uint32_t val = rv32i_gethalf(cpu, addr);
			if (val & 0x8000) val |= 0xffff8000;
			cpu->regs[param.rd] = val;
			break;
		}
		case RV32I_OPCODE_LOAD_WORD:
		{
			cpu->regs[param.rd] = rv32i_getword(cpu, addr);
			break;
		}
		case RV32I_OPCODE_LOAD_BYTU:
		{
			cpu->regs[param.rd] = rv32i_getbyte(cpu, addr);
			break;
		}
		case RV32I_OPCODE_LOAD_HLFU:
		{
			cpu->regs[param.rd] = rv32i_gethalf(cpu, addr);
			break;
		}
		default:
		{

			rv32i_error_inval("LOAD ", param.funct3, inst, cpu->pc - 4);
			rv32i_backtrace(cpu);
			return true;
		}
	}

	return false;
}

bool rv32i_inst_loadfp(int inst, rv32i_hart_s* cpu)
{

	rv32i_error_inst_noimpl("LOAD-FP ", inst, cpu->pc);
	rv32i_backtrace(cpu);

	return true;
}

bool rv32i_inst_custom0(int inst, rv32i_hart_s* cpu)
{

	rv32i_error_inst_noimpl("CUSTOM0 ", inst, cpu->pc);
	rv32i_backtrace(cpu);

	return true;
}

bool rv32i_inst_miscmem(int inst, rv32i_hart_s* cpu)
{

	/* Dummy operation so compiler shuts up about unused vars */
	rv32i_inst_param dummy = rv32i_parse_itype(inst); dummy.immd += 1;

	/* FENCE and friends are no-op */
	cpu->pc += 4;

//	rv32i_error_inst_noimpl("MISC-MEM ", inst, cpu->pc - 4);
//	rv32i_backtrace(cpu);

	return false;
}

bool rv32i_inst_opimm(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_itype(inst);

	cpu->pc += 4;
	if (param.rd == 0) return false;

	switch (param.funct3)
	{
		case RV32I_OPCODE_OPIMM_ADDI:
		{
			cpu->regs[param.rd] = cpu->regs[param.rs1] + param.immd;
			break;
		}
		case RV32I_OPCODE_OPIMM_SLTI:
		{
			cpu->regs[param.rd] = (signed) cpu->regs[param.rs1] < (signed) param.immd ? 1 : 0;
			break;
		}
		case RV32I_OPCODE_OPIMM_SLTIU:
		{
			cpu->regs[param.rd] = (unsigned) cpu->regs[param.rs1] < (unsigned) param.immd ? 1 : 0;
			break;
		}
		case RV32I_OPCODE_OPIMM_ANDI:
		{
			cpu->regs[param.rd] = cpu->regs[param.rs1] & param.immd;
			break;
		}
		case RV32I_OPCODE_OPIMM_ORI:
		{
			cpu->regs[param.rd] = cpu->regs[param.rs1] | param.immd;
			break;
		}
		case RV32I_OPCODE_OPIMM_XORI:
		{
			cpu->regs[param.rd] = cpu->regs[param.rs1] ^ param.immd;
			break;
		}
		case RV32I_OPCODE_OPIMM_SLLI:
		{
			int immd115 = param.immd & 0b111111100000;
			if (immd115)
			{
				rv32i_error_inval("SSLI ", immd115, inst, cpu->pc - 4);
				rv32i_backtrace(cpu);
				return true;
			}
			cpu->regs[param.rd] = cpu->regs[param.rs1] << (param.immd & 0b11111);
			break;
		}
		case RV32I_OPCODE_OPIMM_SRLISRAI:
		{
			int immd115 = param.immd & 0b111111100000;
			if (immd115 == 0b010000000000)
			{
				cpu->regs[param.rd] = (signed) cpu->regs[param.rs1] >> (signed) (param.immd & 0b11111);
			} else if (immd115) {
				rv32i_error_inval("SRLI ", immd115, inst, cpu->pc - 4);
				rv32i_backtrace(cpu);
				return true;
			} else {
				cpu->regs[param.rd] = cpu->regs[param.rs1] >> (param.immd & 0b11111);
			}
			break;
		}
		default:
		{
			rv32i_error_inval("OP-IMM ", param.funct3, inst, cpu->pc - 4);
			rv32i_backtrace(cpu);
			return true;
		}
	}
	return false;
}

bool rv32i_inst_auipc(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_utype(inst);

	cpu->pc += 4;
	if (param.rd == 0) return false;

	cpu->regs[param.rd] = cpu->pc + param.immd - 4;

	return false;
}

bool rv32i_inst_opimm32(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("OP-IMM-32 ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_store(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_stype(inst);

	cpu->pc += 4;

	uint32_t addr = cpu->regs[param.rs1] + (signed) param.immd;
	if (rv32i_oob_addr(cpu, addr)) { rv32i_error_oob("STORE", addr); return true; }

	switch (param.funct3)
	{
		case RV32I_OPCODE_STORE_BYTE:
		{
			rv32i_setbyte(cpu, addr, cpu->regs[param.rs2]);
			break;
		}
		case RV32I_OPCODE_STORE_HALF:
		{
			rv32i_sethalf(cpu, addr, cpu->regs[param.rs2]);
			break;
		}
		case RV32I_OPCODE_STORE_WORD:
		{
			rv32i_setword(cpu, addr, cpu->regs[param.rs2]);
			break;
		}
		default:
		{
			rv32i_error_inval("STORE ", param.funct3, inst, cpu->pc - 4);
			rv32i_backtrace(cpu);
			return true;
		}
	}


	return false;
}

bool rv32i_inst_storefp(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("STORE-FP ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_custom1(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("CUSTOM1 ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_amo(int inst, rv32i_hart_s* cpu)
{

	rv32i_error_inst_noimpl("AMO ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_op(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_rtype(inst);

	cpu->pc += 4;
	if (param.rd == 0) return false;

	if (param.funct7 == 0b0100000)
	{
		switch (param.funct3)
		{
			case RV32I_OPCODE_OP_ADDSUB:
			{
				cpu->regs[param.rd] = cpu->regs[param.rs1] - cpu->regs[param.rs2];
				break;
			}
			case RV32I_OPCODE_OP_SRLSRA:
			{
				cpu->regs[param.rd] = (signed) cpu->regs[param.rs1] >> (signed) (cpu->regs[param.rs2] & 0b11111);
				break;
			}
			default:
			{
				rv32i_error_inval("OP 40", param.funct3, inst, cpu->pc - 4);
				rv32i_backtrace(cpu);
				return true;
			}
		}

	} else if (!param.funct7) {
		switch (param.funct3)
		{
			case RV32I_OPCODE_OP_ADDSUB:
			{
				cpu->regs[param.rd] = cpu->regs[param.rs1] + cpu->regs[param.rs2];
				break;
			}
			case RV32I_OPCODE_OP_SLL:
			{
				cpu->regs[param.rd] = cpu->regs[param.rs1] << (cpu->regs[param.rs2] & 0b11111);
				break;
			}
			case RV32I_OPCODE_OP_SLT:
			{
				cpu->regs[param.rd] = (signed) cpu->regs[param.rs1] < (signed) cpu->regs[param.rs2] ? 1 : 0;
				break;
			}
			case RV32I_OPCODE_OP_SLTU:
			{
				cpu->regs[param.rd] = (unsigned) cpu->regs[param.rs1] < (unsigned) cpu->regs[param.rs2] ? 1 : 0;
				break;
			}
			case RV32I_OPCODE_OP_XOR:
			{
				cpu->regs[param.rd] = cpu->regs[param.rs1] ^ cpu->regs[param.rs2];
				break;
			}
			case RV32I_OPCODE_OP_SRLSRA:
			{
				cpu->regs[param.rd] = (unsigned) cpu->regs[param.rs1] >> (unsigned) (cpu->regs[param.rs2] & 0b11111);
				break;
			}
			case RV32I_OPCODE_OP_OR:
			{
				cpu->regs[param.rd] = cpu->regs[param.rs1] | cpu->regs[param.rs2];
				break;
			}
			case RV32I_OPCODE_OP_AND:
			{
				cpu->regs[param.rd] = cpu->regs[param.rs1] & cpu->regs[param.rs2];
				break;
			}
		}
	} else {
		rv32i_error_inval2("OP ", param.funct7, param.funct3, inst, cpu->pc - 4);
		rv32i_backtrace(cpu);
		return true;
	}

	return false;
}

bool rv32i_inst_lui(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_utype(inst);

	cpu->pc += 4;
	if (param.rd == 0) return false;

	cpu->regs[param.rd] = param.immd;

	return false;
}

bool rv32i_inst_op32(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("OP-32 ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_madd(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("MADD ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_msub(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("MSUB ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_nmsub(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("NMSUB ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_nmadd(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("NMADD ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_opfp(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("OP-FP ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_reserved0(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inval_nosubtype("RESERVED0", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_custom2(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inst_noimpl("CUSTOM2 ", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_branch(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_btype(inst);

	if (param.immd & 0b11)
	{
		rv32i_error_mis_jump("BRANCH ", param.immd, inst, cpu->pc - 4);
		rv32i_backtrace(cpu);
		return true;
	}

	switch (param.funct3)
	{
		case RV32I_OPCODE_BRANCH_BEQ:
		{
			if (cpu->regs[param.rs1] == cpu->regs[param.rs2]) cpu->pc += (signed) param.immd;
			else cpu->pc += 4;
			break;
		}
		case RV32I_OPCODE_BRANCH_BNE:
		{
			if (cpu->regs[param.rs1] != cpu->regs[param.rs2]) cpu->pc += (signed) param.immd;
			else cpu->pc += 4;
			break;
		}
		case RV32I_OPCODE_BRANCH_BLT:
		{
			if (cpu->regs[param.rs1] < cpu->regs[param.rs2]) cpu->pc += (signed) param.immd;
			else cpu->pc += 4;
			break;
		}
		case RV32I_OPCODE_BRANCH_BGE:
		{
			if (cpu->regs[param.rs1] >= cpu->regs[param.rs2]) cpu->pc += (signed) param.immd;
			else cpu->pc += 4;
			break;
		}
		case RV32I_OPCODE_BRANCH_BLTU:
		{
			if ((unsigned) cpu->regs[param.rs1] < (unsigned) cpu->regs[param.rs2]) cpu->pc += (signed) param.immd;
			else cpu->pc += 4;
			break;
		}
		case RV32I_OPCODE_BRANCH_BGEU:
		{
			if ((unsigned) cpu->regs[param.rs1] >= (unsigned) cpu->regs[param.rs2]) cpu->pc += (signed) param.immd;
			else cpu->pc += 4;
			break;
		}
		default:
		{
			rv32i_error_inval("BRANCH ", param.funct3, inst, cpu->pc - 4);
			rv32i_backtrace(cpu);
			return true;
		}
	}

	return false;
}

bool rv32i_inst_jalr(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_itype(inst);

	if (param.funct3)
	{
		rv32i_error_inval("JARL ", param.funct3, inst, cpu->pc);
		rv32i_backtrace(cpu);
		return true;
	}

	if (param.rd) cpu->regs[param.rd] = cpu->pc + 4;

	int immd = (cpu->regs[param.rs1] + param.immd) & 0xfffffffe;

	if (immd & 0b11)
	{
		rv32i_error_mis_jump("JARL ", immd, inst, cpu->pc);
		rv32i_backtrace(cpu);
		return true;
	}

	cpu->pc = (signed) immd;

	return false;
}

bool rv32i_inst_reserved1(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inval_nosubtype("RESERVED1", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_jal(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_jtype(inst);

	if (param.immd & 0b11)
	{
		rv32i_error_mis_jump("JAL ", param.immd, inst, cpu->pc);
		rv32i_backtrace(cpu);
		return true;
	}

	if (param.rd) cpu->regs[param.rd] = cpu->pc + 4;

	cpu->pc += (signed) param.immd;

	return false;
}

/*
 * This is a stub system instruction
 * Doesn't follow the spec
 */
bool rv32i_inst_system(int inst, rv32i_hart_s* cpu)
{

	rv32i_inst_param param = rv32i_parse_itype(inst);

	cpu->pc += 4;

	if (param.funct3 != RV32I_OPCODE_SYSTEM_PRIV || param.rd || param.rs1)
	{
		rv32i_error_inval("SYSTEM ", param.funct3, inst, cpu->pc - 4);
		rv32i_backtrace(cpu);
		return true;
	}

	switch (param.immd)
	{
		case RV32I_OPCODE_SYSTEM_PRIV_ECALL:
		{
			switch (cpu->regs[10])
			{
				case 0:
					puts("\033[32;01mProgram finished gracefully.\033[0m");
					return true;
				case 1:
				{
					if (!noopstub)
					{
						char* buf = (char*) &cpu->ram.buf[cpu->regs[11]];
						size_t size = cpu->regs[12];

						if ((size + cpu->regs[11]) > cpu->ram.size)
						{
							rv32i_error_oob("read", cpu->pc - 4);
							rv32i_backtrace(cpu);
							return true;
						}

						if (debug) system("clear");
						fwrite(buf, (signed) size, 1, stdout);
						if (debug) getchar();
					}
					break;
				}
				case 2:
				{

					if (!noopstub)
					{
						char* input = NULL;

						if (debug) system("clear"), printf("\033[1;33mInput required:\033[0m\n");
						scanf("%ms", &input);

						if (!input) { perror("Fatal error on ECALL Read() scanf"); return true; }

						int count = strlen(input);

						if (count < cpu->regs[12]) cpu->regs[10] = count;
						else cpu->regs[10] = cpu->regs[12];


						if ((unsigned) ( cpu->regs[11] + cpu->regs[10]) > cpu->ram.size)
						{
							rv32i_error_oob("write", cpu->pc - 4);
							rv32i_backtrace(cpu);
							return true;
						}

						memcpy(&cpu->ram.buf[cpu->regs[11]], input, cpu->regs[10]);
					} else { cpu->regs[10] = 0; }
					break;
				}
				default:
					fprintf
					(
						stderr,
						"\033[1;32m[RT] \033[39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: "
						"Invalid built-in system call: \033[1m0x%08x\033[0m. Instruction \033[1m0x%08x\033[0m, program counter \033[1m0x%08x\033[0m.\n",
						__FILE__, __LINE__, __func__, cpu->regs[2], inst, cpu->pc - 4
					);
					rv32i_backtrace(cpu);
					return true;
			}
			break;
		}
		case RV32I_OPCODE_SYSTEM_PRIV_EBREAK:
		{
			cpu->pc -= 4;
			rv32i_backtrace(cpu);
			cpu->pc += 4;
			if (getchar() == '\n') getchar();
			break;
		}
		default:
		{
			rv32i_error_inval2("SYSTEM ", param.immd, 0, inst, cpu->pc - 4);
			rv32i_backtrace(cpu);
			return true;
		}
	}

	return false;
}

bool rv32i_inst_reserved2(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inval_nosubtype("RESERVED2", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool rv32i_inst_custom3(int inst, rv32i_hart_s* cpu)
{
	rv32i_error_inval_nosubtype("CUSTOM3", inst, cpu->pc);
	rv32i_backtrace(cpu);
	return true;
}

bool (*rv32i_inst_instructions[RV32I_OPCODE_CUSTOM3+1])(int inst, rv32i_hart_s* cpu) =
{
	[RV32I_OPCODE_LOAD]      = rv32i_inst_load,
	[RV32I_OPCODE_LOADFP]    = rv32i_inst_loadfp,
	[RV32I_OPCODE_CUSTOM0]   = rv32i_inst_custom0,
	[RV32I_OPCODE_MISCMEM]   = rv32i_inst_miscmem,
	[RV32I_OPCODE_OPIMM]     = rv32i_inst_opimm,
	[RV32I_OPCODE_AUIPC]     = rv32i_inst_auipc,
	[RV32I_OPCODE_OPIMM32]   = rv32i_inst_opimm32,
	[RV32I_OPCODE_STORE]     = rv32i_inst_store,
	[RV32I_OPCODE_STOREFP]   = rv32i_inst_storefp,
	[RV32I_OPCODE_CUSTOM1]   = rv32i_inst_custom1,
	[RV32I_OPCODE_AMO]       = rv32i_inst_amo,
	[RV32I_OPCODE_OP]        = rv32i_inst_op,
	[RV32I_OPCODE_LUI]       = rv32i_inst_lui,
	[RV32I_OPCODE_OP32]      = rv32i_inst_op32,
	[RV32I_OPCODE_MADD]      = rv32i_inst_madd,
	[RV32I_OPCODE_MSUB]      = rv32i_inst_msub,
	[RV32I_OPCODE_NMSUB]     = rv32i_inst_nmsub,
	[RV32I_OPCODE_NMADD]     = rv32i_inst_nmadd,
	[RV32I_OPCODE_OPFP]      = rv32i_inst_opfp,
	[RV32I_OPCODE_RESERVED0] = rv32i_inst_reserved0,
	[RV32I_OPCODE_CUSTOM2]   = rv32i_inst_custom2,
	[RV32I_OPCODE_BRANCH]    = rv32i_inst_branch,
	[RV32I_OPCODE_JALR]      = rv32i_inst_jalr,
	[RV32I_OPCODE_RESERVED1] = rv32i_inst_reserved1,
	[RV32I_OPCODE_JAL]       = rv32i_inst_jal,
	[RV32I_OPCODE_SYSTEM]    = rv32i_inst_system,
	[RV32I_OPCODE_RESERVED2] = rv32i_inst_reserved2,
	[RV32I_OPCODE_CUSTOM3]   = rv32i_inst_custom3
};
