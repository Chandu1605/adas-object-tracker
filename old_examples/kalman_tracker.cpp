
// #include <iostream>
// #include <vector>
// #include <iomanip>

// class KalmanFilter {
// private:
//     double position;
//     double velocity;
//     double uncertainty;
//     double processNoise;
//     double measurementNoise;

// public:
//     KalmanFilter(double initialPosition,double initialVelocity)
//         : position(initialPosition),velocity(initialVelocity),
//           uncertainty(1.0),processNoise(0.1),measurementNoise(1.0) {}

//     void predict(double dt) {
//         position += velocity * dt;
//         uncertainty += processNoise;
//     }

//     void update(double measurement) {
//         double gain = uncertainty / (uncertainty + measurementNoise);

//         position += gain * (measurement - position);
//         uncertainty *= (1.0 - gain);
//     }

//     double getPosition() const {
//         return position;
//     }
// };

// int main() {
//     std::vector<double> radarMeasurements = {
//         20.0, 18.5, 17.8, 15.9, 15.2
//     };

//     KalmanFilter tracker(20.0, -1.0);

//     std::cout << std::fixed << std::setprecision(2);

//     for (double measurement : radarMeasurements) 
//     {
//         tracker.predict(1.0);
//         tracker.update(measurement);

//         std::cout << "Radar: " << measurement
//                   << " m | Estimated: "
//                   << tracker.getPosition()
//                   << " m\n";
//     }

//     return 0;
// }





#include <iostream>
#include <iomanip>
#include <vector>

class KalmanFilter {
private:
    // State: position and velocity
    double position;
    double velocity;

    // Covariance matrix
    double p00 = 10.0;
    double p01 = 0.0;
    double p10 = 0.0;
    double p11 = 10.0;

    double processNoise = 0.1;
    double measurementNoise = 1.0;

public:
    KalmanFilter(double initialPosition,
                 double initialVelocity)
        : position(initialPosition),
          velocity(initialVelocity) {}

    void predict(double dt) {
        // State prediction
        position += velocity * dt;

        // Covariance prediction: P = F P F^T + Q
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
        // Innovation
        double residual = measurement - position;

        // Innovation covariance
        double s = p00 + measurementNoise;

        // Kalman gain
        double k0 = p00 / s;
        double k1 = p10 / s;

        // State correction
        position += k0 * residual;
        velocity += k1 * residual;

        // Covariance correction (Joseph form)
        double a00 = 1.0 - k0;
        double a10 = -k1;

        double q00 = a00 * a00 * p00;
        double q01 = a00 * (a10 * p00 + p01);
        double q10 = a00 * (a10 * p00 + p10);
        double q11 = a10 * a10 * p00
                    + a10 * (p01 + p10) + p11;

        p00 = q00 + k0 * k0 * measurementNoise;
        p01 = q01 + k0 * k1 * measurementNoise;
        p10 = q10 + k1 * k0 * measurementNoise;
        p11 = q11 + k1 * k1 * measurementNoise;
    }

    double getPosition() const { return position; }
    double getVelocity() const { return velocity; }
};

int main() {
    std::vector<double> radarMeasurements = {
        20.5, 18.3, 18.8, 16.4, 16.7,
        14.2, 14.5, 12.8, 12.4, 10.9
    };

    KalmanFilter tracker(20.0, -1.0);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Time | Radar(m) | Estimate(m) | Velocity(m/s)\n";

    for (size_t i = 0; i < radarMeasurements.size(); ++i) {
        tracker.predict(1.0);
        tracker.update(radarMeasurements[i]);

        std::cout << std::setw(4) << i + 1 << " | "
                  << std::setw(8) << radarMeasurements[i] << " | "
                  << std::setw(11) << tracker.getPosition() << " | "
                  << std::setw(13) << tracker.getVelocity()
                  << '\n';
    }
}
