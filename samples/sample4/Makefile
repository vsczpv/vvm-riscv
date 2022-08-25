CARGS = -ffreestanding -Wno-builtin-declaration-mismatch -nostdlib -nostdinc -c -march=rv32i2p0 -mabi=ilp32 -I./ 

.PHONY: run clean

default: main.bin

clean:
	rm -vf *.out *.bin *.o

run: main.bin
	../../vvm-riscv --load-at 0x8000 \
                --memory-map 0x8000 4 \
                --memory-map 0x3c00 1 \
                main.bin

# TODO: Fix ISA mis-match warning
#       Fix reordering of non-static arrays
main.bin: main.c ministd.c ministd.h arith.c main.ld _start.S
	riscv64-elf-as -march=rv32i2p0 -mabi=ilp32 _start.S -o _start.o
	riscv64-elf-gcc $(CARGS) main.c
	riscv64-elf-gcc $(CARGS) arith.c
	riscv64-elf-gcc $(CARGS) ministd.c
	riscv64-elf-ld _start.o arith.o ministd.o main.o -T main.ld --oformat=elf32-littleriscv -o main.out
	riscv64-elf-objcopy --dump-section .text=main.bin main.out
