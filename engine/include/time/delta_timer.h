#pragma once

#include "injector/injectable.h"
#include "time/time_types.h"

namespace nebula {
    class DeltaTimer : public Injectable {
        HRChrono::time_point _initial;

        std::chrono::duration<double, std::ratio<1, 1000>> _deltaTime;
        HRChrono::time_point _lastFrameTime;

    public:
        struct FrameData {
            double ms{0.0};
            long fps{0};
        };

        DeltaTimer();

        FrameData getFrameData();
        double delta();
        void update();

    NEBULA_INJECTABLE_E(DeltaTimer);

    private:
        FrameData _frameData;
    };
}
