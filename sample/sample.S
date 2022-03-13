.org 0
.global _start

_start:

	addi a0, zero, 1
	addi a1, zero, 91
	addi a2, zero, 19
	ecall

	addi a0, zero, 2
	addi a1, zero, 256
	addi a2, zero, 0x1ff
	ecall

	add s1, a0, zero

	addi a0, zero, 1
	addi a1, zero, 109
	addi a2, zero, 8
	ecall

	addi a0, zero, 1
	addi a1, zero, 256
	addi a2, s1, 0
	ecall

	addi a0, zero, 1
	addi a1, zero, 117
	addi a2, zero, 2
	ecall

	add a0, zero, 0
	ecall

string1:	.ascii "What's your name?\n"
string2:	.ascii "Hello, "
string3:	.ascii "!\n"
