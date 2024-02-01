/*
 * Copyright © 2024 Vinícius Schütz Piva
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

#ifndef RV32I_STRINGLIST_H_
#define RV32I_STRINGLIST_H_

#include <stddef.h>

typedef struct rv32i_counted_string_s
{
	char* string;
	size_t count;
} rv32i_counted_string_s;

rv32i_counted_string_s rv32i_cnstring_init(void);
void rv32i_cnstring_append_rcnstr(rv32i_counted_string_s* a, const char* b, size_t bn);

void rv32i_cnstring_delete(rv32i_counted_string_s* str);

#endif // RV32I_STRINGLIST_H_
