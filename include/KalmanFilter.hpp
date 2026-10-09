
#pragma once

class KalmanFilter {
public:
    KalmanFilter(double position, double velocity = 0.0);

    void predict(double dt);
    void update(double measurement);

    double getPosition() const;
    double getVelocity() const;

private:
    double position_;
    double velocity_;

    // Position-velocity covariance matrix
    double p00_ = 10.0;
    double p01_ = 0.0;
    double p10_ = 0.0;
    double p11_ = 10.0;

    double processNoise_ = 0.1;
    double measurementNoise_ = 1.0;
};
