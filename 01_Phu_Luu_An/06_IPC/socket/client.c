#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
	char recv_buffer[1024];
	time_t ticks;

	memset(&server_addr, 0, sizeof(server_addr));
	memset(recv_buffer, 0, sizeof(recv_buffer));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5000);

	if (connect(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
		read(sockfd, recv_buffer, sizeof(recv_buffer) - 1);
		printf("\n%s\n", recv_buffer);
		close(sockfd);
	}

	return EXIT_SUCCESS;
}