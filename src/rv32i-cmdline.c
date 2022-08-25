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
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "rv32i-emu.h"
#include "rv32i-mem.h"
#include "rv32i-error.h"

bool debug    = false;
bool noopstub = false;

void usage(void)
{
	puts
	(
		"\n"
		"Usage: vvm-riscv [-m <ram>|--memory-map <addr> <size>] [--show-map] [-h|--help]\n"
		"                 [--debug] [--noop-stub] [--version] FILENAME"
		"\n\n"
		"       -m <ram>                    Specify amount of ram in kibibytes (1024).\n"
		"                                   Value cannot be 0.\n"
		"                                   Default value is 16KiB."
		"\n\n"
		"       --memory-map <addr> <size>  Specify a new memory map at <addr> with <size> kibibytes.\n"
		"                                   Size cannot be 0.\n"
		"                                   Address has to be aligned to 1KiB boundaries.\n"
		"                                   Cannot be used with -m."
		"\n\n"
		"       --show-map                  Display current memory mappings and quit."
		"\n\n"
		"       --load-at <addr>            Load and run program at specified address."
		"\n\n"
		"       -h, --help                  Show this prompt.\n"
		"       --debug                     Step-by-step debbuger\n"
		"       --noop-stub                 Treat ecall stub as noop.\n"
		"       --version                   Show version and license."
		"\n\n"
		"    FILENAME must be a path to a raw RISCV RV32I flat binary.\n"
		"    Numbers can be any positive value readable by strtol(3) [0x1234, 1234, 01234]."
		"\n"
	);

}

void version(void)
{
	puts
	(
		"vvm-riscv, version " VERSION "\n"
		"Copyright (C) 2022 Vinícius Schütz Piva\n"
		"Licensed under GNU GPL-3-or-later <http://gnu.org/licenses/gpl.html>\n"
		"\n"
		"This is free software; you are free to change and redistribute it.\n"
		"There is NO WARRANTY, to the extent permitted by law."
	);
}

rv32i_cmdline_s rv32i_parse_cmdline(int argc, char* argv[])
{

	rv32i_cmdline_s r =
	{
		.filename            = NULL,
		.ramamnt             = 16 * KiB,
		.is_using_mmap       = false,
		.choosen_iomaps      = { {0} },
		.choosen_iomaps_amnt = 0
	};

	bool ramset      = false;
	bool fileset     = false;
	bool show_map    = false;
	bool load_at_set = false;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if ( !strcmp(argv[i], "-h") && !strcmp(argv[i], "--help") ) { usage(); exit(EXIT_SUCCESS); }
			else if ( !strcmp(argv[i], "--version") ) { version(); exit(EXIT_SUCCESS); }
			else if ( !strcmp(argv[i], "--debug") )
			{
				if (debug) { usage(); exit(EXIT_FAILURE); }
				debug = true;
			}
			else if ( !strcmp(argv[i], "--noop-stub") )
			{
				if (noopstub) { usage(); exit(EXIT_FAILURE); }
				noopstub = true;
			}
			else if ( !strcmp(argv[i], "--load-at") )
			{
				if (load_at_set || i+1 >= argc) { usage(); exit(EXIT_FAILURE); }
				load_at_set = true;

				char* endptr;
				uint32_t addr = strtol(argv[++i], &endptr, 0);
				if (argv[i-1] == endptr) { usage(); exit(EXIT_FAILURE); }
				if ((signed) addr < 0) { usage(); exit(EXIT_FAILURE); }

				r.load_at = addr;
			}
			else if ( !strcmp(argv[i], "--show-map") )
			{
				if (show_map) { usage(); exit(EXIT_FAILURE); }
				show_map = true;
			}
			else if ( !strcmp(argv[i], "--memory-map") )
			{

				if (ramset || i+2 >= argc) { usage(); exit(EXIT_FAILURE); }
				r.is_using_mmap = true;

				if (r.choosen_iomaps_amnt == IOMAP_HARDCAP) { rv32i_too_many_maps(); exit(EXIT_FAILURE); }

				char* endptr;
				uint32_t addr = strtol(argv[++i], &endptr, 0);
				if (argv[i-1] == endptr) { usage(); exit(EXIT_FAILURE); }
				if ((signed) addr < 0) { usage(); exit(EXIT_FAILURE); }

				if (addr & ALIGNMENT_MASK) { rv32i_nonaligned_iomap(addr); exit(EXIT_FAILURE); }

				size_t   size = strtol(argv[++i], NULL, 0) * KiB;
				if (!size) { usage(); exit(EXIT_FAILURE); }
				if ((signed) size < 0) { usage(); exit(EXIT_FAILURE); }

				r.choosen_iomaps[r.choosen_iomaps_amnt].addr = addr;
				r.choosen_iomaps[r.choosen_iomaps_amnt].size = size;

				r.choosen_iomaps_amnt++;

			}
			else if ( !strcmp(argv[i], "-m"))
			{
				if (ramset || i+1 >= argc) { usage(); exit(EXIT_FAILURE); }
				ramset = true;
				r.ramamnt = strtol(argv[++i], NULL, 0) * KiB;
				if ((!r.ramamnt && errno == EINVAL) || !r.ramamnt || r.is_using_mmap) { usage(); exit(EXIT_FAILURE); }
				if ((signed) r.ramamnt < 0) { usage(); exit(EXIT_FAILURE); }
			}
			else if (argv[i][0] == '-') { usage(); exit(EXIT_FAILURE); }
			else
			{
				if (fileset) { usage(); exit(EXIT_FAILURE); }
				r.filename = argv[i]; fileset = true;
			}
		}
	}
	else { usage(); exit(EXIT_FAILURE); }

	if (show_map) { rv32i_info_showmmap(r); exit(EXIT_SUCCESS); }

	if (!fileset) { usage(); exit(EXIT_FAILURE); }

	return r;
}

