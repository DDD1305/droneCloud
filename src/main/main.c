#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "../drone/drone.h"
#include "../UDPcommunication/udp_socket.h"


int main(void){
    DroneState drone1 = newDrone(1, 11, 11);
    DroneState drone2 = newDrone(2, 22, 22);

   

    char *text1 = show(drone1);
    char *text2 = show(drone2);

    if (text1 != NULL) {
        printf("%s\n", text1);
    }

    if (text2 != NULL) {
        printf("%s\n", text2);
    }

    free(text1);
    free(text2);

    int socket = createSocket();
    if(socket == -1){
        return 1;
    }
    printf("%d\n", socket);

    int res =bindSocket(socket, "127.0.0.1", 5001);

    if(res != -1){
        printf("La valeur retour est : %d\n", res);
    }else{
        closeSocket(socket);
        return 1;
    }

    char buffer[1024];

    res = receiveMsg(socket, buffer, 1024);

    if(res >0){
        printf("Le message est : %s\n", buffer);
    }

    closeSocket(socket);
    return 0;

}