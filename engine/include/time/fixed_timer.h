#pragma once

#include "injector/injectable.h"
#include "time/time_types.h"

namespace nebula {
    class FixedTimer : public Injectable {
        HRChrono::time_point _initial;
        HRChrono::time_point _expectedTime;

        Milliseconds _ratio{0};

    public:
        FixedTimer() = default;
        explicit FixedTimer(double ratio);

        bool shouldUpdate();
        void update();

        NEBULA_INJECTABLE_E(FixedTimer);
    };
}
