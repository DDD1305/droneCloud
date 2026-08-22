#include "drone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DroneState newDrone(int id, double x, double y){
    DroneState drone ={
    .id = id,
    .x = x,
    .y = y,
    };

    return drone;
}

char* show(const DroneState drone){
    int lengthID = snprintf(NULL, 0, "%d", drone.id);
    int lengthX  = snprintf(NULL, 0, "%.6f", drone.x);
    int lengthY  = snprintf(NULL, 0, "%.6f", drone.y);

    size_t length = (size_t)(lengthID + lengthX + lengthY) + (size_t)strlen("STATUS;;;");


    char* text = malloc(length + 1);

    if(text == NULL){
        printf("show: malloc failed\n");
        goto clean;
    }

    int written = snprintf(text, length+1, "STATUS;%d;%.6f;%.6f",drone.id, drone.x, drone.y );

    if(written <=0){
        printf("Nothing written\n");
        goto clean;
    }else if ((size_t) written > length){
        printf("Truncated message\n");
        goto clean;
    }else{
        return text;
    }

    clean :

    free(text);
    return NULL;

}