#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern char **environ;

int main(int argc, char *argv[])
{
	for (int i = 0; environ[i] != NULL; i++)
	{
		printf("environ[%d] = %s\n", i, environ[i]);
	}

	FILE *fp = NULL;
	const char path[] = "/home/jk/Desktop/04_process/data";

	fp = fopen(path,"a");
	if (NULL == fp) {
		printf("Faid to open file! errno = %d\n", errno);
		return EXIT_FAILURE;
	} else {
		printf("Open file successfully!\n");
		return EXIT_SUCCESS;
	}
}