#include "system/system_info.h"

#ifdef NEBULA_LINUX
#include <sys/times.h>
#include <sys/sysinfo.h>
#include <sys/vtimes.h>

namespace nebula {
    string SystemInfo::getCpuModel() {
        string line;
        std::ifstream finfo("/proc/cpuinfo");
        while(getline(finfo,line)) {
            stringstream str(line);
            string itype;
            string info;
            if ( getline( str, itype, ':' ) && getline(str,info) && itype.substr(0,10) == "model name" ) {
                return info;
            }
        }
        return "ERR";
    }

    SystemInfo::Data SystemInfo::getSystemInfo() {
        _systemInfo.cpuUsage = getCpuUsage();
        _systemInfo.ramUsage = getRamUsage();

        return _systemInfo;
    }

    int SystemInfo::parseRamUsageLine(char *line) {
        int i = (int)strlen(line);
        const char* p = line;
        while (*p <'0' || *p > '9') p++;
        line[i-3] = '\0';
        i = atoi(p);
        return i;
    }

    long SystemInfo::getRamUsage() {
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while (fgets(line, 128, file) != nullptr){
            if (strncmp(line, "VmRSS:", 6) == 0){
                result = parseRamUsageLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    void SystemInfo::init(){
        FILE* file;
        struct tms timeSample{};
        char line[128];

        struct sysinfo si{};
        sysinfo(&si);

        _systemInfo.totalRam = si.totalram;

        _cpuTimes.lastCpu = times(&timeSample);
        _cpuTimes.lastSysCpu = timeSample.tms_stime;
        _cpuTimes.lastUserCpu = timeSample.tms_utime;

        file = fopen("/proc/cpuinfo", "r");
        while(fgets(line, 128, file) != nullptr){
            if (strncmp(line, "processor", 9) == 0) _cpuTimes.numProcs++;
        }
        fclose(file);
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
