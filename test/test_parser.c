#include "dronecloud/drone.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

static void test_valid_status(void) {
    char message[] = "STATUS;12;1.5;-2.25";
    DroneState drone = {0};

    int result = parseDroneState(message, &drone);

    assert(result == 0);
    assert(drone.id == 12);
    assert(fabs(drone.x - 1.5) < 0.000001);
    assert(fabs(drone.y - (-2.25)) < 0.000001);
}

static void test_invalid_status(void) {
    char wrong_type[] = "POSITION;12;1.5;-2.25";
    char missing_y[] = "STATUS;12;1.5";
    char invalid_id[] = "STATUS;twelve;1.5;-2.25";
    DroneState drone = {0};

    assert(parseDroneState(wrong_type, &drone) == -1);
    assert(parseDroneState(missing_y, &drone) == -1);
    assert(parseDroneState(invalid_id, &drone) == -1);
}

int main(void) {
    test_valid_status();
    test_invalid_status();

    printf("All parser tests passed.\n");
    return 0;
}
