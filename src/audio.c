#include "audio.h"
#include "main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t volume = MIX_MAX_VOLUME / 50;
Mix_Music *music = NULL;

/**
 * @brief Play the music passed as parameter
 * 
 * @param path The filepath to the music
 */
static void play_music(char *path) {
    if (music) {
        Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(path);
    if (music == NULL) {
        LOG_MESG(LOG_FATAL, "Error when loading music: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    }

    if (Mix_PlayMusic(music, -1) != 0) {
        LOG_MESG(LOG_FATAL, "Couldn't play music");
        exit(EXIT_FAILURE);
    }
}

static void audio_destroy(void) {
    if (music) {
        Mix_FreeMusic(music);
    }

    Mix_CloseAudio();
}

void audio_init(void) {
    atexit(audio_destroy);

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        LOG_MESG(LOG_FATAL, "Error: %s", Mix_GetError());
        exit(EXIT_FAILURE);
    }

    Mix_VolumeMusic(volume);

    char music_path[2048];
    strcpy(music_path, main_base_path);
    #ifdef __linux__
        strcat(music_path, "dat/gameboy.mp3");
    #elifdef _WIN32
        strcat(music_path, "dat\\gameboy.mp3");
    #else
        #error "Unknow target"
    #endif
    
    play_music(music_path);
}

void audio_tetris(void) {
    char music_path[2048];

    strcpy(music_path, main_base_path);
    #ifdef __linux__
        strcat(music_path, "dat/tetris.mp3");
    #elifdef _WIN32
        strcat(music_path, "dat\\tetris.mp3");
    #else
        #error "Unknow target"
    #endif

    play_music(music_path);
    audio_volume_unmute();
}

uint8_t audio_volume(void) {
    return (uint8_t)((volume / (double)(MIX_MAX_VOLUME)) * 100.0);
}

void audio_volume_up(void) {
    if (volume < MIX_MAX_VOLUME)
        volume++;

    Mix_VolumeMusic(volume);
}

void audio_volume_down(void) {
    if (volume > 0)
        volume--;

    Mix_VolumeMusic(volume);
}

void audio_volume_mute(void) {
    Mix_VolumeMusic(0);
}

void audio_volume_unmute(void) {
    Mix_VolumeMusic(volume);
}