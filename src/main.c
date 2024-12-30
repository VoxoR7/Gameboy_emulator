#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
#include "real_time.h"

char *main_base_path = NULL;

static void main_destroy(void) {
    if (main_base_path != NULL) {
        SDL_free(main_base_path);
        main_base_path = NULL;
    }
}

static void setup_base_path(void) {
    main_base_path = SDL_GetBasePath();

    if (strlen(main_base_path) > BASE_PATH_MAX) {
        LOG_MESG(LOG_FATAL, "The base path is too long!");
        exit(EXIT_FAILURE);
    }

    main_base_path[strlen(main_base_path) - 1] = '\0';

    char *last_sep;
    #ifdef __linux__
    last_sep = strrchr(main_base_path, '/');
    #elifdef _WIN32
    last_sep = strrchr(main_base_path, '\\');
    #endif

    if (last_sep == NULL) {
        LOG_MESG(LOG_FATAL, "last separator can't be found ! (%s)", main_base_path);
        exit(EXIT_FAILURE);
    }

    last_sep[1] = '\0';

    #ifdef DEBUG
    LOG_MESG(LOG_DEBUG, "The base path is : %s", main_base_path);
    #endif
}

static void banner(void) {
    fprintf(stdout, " _   _                     ______   _____                          _                                             _         _    _               \n");
    fprintf(stdout, "| | | |                    | ___ \\ |  __ \\                        | |                                           | |       | |  (_)              \n");
    fprintf(stdout, "| | | |  ___  __  __  ___  | |_/ / | |  \\/  __ _  _ __ ___    ___ | |__    ___   _   _    ___  _ __ ___   _   _ | |  __ _ | |_  _   ___   _ __  \n");
    fprintf(stdout, "| | | | / _ \\ \\ \\/ / / _ \\ |    /  | | __  / _` || '_ ` _ \\  / _ \\| '_ \\  / _ \\ | | | |  / _ \\| '_ ` _ \\ | | | || | / _` || __|| | / _ \\ | '_ \\ \n");
    fprintf(stdout, "\\ \\_/ /| (_) | >  < | (_) || |\\ \\  | |_\\ \\| (_| || | | | | ||  __/| |_) || (_) || |_| | |  __/| | | | | || |_| || || (_| || |_ | || (_) || | | |\n");
    fprintf(stdout, " \\___/  \\___/ /_/\\_\\ \\___/ \\_| \\_|  \\____/ \\__,_||_| |_| |_| \\___||_.__/  \\___/  \\__, |  \\___||_| |_| |_| \\__,_||_| \\__,_| \\__||_| \\___/ |_| |_|\n");
    fprintf(stdout, "                                                                                  __/ |                                                         \n");
    fprintf(stdout, "                                                                                 |___/                                                          \n\n");
    LOG_MESG(LOG_INFO, "Welcome to the VoxoR Gameboy emulator.");
    LOG_MESG(LOG_INFO, "This is the version %d.%d.%d", MAJOR_VERSION, MINOR_VERSION, BUILD);

    #ifdef DEBUG
    #ifdef __linux__
    LOG_MESG(LOG_DEBUG, "target: linux");
    #elifdef _WIN32
    LOG_MESG(LOG_DEBUG, "target: windows");
    #else
    LOG_MESG(LOG_FATAL, "target: target not defined");
    exit(EXIT_FAILURE);
    #endif
    #endif
}

static void choose_rom(char **rom_path) {
    if (get_rom_number() <= 0) {
        LOG_MESG(LOG_FATAL, "No rom gived in rom folder!");
        sleep(5);

        exit(EXIT_FAILURE);
    }

    char *rom_name = malloc(sizeof(char) * ROM_NAME_MAX);
    if (rom_name == NULL) {
        LOG_MESG(LOG_FATAL, "Not enough memory");
        exit(EXIT_FAILURE);
    }

    uint8_t d_pressed = 0, q_pressed = 0;

    get_rom_next(rom_path, &rom_name, GET_ROM_NEXT);

    while (touche_appuyer(ENTER) == RELEASED) {
        display_draw_menu(rom_name, audio_volume());
        SDL_Delay(15);
        touche_get();

        if (touche_appuyer(D) && !d_pressed) {
            get_rom_next(rom_path, &rom_name, GET_ROM_NEXT);
            d_pressed = 1;
        } else if (touche_appuyer(D) == RELEASED)
            d_pressed = 0;
        if (touche_appuyer(Q) && !q_pressed) {
            get_rom_next(rom_path, &rom_name, GET_ROM_BACK);
            q_pressed = 1;
        } else if (touche_appuyer(Q) == RELEASED)
            q_pressed = 0;
        if (touche_appuyer(Z))
            audio_volume_up();
        if (touche_appuyer(S))
            audio_volume_down();
        if (touche_appuyer(ESCAPE)) {
            free(rom_name);
            exit(EXIT_SUCCESS);
        }
    }

    free(rom_name);
}

int main(int argc, char *argv[]) {
    log_init(LOG_DEBUG, NULL);
    atexit(main_destroy);
    banner();

    if (argc != 1) {
        #ifdef DEBUG
        LOG_MESG(LOG_FATAL, "usage: %s\n", argv[0]);
        #endif

        exit(EXIT_FAILURE);
    }

    setup_base_path();
    display_init();
    touche_init();
    #ifdef DEBUG
        display_second_init();
    #endif
    audio_init();

    get_rom_init();
    char *rom_path;
    choose_rom(&rom_path);

    cpu_init();
    memory_init(rom_path);
    timer_init();
    interrupt_init();
    ppu_init();
    real_time_init();

    audio_tetris();
    real_time_start();

    uint64_t cycles;
    do {
        cycles = cpu_run();
        ppu_run(cycles);
        timer_run(cycles);
        interrupt_run();
    } while (1);

    exit(EXIT_SUCCESS);
}