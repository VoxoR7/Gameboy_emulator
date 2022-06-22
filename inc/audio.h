#ifndef __AUDIO__
#define __AUDIO__

#include <inttypes.h>

extern void audio_init( void);

extern uint8_t audio_volume( void);
extern void audio_volume_up( void);
extern void audio_volume_down( void);
extern void audio_volume_mute( void);
extern void audio_volume_unmute( void);

extern void audio_tetris( void);

#endif