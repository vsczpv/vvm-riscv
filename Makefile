VERSION = "0.1.0"

SOURCE =	src
BUILD =		build
INCLUDE =	include

CC =		cc
AS =		riscv32-elf-as
ASLD =          riscv32-elf-ld
OBJCP =		riscv32-elf-objcopy
OPT_CARGS :=
CARGS = 	$(OPT_CARGS) -I$(INCLUDE) -DVERSION='$(VERSION)' -Wall -Wextra -std=c2x -Wno-misleading-indentation -Wno-missing-field-initializers

OBJS = 		$(shell find $(SOURCE) -type f -name '*.c' | sed 's/\.c*$$/\.o/; s/$(SOURCE)\//$(BUILD)\//')
HEADERS =	$(shell find $(INCLUDE) -type f -name '*.h')

.PHONY: all run clean builddir

all: vvm-riscv

builddir:
	@if [ ! -d "build" ] ; then mkdir build ; fi

run: vvm-riscv
	./vvm-riscv sample/sample.bin

clean:
	rm -vf vvm-riscv sample/sample.o sample/sample.bin sample/sample.out $(OBJS)

$(OBJS): $(BUILD)/%.o: $(SOURCE)/%.c $(HEADERS)
	$(CC) -c $(CARGS) $< -o $@

sample/sample.bin: sample/sample.S
	$(AS) -R -march=rv32i sample/sample.S -o sample/sample.o
	$(ASLD) sample/sample.o sample/sample.ld -o sample/sample.out
	$(OBJCP) --dump-section .text=$@ sample/sample.out

vvm-riscv: builddir sample/sample.bin $(OBJS)
	$(CC) $(CARGS) $(OBJS) -o $@
