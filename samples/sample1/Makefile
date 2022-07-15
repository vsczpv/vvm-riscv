.PHONY: run clean

clean:
	rm -vf sample.bin sample.out sample.o

run: sample.bin
	../../vvm-riscv --debug sample.bin

sample.bin:
	riscv32-elf-as -R -march=rv32i sample.S -o sample.o
	riscv32-elf-ld sample.o sample.ld -o sample.out
	riscv32-elf-objcopy --dump-section .text=sample.bin sample.out
