
#include "TrackManager.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

void TrackManager::predictTracks(double dt) {
    for (auto& track : tracks_) {
        track.filterX.predict(dt);
        track.filterY.predict(dt);
    }
}

void TrackManager::removeStaleTracks() {
    tracks_.erase(
        std::remove_if(
            tracks_.begin(),
            tracks_.end(),
            [this](const Track& track) {
                return track.missedFrames > maxMisses_;
            }
        ),
        tracks_.end()
    );
}

void TrackManager::update(
    const std::vector<Detection>& detections,
    double dt
) {
    // 1. Predict existing tracks
    predictTracks(dt);

    struct Candidate {
        size_t trackIndex;
        size_t detectionIndex;
        double distance;
    };

    std::vector<Candidate> candidates;

    // 2. Find possible associations
    for (size_t t = 0; t < tracks_.size(); ++t) {
    for (size_t d = 0; d < detections.size(); ++d) {

        // Only match objects of the same class.
        // -1 represents an unspecified class.
        if (tracks_[t].classId != -1 &&
            detections[d].classId != -1 &&
            tracks_[t].classId != detections[d].classId) {
            continue;
        }

        const double distance = std::hypot(
            tracks_[t].x() - detections[d].x,
            tracks_[t].y() - detections[d].y
        );

        if (distance <= maxDistance_) {
            candidates.push_back({t, d, distance});
        }
    }
}

    // 3. Prefer shorter association distances
    std::sort(
        candidates.begin(),
        candidates.end(),
        [](const Candidate& a, const Candidate& b) {
            return a.distance < b.distance;
        }
    );

    std::vector<bool> trackMatched(tracks_.size(), false);
    std::vector<bool> detectionMatched(detections.size(), false);

    // 4. Correct matched tracks
    for (const auto& candidate : candidates) {
        const size_t t = candidate.trackIndex;
        const size_t d = candidate.detectionIndex;

        if (trackMatched[t] || detectionMatched[d]) {
            continue;
        }

        tracks_[t].filterX.update(detections[d].x);
        tracks_[t].filterY.update(detections[d].y);
        tracks_[t].missedFrames = 0;

        trackMatched[t] = true;
        detectionMatched[d] = true;
    }

    // 5. Count missing detections
    for (size_t t = 0; t < tracks_.size(); ++t) {
        if (!trackMatched[t]) {
            ++tracks_[t].missedFrames;
        }
    }

    // 6. Delete stale tracks
    removeStaleTracks();

    // 7. Create tracks for new detections
    for (size_t d = 0; d < detections.size(); ++d) {
        if (!detectionMatched[d]) {            
            tracks_.emplace_back(
            nextId_++,
            detections[d].x,
            detections[d].y,
            detections[d].classId
        );
        }
    }
}

void TrackManager::printTracks(int frame) const {
    std::cout << "\nFrame " << frame << '\n';

    for (const auto& track : tracks_) {
        std::cout
            << "ID: " << track.id
            << " | X: " << track.x()
            << " | Y: " << track.y()
            << " | VX: " << track.vx()
            << " | VY: " << track.vy()
            << " | Missed: " << track.missedFrames
            << '\n';
    }
}

const std::vector<Track>& TrackManager::getTracks() const {
    return tracks_;
}
