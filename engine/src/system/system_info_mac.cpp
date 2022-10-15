#include "system/system_info.h"

#ifdef NEBULA_MACOS

#include <sys/times.h>
#include <sys/sysctl.h>
#include <mach/mach.h>

namespace nebula {
    string SystemInfo::getCpuModel() {
        char buffer[128];
        size_t bufferlen = 128;
        sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferlen, nullptr, 0);
        return buffer;
    }

    SystemInfo::Data SystemInfo::getSystemInfo() {
        _systemInfo.cpuUsage = getCpuUsage();
        _systemInfo.ramUsage = getRamUsage();

        return _systemInfo;
    }

    int SystemInfo::parseRamUsageLine(char *line) {
        return 0;
    }

    long SystemInfo::getRamUsage() {
        struct mach_task_basic_info info{};
        mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
        if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
                        (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
            return (size_t)0L;
        return (size_t)info.resident_size / 1000;
    }

    void SystemInfo::init(){
        int mib[2];
        int64_t physical_memory;
        size_t length;

        mib[0] = CTL_HW;
        mib[1] = HW_MEMSIZE;
        length = sizeof(int64_t);
        sysctl(mib, 2, &physical_memory, &length, nullptr, 0);

        _systemInfo.totalRam = physical_memory;

        struct tms timeSample{};

        _cpuTimes.lastCpu = times(&timeSample);
        _cpuTimes.lastSysCpu = timeSample.tms_stime;
        _cpuTimes.lastUserCpu = timeSample.tms_utime;

        int mibCpu[4];
        int numCPU;
        std::size_t len = sizeof(numCPU);
        mibCpu[0] = CTL_HW;
        mibCpu[1] = HW_NCPU;
        sysctl(mibCpu, 2, &numCPU, &len, nullptr, 0);
        if (numCPU < 1)
        {
            mibCpu[1] = HW_NCPU;
            sysctl(mibCpu, 2, &numCPU, &len, nullptr, 0);
            if (numCPU < 1)
                numCPU = 1;
        }
        _cpuTimes.numProcs = numCPU;
    }

    double SystemInfo::getCpuUsage(){
        struct tms timeSample{};
        clock_t now;
        double percent{0};

        now = times(&timeSample);
        if (now <= _cpuTimes.lastCpu || timeSample.tms_stime < _cpuTimes.lastSysCpu ||
            timeSample.tms_utime < _cpuTimes.lastUserCpu){
            //Overflow detection. Just skip this value.
            percent = 0.0;
        }
        else{
            percent = (timeSample.tms_stime - _cpuTimes.lastSysCpu) +
                      (timeSample.tms_utime - _cpuTimes.lastUserCpu);
            percent /= (now - _cpuTimes.lastCpu);
            percent /= _cpuTimes.numProcs;
            percent *= 100;
        }
        _cpuTimes.lastCpu = now;
        _cpuTimes.lastSysCpu = timeSample.tms_stime;
        _cpuTimes.lastUserCpu = timeSample.tms_utime;

        return percent;
    }
}

#endif
