#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int is_printf = 0;

void *get_user_input(void *arg)
{
	printf("Do you want to print the result? 1-Yes, 0-No\n");
	scanf("%d",&is_printf);

	return NULL;
}

int main(int argc, char argv[])
{
	int i = 0;
	int n = 1000;
	int sum = 0;

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, get_user_input, NULL);

	for (i = 0; i < n; ++i) {
		sum = sum + i;
	}

	pthread_join(thread_id, NULL);
	if (is_printf) {
		printf("sum = %d\n", sum);
	}

	return EXIT_SUCCESS;
}