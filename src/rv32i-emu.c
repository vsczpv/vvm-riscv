#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rv32i-emu.h"
#include "rv32i-inst.h"
#include "rv32i-misc.h"
#include "rv32i-error.h"

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

		if ( rv32i_invalassert(inst, cpu->pc) ) { rv32i_error_backtrace(*cpu); break; }

		if ( rv32i_inst_getopcode(inst) > RV32I_OPCODE_CUSTOM3 ) { rv32i_error_malinst(inst, cpu->pc); break; }
		if ( rv32i_inst_instructions[rv32i_inst_getopcode(inst)](inst, cpu) ) break;

	}

	return;
}
