#ifndef DRONE_H
#define DRONE_H

typedef struct {
    int id; //identification
    double x; //position x
    double y; //position y
} DroneState;

DroneState newDrone(int id, double x, double y);

char* show(DroneState drone);

#endif