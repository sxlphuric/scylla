#include "CpuMonitor.hpp"

#include <iostream>
#include <fstream>

#include <vector>
#include <numeric>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

std::vector<size_t> CpuMonitor::get_cpu_times() {
    #ifdef _WIN32
    // TODO: Pull out windows machine
    #else
        std::ifstream proc_stat("/proc/stat");
        proc_stat.ignore(5, ' ');
        std::vector<size_t> times;
        for (size_t time; proc_stat >> time; times.push_back(time));
        return times;
    #endif
}

bool CpuMonitor::get_cpu_times(size_t &idle_time, size_t &total_time) {
    const std::vector<size_t> cpu_times = get_cpu_times();
    if (cpu_times.size() < 4) return false;

    idle_time = cpu_times[3];
    total_time = std::accumulate(cpu_times.begin(),cpu_times.end(), 0);
    return true;
}

float CpuMonitor::get_usage() {
    for (size_t idle, total; get_cpu_times(idle, total); sleep(1)) {
        const float idle_delta = idle - CpuMonitor::previous_idle;
        const float total_delta = idle - CpuMonitor::previous_total;
        const float usage = 100.0f * (1.0f - idle_delta / total_delta);

        CpuMonitor::previous_idle = idle;
        CpuMonitor::previous_total = total;

        return usage;
    }
}
