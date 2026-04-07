#include <vector>

class CpuMonitor {
    private:
        size_t previous_idle;
        size_t previous_total;
        std::vector<size_t> get_cpu_times();
        bool get_cpu_times(size_t &idle_time, size_t &total_time);
    public:
        float get_usage();
};
