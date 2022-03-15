.org 0
.global _start

_start:

	addi a0, a1, 123
	slli a3, fp, 5
	xori a0, a0, 255
	srli t2, a6, 2
	srai s7, sp, 4

	lui a3, 1
	auipc sp, 2

	add a0, a1, a2
	sub t0, sp, fp

	sll s0, t0, a3
	xor a0, a0, a0
	srl t0, a5, s3
	sra fp, sp, t0
	or a0, a2, a5
	and t0, s2, a4

	add a0, zero, zero

	jal ra, 1f
1:
	jalr ra, 4(a0)

	beq zero, zero, 2f
	bne a0, a1, 2f
	blt a2, a3, 2f
	bge s3, s3, 2f
	bltu ra, fp, 2f
	bgeu s6, t0, 2f
2:

	lb a0, 0(zero)
	lh a0, 0(zero)
	lw a0, 0(zero)
	lbu a0, 0(zero)
	lhu a0, 0(zero)

	sb a0, 0(zero)
	sh a0, 0(zero)
	sw a0, 0(zero)

	fence

	ebreak

	add a0, zero, 0
	ecall

	mul a0, a1, a2
