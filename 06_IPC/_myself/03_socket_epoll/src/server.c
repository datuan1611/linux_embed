//server.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "../inc/common.h"

void set_nonblocking(int sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char **argv)
{
	int listen_fd, conn_fd, epoll_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buffer[BUFFER_SIZE];

	//Create socket to listen
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0) {
		perror("socket failed\n");
		return EXIT_FAILURE;
	}

	//Bind address and listen
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(listen_fd, 10);

	//Create epoll
	epoll_fd = epoll_create1(0);	//0: not using special flag, such as EPOLL_CLOEXEC
	struct epoll_event ev, events[MAX_EVENTS];
	ev.events = EPOLLIN;		//EPOLLIN: observe event of reading data
	ev.data.fd = listen_fd;		//assign observed fd is listen_fd
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

	printf("Server is listenning in socket %d port %d...\n", listen_fd, PORT);
	

	while (1) {
		int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);	//-1: wait forever
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == listen_fd) {
				//if event to socket listen_fd >> new connection from client
				conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);
				if (conn_fd < 0) {
					perror("accept failed\n");
					continue;
				}
				set_nonblocking(conn_fd);
				ev.events = EPOLLIN;
				ev.data.fd = conn_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);
				printf("New client connected: %d\n", conn_fd);
			} else {
				//if event to socket client >> have data to read
				int client_fd = events[i].data.fd;
				while (1) {
					int byteRead = read(client_fd, buffer, BUFFER_SIZE);
					if (byteRead == -1) {
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
							//Read done data
							break;
						} else {
							perror("read failed\n");
							close(client_fd);
							epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
						}
					} else if (byteRead == 0) {
						//Client disconnected
						close(client_fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
						printf("Client disconnected: %d\n", client_fd);
						break;
					} else {
						//Continue receiving data normally
						buffer[byteRead] = '\0';
						printf("Received from client %d: %s\n", client_fd, buffer);
					}
				}
			}
		}
	}

	close(listen_fd);
	return EXIT_SUCCESS;
}
