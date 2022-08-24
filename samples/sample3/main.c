#include <ministd.h>

#define KiB    sizeof(char) * 1024

int main(void)
{

	/* BUG:
	 *
	 * Declaring these as globals makes GCC invert their endianess (???)
	 *
         */

	char* funny_buffer_1 = (char*) 0x123456;
	char* funny_buffer_2 = (char*) 0xdeadbeef;

	for (int i = 0; i < 1*KiB; i++)
		funny_buffer_1[i] = (char) ( (i % 20) + 'A' );

	int j = 0; for (int i = 0; i < 2*KiB; i+=2)
	{
		funny_buffer_2[i]   = funny_buffer_1[j]   + 0x20;
		funny_buffer_2[i+1] = funny_buffer_1[j++] + 0x20;
	}

	putstring("Buffer @ 0x123456:\n");
	write(funny_buffer_1, 1*KiB);

	putstring("\n\nBuffer @ 0xdeadbeef:\n");
	write(funny_buffer_2, 2*KiB);

	putstring("\n\n");

	return 0;
}
