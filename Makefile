VERSION = "0.2.1"

SOURCE =	src
BUILD =		build
INCLUDE =	include

CC =		cc
OPT_CARGS :=
CARGS = 	$(OPT_CARGS) -I$(INCLUDE) -DVERSION='$(VERSION)' -Wall -Wextra -std=c2x -Wno-misleading-indentation -Wno-missing-field-initializers -g

OBJS = 		$(shell find $(SOURCE) -type f -name '*.c' | sed 's/\.c*$$/\.o/; s/$(SOURCE)\//$(BUILD)\//')
HEADERS =	$(shell find $(INCLUDE) -type f -name '*.h')

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
	$(CC) $(CARGS) $(OBJS) -o $@
