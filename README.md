# VVM-RISCV
---
RISC-V emulator for RV32I architecture.

Nearly up to Unprivileged-20191213 specification.

# Compiling
---
To compile run:

`make all`

To execute run:

`make run`

Or alternatively:

`./vvm-riscv sample/sample.bin`

# Stub ECALL
---

A stub ecall is implemented so that emulated programs can be interacted with.

Three calls are present, that are selected with the a0 register, theirs arguments being passed in a1 and a2 respectively:

* 0: Exit(). Ends the emulation
* 1: Write(void* buf, size_t n). Writes buffer to stdout up to n bytes
* 2: Read(void* dest, size_t max). Reads from stdin to dest up to max bytes. Excess is discarded. Amount read is returned in a0.

# Nuances
---

Invalid or missing instructions are treated as emulation errors as without the privileged spec being implemented it's impossible to catch them.

Programs are to be loaded on address 0, and with a default of 16KiB of memory.
Out of bounds access stops the emulation. Memory can be increased.

Invalid or unimplemented instructions are treated as "Unknown" by the backtrace dissasembler.

Programs are to be loaded as flat binaries. Assuming the GNU toolchain, you can do:

`riscv32-elf-as -R -march=rv32i FILE.S -o FILE.o`

`riscv32-elf-ld FILE.o FILE.ld -o FILE.out`

`riscv32-elf-objcopy --dump-section .text=FILE.bin FILE.out`

Linker script is used to relocate everything to address 0, example is provided inside sample/.

# Full usage
---


    Usage: vvm-riscv [-m <ram>] [-h|--help] [--debug] [--noop-stub] [--version] FILENAME

           -m <ram>        Specify amount of ram in kibibytes (1024).
                           Value cannot be 0.
                           Default value is 16KiB.

           -h, --help      Show this prompt.
		   --debug         Step-by-step debbuger
           --noop-stub     Treat ecall stub as noop.
           --version       Show version and license.

        FILENAME must be a path to a raw RISCV RV32I flat binary.


# Supported Instructions
---

* LUI   → Fully implemented
* AUIPC → Fully implemented
* JAL   → Fully implemented
* JALR  → Fully implemented
* BEQ   → Fully implemented
* BNE   → Fully implemented
* BLT   → Fully implemented
* BGE   → Fully implemented
* BLTU  → Fully implemented
* BGEU  → Fully implemented
* LB    → Fully implemented
* LH    → Fully implemented
* LW    → Fully implemented
* LBU   → Fully implemented
* LHU   → Fully implemented
* SB    → Fully implemented
* SH    → Fully implemented
* SW    → Fully implemented
* ADDI  → Fully implemented
* SLTI  → Fully implemented
* SLTIU → Fully implemented
* XORI  → Fully implemented
* ORI   → Fully implemented
* ANDI  → Fully implemented
* SLLI  → Fully implemented
* SRLI  → Fully implemented
* SRAI  → Fully implemented
* ADD   → Fully implemented
* SUB   → Fully implemented
* SLL   → Fully implemented
* SLT   → Fully implemented
* SLTU  → Fully implemented
* XOR   → Fully implemented
* SRL   → Fully implemented
* SRA   → Fully implemented
* OR → Fully implemented
* AND → Fully implemented
* FENCE → No-op
* ECALL → Stub system call
* EBREAK → Pause and print backtrace

# Contact
---

Written by Vinícius Schütz Piva <vinicius.vsczpv@outlook.com>.

Started on 16/01/2022, last updated 15/03/2022.

# License
---

vvm-riscv is licensed under GNU GPL-3-or-later;

Full license is available in COPYING file, or alternitively at <https://www.gnu.org/licenses/>.
