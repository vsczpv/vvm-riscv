# VVM-RISCV
---

vvm-riscv is a minimal RISC-V instruction-set-simulator oriented for educational and recreational purposes. It exclusively runs flat binaries and implements the Unprivileged-20191213 specification.

# Dependencies
---

The only compile time dependency is a C23 compiler.
Tested compilers are: GCC 12.2.0, Clang 14.0.6.

NOTE: cproc is [currently uncompilable due to a bug](https://todo.sr.ht/~mcf/cproc/83), and compatibility with it will be tested in the next release.

To compile the examples, you need the GNU RISCV64 toolchain (or compatible).
More specifically you want the `riscv64-elf-*` series of binaries.

# Compiling and Running
---
To compile, run:

`make`

It is possible to compile with debug information:

`RV32I_DEBUG=1 make`

To execute the examples, run:

 * For the User-Input example: `make example_userinput`
 * For the Fibonacci example:  `make example_fibonacci`
 * For the Debugger example:   `make example_debugger`
 * For the memory-map example: `make example_buffer`
 * For the load-at example:    `make example_loadat`

Or alternatively, you can run invoke vvm-riscv manually:

`./vvm-riscv [filename]`

# Arbitrary memory maps
---
vvm-riscv supports non-contiguous memory regions, that can be specified with the `--memory-map <addr> <size>` command-line option.

Memory maps are aligned to 1KiB, and are sized in increments of 1KiB.

For example, to achieve the following memory layout:

```
0x0000                                              0xffff
┌───┬───────────┬─────────┬────┬───────────────┬─────────┐
│   |           |         |    |               |         │
│ - |   Map1    |    -    | M2 |     Map3      |    -    │
│   |           |         |    |               |         │
└───┴───────────┴─────────┴────┴───────────────┴─────────┘
```

|      | **Address** | **Size** |
|------|-------------|----------|
| Map1 | 0x1000      | 16KiB    |
| Map2 | 0x8000      | 4KiB     |
| Map3 | 0x9000      | 24KiB    |

You'd do the following invocation:

```
$ vvm-riscv --memory-map 0x1000 16 \
            --memory-map 0x8000 4  \
            --memory-map 0x9000 24 \
            --load-at    0x1000    \
            program.bin
```

NOTE: `--load-at` is not necessary to use `--memory-map`, and is there for cosmetical reasons.

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

By default, programs are loaded at address 0, with 16KiB of memory.
Out of bounds access stops the emulation.

Invalid or unimplemented instructions are treated as "Unknown" by the backtrace dissasembler.

As cited, programs are to be loaded as flat binaries. Assuming the GNU toolchain, you can do:

`riscv64-elf-as -R -march=rv32i -mabi=ilp32 FILE.S -o FILE.o`

`riscv64-elf-ld FILE.o -T FILE.ld -o FILE.out --oformat=elf32-littleriscv`

`riscv64-elf-objcopy --dump-section .text=FILE.bin FILE.out`

The linker script is used to relocate everything to the target execution address; some linker script examples are provided inside `samples/`.

# Full usage
---

```

Usage: vvm-riscv [-m <ram>|--memory-map <addr> <size>] [--show-map] [-h|--help]
                 [--debug] [--noop-stub] [--version] FILENAME

       -m <ram>                    Specify amount of ram in kibibytes (1024).
                                   Value cannot be 0.
                                   Default value is 16KiB.

       --memory-map <addr> <size>  Specify a new memory map at <addr> with <size> kibibytes.
                                   Size cannot be 0.
                                   Address has to be aligned to 1KiB boundaries.
                                   Cannot be used with -m.

       --show-map                  Display current memory mappings and quit.

       --load-at <addr>            Load and run program at specified address.

       -h, --help                  Show this prompt.
       --debug                     Step-by-step debbuger
       --noop-stub                 Treat ecall stub as noop.
       --version                   Show version and license.

    FILENAME must be a path to a raw RISCV RV32I flat binary.
    Numbers can be any positive value readable by strtol(3) [0x1234, 1234, 01234].


```

# Supported Instructions
---

| Inst   | Status                    |
|--------|---------------------------|
| LUI    | Fully implemented         |
| AUIPC  | Fully implemented         |
| JAL    | Fully implemented         |
| JALR   | Fully implemented         |
| BEQ    | Fully implemented         |
| BNE    | Fully implemented         |
| BLT    | Fully implemented         |
| BGE    | Fully implemented         |
| BLTU   | Fully implemented         |
| BGEU   | Fully implemented         |
| LB     | Fully implemented         |
| LH     | Fully implemented         |
| LW     | Fully implemented         |
| LBU    | Fully implemented         |
| LHU    | Fully implemented         |
| SB     | Fully implemented         |
| SH     | Fully implemented         |
| SW     | Fully implemented         |
| ADDI   | Fully implemented         |
| SLTI   | Fully implemented         |
| SLTIU  | Fully implemented         |
| XORI   | Fully implemented         |
| ORI    | Fully implemented         |
| ANDI   | Fully implemented         |
| SLLI   | Fully implemented         |
| SRLI   | Fully implemented         |
| SRAI   | Fully implemented         |
| ADD    | Fully implemented         |
| SUB    | Fully implemented         |
| SLL    | Fully implemented         |
| SLT    | Fully implemented         |
| SLTU   | Fully implemented         |
| XOR    | Fully implemented         |
| SRL    | Fully implemented         |
| SRA    | Fully implemented         |
| OR     | Fully implemented         |
| AND    | Fully implemented         |
| FENCE  | No-op                     |
| ECALL  | Stub system call          |
| EBREAK | Pause and print backtrace |

# Contact
---

Written by Vinícius Schütz Piva <vinicius.vsczpv@outlook.com>.

Started on 16/01/2022, last updated 24/01/2023.

# License
---

vvm-riscv is licensed under GNU GPL-3-or-later;

Full license is available in COPYING file, or alternitively at <https://www.gnu.org/licenses/>.
