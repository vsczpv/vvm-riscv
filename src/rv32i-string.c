#include <string.h>
#include <stdlib.h>
#include "rv32i-string.h"

#include <stdio.h>

rv32i_counted_string_s rv32i_cnstring_init(void)
{

	rv32i_counted_string_s str;

	str.string = NULL;
	str.count  = 0;

	return str;
}

void rv32i_cnstring_delete(rv32i_counted_string_s* str)
{

	free(str->string);

	return;
}

void rv32i_cnstring_append_rcnstr(rv32i_counted_string_s* a, const char* b, size_t bn)
{

	int amnt = a->count + sizeof(char) * bn;

	fprintf(stderr, "A: %i\n", amnt);

	if (amnt == 0) return;

	a->string = realloc(a->string, amnt);
	memcpy(a->string + a->count, b, bn);
	a->count += bn;

	return;
}
