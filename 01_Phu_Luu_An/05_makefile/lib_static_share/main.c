#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "inc/hello_A.h"
#include "inc/hello_B.h"

int main(int argc, char** argv)
{
	printf("---------main.c---------\n");
	HELLO_A;
	HELLO_B;
	printf("---------main.c---------\n");
	return EXIT_SUCCESS;
}
