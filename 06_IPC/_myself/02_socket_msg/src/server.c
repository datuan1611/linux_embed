//server.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "../inc/common.h"

int main(int argc, char **argv)
{
	int server_fd, client_fd, max_fd, activity;
	int client_sockets[MAX_CLIENTS] = {0};
	struct sockaddr_in client_addr;
	char buffer[BUFFER_SIZE];
	fd_set readfds;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		perror("socket server failed\n");
		return EXIT_FAILURE;
	}

	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(PORT);

	bind(server_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
	listen(server_fd, 3);

	printf("Server is listenning in port %d...\n", PORT);

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		max_fd = server_fd;

		for (int i = 0; i < MAX_CLIENTS; i++) {
			int sd = client_sockets[i];
			if (sd > 0) FD_SET(sd, &readfds);
			if (sd > max_fd) max_fd = sd;
		}

		activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

		if (FD_ISSET(server_fd, &readfds)) {
			size_t addrlen = sizeof(client_addr);
			client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
			printf("New connection: socket fd %d\n", client_fd);

			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (client_sockets[i] == 0) {
					client_sockets[i] = client_fd;
					break;
				}
			}
		}

		for (int i = 0; i < MAX_CLIENTS; i++) {
			int sd = client_sockets[i];
			if (FD_ISSET(sd, &readfds)) {
				int valread = read(sd, buffer, BUFFER_SIZE);
				if (valread == 0) {
					close(sd);
					client_sockets[i] = 0;
					printf("Client disconnect: socket fd %d\n", sd);
				} else {
					buffer[valread] = '\0';
					printf("Received from client %d: %s\n", sd, buffer);

					//Broadcast to other clients
					for (int j = 0; j < MAX_CLIENTS; j++) {
						if ((client_sockets[j] != 0) && (j != i)) {
							send(client_sockets[j], buffer, strlen(buffer), 0);
						}
					}
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
