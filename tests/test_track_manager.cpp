#include "TrackManager.hpp"

#include <cassert>

int main() {
    TrackManager manager;

    // First detection creates track 101.
    manager.update({{10.0, 2.0}}, 1.0);
    assert(manager.getTracks().size() == 1);
    assert(manager.getTracks()[0].id == 101);

    // A missed detection should not create a replacement track or change its ID.
    manager.update({}, 1.0);
    assert(manager.getTracks().size() == 1);
    assert(manager.getTracks()[0].id == 101);
    assert(manager.getTracks()[0].missedFrames == 1);

    // Reassociate the same object to confirm the ID remains stable.
    manager.update({{10.1, 2.1}}, 1.0);
    assert(manager.getTracks().size() == 1);
    assert(manager.getTracks()[0].id == 101);
    assert(manager.getTracks()[0].missedFrames == 0);

    // Unmatched detections should receive the next free ID, not reuse 101.
    manager.update({{20.0, -1.0}}, 1.0);
    assert(manager.getTracks().size() == 2);
    assert(manager.getTracks()[0].id == 101);
    assert(manager.getTracks()[1].id == 102);

    return 0;
}
