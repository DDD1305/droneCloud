#include <stdio.h>
#include <stdlib.h>

#include "../drone/drone.h"


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

    return 0;

}