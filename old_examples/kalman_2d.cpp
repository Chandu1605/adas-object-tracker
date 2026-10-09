
#include <iostream>
#include <iomanip>
#include <vector>

class Kalman1D {
private:
    double position;
    double velocity;

    double p00 = 10.0;
    double p01 = 0.0;
    double p10 = 0.0;
    double p11 = 10.0;

    double processNoise = 0.1;
    double measurementNoise = 1.0;

public:
    Kalman1D(double pos, double vel)
        : position(pos), velocity(vel) {}

    void predict(double dt) {
        position += velocity * dt;

        double n00 = p00 + dt * (p01 + p10)
                   + dt * dt * p11
                   + processNoise * dt * dt * dt * dt / 4.0;

        double n01 = p01 + dt * p11
                   + processNoise * dt * dt * dt / 2.0;

        double n10 = p10 + dt * p11
                   + processNoise * dt * dt * dt / 2.0;

        double n11 = p11 + processNoise * dt * dt;

        p00 = n00;
        p01 = n01;
        p10 = n10;
        p11 = n11;
    }

    void update(double measurement) {
        double residual = measurement - position;
        double s = p00 + measurementNoise;

        double k0 = p00 / s;
        double k1 = p10 / s;

        position += k0 * residual;
        velocity += k1 * residual;

        // Joseph-form covariance update
        double a00 = 1.0 - k0;
        double a10 = -k1;

        double n00 = a00 * a00 * p00
                   + k0 * k0 * measurementNoise;

        double n01 = a00 * (a10 * p00 + p01)
                   + k0 * k1 * measurementNoise;

        double n10 = a00 * (a10 * p00 + p10)
                   + k1 * k0 * measurementNoise;

        double n11 = a10 * a10 * p00
                   + a10 * (p01 + p10)
                   + p11
                   + k1 * k1 * measurementNoise;

        p00 = n00;
        p01 = n01;
        p10 = n10;
        p11 = n11;
    }

    double getPosition() const { return position; }
    double getVelocity() const { return velocity; }
};

struct Measurement {
    double x;
    double y;
};

int main() {
    std::vector<Measurement> sensorData = {
        {19.4, 2.3},
        {17.6, 3.2},
        {17.3, 3.4},
        {15.7, 4.1},
        {15.3, 4.7}
    };

    Kalman1D filterX(20.0, -1.0);
    Kalman1D filterY(2.0, 0.5);

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Time | Sensor X,Y | Estimated X,Y | VX,VY\n";

    for (size_t i = 0; i < sensorData.size(); ++i) {
        filterX.predict(1.0);
        filterY.predict(1.0);

        filterX.update(sensorData[i].x);
        filterY.update(sensorData[i].y);

        std::cout << i + 1 << " s  | "
                  << sensorData[i].x << ", "
                  << sensorData[i].y << " | "
                  << filterX.getPosition() << ", "
                  << filterY.getPosition() << " | "
                  << filterX.getVelocity() << ", "
                  << filterY.getVelocity()
                  << '\n';
    }

    return 0;
}
