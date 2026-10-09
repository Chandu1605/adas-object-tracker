
#include <iostream>
#include <iomanip>

struct ObjectState {
    int id;
    double x;
    double y;
    double vx;
    double vy;
};

void predict(ObjectState& object, double dt) {
    object.x += object.vx * dt;
    object.y += object.vy * dt;
}

void printState(const ObjectState& object, int time) {
    std::cout << "Time: " << time << " s"
              << " | ID: " << object.id
              << " | X: " << object.x
              << " | Y: " << object.y
              << " | VX: " << object.vx
              << " | VY: " << object.vy
              << '\n';
}

int main() {
    ObjectState vehicle{
        101,
        20.0,  // longitudinal position
        2.0,   // lateral position
        -1.0,  // longitudinal velocity
        0.5    // lateral velocity
    };

    std::cout << std::fixed << std::setprecision(2);

    printState(vehicle, 0);

    for (int t = 1; t <= 5; ++t) {
        predict(vehicle, 1.0);
        printState(vehicle, t);
    }

    return 0;
}
