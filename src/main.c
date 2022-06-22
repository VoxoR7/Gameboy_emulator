#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "get_rom.h"
#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "timer.h"
#include "interrupt.h"
#include "ppu.h"
#include "touche.h"
#include "audio.h"

char *main_base_path;

void main_destroy( void) {

    if ( main_base_path != NULL)
        SDL_free( main_base_path);
}

void setup_base_path( void) {

    main_base_path = SDL_GetBasePath();

    if ( strlen( main_base_path) > BASE_PATH_MAX) {

        fprintf( stderr, "[FATAL] the base path is too long !\n");
        exit( EXIT_FAILURE);
    }

    main_base_path[ strlen( main_base_path) - 1] = '\0';

    char *last_sep;
    #ifdef __WIN
        last_sep = strrchr( main_base_path, '\\');
    #elif __LIN
        last_sep = strrchr( main_base_path, '/');
    #endif

    if ( last_sep == NULL) {

        fprintf( stderr, "[FATAL] last separator can't be found !\n");
        exit( EXIT_FAILURE);
    }

    last_sep[1] = '\0';

    #ifdef __DEBUG
        fprintf( stdout, "[INFO] the base path is : %s\n", main_base_path);
    #endif
}

void banner( void) {

    fprintf( stdout, " _   _                     ______   _____                          _                                             _         _    _               \n");
    fprintf( stdout, "| | | |                    | ___ \\ |  __ \\                        | |                                           | |       | |  (_)              \n");
    fprintf( stdout, "| | | |  ___  __  __  ___  | |_/ / | |  \\/  __ _  _ __ ___    ___ | |__    ___   _   _    ___  _ __ ___   _   _ | |  __ _ | |_  _   ___   _ __  \n");
    fprintf( stdout, "| | | | / _ \\ \\ \\/ / / _ \\ |    /  | | __  / _` || '_ ` _ \\  / _ \\| '_ \\  / _ \\ | | | |  / _ \\| '_ ` _ \\ | | | || | / _` || __|| | / _ \\ | '_ \\ \n");
    fprintf( stdout, "\\ \\_/ /| (_) | >  < | (_) || |\\ \\  | |_\\ \\| (_| || | | | | ||  __/| |_) || (_) || |_| | |  __/| | | | | || |_| || || (_| || |_ | || (_) || | | |\n");
    fprintf( stdout, " \\___/  \\___/ /_/\\_\\ \\___/ \\_| \\_|  \\____/ \\__,_||_| |_| |_| \\___||_.__/  \\___/  \\__, |  \\___||_| |_| |_| \\__,_||_| \\__,_| \\__||_| \\___/ |_| |_|\n");
    fprintf( stdout, "                                                                                  __/ |                                                         \n");
    fprintf( stdout, "                                                                                 |___/                                                          \n\n");
    fprintf( stdout, "Welcome to the VoxoR Gameboy emulation\nThis is the version %d.%d.%d\n", MAJOR_VERSION, MINOR_VERSION, BUILD);

    #ifdef __DEBUG
        #ifdef __WIN
            fprintf( stdout, "target : WINDOWS\nstarting ...\n");
        #elif __LIN
            fprintf( stdout, "target : LINUX\nstarting ...\n");
        #else
            fprintf( stderr, "target : ERROR TARGET NOT DEFINED\naborting !\n");
        #endif
    #endif

    #ifndef __LIN
        #ifndef __WIN
            exit( EXIT_FAILURE);
        #endif
    #endif
}

void choose_rom( char **rom_path) {

    if ( get_rom_number() <= 0) {

        #ifdef __DEBUG
            fprintf( stderr, "[FATAL] no rom gived in rom folder !\n");
        #endif

        exit( EXIT_FAILURE);
    }

    fprintf( stdout, "listing all available rom ...\n");

    char *rom_name = malloc( sizeof( char) * 2048);

    for ( uint16_t i = 0; i < get_rom_number(); i++) {

        get_rom_next( NULL, &rom_name, NEXT);
        fprintf( stdout, "\t%s\n", rom_name);
    }

    uint8_t d_pressed = 0, q_pressed = 0;

    get_rom_next( rom_path, &rom_name, NEXT);

    while ( touche_appuyer( ENTER) == RELEASED) {

        display_draw_menu( rom_name, audio_volume());

        SDL_Delay( 15);

        touche_get();

        if ( touche_appuyer( D) && !d_pressed) {

            get_rom_next( rom_path, &rom_name, NEXT);
            d_pressed = 1;
        } else if ( touche_appuyer( D) == RELEASED)
            d_pressed = 0;

        if ( touche_appuyer( Q) && !q_pressed) {

            get_rom_next( rom_path, &rom_name, BACK);
            q_pressed = 1;
        } else if ( touche_appuyer( Q) == RELEASED)
            q_pressed = 0;

        if ( touche_appuyer( Z))
            audio_volume_up();
        
        if ( touche_appuyer( S))
            audio_volume_down();
    }

    free( rom_name);

    fprintf( stdout, "rom choosed : %s\n", *rom_path);
}

int main( int argc, char *argv[]) {

    main_base_path = NULL;

    banner();
    setup_base_path();

    if ( argc != 1) {

        #ifdef __DEBUG
            fprintf( stderr, "[FATAL] unknow command line !\n");
        #endif

        exit( EXIT_FAILURE);
    }

    display_init();
    touche_init();
    #ifdef __DEBUG
        display_second_init();
    #endif
    audio_init();

    get_rom_init();
    char *rom_path;
    choose_rom( &rom_path);

    cpu_init();
    memory_init( rom_path);
    timer_init();
    interrupt_init();
    ppu_init();

    uint64_t cycles;

    audio_tetris();

    while ( 1) {

        touche_get();

        if ( touche_appuyer( QUITTER) || touche_appuyer( ESCAPE))
            exit( EXIT_SUCCESS);

        cycles = cpu_run();
        ppu_run( cycles);
        timer_run( cycles);
        interrupt_run();

        /*#ifdef __DEBUG
            if ( cycles > 1000000 && cycles < 1000010)
                display_try();
        #endif*/

        /*display_try();
        display_draw();*/
    }

    exit( EXIT_SUCCESS);
}