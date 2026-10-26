
#pragma once

#include "KalmanFilter.hpp"

struct Detection {
    double x;
    double y;
    int classId = -1;
};

struct Track {
    int id;
    int classId;
    KalmanFilter filterX;
    KalmanFilter filterY;
    int missedFrames = 0;

    Track(int id, double x, double y, int classId = -1)
        : id(id),
          classId(classId),
          filterX(x),
          filterY(y) {}

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
