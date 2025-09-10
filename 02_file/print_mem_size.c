//open file /proc/meminfo
//printf capacity free of RAM

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main()
{
    const char path[] = "/proc/meminfo";
    const char keyword[] = "MemFree:";
    char buffer[BUFFER_SIZE];

    int fd = open(path, O_RDONLY);
    if( fd == -1 ) {
        perror("Can not open file.\n");
        return 1;
    }

    off_t position = 0;
    ssize_t bytesRead;
    int found = 0;

    while( (bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0 ) {
        for( int i = 0; i <= bytesRead - strlen(keyword); i++ ) {
            if( memcmp(&buffer[i], keyword, strlen(keyword)) == 0 ) {
                position += i;
                found = 1;
                break;
            }
        }
        if( found ) break;
        position += bytesRead;
    }

    char c = 0;
    if( found ) {
        lseek(fd, position, SEEK_SET);
        while( read(fd, &c, 1) == 1 && c != 10 )
            printf("%c", c);
        printf("\n");
    }

    close( fd );
    return 0;
}