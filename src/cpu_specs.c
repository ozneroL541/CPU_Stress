#include "cpu_specs.h"

unsigned short get_cpu_threads(void) {
    unsigned short threads = 1;

    #ifdef _WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        threads = (unsigned short)sysinfo.dwNumberOfProcessors;
    #elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        threads = (unsigned short)sysconf(_SC_NPROCESSORS_ONLN);
    #endif

    return threads;
}
