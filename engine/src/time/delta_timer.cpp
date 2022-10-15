#include <profiling/profiler.h>
#include "time/delta_timer.h"

namespace nebula {
    DeltaTimer::DeltaTimer() : _initial(HRChrono::now()), _deltaTime(1), _lastFrameTime(_initial), _frameData() {}

    void DeltaTimer::update() {
        auto currentFrame = HRChrono::now();
        _deltaTime = currentFrame - _lastFrameTime;

        _lastFrameTime = currentFrame;

        _frameData.ms = _deltaTime.count();
        _frameData.fps = 1 / _frameData.ms * 1000;
    }

    DeltaTimer::FrameData DeltaTimer::getFrameData() {
        return _frameData;
    }

    double DeltaTimer::delta() {
        return _frameData.ms;
    }
}
