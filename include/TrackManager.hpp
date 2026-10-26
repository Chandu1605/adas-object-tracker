
#pragma once

#include "Track.hpp"
#include <vector>

class TrackManager {
public:
    void update(const std::vector<Detection>& detections,
                double dt);

    void printTracks(int frame) const;

    const std::vector<Track>& getTracks() const;

private:
    std::vector<Track> tracks_;

    int nextId_ = 101;
    int maxMisses_ = 10;
    double maxDistance_ = 60.0;

    void predictTracks(double dt);
    void removeStaleTracks();
};
