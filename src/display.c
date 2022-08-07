#include "main.h"
#include "display.h"

#include "memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MULT 3

#define WIDTH (160 * MULT)
#define HEIGHT (144 * MULT)

#define WIDTH_2D 1080
#define HEIGHT_2ND 720

#define NUMBER_X 16
#define SPACE 8
#define SIZE_X 32
#define TILE_TO_DRAW 512

struct sprite {

    int16_t y;
    int16_t x;
    uint16_t tile;
    uint8_t flags;
};

SDL_Renderer *renderer = NULL;
SDL_Renderer *renderer2nd = NULL;

SDL_Window *window = NULL;
SDL_Window *window2nd = NULL;

SDL_Surface *surface_line = NULL;

TTF_Font *font = NULL;

void display_destroy( void) {

    display_second_destroy();

    if ( window != NULL) {

        SDL_DestroyWindow( window);
        window = NULL;
    }

    if ( renderer != NULL) {

        SDL_DestroyRenderer( renderer);
        renderer = NULL;
    }

    if ( surface_line != NULL) {

        SDL_FreeSurface( surface_line);
        surface_line = NULL;
    }

    if ( font != NULL) {

        TTF_CloseFont( font);
        font = NULL;
    }

    TTF_Quit();
    SDL_Quit();

    return;
}

extern void display_init( void) {

    if ( SDL_Init( SDL_INIT_EVERYTHING) != 0) {

        #ifdef __DEBUG
            fprintf( stderr, "\t\tFAILURE\n");
            SDL_Log( "[FATAL] Unable to initialize SDL: %stest", SDL_GetError());
        #endif
        exit( EXIT_FAILURE);
    }

    if( TTF_Init() != 0) {

        #ifdef __DEBUG
            fprintf( stderr, "TTF_Init: %s\n", TTF_GetError());
        #endif
        SDL_Quit();
        exit( EXIT_FAILURE);
    }

    atexit( display_destroy);

    window = SDL_CreateWindow( "VoxoR Gameboy emulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if ( window == NULL) {

        #ifdef __DEBUG
            fprintf( stdout, "Could not create window: %stest\n", SDL_GetError());
        #endif
        exit( EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    if ( renderer == NULL) {

        #ifdef __DEBUG
            fprintf( stdout, "Could not create renderer: %stest\n", SDL_GetError());
        #endif
        exit( EXIT_FAILURE);
    }

    char path[2048];
    strcpy( path, main_base_path);
    #ifdef __WIN
        strcat( path, "dat\\Roboto-Black.ttf");
    #elif __LIN
        strcat( path, "dat/Roboto-Black.ttf");
    #endif
    font = TTF_OpenFont( path, 16);
    if ( font == NULL) {

        #ifdef __DEBUG
            fprintf( stdout, "Could not open font\n");
        #endif
        exit( EXIT_FAILURE);
    }

    surface_line = SDL_CreateRGBSurface( 0, 160, 144, 32, 0, 0, 0, 0);
}

extern void display_second_destroy( void) {

    if ( window2nd != NULL) {

        SDL_DestroyWindow( window2nd);
        window2nd = NULL;
    }

    if ( renderer2nd != NULL) {

        SDL_DestroyRenderer( renderer2nd);
        renderer2nd = NULL;
    }

    return;
}

extern void display_second_init( void) {

    if ( window2nd != NULL || renderer2nd != NULL) {

        #ifdef __DEBUG
            fprintf( stdout, "2nd display already up !\n");
        #endif

        return;
    }

    window2nd = SDL_CreateWindow( "VoxoR Gameboy emulation 2nd display", 100, 100, NUMBER_X * SIZE_X + NUMBER_X * SPACE - SPACE, (TILE_TO_DRAW / NUMBER_X) * SIZE_X + (TILE_TO_DRAW / NUMBER_X) * SPACE - SPACE, SDL_WINDOW_OPENGL);
    if ( window2nd == NULL) {

        #ifdef __DEBUG
            fprintf( stdout, "Could not create window: %s\n", SDL_GetError());
        #endif
        exit( EXIT_FAILURE);
    }

    renderer2nd = SDL_CreateRenderer( window2nd, -1, SDL_RENDERER_ACCELERATED);
    if ( renderer2nd == NULL) {

        #ifdef __DEBUG
            fprintf( stdout, "Could not create renderer: %sx\n", SDL_GetError());
        #endif
        exit( EXIT_FAILURE);
    }
}

extern void display_try( void) {

    #ifdef __DEBUG
        if ( window2nd == NULL || renderer2nd == NULL) {

            #ifdef __DEBUG
                fprintf( stdout, "2nd display not up !\n");
            #endif

            return;
        }
    #endif

    for ( uint16_t j = 0; j < TILE_TO_DRAW; j++) {

        SDL_Surface *stest = SDL_CreateRGBSurface( 0, 8, 8, 32, 0, 0, 0, 0);

        for ( uint8_t i = 0; i < 8; i++) {

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b10000000) >> 7) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b10000000) >> 7) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 0 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 0 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 0 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 0 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b01000000) >> 6) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b01000000) >> 6) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 1 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 1 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 1 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 1 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b00100000) >> 5) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b00100000) >> 5) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 2 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 2 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 2 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 2 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b00010000) >> 4) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b00010000) >> 4) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 3 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 3 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 3 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 3 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %stest %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b00001000) >> 3) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b00001000) >> 3) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 4 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 4 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 4 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 4 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %stest %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b00000100) >> 2) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b00000100) >> 2) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 5 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 5 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 5 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 5 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %stest %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( ((memory_read8( (0x8001 + j * 16) + i * 2) & 0b00000010) >> 1) * 2 + ((memory_read8( (0x8000 + j * 16) + i * 2) & 0b00000010) >> 1) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 6 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 6 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 6 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 6 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %stest %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

            switch ( (memory_read8( (0x8001 + j * 16) + i * 2) & 0b00000001) * 2 + (memory_read8( (0x8000 + j * 16) + i * 2) & 0b00000001)) {
                case 0:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 7 * stest->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 7 * stest->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 7 * stest->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) stest->pixels + i * stest->pitch + 7 * stest->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %stest %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }
        }

        /**((uint32_t *) ((uint8_t *) stest->pixels + 0 * stest->pitch + 0 * stest->format->BytesPerPixel)) = 0xFFFFFFFF; // y x
        *((uint32_t *) ((uint8_t *) stest->pixels + 1 * stest->pitch + 0 * stest->format->BytesPerPixel)) = 0xFFFFFFFF; // y x
        *((uint32_t *) ((uint8_t *) stest->pixels + 0 * stest->pitch + 1 * stest->format->BytesPerPixel)) = 0xFFFFFFFF; // y x
        *((uint32_t *) ((uint8_t *) stest->pixels + 1 * stest->pitch + 1 * stest->format->BytesPerPixel)) = 0xFFFFFFFF; // y x*/

        SDL_Texture *ttest = SDL_CreateTextureFromSurface( renderer2nd, stest);
        SDL_FreeSurface( stest);

        SDL_Rect rtest;

        rtest.x = (((uint8_t)(j % NUMBER_X)) * SIZE_X + (j % NUMBER_X) * SPACE);
        rtest.y = (((uint8_t)(j / NUMBER_X)) * SIZE_X + ((uint8_t)(j / NUMBER_X)) * SPACE);
        rtest.w = SIZE_X;
        rtest.h = SIZE_X;

        SDL_SetRenderDrawColor( renderer2nd, 0, 0, 0, 255);

        SDL_RenderCopy( renderer2nd, ttest, NULL, &rtest);

        SDL_DestroyTexture( ttest);
    }

    SDL_RenderPresent( renderer2nd);
    SDL_RenderClear( renderer2nd);
}

