#pragma once

#include <GL/glew.h>
#include <cstring>
#include <fstream>
#include <sstream>

#include "memory/containers.h"
#include "engine_platforms.h"
#include "injector/injectable.h"
#include "logging/logger.h"

namespace nebula {
    class SystemInfo : public Injectable {
    public:
        SystemInfo() = default;

        struct Data {
            GLuint64 totalRam{};

            string cpuModel{};
            string gpuModel{};
            string openGl{};

            long ramUsage{0};
            double cpuUsage{0.0};
        };

        struct CpuTimes {
            clock_t lastCpu{0}, lastSysCpu{0}, lastUserCpu{0};
            int numProcs{0};
        };

        Data getSystemInfo();
        double getCpuUsage();
        long getRamUsage();

    private:
        Data _systemInfo{};
        CpuTimes _cpuTimes{};

        int parseRamUsageLine(char* line);

        void init();

        string getCpuModel();

        Logger* _logger{nullptr};
        Configuration* _config{nullptr};

    NEBULA_INJECTABLE(SystemInfo);
    };
}
