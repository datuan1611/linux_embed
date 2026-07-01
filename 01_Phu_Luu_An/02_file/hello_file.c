#include <stdio.h>
#include <fcntl.h>

void main()
{
	int fd = -1;
	const char path[] = "/home/johan/Desktop/02_file/data.txt";
    char c = 0;

	fd = open(path, O_RDONLY);
	if( fd < 0 ) {
		printf("Can not open file\n");
		return;
	}

	while( read(fd, &c, 1) == 1 )
		printf("%c", c);
	printf("\n");

	close(fd);
}