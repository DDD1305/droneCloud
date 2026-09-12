#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "dronecloud/drone.h"
#include "dronecloud/udp_socket.h"

#define CapacityRelay 20
#define Max_len_MSG 1024

int main() {

    char *ip = "127.0.0.1";
    unsigned short port = 5001;

    int socket_relay_fd = createSocket();

    if (socket_relay_fd == -1) {
        perror(NULL);
        return EXIT_FAILURE;
    }

    int res = bindSocket(socket_relay_fd, ip, port);

    if (res == -1) {
        goto close;
    }

    struct pollfd pfds[1];
    pfds[0].fd = socket_relay_fd;
    pfds[0].events = POLLIN;

    char buffer[Max_len_MSG];

    struct sockaddr_in drones[CapacityRelay] = {0};
    int nb_drones = 0;

    struct sockaddr_in source;

    while (1) {
        int poll_result = poll(pfds, 1, 20);

        if (poll_result == -1) {
            perror("poll");
            goto close;
        }

        // receive message
        if (pfds[0].revents & POLLIN) {
            ssize_t receive_result =
                receiveMsg(socket_relay_fd, buffer, sizeof(buffer), &source);
            if (receive_result == -1) {
                goto close;
            } else if (receive_result == 0) {
                // do nothing for now
            } else {
                int firstPositionClear = -1;
                int foundFPC = 0;
                int nbDroneFound = 0;
                int sourceAlreadyExist = 0;

                for (int i = 0; i < CapacityRelay; i++) {
                    if (foundFPC == 0 && drones[i].sin_port == 0) {
                        firstPositionClear = i;
                        foundFPC++;
                    }
                    if (sourceAlreadyExist == 0 && nbDroneFound == nb_drones) {
                        if (firstPositionClear != -1) {
                            drones[firstPositionClear] = source;
                        } else {
                            drones[i] = source;
                        }
                        nb_drones++;
                    }
                    if (sourceAlreadyExist == 0 &&
                        drones[i].sin_addr.s_addr == source.sin_addr.s_addr &&
                        drones[i].sin_port == source.sin_port) {
                        sourceAlreadyExist = 1;
                        nbDroneFound++;
                    } else if (drones[i].sin_port != 0) {
                        sendMsg(socket_relay_fd, buffer, receive_result,
                                (const struct sockaddr *)&drones[i],
                                sizeof(drones[i]));
                        nbDroneFound++;
                    }
                }
            }
        }
    }

close:
    closeSocket(socket_relay_fd);
    return EXIT_FAILURE;
}