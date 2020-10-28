#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include"client.h"



int main(int argc, char** argv)
{
    int32_t conn_fd, n;
    int32_t ret;
    uint8_t data[MAX_DATA_BUFFER];
    struct sockaddr_in server_addr;

    int a=1+2;

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0) {
        // error
        goto error;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    ret = inet_aton(SERVER_IP, &server_addr.sin_addr);
    if(ret == 0) {
        // error
        goto error;
    }

    ret = connect(conn_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        // error
        goto error;
    }

    while(1) {
        printf("send msg to server: ");
        memset(data, 0, MAX_DATA_BUFFER);
        fgets(data, MAX_DATA_BUFFER, stdin);
        printf("msg: %s\n", data);
        ret = send(conn_fd, data, strlen(data), 0);
        if( ret == -1) {
            // error
            goto error;
        }
    }
    close(conn_fd);
    return 0;

error:
    printf("error: %s(errno: %d)\n", strerror(errno), errno);
    if(conn_fd >= 0) {
        close(conn_fd);
    }
    return -1;
}