extern void display_draw_line_background( uint8_t ly) {

    uint16_t tile[20];

    for ( uint8_t i = 0; i < 20; i++)
        tile[i] = memory_read8( 0x9800 + i + ((ly / 8) * 32)) * 16;

    for ( uint8_t i = 0; i < 20; i++) {

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b10000000) >> 7) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b10000000) >> 7) ) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (0 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (0 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (0 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (0 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b01000000) >> 6) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b01000000) >> 6) ) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (1 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (1 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (1 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (1 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n",__FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b00100000) >> 5) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b00100000) >> 5) ) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (2 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *)surface_line->pixels + ly * surface_line->pitch + (2 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *)surface_line->pixels + ly * surface_line->pitch + (2 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *)surface_line->pixels + ly * surface_line->pitch + (2 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b00010000) >> 4) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b00010000) >> 4) ) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (3 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (3 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (3 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (3 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b00001000) >> 3) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b00001000) >> 3) ) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (4 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (4 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (4 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (4 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b00000100) >> 2) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b00000100) >> 2) ) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (5 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (5 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (5 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (5 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( ((memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b00000010) >> 1) * 2 + ((memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b00000010) >> 1)) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (6 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (6 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (6 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (6 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }

        switch ( (memory_read8(0x8001 + tile[i] + ((ly % 8) * 2)) & 0b00000001) * 2 + (memory_read8(0x8000 + tile[i] + ((ly % 8) * 2)) & 0b00000001)) {
            case 0:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (7 + i * 8) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                break;
            case 1:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (7 + i * 8) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                break;
            case 2:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (7 + i * 8) * surface_line->format->BytesPerPixel)) = 0x55555555;
                break;
            case 3:

                *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (7 + i * 8) * surface_line->format->BytesPerPixel)) = 0x00000000;
                break;
            #ifdef __DEBUG
            default:

                fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                exit( EXIT_FAILURE);
            #endif
        }
    }
}

