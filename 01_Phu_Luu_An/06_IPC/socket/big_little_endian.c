#include <stdio.h>

int main(int argc, char **argv)
{
	unsigned int us_var = 0x12345678;
	unsigned char *ptr = (unsigned char*)&us_var;

	printf("%X\n", us_var);
	for (int i = 0; i < 4; i++) {
		printf("[%02X]", ptr[i]);
	}

	printf("\n");
	return 0;
}
