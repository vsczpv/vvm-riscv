# VVM-RISCV
---

vvm-riscv is a minimal RISC-V instruction-set-simulator/emulator oriented for educational and recreational purposes. It exclusively runs flat binaries and implements the Unprivileged-20191213 specification.

# Dependencies
---

The only compile time dependency is a C23 compiler.
Tested compilers are: GCC 12.1.0, Clang 14.0.6 and cproc.

To compile the examples, you need the GNU RISCV64 toolchain (or compatible).
More specifically you want the `riscv64-elf-*` series of binaries.

# Compiling and Running
---
To compile, run:

`make`

To execute the examples, run:

 * For the User-Input example: `make example_userinput`
 * For the Fibonacci example:  `make example_fibonacci`
 * For the Debugger example:   `make example_debugger`

Or alternatively, you can run invoke vvm-riscv manually:

`./vvm-riscv [filename]`

# Stub ECALL
---

A couple of stub ecalls are implemented so that emulated programs can interact with the outside world.

Three calls are present, that are selected with the a0 register, theirs arguments being passed in a1 and a2 respectively:

* 0: `void exit()`. Ends the emulation
* 1: `void write(void* buf, size_t n)`. Writes buffer to stdout up to n bytes
* 2: `size_t read(void* dest, size_t max)`. Reads from stdin to dest up to max bytes. Excess is discarded. Amount read is returned in a0.

# Nuances
---

Invalid or missing instructions are treated as emulation errors since without the privileged spec being implemented it's impossible to catch them.

Programs are loaded at address 0, with a default of 16KiB of memory.
Out of bounds access stops the emulation.

Invalid or unimplemented instructions are treated as "Unknown" by the backtrace dissasembler.

As cited, programs are to be loaded as flat binaries. Assuming the GNU toolchain, you can do:

`riscv64-elf-as -R -march=rv32i -mabi=ilp32 FILE.S -o FILE.o`

`riscv64-elf-ld FILE.o -T FILE.ld -o FILE.out --oformat=elf32-littleriscv`

`riscv64-elf-objcopy --dump-section .text=FILE.bin FILE.out`

The linker script is used to relocate everything to address 0; some linker script examples are provided inside samples/.

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

Started on 16/01/2022, last updated 15/07/2022.

# License
---

vvm-riscv is licensed under GNU GPL-3-or-later;

Full license is available in COPYING file, or alternitively at <https://www.gnu.org/licenses/>.
