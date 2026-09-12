#include "dronecloud/drone.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DroneState newDrone(int id, double x, double y) {
    DroneState drone = {
        .id = id,
        .x = x,
        .y = y,
    };

    return drone;
}

char *show(const DroneState drone) {
    int lengthID = snprintf(NULL, 0, "%d", drone.id);
    int lengthX = snprintf(NULL, 0, "%.6f", drone.x);
    int lengthY = snprintf(NULL, 0, "%.6f", drone.y);

    size_t length =
        (size_t)(lengthID + lengthX + lengthY) + (size_t)strlen("STATUS;;;");

    char *text = malloc(length + 1);

    if (text == NULL) {
        printf("show: malloc failed\n");
        goto clean;
    }

    int written = snprintf(text, length + 1, "STATUS;%d;%.6f;%.6f", drone.id,
                           drone.x, drone.y);

    if (written <= 0) {
        printf("Nothing written\n");
        goto clean;
    } else if ((size_t)written > length) {
        printf("Truncated message\n");
        goto clean;
    } else {
        return text;
    }

clean:

    free(text);
    return NULL;
}

int parseDroneState(char *buffer, DroneState *out_drone) {

    assert(out_drone != NULL);

    char *saveptr;
    char *part = strtok_r(buffer, ";", &saveptr);
    if (part == NULL || strcmp(part, "STATUS") != 0) {
        printf("The message don't start with STATUS\n");
        return -1;
    }

    DroneState drone = {0};

    part = strtok_r(NULL, ";", &saveptr);
    if (part == NULL) {
        printf("The message don't contain an id\n");
        return -1;
    }

    char *end;
    errno = 0;

    long idL = strtol(part, &end, 10);
    if (errno != 0 || end == part || *end != '\0') {
        return -1;
    }
    if (idL >= INT_MIN && idL <= INT_MAX && idL != 0) {
        drone.id = (int)idL;
    } else {
        printf("the id isn't an int\n");
        return -1;
    }

    part = strtok_r(NULL, ";", &saveptr);
    if (part == NULL) {
        printf("The message don't contain a cord x\n");
        return -1;
    }

    errno = 0;
    drone.x = strtod(part, &end);
    if (errno != 0 || end == part || *end != '\0') {
        return -1;
    }

    part = strtok_r(NULL, ";", &saveptr);
    if (part == NULL) {
        printf("The message don't contain a cord y\n");
        return -1;
    }

    errno = 0;
    drone.y = strtod(part, &end);
    if (errno != 0 || end == part || *end != '\0') {
        return -1;
    }

    if (strtok_r(NULL, ";", &saveptr) != NULL) {
        return -1;
    }

    *out_drone = drone;
    return 0;
}

int updateDataDrone(DroneState *dataDrone, int *nb_drone, DroneState *drone) {
    int firstPositionClear = -1;
    int foundFPC = 0;
    int nbDroneFound = 0;
    for (int i = 0; i < NB_FRIENDS; i++) {
        if (foundFPC == 0 && dataDrone[i].id == 0) {
            firstPositionClear = i;
            foundFPC++;
        }
        if (nbDroneFound == *nb_drone) {
            if (firstPositionClear == -1) {
                dataDrone[i] = *drone;
                (*nb_drone)++;
                return 1;
            } else {
                dataDrone[firstPositionClear] = *drone;
                (*nb_drone)++;
                return 1;
            }
        }
        if (dataDrone[i].id == drone->id) {
            dataDrone[i].x = drone->x;
            dataDrone[i].y = drone->y;
            return 0;
        } else if (dataDrone[i].id != 0) {
            nbDroneFound++;
        }
    }
    return -1;
}