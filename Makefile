SOURCE =	src
BUILD =		build
INCLUDE =	include

CC =		cc
AS =		riscv32-elf-as
OBJCP =		riscv32-elf-objcopy
OPT_CARGS :=
CARGS = 	$(OPT_CARGS) -I$(INCLUDE) -Wall -Wextra -std=c2x -Wno-misleading-indentation -Wno-missing-field-initializers

OBJS = 		$(shell find $(SOURCE) -type f -name '*.c' | sed 's/\.c*$$/\.o/; s/$(SOURCE)\//$(BUILD)\//')
HEADERS =	$(shell find $(INCLUDE) -type f -name '*.h')

.PHONY: all run clean

all: vvm-riscv

run: vvm-riscv
	./vvm-riscv sample/sample.bin

clean:
	rm -vf vvm-riscv sample/sample.o $(OBJS)

$(OBJS): $(BUILD)/%.o: $(SOURCE)/%.c $(HEADERS)
	$(CC) -c $(CARGS) $< -o $@

sample/sample.bin: sample/sample.S
	$(AS) -fno-pic -R -march=rv32i sample/sample.S -o sample/sample.o
	$(OBJCP) --dump-section .text=$@ sample/sample.o

vvm-riscv: sample/sample.bin $(OBJS)
	$(CC) $(CARGS) $(OBJS) -o $@
