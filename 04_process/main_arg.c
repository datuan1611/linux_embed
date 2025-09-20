#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

func_a()
{
	exit(0);
}

int main(int argc, char *argv[])
{
	func_a();

	for (int i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s\n",i,argv[i]);
	}
	
	return EXIT_SUCCESS;
}