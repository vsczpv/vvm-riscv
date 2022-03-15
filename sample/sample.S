.org 0
.global _start

.data

string1: .ascii "What's your name?\n"
		 .set string1_size, .-string1

string2: .ascii "Hello, "
         .set string2_size, .-string2

string3: .ascii "!\n"
         .set string3_size, .-string3

.set buf,      0x400
.set buf_size, 0x1ff

.set SYSEXIT,  0
.set SYSWRITE, 1
.set SYSREAD,  2

.text

_start:

	li a0, SYSWRITE
	la a1, string1
	li a2, string1_size
	ecall

	li a0, SYSREAD
	li a1, buf
	li a2, buf_size
	ecall

	mv s1, a0	# Save read amount for later

	li a0, SYSWRITE
	la a1, string2
	li a2, string2_size
	ecall

	li a0, SYSWRITE
	li a1, buf
	mv a2, s1	# Write back read amount
	ecall

	li a0, SYSWRITE
	la a1, string3
	li a2, string3_size
	ecall

	li a0, SYSEXIT
	ecall
