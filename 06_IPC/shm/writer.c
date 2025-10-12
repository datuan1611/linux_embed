#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv)
{
	int shm_fd;		//shared memory file descriptor
	const char* message_0 = "Hello World\n";	
	void *ptr;

	shm_fd = shm_open("hello_class", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, 4096);
	ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	strcpy(ptr, message_0);
	munmap(ptr, 4096);

	return EXIT_SUCCESS;
}