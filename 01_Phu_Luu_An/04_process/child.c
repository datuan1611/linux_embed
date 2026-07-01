#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("I am child >> %s\n",argv[0]);
	return EXIT_SUCCESS;
}