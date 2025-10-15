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
	int client_fd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	fd_set readfds;

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);

	connect(client_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	printf("Connect to server.\n");

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(client_fd, &readfds);
		FD_SET(STDIN_FILENO, &readfds);

		select(client_fd + 1, &readfds, NULL, NULL, NULL);

		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			fgets(buffer, BUFFER_SIZE, stdin);
			send(client_fd, buffer, strlen(buffer), 0);
		}

		if (FD_ISSET(client_fd, &readfds)) {
			int valread = read(client_fd, buffer, BUFFER_SIZE);
			if (valread == 0) {
				printf("Server disconnected.\n");
				break;
			} else {
				buffer[valread] = '\0';
				printf("Receive: %s\n", buffer);
			}
		}
	}

	close(client_fd);
	return EXIT_SUCCESS;
}
