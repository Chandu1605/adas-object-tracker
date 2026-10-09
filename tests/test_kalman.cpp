
#include "KalmanFilter.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    KalmanFilter filter(20.0, -1.0);

    // Test 1: Motion prediction
    filter.predict(1.0);

    assert(std::abs(filter.getPosition() - 19.0) < 1e-6);
    assert(std::abs(filter.getVelocity() + 1.0) < 1e-6);

    std::cout << "PASS: Motion prediction\n";

    // Test 2: Measurement correction
    const double predictedPosition = filter.getPosition();
    filter.update(18.0);

    assert(filter.getPosition() < predictedPosition);
    assert(filter.getPosition() > 18.0);

    std::cout << "PASS: Measurement correction\n";

    // Test 3: Repeated measurements
    for (int i = 0; i < 10; ++i) {
        filter.predict(1.0);
        filter.update(filter.getPosition());
    }

    assert(std::isfinite(filter.getPosition()));
    assert(std::isfinite(filter.getVelocity()));

    std::cout << "PASS: Filter stability smoke test\n";
    std::cout << "All Kalman tests passed!\n";

    return 0;
}
