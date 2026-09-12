#ifndef DRONE_H
#define DRONE_H

#define NB_FRIENDS 10

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

/**
 * Updates an existing drone position or adds a new drone to the array.
 *
 * @param dataDrone Array of NB_FRIENDS entries; id 0 marks a free entry.
 * @param nb_drone Pointer to the number of stored drones (0 to NB_FRIENDS).
 *                 Incremented by this function only when a new drone is added;
 *                 unchanged on an update or failure.
 * @param drone Pointer to the drone state to store; its id must be nonzero.
 * All pointers must be valid, and the count must match the occupied entries.
 * @return 0 if an existing drone is updated, 1 if a new drone is added,
 *         -1 if no entry is available for a new drone.
 */

int updateDataDrone(DroneState *dataDrone, int *nb_drone, DroneState *drone);

#endif
