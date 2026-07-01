#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int b_exit = 0;

void sig_handler(int signum)
{
	if (SIGUSR2 == signum) {	//signum:12
		printf("Handler for SIGUSR2.\n");
		b_exit = 1;
	}
}

int main(int argc, char **argv)
{
	signal(SIGUSR2, sig_handler);
	while(1)
	{
		//TODO
		//gcc -g hello_signal.c -o hellosignal_exe
		//ps -aux | grep hellosignal_exe
		//kill -SIGUSR2 7769
		//Ctrl+C	SIFINT
		//Ctrl+Z	SIGSTOP

		if (b_exit == 1)
		{
			printf("Break out of while loop.\n");
			break;
		}
		
	}
	return EXIT_SUCCESS;
}