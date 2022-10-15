#include "concurrency/messaging.h"

#include "vector/vector_utils.h"
#include "concurrency/thread_pool.h"

namespace nebula {
    void Messaging::mapDependencies(EnvironmentVars& globalEnv) {
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
        mapFactory<FixedTimer, double>(&_fixedTimer, 1.0 / 300.0);
    }

    void Messaging::onConstruct() {
    }

    void Messaging::update() {
        NEBULA_PROFILE;

        if (!_fixedTimer->shouldUpdate()) {
            return;
        }

        // TODO: Add message dispatching for supporting sending messages from other threads, currently only callbacks are supported

        _fixedTimer->update();
    }
}
