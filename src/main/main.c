#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>

#include "../drone/drone.h"
#include "../UDPcommunication/udp_socket.h"


int main(int argc, char * argv[]){

    if(argc != 4){
        printf("Error: you need to use 3 arguments: id IP port\n");
        return 1;
    }

    char * id = argv[1];
    char* ip = argv[2];
    char* portC = argv[3];
    unsigned short port;

    DroneState drone = newDrone(atoi(id), 0, 0);

    int socket_fd = createSocket();

    if(socket_fd ==-1){
        perror(NULL);
        return 1;
    }

    char *end;
    errno=0;

    unsigned long value = strtoul(portC, &end, 10);

    if (errno != 0) {
        printf("wrong format for the port\n");
        goto close;
    } else if (end == portC|| *end != '\0') {
        printf("wrong format for the port\n");
        goto close;
    } else if (value > USHRT_MAX) {
        printf("wrong format for the port\n");
        goto close;
    } else {
        port = (unsigned short)value;
    }

    int res = bindSocket(socket_fd, ip, port);

    if(res ==-1){
        goto close;
    }

    struct pollfd pfds[1];
    pfds[0].fd = socket_fd;
    pfds[0].events = POLLIN;

    char buffer[1024];


    while(1){
        int poll_result = poll(pfds, 1, 20);

        if (poll_result == -1) {
        perror("poll");
        return 1;
        }

        if(pfds[0].revents & POLLIN){
            res = receiveMsg(socket_fd, buffer, sizeof(buffer));
            if(res==-1){
                goto close;
            }else if(res ==0){
                goto close;
            }else{
                DroneState out_drone={0};
                int parse_result = parseDroneState(buffer, &out_drone);
                if(parse_result != -1){
                    char *p = show(out_drone);
                    printf("%s\n", p);
                    free(p);
                }
                
            }
        }
    }

    //dead for now
    closeSocket(socket_fd);
    return 0;

    close :
    closeSocket(socket_fd);
    return 1;

}