
#include "TrackManager.hpp"
#include "CSVLogger.hpp"

#include <iostream>
#include <vector>

int main() {
    TrackManager manager;
    CSVLogger logger("tracking_results.csv");

    const double dt = 1.0;

    std::vector<std::vector<Detection>> frames = {
        {{10.0, 2.0}, {20.0, -1.0}},
        {{11.2, 2.1}, {19.1, -1.1}},
        {{12.1, 2.0}},
        {{13.2, 2.2}},
        {{14.0, 2.1}},
        {{15.1, 2.0}, {8.0, -3.0}}
    };

    std::cout << "ADAS Multi-Object Kalman Tracker\n";

    for (size_t i = 0; i < frames.size(); ++i) {
        int frameNumber = static_cast<int>(i + 1);

        manager.update(frames[i], dt);
        manager.printTracks(frameNumber);

        // Export estimated tracks to CSV
        logger.log(frameNumber, manager.getTracks());
    }

    std::cout << "\nCSV logging completed!\n";

    return 0;
}
