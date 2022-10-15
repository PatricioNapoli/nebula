#include "time/system_time.h"

namespace nebula {
    string SystemTime::getToday() const {
        auto today = SChrono::to_time_t(SChrono::now());
        stringstream ss;
        ss << std::put_time(std::localtime(&today), "%Y-%m-%d %X");
        return ss.str();
    }

    time_t SystemTime::getEpoch() const {
        auto epoch = SChrono::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
    }
}
