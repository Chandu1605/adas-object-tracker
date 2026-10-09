
#pragma once

#include "Track.hpp"
#include <fstream>
#include <string>
#include <vector>

class CSVLogger {
public:
    explicit CSVLogger(const std::string& filename);

    void log(int frame, const std::vector<Track>& tracks);

private:
    std::ofstream file_;
};
