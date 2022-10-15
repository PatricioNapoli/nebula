#include "time/fixed_timer.h"

namespace nebula {
    FixedTimer::FixedTimer(double ratio) : _initial(HRChrono::now()) {
        _ratio = std::chrono::duration_cast<Milliseconds>(std::chrono::milliseconds(int(ratio * 1000)));
    }

    bool FixedTimer::shouldUpdate() {
        return HRChrono::now() > _expectedTime;
    }

    void FixedTimer::update() {
        auto currentFrame = HRChrono::now();
        _expectedTime = currentFrame + _ratio;
    }
}
