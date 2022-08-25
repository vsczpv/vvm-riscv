#include <ministd.h>

int main(void)
{

	putstring("Hello from address 0x8000!\n");

	putstring("In this program, address 0x0 is no longer an valid executable address.\n");

	putstring("Testing complicated functions... ");

	putnumber(7 % 5);

	putstring(". [should be 2].\n");

	return 0;
}
