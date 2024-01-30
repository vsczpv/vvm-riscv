#
# Copyright © 2022-2024 Vinícius Schütz Piva
#
# This file is part of vvm-riscv
#
# vvm-riscv is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
#

VERSION = "0.3.1"

SOURCE =      src
BUILD =       build
INCLUDE =     include

CC =          cc
OPT_CARGS :=
CARGS =       $(OPT_CARGS) -I$(INCLUDE) -DVERSION='$(VERSION)' -Wall -Wextra -std=c2x -Wno-misleading-indentation -Wno-missing-field-initializers
DEBUG_CARGS =  -g -fsanitize=address -fsanitize=pointer-compare -fsanitize=undefined

# Find a riscv toolchain
export CROSS=$(shell sh ./detect-rv-toolchain.sh)

ifeq ($(CROSS),)
$(error No riscv toolchain autodetected, please specify with CROSS envvar)
endif

# Enable debug flags in gcc if asked
ifeq (${RV32I_DEBUG},1)
CARGS += $(DEBUG_CARGS)
endif

# Find all source and header files
OBJS =       $(shell find $(SOURCE)  -type f -name '*.c' | sed 's/\.c*$$/\.o/; s/$(SOURCE)\//$(BUILD)\//')
HEADERS =    $(shell find $(INCLUDE) -type f -name '*.h')

LIBS =       -lncursesw

.PHONY: all clean builddir example_debugger example_userinput example_fibonacci example_buffers

all: vvm-riscv

builddir:
	@if [ ! -d "build" ] ; then mkdir build ; fi

clean:
	rm -vf vvm-riscv $(OBJS)

	make -C samples/sample1 clean
	make -C samples/sample2 clean
	make -C samples/sample3 clean
	make -C samples/sample4 clean

$(OBJS): $(BUILD)/%.o: $(SOURCE)/%.c $(HEADERS)
	$(CC) -c $(CARGS) $< -o $@

example_userinput: vvm-riscv
	make -C samples/sample1 run

example_debugger:  vvm-riscv
	make -C samples/sample1 run_debug

example_fibonacci: vvm-riscv
	make -C samples/sample2 run

example_buffers:   vvm-riscv
	make -C samples/sample3 run

example_loadat:    vvm-riscv
	make -C samples/sample4 run

vvm-riscv: builddir $(OBJS)
	$(CC) $(CARGS) $(OBJS) $(LIBS) -o $@
