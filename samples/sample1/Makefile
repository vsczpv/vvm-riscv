.PHONY: run clean

clean:
	rm -vf sample.bin sample.out sample.o

run: sample.bin
	../../vvm-riscv --debug sample.bin

sample.bin: sample.ld sample.S
	riscv64-elf-as -R -march=rv32i -mabi=ilp32 sample.S -o sample.o
	riscv64-elf-ld sample.o -T sample.ld -o sample.out --oformat=elf32-littleriscv
	riscv64-elf-objcopy --dump-section .text=sample.bin sample.out
