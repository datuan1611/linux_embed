#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "inc/hello_A.h"
#include "inc/hello_B.h"

int main(int argc, char *argv[])
{
	printf("I am in main.c\n");
	HELLO_A;
	HELLO_B;
	return EXIT_SUCCESS;
}