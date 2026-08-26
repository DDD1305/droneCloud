#ifndef DRONE_H
#define DRONE_H

typedef struct {
    int id;   // identification
    double x; // position x
    double y; // position y
} DroneState;

DroneState newDrone(int id, double x, double y);

/**
 * the result need to be free
 */
char *show(DroneState drone);

/**
 * Parses a STATUS message.
 *
 * @param buffer Message buffer modified during parsing.
 * @param out_drone Must point to a valid DroneState.
 * @return 0 on success, -1 if the message is invalid.
 */
int parseDroneState(char *buffer, DroneState *out_drone);

#endif