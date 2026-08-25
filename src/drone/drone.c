#include "drone.h"
#include <limits.h>
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

int parseDroneState(char *buffer, DroneState *out_drone){
    char *saveptr;
    char *part = strtok_r(buffer, ";", &saveptr);
    if(part == NULL || strcmp(part, "STATUS") != 0){
        printf("The message don't start with STATUS\n");
        return -1;
    }

    DroneState drone={0};

    part = strtok_r(NULL, ";", &saveptr);
    if(part == NULL){
        printf("The message don't contain an id");
        return -1;
    }

    long idL = strtol(part , NULL, 10);
    if(idL >= INT_MIN && idL<= INT_MAX ){
        drone.id = idL;
    }else{
        printf("the id isn't an int\n");
        return -1;
    }

    part = strtok_r(NULL, ";", &saveptr);
    if(part == NULL){
        printf("The message don't contain a cord x");
        return -1;
    }

    drone.x = strtod(part , NULL);

    part = strtok_r(NULL, ";", &saveptr);
    if(part == NULL){
        printf("The message don't contain a cord y");
        return -1;
    }
    drone.y = strtod(part, NULL);

    *out_drone = drone;
    return 0;
}