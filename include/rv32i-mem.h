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

#ifndef RV32I_MEM_H_
#define RV32I_MEM_H_

#include <stdint.h>

#include "rv32i-emu.h"

rv32i_iomap_s* rv32i_mem_getiomap_byaddr(rv32i_hart_s* cpu, uint32_t addr);

uint32_t       rv32i_mem_contiguous(rv32i_hart_s* cpu, uint32_t start, uint32_t end);
uint8_t*       rv32i_mem_trueaddr(rv32i_hart_s* cpu, uint32_t addr);

uint32_t       rv32i_getinst(rv32i_hart_s* cpu, uint32_t index);
void           rv32i_setinst(rv32i_hart_s* cpu, uint32_t index, uint32_t val);

uint32_t       rv32i_getword(rv32i_hart_s* cpu, uint32_t index);
uint32_t       rv32i_gethalf(rv32i_hart_s* cpu, uint32_t index);
uint32_t       rv32i_getbyte(rv32i_hart_s* cpu, uint32_t index);

void           rv32i_setword(rv32i_hart_s* cpu, uint32_t index, uint32_t val);
void           rv32i_sethalf(rv32i_hart_s* cpu, uint32_t index, uint32_t val);
void           rv32i_setbyte(rv32i_hart_s* cpu, uint32_t index, uint32_t val);

bool           rv32i_oob_addr(rv32i_hart_s* cpu, uint32_t addr);

void           rv32i_mem_copyfromhost(rv32i_hart_s* cpu, uint32_t addr, void* src, size_t count);

rv32i_overlapping_iomap_offense_s rv32i_chooseniomap_checkoverlap(rv32i_cmdline_s cmd);

#endif // RV32I_MEM_H_
