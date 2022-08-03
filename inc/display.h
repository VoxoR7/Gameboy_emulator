#ifndef __DISPLAY__
#define __DIFPLAY__

#include <inttypes.h>

extern void display_init( void);
extern void display_second_init( void);
extern void display_second_destroy( void);

extern void display_try( void);

extern void display_draw_line( uint8_t ly);
extern void display_draw_final();
extern void display_blank();

extern void display_draw_menu( const char *rom_selected, const uint8_t audio_level);

#endif