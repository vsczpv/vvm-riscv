#ifndef R32I_MISC_H_
#define R32I_MISC_H_

#define rv32i_getinst(cpu, index)       \
	((cpu)->ram.buf[index+0] << 0)  |  \
	((cpu)->ram.buf[index+1] << 8)  |  \
	((cpu)->ram.buf[index+2] << 16) |  \
	((cpu)->ram.buf[index+3] << 24)

// TODO: Check later; I changed some stuff with instruction loading and this macro is probably borked
#define rv32i_setinst(cpu, index, val)             \
	(cpu)->ram.buf[index+0] = (val >> 8)  & 0xff; \
	(cpu)->ram.buf[index+1] = (val >> 0)  & 0xff; \
	(cpu)->ram.buf[index+2] = (val >> 24) & 0xff; \
	(cpu)->ram.buf[index+3] = (val >> 16) & 0xff;

#define rv32i_getword(cpu, index)      \
	((cpu)->ram.buf[index+0] << 24) | \
	((cpu)->ram.buf[index+1] << 16) | \
	((cpu)->ram.buf[index+2] << 8)  | \
	((cpu)->ram.buf[index+3] << 0)

#define rv32i_gethalf(cpu, index)     \
	((cpu)->ram.buf[index+0] << 8) | \
	((cpu)->ram.buf[index+1] << 0)

#define rv32i_getbyte(cpu, index) (cpu)->ram.buf[index]

#define rv32i_setword(cpu, index, val)             \
	(cpu)->ram.buf[index+0] = (val >> 24) & 0xff; \
	(cpu)->ram.buf[index+1] = (val >> 16) & 0xff; \
	(cpu)->ram.buf[index+2] = (val >> 8)  & 0xff; \
	(cpu)->ram.buf[index+3] = (val >> 0)  & 0xff;

#define rv32i_sethalf(cpu, index, val)             \
	(cpu)->ram.buf[index+0] = (val >> 8)  & 0xff; \
	(cpu)->ram.buf[index+1] = (val >> 0)  & 0xff;

#define rv32i_setbyte(cpu, index, val) (cpu)->ram.buf[index] = val & 0xff;

#endif /* R32I_EMU_H_ */
