#ifndef DISPLAY
#define DISPLAY

#include <inttypes.h>

void display_init(void);
void display_second_init(void);
void display_second_destroy(void);

void display_try(void);

void display_draw_line_background(uint8_t ly, uint8_t addressing_mode);
void display_draw_line_sprite(uint8_t ly);

void display_draw_final();
void display_blank();

void display_draw_menu(const char *rom_selected, const uint8_t audio_level);

#endif