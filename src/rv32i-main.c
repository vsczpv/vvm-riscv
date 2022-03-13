#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "rv32i-emu.h"
#include "rv32i-misc.h"
#include "rv32i-macro.h"
#include "rv32i-error.h"

char* filename = NULL;
int ramamnt = 16;

void usage(void)
{
	puts
	(
		"\n"
		"Usage: vvm-riscv [-m <ram>] [-h|--help] FILENAME"
		"\n\n"
		"       -m <ram>        Specify amount of ram in kibibytes (1024). Value cannot be 0.\n"
		"                       Default value is 16KiB\n"
		"\n"
		"       -h, --help      Show this prompt."
		"\n\n"
		"    FILENAME must be a path to a raw RISCV RV32I flat binary."
		"\n"
	);

}

void parse_cmdline(int argc, char* argv[])
{

	bool ramset  = false;
	bool fileset = false;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if ( !strcmp(argv[i], "-h") && !strcmp(argv[i], "--help") ) { usage(); exit(EXIT_SUCCESS); }
			else if ( !strcmp(argv[i], "-m"))
			{
				if (ramset || i+1 >= argc) { usage(); exit(EXIT_FAILURE); }
				ramset = true;
				ramamnt = strtol(argv[++i], NULL, 10);
				if ((!ramamnt && errno == EINVAL) || !ramamnt) { usage(); exit(EXIT_FAILURE); }
			}
			else if (argv[i][0] == '-') { usage(); exit(EXIT_FAILURE); }
			else
			{
				if (fileset) { usage(); exit(EXIT_FAILURE); }
				filename = argv[i]; fileset = true;
			}
		}
	}
	else { usage(); exit(EXIT_FAILURE); }

	if (!fileset) { usage(); exit(EXIT_FAILURE); }
}

int main(int argc, char* argv[])
{
	parse_cmdline(argc, argv);

	struct posix_file_s { int fd; struct stat st; uint8_t* buf; } file = { open(filename, O_RDONLY) };
	if (file.fd < 0) { rv32i_error_nofile(filename); return EXIT_FAILURE; }

	fstat(file.fd, &file.st);
	file.buf = (uint8_t*) mmap(NULL, file.st.st_size, PROT_READ, MAP_PRIVATE, file.fd, 0);
	if (file.buf == MAP_FAILED) { rv32i_error_aintfile(filename); return EXIT_FAILURE; }

	rv32i_hart_s cpu = rv32i_hart_init(ramamnt*KiB);

	if (file.st.st_size > cpu.ram.size) { rv32i_error_oom(file.st.st_size, cpu.ram.size); return EXIT_FAILURE; };
	memcpy(cpu.ram.buf, file.buf, file.st.st_size);

	rv32i_hart_execute(&cpu);

	munmap(file.buf, file.st.st_size);

	rv32i_hart_destroy(cpu);

	return EXIT_SUCCESS;
}