rv32i_posix_file_s rv32i_open_file(rv32i_cmdline_s cmd)
{

	rv32i_posix_file_s file = { .fd = open(cmd.filename, O_RDONLY) };

	if (file.fd < 0) { rv32i_error_nofile(cmd.filename); exit(EXIT_FAILURE); }

	fstat(file.fd, &file.st);

	file.buf = (uint8_t*) mmap(NULL, file.st.st_size, PROT_READ, MAP_PRIVATE, file.fd, 0);

	if (file.buf == MAP_FAILED) { rv32i_error_aintfile(cmd.filename); exit(EXIT_FAILURE); }

	return file;
}

void rv32i_close_file(rv32i_posix_file_s file)
{

	munmap(file.buf, file.st.st_size);
	close(file.fd);

	return;
}

void rv32i_load_program(rv32i_hart_s* cpu, rv32i_posix_file_s file, rv32i_cmdline_s cmd)
{

	uint32_t prgmemsz = rv32i_mem_contiguous(cpu, cmd.load_at, file.st.st_size);

	if ( prgmemsz < file.st.st_size )
	{
		rv32i_error_oom(file.st.st_size, prgmemsz);
		exit(EXIT_FAILURE);
	}

	rv32i_mem_copyfromhost(cpu, cmd.load_at, file.buf, file.st.st_size);

	return;
}

static int rv32i_info_showmmap_sort(const void* a, const void* b)
{

	rv32i_cmdline_chooseniomap_s* ap = *( (rv32i_cmdline_chooseniomap_s**) a );
	rv32i_cmdline_chooseniomap_s* bp = *( (rv32i_cmdline_chooseniomap_s**) b );

	return ap->addr > bp->addr;
}

void rv32i_info_showmmap(rv32i_cmdline_s cmd)
{

	rv32i_overlapping_iomap_offense_s check = rv32i_chooseniomap_checkoverlap(cmd);
	if (check.offended) { rv32i_overlapping_iomaps(check); exit(EXIT_FAILURE); }

	rv32i_cmdline_chooseniomap_s* inorder_iomaps[IOMAP_HARDCAP] = { NULL };
	for (int i = 0; i < cmd.choosen_iomaps_amnt; i++)
		inorder_iomaps[i] = &cmd.choosen_iomaps[i];

	qsort(inorder_iomaps, cmd.choosen_iomaps_amnt, sizeof(rv32i_cmdline_chooseniomap_s*), rv32i_info_showmmap_sort);

	if (!cmd.is_using_mmap)	printf
	(
		"┌──────────────┐\n"
		"│  0x00000000  │\n"
		"│              │    IOMAP #0 → 0x0 ~ 0x%x : %u bytes.\n"
		"│  0x%08x"  "  │\n"
		"├┄┄┄┄┄┄┄┄┄┄┄┄┄┄┤\n"
		"│     none     │\n"
		"└──────────────┘\n",
		cmd.ramamnt, cmd.ramamnt, cmd.ramamnt
	);
	else
	{

		printf
		(
			"┌──────────────┐\n"
		);

		bool has_addr_zero = false;
		for (int i = 0; i < cmd.choosen_iomaps_amnt; i++)
			if (inorder_iomaps[i]->addr == 0) { has_addr_zero = true; break; }

		if (!has_addr_zero) printf
		(
			"│     none     │\n"
			"├┄┄┄┄┄┄┄┄┄┄┄┄┄┄┤\n"
		);

		for (int i = 0; i < cmd.choosen_iomaps_amnt; i++)
		{

			printf
			(
				"│  0x%08x"  "  │\n"
				"│              │    IOMAP #%i → 0x%x ~ 0x%lx : %lu bytes.\n"
				"│  0x%08lx" "  │\n"
				"├┄┄┄┄┄┄┄┄┄┄┄┄┄┄┤\n",
				inorder_iomaps[i]->addr,
				i,
				inorder_iomaps[i]->addr, inorder_iomaps[i]->addr + inorder_iomaps[i]->size - 1,
				inorder_iomaps[i]->size,
				inorder_iomaps[i]->addr + inorder_iomaps[i]->size - 1
			);


			if (i+1 != cmd.choosen_iomaps_amnt)
			{
				if (inorder_iomaps[i]->addr + inorder_iomaps[i]->size != inorder_iomaps[i+1]->addr)
				printf
				(
					"│     none     │\n"
					"├┄┄┄┄┄┄┄┄┄┄┄┄┄┄┤\n"
				);
			}

		}

		printf
		(
			"│     none     │\n"
			"└──────────────┘\n"
		);

	}

	return;
}
