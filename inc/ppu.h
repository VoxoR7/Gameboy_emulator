#ifndef __PPU__
#define __PPU__

#include <inttypes.h>

extern void ppu_init( void);
extern void ppu_reset( void);

extern void ppu_run( uint64_t cycles);
extern void ppu_disable( void);
extern void ppu_enable( void);
extern void ppu_disable_BG_window( void);
extern void ppu_enable_BG_window( void);

#endif