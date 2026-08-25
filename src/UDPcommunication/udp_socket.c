#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "udp_socket.h"

/*
int status; //socket indentification

void initCommunication(void){

    struct addrinfo hints ={
    .ai_family = AF_INET,
    .ai_socktype = SOCK_DGRAM,
    .ai_flags = AI_PASSIVE,
    };
}*/

int createSocket(void) {
    int res = socket(AF_INET, SOCK_DGRAM, 0);
    /*0 it's because we use default protocol dgram->UDP
     * socket return a file descriptor
     */
    if (res == -1) {
        perror(NULL); // no prefix
        return -1;
    }
    return res;
}

int closeSocket(int socket) {
    int res = close(socket);
    if (res == -1) {
        perror(NULL); // no prefix
        return -1;
    }
    return res;
}

int bindSocket(int socket, const char *address, unsigned short int port) {

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int conversion = inet_pton(AF_INET, address, &addr.sin_addr);
    if (conversion == 0) {
        printf("L'adresse fournie n'est pas IPV4");
        return -1;
    } else if (conversion == -1) {
        perror(NULL); // no prefix
        return -1;
    }

    int res = bind(socket, ((struct sockaddr *)&addr), sizeof addr);
    if (res == -1) {
        perror(NULL); // no prefix
        return -1;
    }
    return res;
}

int receiveMsg(int socket, char *buffer, int buffer_size) {

    int res = recvfrom(socket, buffer, buffer_size - 1, 0, NULL, NULL);
    if (res == -1) {
        perror(NULL);
        return -1;
    } else if (res == 0) {
        return 0;
    } else if (res <= buffer_size - 1) {
        buffer[res] = '\0';
        return res;
    } else {
        printf("The buffer size is too small ERROR\n");
        return res;
    }
}

int sendMsg(int socket, char *msg, int msg_size, struct sockaddr *to,
            socklen_t tolen) {

    int res = sendto(socket, msg, msg_size, 0, to, tolen);

    if (res == -1) {
        perror(NULL);
        return -1;
    }
    return res;
}