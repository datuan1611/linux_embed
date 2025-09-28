#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv)
{
	sigset_t my_sig_set;
	sigemptyset(&my_sig_set);

	sigaddset(&my_sig_set, SIGINT);
	sigprocmask(SIG_BLOCK, &my_sig_set, NULL);

	while(1);

	return EXIT_SUCCESS;
}