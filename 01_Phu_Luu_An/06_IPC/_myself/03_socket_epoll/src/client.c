//client.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../inc/common.h"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	//Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket failed\n");
		return EXIT_FAILURE;
	}
	
	//Connnect to server
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);

	connect(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	printf("Connect socket %d to server.\n", sockfd);

	//Send message to server
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		send(sockfd, buffer, strlen(buffer), 0);
	}

	close(sockfd);
	return EXIT_SUCCESS;
}
