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
#include "rv32i-error.h"
#include "rv32i-backtrace.h"
#include "rv32i-mem.h"

bool rv32i_iomap_init(rv32i_hart_s* cpu)
{

	cpu->iomaps = (rv32i_iomap_s*) malloc (sizeof(rv32i_iomap_s)*IOMAP_HARDCAP);

	return cpu->iomaps ? false : true;
}

void rv32i_iomap_add(rv32i_hart_s* cpu, uint32_t addr, uint32_t size, void (*callback)(rv32i_hart_s* cpu), bool memback)
{

	uint32_t curio     = (++(cpu->iomap_amnt)) - 1;

	cpu->iomaps[curio].start_addr = addr;
	cpu->iomaps[curio].map.size   = size;

	if (memback) cpu->iomaps[curio].map.buf = (uint8_t*) malloc(sizeof(uint8_t)*size);

	cpu->iomaps[curio].callback      = callback;
	cpu->iomaps[curio].memory_backed = memback;

	return;

}

rv32i_hart_s rv32i_hart_init(rv32i_cmdline_s cmd)
{

	rv32i_hart_s cpu = { 0 };

	rv32i_iomap_init(&cpu);

	rv32i_overlapping_iomap_offense_s check = rv32i_chooseniomap_checkoverlap(cmd);
	if (check.offended) { rv32i_overlapping_iomaps(check); exit(EXIT_FAILURE); }

	rv32i_iomap_s* map;

	if (!cmd.is_using_mmap)
	{

		rv32i_iomap_add(&cpu, 0, cmd.ramamnt, NULL, true);

		map = rv32i_mem_getiomap_byaddr_nonmemoised(&cpu, 0);
	}
	else
	{

		for (int i = 0; i < cmd.choosen_iomaps_amnt; i++)
			rv32i_iomap_add(&cpu, cmd.choosen_iomaps[i].addr, cmd.choosen_iomaps[i].size, NULL, true);

		map = rv32i_mem_getiomap_byaddr_nonmemoised(&cpu, cmd.load_at);
	}

	if (!map) { rv32i_nomap_atexec(); exit(EXIT_FAILURE); };

	cpu.memoized_iomap = map;

	cpu.pc = cmd.load_at;

	return cpu;
}

void rv32i_hart_destroy(rv32i_hart_s cpu)
{

//	for (uint32_t i = 0; i < cpu.iomap_amnt; i++) free(cpu.iomaps[i].map.buf);

	free(cpu.iomaps);

	return;
}

void rv32i_hart_execute(rv32i_hart_s* cpu)
{

	for (;;)
	{

		if ( rv32i_oob_addr(cpu, cpu->pc) ) { rv32i_error_oob("execution", cpu->pc); break; }

		uint32_t inst = rv32i_getinst(cpu, cpu->pc);

		if ( rv32i_invalassert(inst, cpu->pc) ) { rv32i_backtrace(cpu); break; }

		if ( rv32i_inst_getopcode(inst) > RV32I_OPCODE_CUSTOM3 ) { rv32i_error_malinst(inst, cpu->pc); break; }

		if ( debug ) system("clear"), rv32i_backtrace(cpu), getchar();

		if ( rv32i_inst_instructions[rv32i_inst_getopcode(inst)](inst, cpu) ) break;
	}

	return;
}
