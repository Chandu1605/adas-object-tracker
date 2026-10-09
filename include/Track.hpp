
#pragma once

#include "KalmanFilter.hpp"

struct Detection {
    double x;
    double y;
};

struct Track {
    int id;
    KalmanFilter filterX;
    KalmanFilter filterY;
    int missedFrames = 0;

    Track(int id, double x, double y)
        : id(id), filterX(x), filterY(y) {}

    double x() const {
        return filterX.getPosition();
    }

    double y() const {
        return filterY.getPosition();
    }

    double vx() const {
        return filterX.getVelocity();
    }

    double vy() const {
        return filterY.getVelocity();
    }
};
