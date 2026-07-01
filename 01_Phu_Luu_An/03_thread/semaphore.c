/* Includes */
#include <stdio.h>		// Input/Output
#include <stdlib.h>		// General Utilities
#include <pthread.h>	// POSIX Threads
#include <semaphore.h>	// Semaphore
#include <errno.h>		// Errors

/* prototype for thread routine */
void handler(void *ptr);

/* global vars */
sem_t sem_counter;
long int counter;	// shared variable


int main(int argc, char argv[])
{
	int i[2];
	pthread_t thread_id1;
	pthread_t thread_id2;

	i[0] = 0;	// argument to threads
	i[1] = 1;

	// int sem_init(sem_t *sem, int pshared, unsigned int value)
	//		pshared	= 0: shared between threads of 1 process (local semaphore)
	//		value	= 1: init value for semaphore (binary semaphore)
	sem_init(&sem_counter,0,1);

	pthread_create(&thread_id1,NULL,(void *)&handler,(void *)&i[0]);
	pthread_create(&thread_id2,NULL,(void *)&handler,(void *)&i[1]);
	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);

	sem_destroy(&sem_counter);		// destroy semaphore

	printf("counter = %ld\n",counter);

	return EXIT_SUCCESS;
}

void handler(void *ptr)
{
	int x = *((int *)ptr);

	printf("Thread %d: Waiting to enter critical region\n",x);
	sem_wait(&sem_counter);		// down semaphore

	/* START CRITICAL REGION */
	printf("Thread %d: Now in critical region\n",x);
	printf("Thread %d: Old Counter Value: %ld\n",x,counter);
	printf("Thread %d: Increasing Counter...\n",x);
	counter++;
	printf("Thread %d: New Counter Value: %ld\n",x,counter);
	printf("Thread %d: Exit critical region\n",x);
	/* END CRITICAL REGION */

	sem_post(&sem_counter);		// up semaphore
	pthread_exit(0);	// exit thread
}