#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

long int count1 = 0;
long int count2 = 0;
pthread_mutex_t count_lock1;
pthread_mutex_t count_lock2;

void *first_thread(void *arg)
{
	for (long int i = 0; i < 5000; i++)
	{
		pthread_mutex_lock(&count_lock1);	//thread_1 lock1
		pthread_mutex_lock(&count_lock2);	//thread_1 wait lock2
		count1++;
		count2++;
		pthread_mutex_unlock(&count_lock1);
		pthread_mutex_unlock(&count_lock2);
	}

	return NULL;
}

void *second_thread(void *arg)
{
	for (long int i = 5000; i < 10000; i++)
	{
		pthread_mutex_lock(&count_lock2);	//thread_2 lock2
		pthread_mutex_lock(&count_lock1);	//thread_2 wait lock1
		count1++;
		count2++;
		pthread_mutex_unlock(&count_lock2);
		pthread_mutex_unlock(&count_lock1);
	}

	return NULL;
}

int main(int argc, char argv[])
{
	pthread_t thread_id1;
	pthread_t thread_id2;

	pthread_mutex_init(&count_lock1,NULL);
	pthread_mutex_init(&count_lock2,NULL);

	pthread_create(&thread_id1,NULL,first_thread,NULL);
	pthread_create(&thread_id2,NULL,second_thread,NULL);
	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);

	pthread_mutex_destroy(&count_lock1);
	pthread_mutex_destroy(&count_lock2);

	printf("count1 = %ld\tcount2 = %ld\n",count1,count2);

	return EXIT_SUCCESS;
}