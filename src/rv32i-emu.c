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
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rv32i-emu.h"
#include "rv32i-inst.h"
#include "rv32i-misc.h"
#include "rv32i-error.h"
#include "rv32i-backtrace.h"

bool rv32i_iomap_init(rv32i_hart_s* cpu, uint32_t addr, uint32_t size, void (*callback)(rv32i_hart_s* cpu))
{

	if (cpu->iomap_amnt == IOMAP_HARDCAP) return true;

	if (addr <= cpu->ram.size) return true;

	for (uint32_t i = 0; i < cpu->iomap_amnt; i++)
		if (addr >= cpu->iomaps[i]->start_addr && addr <= ( cpu->iomaps[i]->start_addr + cpu->iomaps[i]->map.size )) return true;

	/* NOTE: These get free()d on rv32i_hart_destroy() */

	uint32_t curio    = (++(cpu->iomap_amnt)) - 1;
	cpu->iomaps[curio] = (rv32i_iomap_s*) malloc(sizeof(rv32i_iomap_s));

	cpu->iomaps[curio]->start_addr = addr;
	cpu->iomaps[curio]->map.size = size;
	cpu->iomaps[curio]->map.buf = (uint8_t*) malloc(sizeof(uint8_t)*size);

	cpu->iomaps[curio]->callback = callback;

	return false;

}

rv32i_hart_s rv32i_hart_init(uint32_t total_ram)
{

	/* cpu.regs also gets initialized here as a side effect. */
	rv32i_hart_s cpu = { 0 };

	cpu.ram.size = total_ram;
	cpu.ram.buf  = (uint8_t*) malloc(sizeof(uint8_t)*total_ram);

	cpu.pc   = 0;

	return cpu;
}

void rv32i_hart_destroy(rv32i_hart_s cpu)
{

	free(cpu.ram.buf);

	for (uint32_t i = 0; i < cpu.iomap_amnt; i++)
		{ free(cpu.iomaps[i]->map.buf); free(cpu.iomaps[i]); }

	return;
}

void rv32i_hart_execute(rv32i_hart_s* cpu)
{

	for (;;)
	{

		if ( cpu->pc > cpu->ram.size ) { rv32i_error_oob("execution", cpu->pc); break; }

		uint32_t inst = rv32i_getinst(cpu, cpu->pc);

		if ( rv32i_invalassert(inst, cpu->pc) ) { rv32i_backtrace(cpu); break; }

		if ( rv32i_inst_getopcode(inst) > RV32I_OPCODE_CUSTOM3 ) { rv32i_error_malinst(inst, cpu->pc); break; }

		if ( debug ) rv32i_backtrace(cpu), getchar(), system("clear");

		if ( rv32i_inst_instructions[rv32i_inst_getopcode(inst)](inst, cpu) ) break;
	}

	return;
}
