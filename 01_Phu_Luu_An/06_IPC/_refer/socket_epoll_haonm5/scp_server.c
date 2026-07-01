#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <linux/limits.h>
#include <time.h>   //for timestamps later
#include <openssl/sha.h>    //for SH256 hash
#include <openssl/rand.h>   //for random number generation

#define BACKLOG 5
#define BUFFER_LEN 4096
#define MAX_EVENTS  50
#define PORT 8080
#define SAFE_DIR "/home" 

//Hardcoded crentials (in real: load from file/env).
#define VALID_USER "admin"
#define VALID_PASS "secret123"

//Auth state per connection.
typedef enum {
    UNAUTH = 0,
    AUTH_OK = 1
}auth_state_t;

//Simple map: fd -> auth_state (use array or hash; here simple array, assume < 1024 fds).
#define MAX_FDS 1024
static auth_state_t auth_states[MAX_FDS] = {0}; //Init all UNAUTH;

#define AUDIT_LOG_FILE "/var/log/scp_server.log"
#define VALID_PASS_HASH fcf730b6d95236ecd3c9fc2d92d7b6b2bb061514961aec041d6c7a7192f592e4
/**
 * Set file descriptor to non-blocking mode.
 * @param fd File descriptor to modify.
 * @return 0 on success, -1 on error.
 */
int setNonBlocking(int fd){
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1){
        perror("fcntl F_GETFL");
        return -1;
    }
    flags |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) == -1){
        perror("fcntl F_SETFL");
        return -1;
    }
    return 1;
}

/**
* Write audit log with timestamp
* @param level: Level (INFO, WARN, ERROR, SECURITY)
* @param fd: Client file descriptor
* @param client_ip: Client IP address
* @param action: Action (eg: "CONNECTION_ACCEPTED")
* @param details: More details
*/
void write_audit_log(const char *level, int fd, const char *client_ip, const char *action, const char *details){
    //Open file in append mode
    FILE *log_file = fopen(AUDIT_LOG_FILE, "a");
    if(!log_file){
        //If can not open file, write to stderr
        log_file = stderr;
    }

    //Get current's time
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(log_file, "[%s] [%s] FD=%d IP=%s ACTION=%s DETAILS=%s", 
            timestamp, level, fd, client_ip ? client_ip : "N/A", 
            action, details ? details : "");

    if(log_file != stderr){
        fclose(log_file);
    }
}

#define AUDIT_INFO(fd, client_ip, action, details)    write_audit_log("INFO", fd, client_ip, action, details)
#define AUDIT_WARN(fd, client_ip, action, details)    write_audit_log("WARN", fd, client_ip, action, details)
#define AUDIT_ERROR(fd, client_ip, action, details)    write_audit_log("ERROR", fd, client_ip, action, details)
#define AUDIT_SECURITY(fd, client_ip, action, details)    write_audit_log("SECURITY", fd, client_ip, action, details)

/**
 * Create and bind socket to the given port.
 * @param port Port to bind.
 * @return Listen FD on success, -1 on error.
 */
int create_and_bind(int port){
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1){
        perror("socket");
        return -1;
    }

    int opt = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
        perror("setsockopt");
        close(listen_fd);
        return  -1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("bind");
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}


/**
 * Read data from FD into buffer (non-blocking safe).
 * @param fd Source FD.
 * @param buffer Buffer to read into.
 * @param length Max length.
 * @return 0 on success, -1 on error or EOF.
 */
int handle_receive_data(int fd, char *buffer, size_t length) {
    ssize_t total_read = 0;
    while (total_read < length) {
        ssize_t bytes_read = read(fd, buffer + total_read, length - total_read);
        if (bytes_read > 0) {
            total_read += bytes_read;
        } else if (bytes_read == 0) {
            // EOF: Client closed.
            return -1;
        } else {  // bytes_read == -1
            if (errno == EAGAIN || errno == EINTR) {
                break;  // No more data now.
            } else {
                perror("read");
                return -1;
            }
        }
    }
    buffer[total_read] = '\0';  // Null-terminate.
    printf("Received: %s\n", buffer);  // Temp log, sau thay bằng logger.
    return 0;
}

/**
 * Send data from buffer to FD (non-blocking safe).
 * @param fd Destination FD.
 * @param msg Message to send.
 * @param length Length of msg.
 * @return 0 on success, -1 on error.
 */
int handle_send_data(int fd, const char *msg, size_t length) {
    ssize_t total_sent = 0;
    while (total_sent < length) {
        ssize_t bytes_sent = write(fd, msg + total_sent, length - total_sent);
        if (bytes_sent > 0) {
            total_sent += bytes_sent;
        } else if (bytes_sent == 0) {
            fprintf(stderr, "Connection closed by client\n");
            return -1;
        } else {  // bytes_sent == -1
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            } else {
                perror("write");
                return -1;
            }
        }
    }
    return 0;
}

