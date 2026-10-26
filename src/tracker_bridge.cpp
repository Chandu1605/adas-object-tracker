
#include "TrackManager.hpp"

#include <iostream>
#include <vector>
#include <iomanip>

int main() {
    TrackManager manager;

    int frameId;
    int detectionCount;
    double dt;

    // Input format:
    // frameId dt detectionCount
    // x y
    // x y
    while (std::cin >> frameId >> dt >> detectionCount) {

        if (dt <= 0.0 || detectionCount < 0) {
            std::cerr << "Invalid frame input\n";
            return 1;
        }

        std::vector<Detection> detections;

        for (int i = 0; i < detectionCount; ++i) {
            double x, y;
            int classId;

            if (!(std::cin >> x >> y >> classId)) {
                return 1;
            }

            detections.push_back({x, y, classId});
        }

        manager.update(detections, dt);

        const auto& tracks = manager.getTracks();

        // One output header per input frame
        std::cout << frameId << " "
                  << tracks.size() << "\n";

        for (const auto& track : tracks) {
            std::cout << track.id << " "
                      << std::fixed << std::setprecision(2)
                      << track.x() << " "
                      << track.y() << " "
                      << track.vx() << " "
                      << track.vy() << " "
                      << track.missedFrames << "\n";
        }

        // Make output immediately available to Python
        std::cout << std::flush;
    }

    return 0;
}
