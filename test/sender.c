#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#include "../src/drone/drone.h"



#include "../src/UDPcommunication/udp_socket.h"


int main(void){


    int socket = createSocket();

    if(socket == -1){
        return 1;
    }
    printf("%d\n", socket);

    int res =bindSocket(socket, "127.0.0.2", 5002);
    if(res != -1){
        printf("La valeur retour est : %d\n", res);
    }else{
        closeSocket(socket);
        return 1;
    }

    struct sockaddr_in addr={0};

    addr.sin_family= AF_INET;
    addr.sin_port = htons(5001);

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    DroneState drone = {
        .id=2,
        .x=10,
        .y=10,
    };

    char *msg = show(drone);
    printf("%s\n", msg);
    sendMsg(socket, msg, strlen(msg), (struct sockaddr*)&addr, sizeof addr);
    closeSocket(socket);
    return 0;

}