/**
 * Check and update auth state for FD.
 * @param fd Client FD.
 * @param buffer Request buffer.
 * @return 0 if auth OK or handled, -1 if close connection.
 */

int handle_auth(int fd, char *buffer){
    if(auth_states[fd] == AUTH_OK){
        const char *msg = "Authen OK, plese send request";
        handle_send_data(fd, msg, strlen(msg));
        return 0;
    }

    if(strncmp(buffer, "AUTH ", 5) != 0){
        const char *err_msg = "AUTH required first\n";
        handle_send_data(fd, err_msg, strlen(err_msg));
        return 0;   // Wait for next message.
    }

    // Parse "AUTH user:pass"
    char *creds = strndup(buffer + 5, 50);  // Skip "AUTH "
    if(creds == NULL){
        perror("strndup");
        return -1;
    }

    char *colon = strchr(creds, ':');
    if(colon == NULL){
        free(creds);
        const char *err_msg = "Invalid AUTH format (use user:pass)\n";
        handle_send_data(fd, err_msg, strlen(err_msg));
        return 0;
    }
    *colon = '\0';  // Split user and pass.
    char *user = creds;
    char *pass = colon + 1;

    // Trim trailing newline/spaces.
    size_t pass_len = strlen(pass);
    if (pass_len > 0 && pass[pass_len - 1] == '\n') pass[pass_len - 1] = '\0';

    int auth_ok = (strcmp(user, VALID_USER) == 0 && strcmp(pass, VALID_PASS) == 0);
    free(creds);

    if (auth_ok) {
        auth_states[fd] = AUTH_OK;
        const char *ok_msg = "AUTH OK\n";
        handle_send_data(fd, ok_msg, strlen(ok_msg));
        printf("Client %d authenticated as %s\n", fd, VALID_USER);
    } else {
        const char *err_msg = "AUTH DENIED\n";
        handle_send_data(fd, err_msg, strlen(err_msg));
        printf("Auth failed for client %d\n", fd);
        // Optional: Close after 3 fails, but for now, allow retry.
    }
    return 0;

}

/**
 * Send file using sendfile (efficient for binary files too).
 * @param path File path.
 * @param out_fd Client FD.
 * @return 0 on success, -1 on error.
 */
int send_file(const char* path, int out_fd){
    int in_fd = open(path, O_RDONLY);
    if(in_fd == -1){
        perror("open");
        return -1;
    }

    struct stat statbuf;
    if(fstat(in_fd, &statbuf) == -1){
        perror("fstat");
        close(in_fd);
        return -1;
    }

    off_t offset = 0;
    ssize_t total_sent = 0;
    while (total_sent < statbuf.st_size) {
        ssize_t bytes_sent = sendfile(out_fd, in_fd, &offset, statbuf.st_size - total_sent);
        if (bytes_sent > 0) {
            total_sent += bytes_sent;
        } else if (bytes_sent == 0) {
            break;  // Should not happen.
        } else {  // bytes_sent == -1
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            } else {
                perror("sendfile");
                close(in_fd);
                return -1;
            }
        }
    }
    printf("Sent file '%s' (%ld bytes)\n", path, (long)total_sent);
    close(in_fd);
    return 0;
}

/**
 * Check if path is safe (within SAFE_DIR).
 * @param path Path to check.
 * @param safe_dir Safe directory prefix.
 * @return 0 if safe, -1 if not.
 */
int is_safe_path(char *path, const char *safe_dir){
    char resolved_path[PATH_MAX];
    if(realpath(path, resolved_path) == NULL){
        perror("realpath: invalid path ");
        return -1;
    }

    size_t len = strlen(safe_dir);
    if(strncmp(resolved_path, safe_dir, strlen(safe_dir)) != 0 ||
        resolved_path[len] != '/'){
        fprintf(stderr, "Path %s is not in safe directory %s\n", resolved_path, safe_dir);
        return -1;
    }

    return 0;
}

/**
 * Parse request: "GET /path/to/file"
 * @param buffer Raw request.
 * @param command Output command (e.g., "GET").
 * @param path Output path (caller must free if needed, but here we copy).
 * @return 0 on success, -1 on error.
 */

