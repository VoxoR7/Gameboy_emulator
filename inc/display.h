#ifndef __DISPLAY__
#define __DIFPLAY__

#include <inttypes.h>

extern void display_init( void);
extern void display_second_init( void);
extern void display_second_destroy( void);

extern void display_draw( void);
extern void display_draw2nd( void);
extern void display_try( void);

extern void display_clear( void);
extern void display_draw_line( uint8_t ly);

extern void display_tile( uint8_t tile);
extern void display_wait( void);

extern void display_draw_menu( const char *rom_selected, const uint8_t audio_level);

#endif