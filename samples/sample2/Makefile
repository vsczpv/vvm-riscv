CARGS = -ffreestanding -Wno-builtin-declaration-mismatch -nostdlib -nostdinc -c -march=rv32i2p0 -mabi=ilp32 -I./

.PHONY: run clean

default: main.bin

clean:
	rm -vf *.out *.bin *.o

run: main.bin
	../../vvm-riscv main.bin

# TODO: Fix ISA mis-match warning
#       Fix reordering of non-static arrays
main.bin: main.c ministd.c ministd.h arith.c main.ld _start.S
	$(CROSS)-as -march=rv32i2p0 -mabi=ilp32 _start.S -o _start.o
	$(CROSS)-gcc $(CARGS) main.c
	$(CROSS)-gcc $(CARGS) arith.c
	$(CROSS)-gcc $(CARGS) ministd.c
	$(CROSS)-ld _start.o arith.o ministd.o main.o -T main.ld --oformat=elf32-littleriscv -o main.out
	$(CROSS)-objcopy --dump-section .text=main.bin main.out
