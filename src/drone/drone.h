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
 *@return 0 if the msg was valid and -1 if not
 */
int parseDroneState(char *buffer, DroneState *out_drone);

#endif