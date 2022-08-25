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

#ifndef RV32I_EMU_H_
#define RV32I_EMU_H_

#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>

// VERSION is defined in the Makefile
#ifndef VERSION
#define VERSION "unreleased"
#endif

#define IOMAP_HARDCAP 32
#define KiB           sizeof(uint8_t)*1024
#define PAGE_SIZE     1024

#define ALIGN_TO_PAGE(addr) ( (addr - 1) + PAGE_SIZE - ( (addr - 1) % PAGE_SIZE) )

#define MEMOIZATION_MASK 0xfffffe00
#define ALIGNMENT_MASK   0x000001ff

struct rv32i_hart_s;

typedef struct rv32i_iomap_s
{
	uint32_t start_addr;
	struct { uint8_t* buf; uint32_t size; } map;
	void (*callback)(struct rv32i_hart_s* cpu);
	bool memory_backed;
} rv32i_iomap_s;

typedef struct rv32i_hart_s
{
	uint32_t iomap_amnt;
	rv32i_iomap_s* iomaps;
	struct { uint8_t* buf; uint32_t size; } ram;
	int32_t regs[32];
	uint32_t pc;
	rv32i_iomap_s* memoized_iomap;
} rv32i_hart_s;

typedef struct rv32i_cmdline_chooseniomap_s
{
	uint32_t addr;
	size_t   size;
} rv32i_cmdline_chooseniomap_s;

typedef struct rv32i_cmdline_s
{
	char* filename;
	int ramamnt;
	bool is_using_mmap;
	int choosen_iomaps_amnt;
	rv32i_cmdline_chooseniomap_s choosen_iomaps[IOMAP_HARDCAP];
	uint32_t load_at;
} rv32i_cmdline_s;

typedef struct rv32i_posix_file_s
{
	int fd;
	struct stat st;
	uint8_t* buf;
} rv32i_posix_file_s;

typedef struct rv32i_overlapping_iomap_offense_s
{
	bool offended;
	rv32i_cmdline_chooseniomap_s a;
	rv32i_cmdline_chooseniomap_s b;
} rv32i_overlapping_iomap_offense_s;

bool rv32i_iomap_init (rv32i_hart_s* cpu);
bool rv32i_iomap_add  (rv32i_hart_s* cpu, uint32_t addr, uint32_t size, void (*callback)(rv32i_hart_s* cpu), bool memback);

rv32i_hart_s rv32i_hart_init    (rv32i_cmdline_s cmd);
void         rv32i_hart_destroy (rv32i_hart_s cpu);

void rv32i_hart_execute(rv32i_hart_s* cpu);

extern bool debug;
extern bool noopstub;

rv32i_cmdline_s    rv32i_parse_cmdline (int argc, char* argv[]);
rv32i_posix_file_s rv32i_open_file     (rv32i_cmdline_s cmd);
void               rv32i_close_file    (rv32i_posix_file_s file);
void               rv32i_load_program  (rv32i_hart_s* cpu, rv32i_posix_file_s file, rv32i_cmdline_s cmd);

void rv32i_info_showmmap(rv32i_cmdline_s cmd);

#endif /* RV32I_EMU_H_ */
