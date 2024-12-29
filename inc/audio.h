#ifndef AUDIO
#define AUDIO

#include <inttypes.h>

/**
 * @brief Initialize the audio module
 *
 * @warning Module not thread-safe
 */
void audio_init(void);

/**
 * @brief Return the audio volume
 *
 * @return A percentage of the audio volume (0-100)
 */
uint8_t audio_volume(void);

/**
 * @brief Increase the audio volume by one point
 */
void audio_volume_up(void);

/**
 * @brief Reduce the audio volume by one point
 */
void audio_volume_down(void);

/**
 * @brief Mute the audio
 */
void audio_volume_mute(void);

/**
 * @brief Unmute the audio
 */
void audio_volume_unmute(void);

void audio_tetris(void);

#endif