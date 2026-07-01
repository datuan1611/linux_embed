#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	execl("./child_exe","xin chao",NULL);
	printf("I am parent\n");
	return EXIT_SUCCESS;
}