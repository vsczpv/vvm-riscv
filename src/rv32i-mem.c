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

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#include "rv32i-emu.h"
#include "rv32i-mem.h"

uint8_t* rv32i_mem_trueaddr(rv32i_hart_s* cpu, uint32_t addr)
{

	uint8_t* reladdr = NULL;
	bool found       = false;
	errno            = 0;

	for (uint32_t i = 0; i < cpu->iomap_amnt; i++)
	{
		if ( addr >= cpu->iomaps[i].start_addr && addr < cpu->iomaps[i].map.size + cpu->iomaps[i].start_addr )
		{

			uint32_t relative_index = addr - cpu->iomaps[i].start_addr;

			reladdr = &(cpu->iomaps[i].map.buf[relative_index]);
			found   = true;

			break;
		}
	}

	if (!found)
	{
		errno = EADDRNOTAVAIL;
		return reladdr;
	}


	return reladdr;
}

uint32_t rv32i_getinst(rv32i_hart_s* cpu, uint32_t index)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

 	uint32_t inst = addr[0] << 0  |
				    addr[1] << 8  |
 	                addr[2] << 16 |
 	                addr[3] << 24;

	return inst;
}

void rv32i_setinst(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	addr[0] = (val >> 0)  & 0xff;
	addr[1] = (val >> 8)  & 0xff;
	addr[2] = (val >> 16) & 0xff;
	addr[3] = (val >> 24) & 0xff;

	return;
}

uint32_t rv32i_getword(rv32i_hart_s* cpu, uint32_t index)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	uint32_t inst = addr[0] << 24 |
	                addr[1] << 16 |
	                addr[2] << 8  |
	                addr[3] << 0;

	return inst;
}

uint32_t rv32i_gethalf(rv32i_hart_s* cpu, uint32_t index)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	uint32_t inst = addr[0] << 8 |
	                addr[1] << 0;

	return inst;
}

uint32_t rv32i_getbyte(rv32i_hart_s* cpu, uint32_t index)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	uint32_t inst = addr[0];

	return inst;
}

void rv32i_setword(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	addr[0] = (val >> 24) & 0xff;
	addr[1] = (val >> 16) & 0xff;
	addr[2] = (val >> 8)  & 0xff;
	addr[3] = (val >> 0)  & 0xff;

	return;
}

void rv32i_sethalf(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	addr[0] = (val >> 8)  & 0xff;
	addr[1] = (val >> 0)  & 0xff;

	return;
}

void rv32i_setbyte(rv32i_hart_s* cpu, uint32_t index, uint32_t val)
{

	uint8_t* addr = rv32i_mem_trueaddr(cpu, index);

	addr[0] = val & 0xff;

	return;
}

bool rv32i_oob_addr(rv32i_hart_s* cpu, uint32_t addr)
{

	if ( !rv32i_mem_trueaddr(cpu, addr) )
	{
		if (errno == EADDRNOTAVAIL) return true;
		else return false;
	}

	return false;
}
