#ifndef THREADS_H
#define THREADS_H
    #ifdef _WIN32
        #include <windows.h>
        #define THREAD_RETURN DWORD WINAPI
        typedef HANDLE thread_t;
    #else
        #include <pthread.h>
        #define THREAD_RETURN void *
        typedef pthread_t thread_t;
    #endif
#endif
