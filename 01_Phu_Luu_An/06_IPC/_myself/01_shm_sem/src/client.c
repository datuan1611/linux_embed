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
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
	void *shm_ptr = mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

	//Start SEM
	sem_t *sem_server = sem_open(SEM_SERVER, 0);
	sem_t *sem_client = sem_open(SEM_CLIENT, 0);

	sem_post(sem_server); 	//request server

	for (int i = 0; i < 3; i++) {
		printf("client: start wait\n");
		sem_wait(sem_client);	//wait session
		printf("client: end wait\n");

		//TODO
		printf("SERVER: %s\n", (char*)shm_ptr);
		char sc_msg[SHM_SIZE];
		printf("CLIENT >>> ");
		fgets(sc_msg, sizeof(sc_msg), stdin);
		strncpy(shm_ptr, sc_msg, SHM_SIZE);

		printf("client: start post\n");
		sem_post(sem_server);	//signal server
		printf("client: end post\n");
	}

	//End SEM
	sem_close(sem_server);
	sem_close(sem_client);

	//End SHM
	munmap(shm_ptr, SHM_SIZE);
	close(shm_fd);

	//Cleanup
	shm_unlink(SHM_NAME);

	return EXIT_SUCCESS;
}
