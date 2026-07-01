#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main(){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd == -1){
        perror("socket");
        return -1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) != 1){
        fprintf(stderr, "Invalid IP address\n");
        close(client_fd);
        return -1;
    }

    if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("connect");
        close(client_fd);
        return -1;
    }

    char buffer[64] = {0};
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if(bytes_received > 0){
        buffer[bytes_received] = '\0';
        printf("Current time from server: %s\n", buffer);
    }else if(bytes_received == 0){
        printf("Connection closed by server\n");
    }else{
        perror("Recv");
    }

    close(client_fd);
    return 0;
}


