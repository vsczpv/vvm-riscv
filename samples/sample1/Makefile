.PHONY: run clean

default: sample.bin

clean:
	rm -vf sample.bin sample.out sample.o

run: sample.bin
	../../vvm-riscv sample.bin

run_debug: sample.bin
	../../vvm-riscv --debug sample.bin

sample.bin: sample.ld sample.S
	$(CROSS)-as -R -march=rv32i -mabi=ilp32 sample.S -o sample.o
	$(CROSS)-ld sample.o -T sample.ld -o sample.out --oformat=elf32-littleriscv
	$(CROSS)-objcopy --dump-section .text=sample.bin sample.out
