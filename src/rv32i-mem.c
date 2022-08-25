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
#include <string.h>
#include <sys/param.h>
#include <errno.h>

#include "rv32i-emu.h"
#include "rv32i-mem.h"

#include <stdio.h>

rv32i_iomap_s* rv32i_mem_getiomap_byaddr_nonmemoised(rv32i_hart_s* cpu, uint32_t addr)
{

	rv32i_iomap_s* map = NULL;

	for (uint32_t i = 0; i < cpu->iomap_amnt; i++)
	{
		if ( addr >= cpu->iomaps[i].start_addr && addr < cpu->iomaps[i].map.size + cpu->iomaps[i].start_addr )
		{
			map = &(cpu->iomaps[i]);
			break;
		}
	}

	return map;
}

rv32i_iomap_s* rv32i_mem_getiomap_byaddr(rv32i_hart_s* cpu, uint32_t addr)
{

	rv32i_iomap_s* map = NULL;

 	if ((addr & MEMOIZATION_MASK) == (cpu->memoized_iomap->start_addr))
 		return cpu->memoized_iomap;

	for (uint32_t i = 0; i < cpu->iomap_amnt; i++)
	{
		if ( addr >= cpu->iomaps[i].start_addr && addr < cpu->iomaps[i].map.size + cpu->iomaps[i].start_addr )
		{
			map = &(cpu->iomaps[i]);
			break;
		}
	}

 	if (map) cpu->memoized_iomap = map;

	return map;
}

uint32_t rv32i_mem_contiguous(rv32i_hart_s* cpu, uint32_t start, uint32_t end)
{

	if (end < start) return 0;

	rv32i_iomap_s* map = rv32i_mem_getiomap_byaddr(cpu, start);

	if (!map) return 0;

	uint32_t memsize = map->map.size;

	if (memsize < end - start) return memsize + rv32i_mem_contiguous(cpu, start + map->map.size, end);

	return memsize;
}

uint8_t* rv32i_mem_trueaddr(rv32i_hart_s* cpu, uint32_t addr)
{

	errno = 0;

	rv32i_iomap_s* map = rv32i_mem_getiomap_byaddr(cpu, addr);

	if (!map) { errno = EADDRNOTAVAIL; return NULL; }

	uint32_t relative_index = addr - map->start_addr;

	return &(map->map.buf[relative_index]);
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

	errno = 0;

	if ( !rv32i_mem_trueaddr(cpu, addr) )
	{
		if (errno == EADDRNOTAVAIL) return true;
		else return false;
	}

	return false;
}

void rv32i_mem_copyfromhost(rv32i_hart_s* cpu, uint32_t addr, void* src, size_t count)
{

	uint8_t* src_as_bytes = (uint8_t*) src;

	rv32i_iomap_s* iomaps[IOMAP_HARDCAP] = { NULL };

	int i = 0; for (size_t j = addr; j < addr+count; j = ALIGN_TO_PAGE(j) + PAGE_SIZE)
	{

		rv32i_iomap_s* new_iomap = rv32i_mem_getiomap_byaddr(cpu, j);

		if      (!i)                     iomaps[i++] = new_iomap;
		else if (new_iomap != iomaps[i]) iomaps[i++] = new_iomap;

	}

	if (!iomaps[0]) return;

	uint32_t src_offset      = iomaps[0]->start_addr;
	size_t   src_transferred = 0;

	uint32_t rela_addr       = addr - iomaps[0]->start_addr;

	int j = 0; while (true)
	{

		if (!iomaps[j]) break;

		uint32_t src_index = iomaps[j]->start_addr - src_offset;

		size_t to_transfer = MIN(iomaps[j]->map.size, count - src_transferred);

		memcpy (iomaps[j]->map.buf + rela_addr, src_as_bytes + src_index, to_transfer);

		src_transferred += to_transfer;

		j++; rela_addr = 0;
	}

	return;
}

void rv32i_mem_copyfromguest(rv32i_hart_s* cpu, uint32_t addr, void* dest, size_t count)
{

	uint8_t* dest_as_bytes = (uint8_t*) dest;

	rv32i_iomap_s* iomaps[IOMAP_HARDCAP] = { NULL };

	int i = 0; for (size_t j = addr; j < addr+count; j = ALIGN_TO_PAGE(j) + PAGE_SIZE)
	{

		rv32i_iomap_s* new_iomap = rv32i_mem_getiomap_byaddr(cpu, j);

		if      (!i)                     iomaps[i++] = new_iomap;
		else if (new_iomap != iomaps[i]) iomaps[i++] = new_iomap;

	}

	if (!iomaps[0]) return;

	uint32_t dest_offset      = iomaps[0]->start_addr;
	size_t   dest_transferred = 0;

	uint32_t rela_addr       = addr - iomaps[0]->start_addr;

	int j = 0; while (true)
	{

		if (!iomaps[j]) break;

		uint32_t dest_index = iomaps[j]->start_addr - dest_offset;

		size_t to_transfer = MIN(iomaps[j]->map.size, count - dest_transferred);

		memcpy (dest_as_bytes + dest_index, iomaps[j]->map.buf + rela_addr, to_transfer);

		dest_transferred += to_transfer;

		j++; rela_addr = 0;
	}

	return;
}

rv32i_overlapping_iomap_offense_s rv32i_chooseniomap_checkoverlap(rv32i_cmdline_s cmd)
{

	rv32i_overlapping_iomap_offense_s of = { .offended = false };

	for (int i = 0; i < cmd.choosen_iomaps_amnt; i++)
	{
		for (int j = 0; j < cmd.choosen_iomaps_amnt; j++)
		{
			if (i == j) continue;

			if (cmd.choosen_iomaps[j].addr <  cmd.choosen_iomaps[i].addr) continue;

			if (cmd.choosen_iomaps[j].addr == cmd.choosen_iomaps[i].addr)
			{
				of.offended = true;

				of.a = cmd.choosen_iomaps[i];
				of.b = cmd.choosen_iomaps[j];

				return of;
			};

			if (cmd.choosen_iomaps[i].addr +  cmd.choosen_iomaps[i].size - 1 >= cmd.choosen_iomaps[j].addr)
			{
				of.offended = true;

				of.a = cmd.choosen_iomaps[i];
				of.b = cmd.choosen_iomaps[j];

				return of;
			};
		}
	}

	return of;
}
