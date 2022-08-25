#include <ministd.h>

size_t strlen(char* str)
{
	int len = -1;
	while (str[++len]);
	return (unsigned) len;
}

void putstring(char* str)
{

	write(str, strlen(str));

	return;
}

void putnumber(int n)
{

	/*
	 * BUG: Not declaring this as static makes elements of the array swap in order
	 * I'm yet to formulate if this is usererror by my part or some bug in the toolchain
	 */

	static char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	char result[33] = { [32] = '\0' };
	int index = 31;

	if (n == 0) { putstring("0"); return; }

	while (n)
	{

		int dig = n % 10;
		n /= 10;

		result[index--] = digits[dig];
	}

	putstring(&result[index+1]);

	return;
}

long int power(int base, short exp)
{

	long int product = 1;
	for (int i = 0; i < exp; i++)
		product *= base;

	return product;

}
