#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

int main(){
    
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1){
        perror("socket");
        return -1;
    }

    //de tranh loi bind: Address already in use
    //khi set thuoc tinh SO_REUSEADDR cho socket, neu socket gap port/dia chi ip dang TIME_WAIT, kernel cho phep socket bind port/dia chi nay
    int opt = 1;
    if(setsockopt((listen_fd), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
        perror("setsockopt");
        close(listen_fd);
        return -1;
    }
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Lang nghe tren tat ca interfaces: lan, wifi, loopback address
    server_addr.sin_port = htons(8080);

    if(bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){  //Gan dia chi ip+port cho socket
        perror("bind failed");
        close(listen_fd);
        return -1;
    }
    if(listen(listen_fd, 5) == -1){  //cho phep 5 ket noi cho
        perror("listen");
        close(listen_fd);
        return -1;
    }
	printf("Time server listening on port 8080\n");
	
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);   
    
    while(1){
        int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if(client_fd == -1){
            perror("accept failed");
            continue;
        }

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str));
        int port = ntohs(client_addr.sin_port);
        printf("Client connected from %s:%d\n", ip_str, port);
        
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *tm_info = localtime(&tv.tv_sec);
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

        ssize_t bytes_sent = send(client_fd, buffer, strlen(buffer), 0);
        if(bytes_sent == -1){
            perror("send");
        }
        close(client_fd);

    }
    
    close(listen_fd);
    wait(NULL);
    return 0;
}
