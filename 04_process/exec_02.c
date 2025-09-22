
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int pid = -1;

	for (int i = 0; i < 10; i++) {
		pid = fork();
		if (pid == 0) {
			execl("./child_exe","xin chao",NULL);
		}
		
	}

	for (int j = 0; j < 10; j++) {
		wait(NULL);		//prevent process children become zombie status
	}
	while (1);

	return EXIT_SUCCESS;
}