#ifndef RV32I_ERROR_H_
#define RV32I_ERROR_H_

#define rv32i_error_inval(inst_name, subinst, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Invalid instruction %s%x: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, subinst, inst, pc \
)

#define rv32i_error_inval2(inst_name, subinst7, subinst, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Invalid instruction %s%x:%x: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, subinst7, subinst, inst, pc \
)

#define rv32i_error_inval_nosubtype(inst_name, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Invalid instruction %s: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, inst, pc \
)

#define rv32i_error_unsupported_inst_fmt(inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Unsupported instruction format. Instruction \033[1m0x%08x\033[0m, program counter \033[1m0x%08x\033[0m.\n",\
	__FILE__, __LINE__, __func__, inst, pc \
)

#define rv32i_error_inst_noimpl(inst_name, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Opcode %snot implemented: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, inst, pc \
)

#define rv32i_error_mis_jump(inst_name, immd, inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Misaligned %sjump: \033[1m0x%08x\033[0m. Instruction: \033[1m0x%08x\033[0m, program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst_name, immd, inst, pc \
)

#define rv32i_error_oob(op_name, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Out of bounds %s: \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, op_name, pc \
)

#define rv32i_error_nofile(filename) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"No such file '%s'.\n", \
	__FILE__, __LINE__, __func__, filename \
)

#define rv32i_error_aintfile(filename) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"'%s' is not a file.\n", \
	__FILE__, __LINE__, __func__, filename \
)

/* """"Malformed"""" instructions means I have no idea what bug causes them falling through everything else */
#define rv32i_error_malinst(inst, pc) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Malformed instruction: \033[1m0x%08x\033[0m. Program counter \033[1m0x%08x\033[0m.\n", \
	__FILE__, __LINE__, __func__, inst, pc \
)

#define rv32i_error_oom(need, have) fprintf \
( \
	stderr, \
	"\033[1;39m%s:%i: \033[31merror: \033[0mIn function \033[1m'%s'\033[0m: " \
	"Not enough memory to run program: Need %lu bytes, have %u.\n", \
	__FILE__, __LINE__, __func__, need, have \
)

#define rv32i_error_backtrace(cpu) fprintf \
( \
	stderr, \
	"\033[1m" \
	"Backtrace: \033[1;33mra=%08x  sp=%08x  gp=%08x  tp=%08x\n\033[0m" \
	"           \033[1ma0=%08x  a1=%08x  a2=%08x  a3=%08x\n" \
	"           a4=%08x  a5=%08x  a7=%08x  a7=%08x\n" \
	"           \033[31mfp=%08x  \033[34ms1=%08x  s2=%08x  s3=%08x\n" \
	"           s4=%08x  s5=%08x  s6=%08x  s7=%08x\n" \
	"           s8=%08x  s9=%08x s10=%08x s11=%08x\n\033[36m" \
	"           t0=%08x  t1=%08x  t2=%08x  t3=%08x\n" \
	"           t4=%08x  t5=%08x  t6=%08x  \033[33mpc=%08x\033[0m\n\n", \
	(cpu).regs[1], (cpu).regs[2], (cpu).regs[3], (cpu).regs[4], \
	(cpu).regs[10], (cpu).regs[11], \
	(cpu).regs[12], (cpu).regs[13], (cpu).regs[14], (cpu).regs[15], \
	(cpu).regs[16], (cpu).regs[17], 	(cpu).regs[8], (cpu).regs[9], (cpu).regs[18], (cpu).regs[19], \
	(cpu).regs[20], (cpu).regs[21], (cpu).regs[22], (cpu).regs[23], \
	(cpu).regs[24], (cpu).regs[25], (cpu).regs[26], (cpu).regs[27], \
	(cpu).regs[5], (cpu).regs[6], (cpu).regs[7], (cpu).regs[28], \
	(cpu).regs[29], (cpu).regs[30], (cpu).regs[31], \
	(cpu).pc \
)

#endif // RV32I_ERROR_H_