int parse_request(const char* buffer, char **command, char **path){
    char *buf_copy = strdup(buffer); // Safe copy for strtok.
    if(!buf_copy){
        perror("strdup");
        return -1;
    }

    char *saveptr;
    char *cmd = strtok_r(buf_copy, " \t\n\r", &saveptr);
    char *pth = strtok_r(NULL, " \t\n\r", &saveptr);
    if(!cmd || !pth){
        fprintf(stderr, "Invalid request: missing command or path\n");
        free(buf_copy);
        return -1;
    }

    // Loại bỏ \n ở cuối nếu có
    size_t path_len = strlen(pth);
    if (path_len > 0 && pth[path_len - 1] == '\n') {
        pth[path_len - 1] = '\0';
        path_len--; // cập nhật lại length
    }

    *command = strdup(cmd);
    *path = strdup(pth);

    free(buf_copy);
    return 0;

}

/**
 * Handle new connection: Accept and add to epoll.
 * @param listen_fd Listen FD.
 * @param epoll_fd Epoll instance.
 */
void handle_new_connection(int listen_fd, int epoll_fd){
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if(conn_fd == -1){
        perror("accept");
        close(listen_fd);
        return;
    }

    int client_port = ntohs(client_addr.sin_port);
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Connect to client has address is: %s:%d\n", client_ip, client_port);
    AUDIT_INFO(conn_fd, client_ip, "CONNECTION_ACCEPTED", "New client connected");

    if(setNonBlocking(conn_fd) == -1){
        printf("Can not set non blocking to conn_fd \n");
        AUDIT_ERROR(conn_fd, client_ip, "SET_NONBLOCK_FAILED", "Cannot set non-blocking mode");
        close(conn_fd);
        return;
    }

    struct epoll_event ev = {0};
    ev.data.fd = conn_fd;
    ev.events = EPOLLET | EPOLLIN;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1){                    
        AUDIT_ERROR(conn_fd, client_ip, "EPOLL_ADD_FAILED", "Cannot add to epoll");
        close(conn_fd);
        return;
    }

    if(conn_fd < MAX_FDS){
        auth_states[conn_fd] = UNAUTH; // Explicit init.
    }
}

/**
 * Handle client request on ready FD.
 * @param ready_fd Client FD.
 * @param epoll_fd Epoll instance.
 */
void handle_client_request(int ready_fd, int epoll_fd){
    char buffer[BUFFER_LEN] = {0};

    if(handle_receive_data(ready_fd, buffer, BUFFER_LEN) == -1){
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ready_fd, NULL);                        
        close(ready_fd);                        
    }

    if(handle_auth(ready_fd, buffer) == -1){
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ready_fd, NULL);
        close(ready_fd); 
    }

    if(auth_states[ready_fd] == UNAUTH){
        return;
    }


    char *command = NULL;
    char *path = NULL;
    if(parse_request(buffer, &command, &path) == -1){
        const char *err_msg = "Invalid request\n";
        handle_send_data(ready_fd, err_msg, strlen(err_msg));
        return;
    }

    if(command !=NULL && strncmp(command, "GET", 3) == 0){
        if(is_safe_path(path, SAFE_DIR) == -1){
            const char *err_msg = "Access denied: Unsafe path\n";
            handle_send_data(ready_fd, err_msg, strlen(err_msg));
        }else if(send_file(path, ready_fd) == -1){
            const char *err_msg = "Failed to send file\n";
            handle_send_data(ready_fd, err_msg, strlen(err_msg));
        }
    }else{
        const char *err_msg = "Unknown command\n";
        handle_send_data(ready_fd, err_msg, strlen(err_msg));
    }
    free(command);
    free(path);
}

int main(){
    int listen_fd = create_and_bind(PORT);

    if(setNonBlocking(listen_fd) == -1){
        printf("Can not set non blocking to listen_fd \n");
        close(listen_fd);
        return -1;
    }

    if(listen(listen_fd, BACKLOG) == -1){
        perror("listen");
        close(listen_fd);
        return -1;
    }

    int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if(epoll_fd == -1){
        perror("epoll_create1");
        close(listen_fd);
        return -1;
    }

    struct epoll_event ev;
    ev.data.fd = listen_fd;
    ev.events = EPOLLET | EPOLLIN;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1){
        perror("epoll_add");
        close(epoll_fd);
        close(listen_fd);
        return -1;
    }
    
    printf("Server is listening in port %d...\n", PORT);

    struct epoll_event events[MAX_EVENTS];
    while(1){
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if(nfds == -1){
            perror("epoll_wait");
            break;
        }

        for(int i = 0; i < nfds; i++){
            int ready_fd = events[i].data.fd;
            if(ready_fd == listen_fd){
                handle_new_connection(listen_fd, epoll_fd);
            }else{
                if(events[i].events & EPOLLIN){
                    handle_client_request(ready_fd, epoll_fd);
                }
            }            
        }        
    }

    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, listen_fd, NULL);
    close(epoll_fd);
    close(listen_fd);
    return -1;
}
