#include<sys/types.h>
#include<stdint.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include"server.h"
// int socket(int domain, int type, int protocol);
// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// int listen(int sockfd, int backlog);
// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

//  int32_t fd = socket(AF_INET, SOCK_STREAM, 0);

int main()
{
    int32_t listen_fd, conn_fd;
    int32_t ret;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int8_t data[MAX_DATA_BUFFER];
    ssize_t data_len;
    
    uint32_t count;

    // create a socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) {
        // error
        goto error;
    }

    // bind with IP and Port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family  = AF_INET;
    ret = inet_aton(SERVER_IP, &server_addr.sin_addr);
    if(ret == 0) {
        // error
        goto error;
    }
    server_addr.sin_port = htons(SERVER_PORT); // use host to network function to converts bytes
    ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        // error
        goto error;
    }

    // limit max client count
    ret = listen(listen_fd, MAX_CLIENT);
    if(ret == -1) {
        // error
        goto error;
    }

    // waitting for client data come in
    while (1) {
        printf("waitting msg......\n");

        conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if(conn_fd == -1) {
            // error
            goto error;
        }
        // client IP : client_addr.sin_addr, Port: client_addr.sin_port

        data_len = recv(conn_fd, data, MAX_DATA_BUFFER - 1, 0);
        data[data_len] = '\0';
        printf("recv msg from client: %s\n", data);
        close(conn_fd);
    }
    close(listen_fd);
    return 0;

error:
    printf("error: %s(errno: %d)\n", strerror(errno), errno);
    if(listen_fd >= 0) {
        close(listen_fd);
    }
    if(conn_fd >= 0) {
        close(conn_fd);
    }
    return -1;
}