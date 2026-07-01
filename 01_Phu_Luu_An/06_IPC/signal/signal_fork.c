/*
PROGRAM:
.using fork() to create child process
.parent process: ask user fill result of formular [3 x 5 = ???]
.child process:	if user still not answer after 5 seconds,
				send signal to parent process to show HINT
.HINT is only showed 1 time.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int b_hint = 0;

void handle_sigusr1(int signum)
{
	//TODO
	if (b_hint == 0) {
		printf("\n(HINT) Remember that multiplication is repetitive addition!\n");
	}

	return;
}

int main(int argc, char **argv)
{
	int pid = fork();
	if (pid == -1) {
		perror("fork() failed!\n");
		return EXIT_FAILURE;
	}
	
	if (pid == 0) {
		//Child process
		printf("\nChild[pid:%d]: my parent has pid[%d]\n", getpid(), getppid());
		printf("Sleep 5 seconds.\n");
		sleep(5);
		kill(getppid(), SIGUSR1);
				//getpid() : get PID of current process
				//getppid(): get PID of parent process
	} else {
		struct sigaction act = {0};
		act.sa_flags = SA_RESTART;
		act.sa_handler = &handle_sigusr1;

		if (sigaction(SIGUSR1, &act, NULL) < 0) {
			perror("sigaction() SIGUSR1 failed!\n");
			return EXIT_FAILURE;
		}

		//Parent process
		int x;
		printf("Parent[pid:%d]: my child has pid[%d]\n", getpid(), pid);
		printf("3 x 5 = ");
		scanf("%d", &x);
		if (x == 15) {
			printf("Right!\n");
		} else {
			printf("Wrong!\n");
		}
		wait(NULL);
	}
	

	return EXIT_SUCCESS;
}