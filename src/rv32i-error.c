#include <stdarg.h>
#include "rv32i-error.h"

void rv32i_error_fatal_fprintf(FILE* stream, const char* restrict fmt, ...)
{
	va_list va;
	va_start(va, fmt);

	rv32i_destroy_ncurses(NULL);

	vfprintf(stream, fmt, va);

	va_end(va);
}
