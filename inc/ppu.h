#ifndef __PPU__
#define __PPU__

#include <inttypes.h>

extern void ppu_init( void);
extern void ppu_reset( void);

extern void ppu_run( uint64_t cycles);

#endif