extern void display_draw_line_sprite( uint8_t ly) {

    struct sprite sprite[40];
    uint8_t sprite_count = 0;

    for ( uint8_t i = 0; i < 40; i++) {

        if ( memory_read8( 0xFE00 + i * 4) - 16 <= ly && memory_read8( 0xFE00 + i * 4) - 8 > ly) {

            sprite[sprite_count].y = memory_read8( 0xFE00 + i * 4) - 16;
            sprite[sprite_count].x = memory_read8( 0xFE01 + i * 4) - 8;
            sprite[sprite_count].tile = memory_read8( 0xFE02 + i * 4) * 16;
            sprite[sprite_count].flags = memory_read8( 0xFE03 + i * 4);
            sprite_count++;
        }
    }

    for ( uint8_t i = 0; i < sprite_count; i++) {

        if ( i == 0 && sprite[i].tile == 0 && ly == 42)
            fprintf( stdout, "tile sprite[i].y 0x%02x\n", sprite[i].y);

        if ( sprite[i].x >= 0 && sprite[i].x <= 159)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2) ) & 0b10000000) >> 7) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b10000000) >> 7) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + sprite[i].x * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + sprite[i].x * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + sprite[i].x * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + sprite[i].x * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -1 && sprite[i].x <= 158)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b01000000) >> 6) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b01000000) >> 6) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 1) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 1) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 1) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 1) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n",__FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -2 && sprite[i].x <= 157)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00100000) >> 5) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00100000) >> 5) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 2) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *)surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 2) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *)surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 2) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *)surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 2) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -3 && sprite[i].x <= 156)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00010000) >> 4) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00010000) >> 4) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 3) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 3) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 3) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 3) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -4 && sprite[i].x <= 155)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00001000) >> 3) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00001000) >> 3) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 4) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 4) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 4) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 4) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -5 && sprite[i].x <= 154)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00000100) >> 2) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00000100) >> 2) ) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 5) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 5) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 5) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 5) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -6 && sprite[i].x <= 153)
            switch ( ((memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00000010) >> 1) * 2 + ((memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00000010) >> 1)) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 6) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 6) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 6) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 6) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }

        if ( sprite[i].x >= -7 && sprite[i].x <= 152)
            switch ( (memory_read8(0x8001 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00000001) * 2 + (memory_read8(0x8000 + sprite[i].tile + ((ly - sprite[i].y) * 2)) & 0b00000001)) {
                case 0:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 7) * surface_line->format->BytesPerPixel)) = 0xFFFFFFFF;
                    break;
                case 1:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 7) * surface_line->format->BytesPerPixel)) = 0xAAAAAAAA;
                    break;
                case 2:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 7) * surface_line->format->BytesPerPixel)) = 0x55555555;
                    break;
                case 3:

                    *((uint32_t *) ((uint8_t *) surface_line->pixels + ly * surface_line->pitch + (sprite[i].x + 7) * surface_line->format->BytesPerPixel)) = 0x00000000;
                    break;
                #ifdef __DEBUG
                default:

                    fprintf( stdout, "[FATAL] %s %d\n", __FILE__, __LINE__);
                    exit( EXIT_FAILURE);
                #endif
            }
    }
}

extern void display_draw_final() {

    SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, surface_line);
    SDL_RenderCopy( renderer, texture, NULL, NULL);

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
    SDL_RenderPresent( renderer);
    SDL_RenderClear( renderer);

    SDL_DestroyTexture( texture);

    return;
}

extern void display_blank() {

    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
    SDL_RenderPresent( renderer);
    SDL_RenderClear( renderer);
}

extern void display_draw_menu( const char *rom_selected, const uint8_t audio_level) {

    SDL_Color color = {255, 255, 255, 255};        
    SDL_Surface *s_rom = TTF_RenderText_Blended( font, rom_selected, color);
    SDL_Texture *t_rom = SDL_CreateTextureFromSurface( renderer, s_rom);

    char text_audio[128];
    sprintf( text_audio, "audio level : %"PRIu8"%%", audio_level);

    SDL_Surface *s_audio = TTF_RenderText_Blended( font, text_audio, color);
    SDL_Texture *t_audio = SDL_CreateTextureFromSurface( renderer, s_audio);

    SDL_Rect drect;
    drect.x = WIDTH / 4;
    drect.y = (HEIGHT / 4) * 3;
    drect.w = WIDTH / 2;
    drect.h = HEIGHT / 4;

    SDL_RenderCopy(renderer, t_rom, NULL, &drect);

    drect.x = WIDTH / 8;
    drect.y = HEIGHT / 4;
    drect.w = ( WIDTH / 8.0) * 6;
    drect.h = HEIGHT / 4;
    SDL_RenderCopy(renderer, t_audio, NULL, &drect);
    SDL_RenderPresent( renderer);
    SDL_RenderClear( renderer);

    SDL_FreeSurface( s_rom);
    SDL_DestroyTexture( t_rom);
    SDL_FreeSurface( s_audio);
    SDL_DestroyTexture( t_audio);
}