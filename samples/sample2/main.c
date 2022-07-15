#include <ministd.h>

int main(void)
{

	long int lim = power(2, 5);

	putstring("Hello, World, for I here present you fibbonaci:\n");

	int x = 1, y = 0, z = 0;

	for (int i = 0; i < lim; i++)
	{

		putstring("Behold, ");
		putnumber(z);
		putstring("!\n");

		z = x + y;
		x = y;
		y = z;
	}

	return 0;
}
