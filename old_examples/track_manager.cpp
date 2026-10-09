
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Detection {
    double x;
    double y;
};

struct Track {
    int id;
    double x;
    double y;
    int missedFrames = 0;
};

class TrackManager {
private:
    std::vector<Track> tracks;
    int nextId = 101;

    double maxAssociationDistance = 3.0;
    int maxMissedFrames = 2;

    double distance(const Track& t,
                    const Detection& d) {
        return std::hypot(t.x - d.x, t.y - d.y);
    }

public:
    void update(const std::vector<Detection>& detections) {
        std::vector<bool> matched(detections.size(), false);

        for (auto& track : tracks) {
            int bestIndex = -1;
            double bestDistance = maxAssociationDistance;

            for (size_t i = 0; i < detections.size(); ++i) {
                if (matched[i]) continue;

                double d = distance(track, detections[i]);

                if (d < bestDistance) {
                    bestDistance = d;
                    bestIndex = static_cast<int>(i);
                }
            }

            if (bestIndex >= 0) {
                track.x = detections[bestIndex].x;
                track.y = detections[bestIndex].y;
                track.missedFrames = 0;
                matched[bestIndex] = true;
            } else {
                ++track.missedFrames;
            }
        }

        // Remove tracks that have been missing too long
        tracks.erase(
            std::remove_if(
                tracks.begin(),
                tracks.end(),
                [this](const Track& t) {
                    return t.missedFrames > maxMissedFrames;
                }
            ),
            tracks.end()
        );

        // Create tracks for unmatched detections
        for (size_t i = 0; i < detections.size(); ++i) {
            if (!matched[i]) {
                tracks.push_back({
                    nextId++,
                    detections[i].x,
                    detections[i].y,
                    0
                });
            }
        }
    }

    void printTracks(int frame) const {
        std::cout << "\nFrame " << frame << '\n';

        for (const auto& t : tracks) {
            std::cout << "ID: " << t.id
                      << " | X: " << t.x
                      << " | Y: " << t.y
                      << " | Missed: " << t.missedFrames
                      << '\n';
        }
    }
};

int main() {
    TrackManager manager;

    std::vector<std::vector<Detection>> frames = {
        {{10, 2}, {20, -1}},
        {{11, 2}, {19, -1}},
        {{12, 2}},
        {{13, 2}},
        {{14, 2}},
        {{15, 2}, {8, -3}}
    };

    for (size_t i = 0; i < frames.size(); ++i) {
        manager.update(frames[i]);
        manager.printTracks(static_cast<int>(i + 1));
    }
}
