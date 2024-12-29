#ifndef PPU
#define PPU

#include <inttypes.h>

void ppu_init(void);
void ppu_reset(void);

void ppu_run(uint64_t cycles);

void ppu_disable(void);
void ppu_enable(void);
void ppu_disable_BG_window(void);
void ppu_enable_BG_window(void);
void ppu_disable_sprite(void);
void ppu_enable_sprite(void);
void ppu_addressing_8000(void);
void ppu_addressing_8800(void);

#endif