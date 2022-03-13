#ifndef RV32I_EMU_H_
#define RV32I_EMU_H_

#include <stdint.h>
#include <stdbool.h>

#define IOMAP_HARDCAP 32
#define KiB sizeof(uint8_t)*1024

struct rv32i_hart_s;

typedef struct rv32i_iomap_s
{
	uint32_t start_addr;
	struct { uint8_t* buf; uint32_t size; } map;
	void (*callback)(struct rv32i_hart_s* cpu);
} rv32i_iomap_s;

typedef struct rv32i_hart_s
{
	uint32_t iomap_amnt;
	rv32i_iomap_s* iomaps[IOMAP_HARDCAP];
	struct { uint8_t* buf; uint32_t size; } ram;
	int32_t regs[32];
	uint32_t pc;
} rv32i_hart_s;

bool rv32i_iomap_init(rv32i_hart_s* cpu, uint32_t addr, uint32_t size, void (*callback)(rv32i_hart_s* cpu));

rv32i_hart_s rv32i_hart_init(uint32_t total_ram);
void rv32i_hart_destroy(rv32i_hart_s cpu);

void rv32i_hart_execute(rv32i_hart_s* cpu);

#endif /* RV32I_EMU_H_ */
