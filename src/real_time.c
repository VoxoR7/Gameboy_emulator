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
#else
    NOT DEFINED
#endif

#define SYNC (16.742005692281935375858027791729 * 1000.0) // us (59.73 Hz)

uint64_t sync;
uint64_t frame;

#ifdef __WIN
    LARGE_INTEGER last_lppc;
    #ifdef __DEBUG
        LARGE_INTEGER debug_lppc;
        LARGE_INTEGER st_lppc;
    #endif
#else
    NOT DEFINED
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

    #ifdef __DEBUG
        if ( !QueryPerformanceCounter( &last_lppc)) {

            fprintf( stderr, "[FATAL] unable to call the high performance counter\n");
            exit( EXIT_FAILURE);
        }

        debug_lppc.QuadPart = last_lppc.QuadPart;
        st_lppc.QuadPart = last_lppc.QuadPart;
    #else
        QueryPerformanceCounter( &last_lppc);
    #endif
}

extern void real_time_wait( void) {

    #ifdef __WIN
        LARGE_INTEGER lppc;
    #else
        NOT DEFINED
    #endif

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

    //fprintf( stdout, "[INFO] %"PRIu64" > %"PRIu64"\n", (uint64_t)(frame * SYNC), sync + 1000);

    if ( frame * SYNC > sync + 1000) {
        //fprintf( stdout, "[INFO] Delay : %"PRIu64"\n", (((uint64_t)(frame * SYNC)) - (sync + 1000)) / 1000);
        SDL_Delay( (((uint64_t)(frame * SYNC)) - (sync + 1000)) / 1000);
    }

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

    //fprintf( stdout, "[INFO] %"PRIu64" > %"PRIu64"\n", (uint64_t)(frame * SYNC), sync);

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

        //fprintf( stdout, "[INFO] %"PRIu64" > %"PRIu64"\n", lppc.QuadPart / 10000, ( debug_lppc.QuadPart / 10000) + 1000);
        if ( lppc.QuadPart / 10000 > ( debug_lppc.QuadPart / 10000) + 1000) {

            fprintf( stdout, "[INFO] 1 second ellapsed info :\n");
            //fprintf( stdout, "[INFO]\t\ttime ellapsed  : %5"PRIu64".%"PRIu64"\n", (lppc.QuadPart - st_lppc.QuadPart) / 10000000, ((lppc.QuadPart - st_lppc.QuadPart) / 10000) % 1000);
            fprintf( stdout, "[INFO]\t\ttime ellapsed  : %5"PRIu64".%"PRIu64"\n", (sync / 100000) / 10, (sync / 10000) % 100);
            fprintf( stdout, "[INFO]\t\tframe ellapsed : %"PRIu64"\n", frame - lf);

            lf = frame;
            debug_lppc.QuadPart += 10000000;
        }
    #endif
}