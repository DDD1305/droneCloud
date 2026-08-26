#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dronecloud/udp_socket.h"

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

ssize_t receiveMsg(int socket, char *buffer, size_t buffer_size) {

    if (buffer_size == 0) {
        return -1;
    }

    ssize_t res = recvfrom(socket, buffer, buffer_size - 1, 0, NULL, NULL);
    if (res == -1) {
        perror(NULL);
        return -1;
    } else if (res == 0) {
        return 0;
    } else if ((size_t)res <= buffer_size - 1) {
        buffer[res] = '\0';
        return res;
    } else {
        printf("The buffer size is too small ERROR\n");
        return res;
    }
}

ssize_t sendMsg(int socket, const char *msg, size_t msg_size,
                const struct sockaddr *to, socklen_t tolen) {

    ssize_t res = sendto(socket, msg, msg_size, 0, to, tolen);

    if (res == -1) {
        perror(NULL);
        return -1;
    } else if ((size_t)res != msg_size) {
        printf("Not all bytes were sent.\n");
        return -1;
    }
    return res;
}
