#ifndef CPU_SPECS_H
#define CPU_SPECS_H

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #include <unistd.h>
#endif

/** 
 * Get the number of CPU threads available on the system.
 * @return Number of CPU threads available
 */
unsigned short get_cpu_threads(void);

#endif
