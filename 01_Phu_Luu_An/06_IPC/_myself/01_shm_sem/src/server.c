#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include "../inc/common.h"

int main(int argc, char **argv)
{
	//Start SHM
	int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SHM_SIZE);
	void *shm_ptr = mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

	//Start SEM
	sem_t *sem_server = sem_open(SEM_SERVER, O_CREAT, 0666, 0);
	sem_t *sem_client = sem_open(SEM_CLIENT, O_CREAT, 0666, 0);

	for (int i = 0; i < 3; i++) {
		printf("server: start wait session\n");
		sem_wait(sem_server);	//wait session
		printf("server: end  wait session\n");

		//TODO
		printf("CLIENT: %s\n", (char*)shm_ptr);
		char sc_msg[SHM_SIZE];
		printf("SERVER >>> ");
		fgets(sc_msg, sizeof(sc_msg), stdin);
		strncpy(shm_ptr, sc_msg, SHM_SIZE);

		printf("server: start signal client\n");
		sem_post(sem_client);	//signal client
		printf("server: end signal client\n");
	}

	//End SEM
	sem_close(sem_server);
	sem_close(sem_client);

	//End SHM
	munmap(shm_ptr, SHM_SIZE);
	close(shm_fd);

	return EXIT_SUCCESS;
}
