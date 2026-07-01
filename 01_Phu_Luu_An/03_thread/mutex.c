#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

long int count = 0;
pthread_mutex_t count_lock;

void *first_thread(void *arg)
{
	for (long int i = 0; i < 5000; i++)
	{
		pthread_mutex_lock(&count_lock);
		count++;
		pthread_mutex_unlock(&count_lock);
	}

	return NULL;
}

void *second_thread(void *arg)
{
	for (long int i = 5000; i < 10000; i++)
	{
		pthread_mutex_lock(&count_lock);
		count++;
		pthread_mutex_unlock(&count_lock);
	}

	return NULL;
}

int main(int argc, char argv[])
{
	pthread_t thread_id1;
	pthread_t thread_id2;

	pthread_mutex_init(&count_lock,NULL);

	pthread_create(&thread_id1,NULL,first_thread,NULL);
	pthread_create(&thread_id2,NULL,second_thread,NULL);
	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);

	pthread_mutex_destroy(&count_lock);

	printf("count = %ld\n",count);

	return EXIT_SUCCESS;
}