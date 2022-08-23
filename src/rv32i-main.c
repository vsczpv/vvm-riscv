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

#include <stdlib.h>

#include "rv32i-emu.h"
#include "rv32i-macro.h"
#include "rv32i-error.h"
#include "rv32i-mem.h"

#include "rv32i-inst.h"

int main(int argc, char* argv[])
{
	rv32i_cmdline_s cmd = rv32i_parse_cmdline(argc, argv);

	rv32i_posix_file_s file = rv32i_open_file(cmd);

	rv32i_hart_s cpu = rv32i_hart_init(cmd);

	rv32i_load_program(&cpu, file);

	rv32i_close_file(file);

	rv32i_hart_execute(&cpu);

	rv32i_hart_destroy(cpu);

	return EXIT_SUCCESS;
}
