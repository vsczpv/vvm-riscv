#include <stdint.h>

#include "rv32i-emu.h"
#include "rv32i-mem.h"

uint32_t rv32i_getinst(rv32i_hart_s* cpu, uint32_t index)
{

	uint32_t inst = ((cpu)->ram.buf[index+0] << 0)  |
	                ((cpu)->ram.buf[index+1] << 8)  |
	                ((cpu)->ram.buf[index+2] << 16) |
	                ((cpu)->ram.buf[index+3] << 24);

	return inst;
}

void rv32i_setinst(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	(cpu)->ram.buf[index+0] = (val >> 0)  & 0xff;
	(cpu)->ram.buf[index+1] = (val >> 8)  & 0xff;
	(cpu)->ram.buf[index+2] = (val >> 16) & 0xff;
	(cpu)->ram.buf[index+3] = (val >> 24) & 0xff;

	return;
}

uint32_t rv32i_getword(rv32i_hart_s* cpu, uint32_t index)
{

	uint32_t inst = ((cpu)->ram.buf[index+0] << 24) |
	                ((cpu)->ram.buf[index+1] << 16) |
	                ((cpu)->ram.buf[index+2] << 8)  |
	                ((cpu)->ram.buf[index+3] << 0);

	return inst;
}

uint32_t rv32i_gethalf(rv32i_hart_s* cpu, uint32_t index)
{

	uint32_t inst = ((cpu)->ram.buf[index+0] << 8) |
	                ((cpu)->ram.buf[index+1] << 0);

	return inst;
}

uint32_t rv32i_getbyte(rv32i_hart_s* cpu, uint32_t index)
{
	uint32_t inst = (cpu)->ram.buf[index];

	return inst;
}

void rv32i_setword(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	(cpu)->ram.buf[index+0] = (val >> 24) & 0xff;
	(cpu)->ram.buf[index+1] = (val >> 16) & 0xff;
	(cpu)->ram.buf[index+2] = (val >> 8)  & 0xff;
	(cpu)->ram.buf[index+3] = (val >> 0)  & 0xff;

	return;
}

void rv32i_sethalf(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	(cpu)->ram.buf[index+0] = (val >> 8)  & 0xff;
	(cpu)->ram.buf[index+1] = (val >> 0)  & 0xff;

	return;
}

void rv32i_setbyte(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	(cpu)->ram.buf[index] = val & 0xff;

	return;
}

bool rv32i_oob_addr(rv32i_hart_s* cpu, uint32_t addr)
{

	if (addr > cpu->ram.size) return true;
	else return false;

}
