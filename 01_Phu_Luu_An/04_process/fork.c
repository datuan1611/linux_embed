#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int pid = -1;

	pid = fork();
	if (pid == 0)
	{
		printf("I am child\n");
	}
	else
	{
		printf("I am parent, my child is %d\n", pid);
	}

	return EXIT_SUCCESS;
}