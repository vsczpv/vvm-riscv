#include <stdint.h>
#include <stdbool.h>

#include "rv32i-emu.h"
#include "rv32i-misc.h"

bool rv32i_oob_addr(rv32i_hart_s* cpu, uint32_t addr)
{

	if (addr > cpu->ram.size) return true;
	else return false;

}
