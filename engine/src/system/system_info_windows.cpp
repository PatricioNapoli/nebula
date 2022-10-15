#include "system/system_info.h"

#ifdef NEBULA_WINDOWS

#include <sys/times.h>
#include <sys/sysinfo.h>
#include <sys/vtimes.h>

namespace nebula {
    string SystemInfo::getCpuModel() {
        return "ERR";
    }

    SystemInfo::Data SystemInfo::getSystemInfo() {
        return _systemInfo;
    }

    int SystemInfo::parseRamUsageLine(char *line) {
        return 0;
    }

    int SystemInfo::getRamUsage() {
        return 0;
    }

    void SystemInfo::initCpuTimes(){
    }

    double SystemInfo::getCpuUsage(){
        return 0.0;
    }
}

#endif
