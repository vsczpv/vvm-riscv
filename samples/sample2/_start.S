.org 0
.global _start
.global exit
.global write
.global read
.extern main

.data

.set STACK_CEIL, 0x4000

.set SYSEXIT,  0
.set SYSWRITE, 1
.set SYSREAD,  2

.text

_start:

	li sp, STACK_CEIL

	call main

	li a0, SYSEXIT
	ecall

exit:
	li a0, SYSEXIT
	ecall
#	doesn't return

write:

	mv a2, a1
	mv a1, a0
	li a0, SYSWRITE
	ecall
	ret
	
read:

	mv a2, a1
	mv a1, a0
	li a0, SYSREAD
	ecall
	ret
