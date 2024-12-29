#include "main.h"
#include "real_time.h"

#include <stdio.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <inttypes.h>

#include <SDL2/SDL.h>

#ifdef __linux__
#define __USE_POSIX199309
#include <time.h>
#elifdef _WIN32
#include "windows.h"
#else
#error "target unknow"
#endif

#define TIME_MULT 1 // for debug purpose only. Should be 1
#define SYNC ((16.742005692281935375858027791729 * 1000.0) / TIME_MULT) // us (59.73 Hz)

uint64_t sync;
uint64_t frame;

#ifdef __linux__
struct timespec last_lppc;
#ifdef DEBUG
struct timespec debug_lppc;
#endif
#elifdef _WIN32
LARGE_INTEGER last_lppc;
#ifdef DEBUG
LARGE_INTEGER debug_lppc;
#endif
#else
#error "target unknow"
#endif

static void real_time_destroy(void) {
    return;
}

void real_time_reset(void) {
    sync = 0;
    frame = 0;
}

void real_time_init(void) {
    atexit(real_time_destroy);
    real_time_reset();
}

void real_time_start(void) {
    #ifdef __linux__
    #ifdef DEBUG
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &last_lppc)) {
        LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
        exit(EXIT_FAILURE);
    }

    debug_lppc = last_lppc;
    #else
    clock_gettime(CLOCK_MONOTONIC_RAW, &last_lppc);
    #endif  
    #elifdef _WIN32
    #ifdef DEBUG
    if (!QueryPerformanceCounter(&last_lppc)) {
        LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
        exit(EXIT_FAILURE);
    }

    debug_lppc.QuadPart = last_lppc.QuadPart;
    #else
    QueryPerformanceCounter(&last_lppc);
    #endif
    #else
    #error "target unknow"
    #endif
}

void real_time_wait(void) {
    #ifdef __linux__
    struct timespec lppc;

    #ifdef DEBUG
    static uint64_t lf = 0;
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &lppc)) {
        LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
        exit(EXIT_FAILURE);
    }
    #else
    clock_gettime(CLOCK_MONOTONIC_RAW, &lppc);
    #endif

    sync += ((lppc.tv_sec * 1000000000  + lppc.tv_nsec) - (last_lppc.tv_sec * 1000000000 + last_lppc.tv_nsec)) / 1000;
    last_lppc = lppc;
    frame++;

    if (frame * SYNC > sync + 1000)
        SDL_Delay((((uint64_t)(frame * SYNC)) - (sync + 1000)) / 1000);

    #ifdef DEBUG
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &lppc)) {
        LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
        exit(EXIT_FAILURE);
    }
    #else
    clock_gettime(CLOCK_MONOTONIC_RAW, &lppc);
    #endif

    sync += ((lppc.tv_sec * 1000000000  + lppc.tv_nsec) - (last_lppc.tv_sec * 1000000000 + last_lppc.tv_nsec)) / 1000;
    last_lppc = lppc;

    while (frame * SYNC > sync) {
        SDL_Delay(1);

        #ifdef DEBUG
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &lppc)) {
            LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
            exit(EXIT_FAILURE);
        }
        #else
        clock_gettime(CLOCK_MONOTONIC_RAW, &lppc);
        #endif

        sync += ((lppc.tv_sec * 1000000000  + lppc.tv_nsec) - (last_lppc.tv_sec * 1000000000 + last_lppc.tv_nsec)) / 1000;
        last_lppc = lppc;
    }

    #ifdef DEBUG
    if ((lppc.tv_sec * 1000000000 + lppc.tv_nsec) / 1000 > ((debug_lppc.tv_sec * 1000000000 + debug_lppc.tv_nsec) / 1000) + 1000000) {
        LOG_MESG(LOG_DEBUG, "1 second ellapsed info");
        LOG_MESG(LOG_DEBUG, "\t\ttime ellapsed  : %5"PRIu64".%"PRIu64"", sync / 1000000, (sync / 10000) % 100);
        LOG_MESG(LOG_DEBUG, "\t\tframe ellapsed : %"PRIu64"", frame - lf);

        lf = frame;
        debug_lppc.tv_sec += 1;
    }
    #endif
    #elifdef _WIN32
    LARGE_INTEGER lppc;

    #ifdef DEBUG
    static uint64_t lf = 0;

    if (!QueryPerformanceCounter(&lppc)) {
        LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
        exit(EXIT_FAILURE);
    }
    #else
    QueryPerformanceCounter(&lppc);
    #endif

    sync += (lppc.QuadPart - last_lppc.QuadPart) / 10;
    last_lppc.QuadPart = lppc.QuadPart;
    frame++;

    if (frame * SYNC > sync + 1000)
        SDL_Delay((((uint64_t)(frame * SYNC)) - (sync + 1000)) / 1000);

    #ifdef DEBUG
    if (!QueryPerformanceCounter(&lppc)) {
        LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
        exit(EXIT_FAILURE);
    }
    #else
    QueryPerformanceCounter(&lppc);
    #endif

    sync += (lppc.QuadPart - last_lppc.QuadPart) / 10;
    last_lppc.QuadPart = lppc.QuadPart;

    while (frame * SYNC > sync) {
        SDL_Delay(1);

        #ifdef DEBUG
        if (!QueryPerformanceCounter(&lppc)) {
            LOG_MESG(LOG_FATAL, "Unable to call the high performance counter");
            exit(EXIT_FAILURE);
        }
        #else
        QueryPerformanceCounter(&lppc);
        #endif

        sync += (lppc.QuadPart - last_lppc.QuadPart) / 10;
        last_lppc.QuadPart = lppc.QuadPart;
    }

    #ifdef DEBUG
    if (lppc.QuadPart / 10000 > (debug_lppc.QuadPart / 10000) + 1000) {
        LOG_MESG(LOG_DEBUG, "1 second ellapsed info");
        LOG_MESG(LOG_DEBUG, "\t\ttime ellapsed  : %5"PRIu64".%"PRIu64"", (sync / 100000) / 10, (sync / 10000) % 100);
        LOG_MESG(LOG_DEBUG, "\t\tframe ellapsed : %"PRIu64"", frame - lf);

        lf = frame;
        debug_lppc.QuadPart += 10000000;
    }
    #endif
    #else
    #error "target unknow"
    #endif
}