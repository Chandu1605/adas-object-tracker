
#include "KalmanFilter.hpp"

KalmanFilter::KalmanFilter(double position, double velocity)
    : position_(position), velocity_(velocity) {}

void KalmanFilter::predict(double dt) {
    position_ += velocity_ * dt;

    const double q = processNoise_;

    const double n00 = p00_ + dt * (p01_ + p10_)
                     + dt * dt * p11_
                     + q * dt * dt * dt * dt / 4.0;

    const double n01 = p01_ + dt * p11_
                     + q * dt * dt * dt / 2.0;

    const double n10 = p10_ + dt * p11_
                     + q * dt * dt * dt / 2.0;

    const double n11 = p11_ + q * dt * dt;

    p00_ = n00;
    p01_ = n01;
    p10_ = n10;
    p11_ = n11;
}

void KalmanFilter::update(double measurement) {
    const double residual = measurement - position_;
    const double s = p00_ + measurementNoise_;

    const double k0 = p00_ / s;
    const double k1 = p10_ / s;

    position_ += k0 * residual;
    velocity_ += k1 * residual;

    // Joseph-form covariance correction
    const double a = 1.0 - k0;
    const double b = -k1;

    const double n00 = a * a * p00_
                     + k0 * k0 * measurementNoise_;

    const double n01 = a * (b * p00_ + p01_)
                     + k0 * k1 * measurementNoise_;

    const double n10 = a * (b * p00_ + p10_)
                     + k1 * k0 * measurementNoise_;

    const double n11 = b * b * p00_
                     + b * (p01_ + p10_)
                     + p11_
                     + k1 * k1 * measurementNoise_;

    p00_ = n00;
    p01_ = n01;
    p10_ = n10;
    p11_ = n11;
}

double KalmanFilter::getPosition() const {
    return position_;
}

double KalmanFilter::getVelocity() const {
    return velocity_;
}
