#include "main.h"
#include "real_time.h"

#include <stdio.h>
#ifdef __DEBUG
    #include <stdio.h>
#endif
#include <stdlib.h>
#include <inttypes.h>

#include <SDL2/SDL.h>

#ifdef __WIN
    #include "windows.h"
#elif __LIN
    #define __USE_POSIX199309
    #include <time.h>
#endif

#ifndef __DEBUG
    #define __DEBUG
#endif

#define TIME_MULT 1 // for debug purpose only. Should be 1
#define SYNC ((16.742005692281935375858027791729 * 1000.0) / TIME_MULT) // us (59.73 Hz)

uint64_t sync;
uint64_t frame;

#ifdef __WIN
    LARGE_INTEGER last_lppc;
    #ifdef __DEBUG
        LARGE_INTEGER debug_lppc;
    #endif
#elif __LIN
    struct timespec last_lppc;
    #ifdef __DEBUG
        struct timespec debug_lppc;
    #endif
#endif

static void real_time_destroy( void) {

    return;
}

extern void real_time_reset( void) {

    sync = 0;
    frame = 0;
}

extern void real_time_init( void) {

    atexit( real_time_destroy);

    real_time_reset();
}

extern void real_time_start( void) {

    #ifdef __WIN
        #ifdef __DEBUG
            if ( !QueryPerformanceCounter( &last_lppc)) {

                fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                exit( EXIT_FAILURE);
            }

            debug_lppc.QuadPart = last_lppc.QuadPart;
        #else
            QueryPerformanceCounter( &last_lppc);
        #endif
    #elif __LIN
        #ifdef __DEBUG
            if ( clock_gettime( CLOCK_MONOTONIC_RAW, &last_lppc)) {

                fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                exit( EXIT_FAILURE);
            }

            debug_lppc = last_lppc;
        #else
            clock_gettime( CLOCK_MONOTONIC_RAW, &last_lppc);
        #endif  
    #endif
}

extern void real_time_wait( void) {

    #ifdef __WIN
        LARGE_INTEGER lppc;

        #ifdef __DEBUG
            static uint64_t lf = 0;

            if ( !QueryPerformanceCounter( &lppc)) {

                fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                exit( EXIT_FAILURE);
            }
        #else
            QueryPerformanceCounter( &lppc);
        #endif

        sync += (lppc.QuadPart - last_lppc.QuadPart) / 10;
        last_lppc.QuadPart = lppc.QuadPart;
        frame++;

        if ( frame * SYNC > sync + 1000)
            SDL_Delay( (((uint64_t)(frame * SYNC)) - (sync + 1000)) / 1000);

        #ifdef __DEBUG
            if ( !QueryPerformanceCounter( &lppc)) {

                fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                exit( EXIT_FAILURE);
            }
        #else
            QueryPerformanceCounter( &lppc);
        #endif

        sync += (lppc.QuadPart - last_lppc.QuadPart) / 10;
        last_lppc.QuadPart = lppc.QuadPart;

        while ( frame * SYNC > sync) {

            SDL_Delay( 1);

            #ifdef __DEBUG
                if ( !QueryPerformanceCounter( &lppc)) {

                    fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                    exit( EXIT_FAILURE);
                }
            #else
                QueryPerformanceCounter( &lppc);
            #endif

            sync += (lppc.QuadPart - last_lppc.QuadPart) / 10;
            last_lppc.QuadPart = lppc.QuadPart;
        }

        #ifdef __DEBUG

            if ( lppc.QuadPart / 10000 > ( debug_lppc.QuadPart / 10000) + 1000) {

                fprintf( stdout, "[INFO] 1 second ellapsed info :\n");
                fprintf( stdout, "[INFO]\t\ttime ellapsed  : %5"PRIu64".%"PRIu64"\n", (sync / 100000) / 10, (sync / 10000) % 100);
                fprintf( stdout, "[INFO]\t\tframe ellapsed : %"PRIu64"\n", frame - lf);

                lf = frame;
                debug_lppc.QuadPart += 10000000;
            }
        #endif
    #elif __LIN
        struct timespec lppc;

        #ifdef __DEBUG
            static uint64_t lf = 0;

            if ( clock_gettime( CLOCK_MONOTONIC_RAW, &lppc)) {

                fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                exit( EXIT_FAILURE);
            }
        #else
            clock_gettime( CLOCK_MONOTONIC_RAW, &lppc);
        #endif

        sync += ((lppc.tv_sec * 1000000000  + lppc.tv_nsec) - (last_lppc.tv_sec * 1000000000 + last_lppc.tv_nsec)) / 1000;
        last_lppc = lppc;
        frame++;

        if ( frame * SYNC > sync + 1000)
            SDL_Delay( (((uint64_t)(frame * SYNC)) - (sync + 1000)) / 1000);

        #ifdef __DEBUG
            if ( clock_gettime( CLOCK_MONOTONIC_RAW, &lppc)) {

                fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                exit( EXIT_FAILURE);
            }
        #else
            clock_gettime( CLOCK_MONOTONIC_RAW, &lppc);
        #endif

        sync += ((lppc.tv_sec * 1000000000  + lppc.tv_nsec) - (last_lppc.tv_sec * 1000000000 + last_lppc.tv_nsec)) / 1000;
        last_lppc = lppc;

        while ( frame * SYNC > sync) {

            SDL_Delay( 1);

            #ifdef __DEBUG
                if ( clock_gettime( CLOCK_MONOTONIC_RAW, &lppc)) {

                    fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
                    exit( EXIT_FAILURE);
                }
            #else
                clock_gettime( CLOCK_MONOTONIC_RAW, &lppc);
            #endif

            sync += ((lppc.tv_sec * 1000000000  + lppc.tv_nsec) - (last_lppc.tv_sec * 1000000000 + last_lppc.tv_nsec)) / 1000;
            last_lppc = lppc;
        }

        #ifdef __DEBUG

            if ( (lppc.tv_sec * 1000000000 + lppc.tv_nsec) / 1000 > (( debug_lppc.tv_sec * 1000000000 + debug_lppc.tv_nsec) / 1000) + 1000000) {

                fprintf( stdout, "[INFO] 1 second ellapsed info :\n");
                fprintf( stdout, "[INFO]\t\ttime ellapsed  : %5"PRIu64".%"PRIu64"\n", sync / 1000000, (sync / 10000) % 100);
                fprintf( stdout, "[INFO]\t\tframe ellapsed : %"PRIu64"\n", frame - lf);

                lf = frame;
                debug_lppc.tv_sec += 1;
            }
        #endif
    #endif
}