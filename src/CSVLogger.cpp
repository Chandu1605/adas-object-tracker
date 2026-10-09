
#include "CSVLogger.hpp"

#include <iomanip>
#include <stdexcept>

CSVLogger::CSVLogger(const std::string& filename)
    : file_(filename) {

    if (!file_) {
        throw std::runtime_error("Unable to open CSV file");
    }

    file_ << "frame,id,x,y,vx,vy,missed\n";
    file_ << std::fixed << std::setprecision(3);
}

void CSVLogger::log(
    int frame,
    const std::vector<Track>& tracks
) {
    for (const auto& track : tracks) {
        file_ << frame << ","
              << track.id << ","
              << track.x() << ","
              << track.y() << ","
              << track.vx() << ","
              << track.vy() << ","
              << track.missedFrames << "\n";
    }
}
