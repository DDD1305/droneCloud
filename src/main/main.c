#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
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

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Error: you need to use 3 arguments: id IP port\n");
        return 1;
    }

    char *id = argv[1];
    char *ip = argv[2];
    char *portC = argv[3];
    unsigned short port;

    DroneState drone = newDrone(atoi(id), 0, 0);

    // the relay
    struct sockaddr_in relay = {0};

    relay.sin_family = AF_INET;
    relay.sin_port = htons(5003);

    int conversion = inet_pton(AF_INET, "127.0.0.3", &relay.sin_addr);
    if (conversion == 0) {
        printf("L'adresse du relais n'est pas IPV4");
        return EXIT_FAILURE;
    } else if (conversion == -1) {
        perror(NULL); // no prefix
        return EXIT_FAILURE;
    }

    int socket_fd = createSocket();

    if (socket_fd == -1) {
        perror(NULL);
        return EXIT_FAILURE;
    }

    char *end;
    errno = 0;

    unsigned long value = strtoul(portC, &end, 10);

    if (errno != 0) {
        printf("wrong format for the port\n");
        goto close;
    } else if (end == portC || *end != '\0') {
        printf("wrong format for the port\n");
        goto close;
    } else if (value > USHRT_MAX) {
        printf("wrong format for the port\n");
        goto close;
    } else {
        port = (unsigned short)value;
    }

    int res = bindSocket(socket_fd, ip, port);

    if (res == -1) {
        goto close;
    }

    DroneState dataDrone[NB_FRIENDS] = {0};
    int nb_drone = 0;

    struct pollfd pfds[1];
    pfds[0].fd = socket_fd;
    pfds[0].events = POLLIN;

    char buffer[1024];

    struct timespec last_send;

    if (clock_gettime(CLOCK_MONOTONIC, &last_send) == -1) {
        perror("clock_gettime");
        goto close;
    }

    while (1) {
        int poll_result = poll(pfds, 1, 20);

        if (poll_result == -1) {
            perror("poll");
            goto close;
        }

        // receive message
        if (pfds[0].revents & POLLIN) {
            ssize_t receive_result =
                receiveMsg(socket_fd, buffer, sizeof(buffer));
            if (receive_result == -1) {
                goto close;
            } else if (receive_result == 0) {
                // do nothing for now
            } else {
                DroneState out_drone = {0};
                int parse_result = parseDroneState(buffer, &out_drone);
                if (parse_result != -1) {
                    char *p = show(out_drone);
                    if (p == NULL) {
                        goto close;
                    }
                    int updateDataDrone_result =
                        updateDataDrone(dataDrone, &nb_drone, &out_drone);
                    if (updateDataDrone_result == -1) {
                    }
                    for (int i = 0; i < NB_FRIENDS; i++) {
                        if (dataDrone[i].id != 0) {
                            char *texte = show(dataDrone[i]);
                            if (texte != NULL) {
                                printf("%s\n", texte);
                                free(texte);
                            }
                        }
                    }

                    // printf("%s\n", p);
                    free(p);
                }
            }
        }

        // sends his status every 5 seconds

        struct timespec now;
        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
            perror("clock_gettime");
            goto close;
        }

        double elapsed =
            (double)(now.tv_sec - last_send.tv_sec) +
            (double)(now.tv_nsec - last_send.tv_nsec) / 1000000000.0;

        if (elapsed >= 5) {

            char *msg = show(drone);
            if (msg != NULL) {
                ssize_t send_result =
                    sendMsg(socket_fd, msg, strlen(msg),
                            (struct sockaddr *)&relay, sizeof(relay));
                if (send_result == -1) {
                    // do nothing for now
                }
                free(msg);
            }
            last_send = now;
        }
    }

    // dead for now
    closeSocket(socket_fd);
    return EXIT_SUCCESS;

close:
    closeSocket(socket_fd);
    return EXIT_FAILURE;
}
