#include "audio.h"
#include "main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t volume = MIX_MAX_VOLUME / 50;
uint8_t mute = 0;

void audio_destroy( void) {

    //system( "del ..\\dat\\dat.mp3");
    Mix_CloseAudio();
}

extern void audio_init( void) {

    atexit( audio_destroy);

    /*FILE *f = fopen( "..\\dat\\dat.VGBE", "r");

    if (f == NULL) {

        fprintf( stdout, "unable to load audio file\n");
        Mix_CloseAudio();
        exit( EXIT_FAILURE);
    }

    FILE *fo = fopen( "..\\dat\\dat.mp3", "w+");

    if (fo == NULL) {

        fprintf( stdout, "unable to open audio file\n");
        Mix_CloseAudio();
        exit( EXIT_FAILURE);
    }

    fseek( f, 0, SEEK_END);
    uint64_t f_size = ftell( f);
    rewind( f);

    char dat[f_size];

    fread( dat, 1, f_size, f);
    fwrite( dat, 1, f_size, fo);*/

    if ( Mix_OpenAudio( 96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        exit( EXIT_FAILURE);
    }

    Mix_VolumeMusic( volume);

    char music_path[2048];

    strcpy( music_path, main_base_path);
    #ifdef __WIN
        strcat( music_path, "dat\\gameboy.mp3");
    #elif __LIN
        strcat( music_path, "dat/gameboy.mp3");
    #endif
    Mix_Music *music = Mix_LoadMUS( music_path);

    if ( music == NULL) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
        Mix_CloseAudio();
        exit( EXIT_FAILURE);
    }

    Mix_PlayMusic( music, -1);
}

extern void audio_tetris( void) {

    char music_path[2048];

    strcpy( music_path, main_base_path);
    #ifdef __WIN
        strcat( music_path, "dat\\tetris.mp3");
    #elif __LIN
        strcat( music_path, "dat/tetris.mp3");
    #endif

    Mix_Music *music = Mix_LoadMUS( music_path);

    if ( music == NULL) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
        Mix_CloseAudio();
        exit( EXIT_FAILURE);
    }

    Mix_HaltMusic();
    Mix_PlayMusic( music, -1);
    audio_volume_unmute();
}

extern uint8_t audio_volume( void) {

    return (uint8_t)((volume / (double)(MIX_MAX_VOLUME)) * 100.0);
}

extern void audio_volume_up( void) {

    if ( volume < MIX_MAX_VOLUME)
        volume++;

    Mix_VolumeMusic( volume);
}

extern void audio_volume_down( void) {

    if ( volume > 0)
        volume--;

    Mix_VolumeMusic( volume);
}

extern void audio_volume_mute( void) {

    Mix_VolumeMusic( 0);
}

extern void audio_volume_unmute( void) {

    Mix_VolumeMusic( volume);
}