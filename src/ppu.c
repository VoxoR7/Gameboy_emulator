#include "main.h"
#include "ppu.h"

#include <stdlib.h>

#include "memory.h"
#include "interrupt.h"
#include "display.h"
#include "real_time.h"
#include "touche.h"

#ifdef __DEBUG
    #include <stdio.h>
#endif

#define PPU_CYCLE_BEFORE_VBLANK (70224 / CLOCK_DIVIDER)

#define PPU_OAM 0 // values dont matter
#define PPU_PT 1 // pixel transfer
#define PPU_HBLANK 2
#define PPU_VBLANK 3

#define PPU_STAT_LYC_LY 0b01000000
#define PPU_STAT_OAM 0b00100000
#define PPU_STAT_VBLANK 0b00010000
#define PPU_STAT_HBLANK 0b00001000

struct ppu {

    uint8_t state;
    uint8_t ly;
    uint16_t cycles;
};

struct ppu ppu;

void ppu_destroy( void) {

    return;
}

extern void ppu_reset( void) {

    ppu.state = PPU_OAM;
    ppu.ly = 0;
    ppu.cycles = 0;

    return;
}

extern void ppu_init( void) {

    atexit( ppu_destroy);
    
    ppu_reset();
    return;
}

void vblank( void) {

    interrupt_request( INTERRUPT_BIT_VBLANK);

    if ( memory_read8( 0xFF41) & PPU_STAT_VBLANK)
        interrupt_request( INTERRUPT_BIT_LCDSTAT);
    return;
}

void hblank( void) {

    if ( memory_read8( 0xFF41) & PPU_STAT_HBLANK)
        interrupt_request( INTERRUPT_BIT_LCDSTAT);

    return;
}

void oam( void) {

    if ( memory_read8( 0xFF41) & PPU_STAT_OAM)
        interrupt_request( INTERRUPT_BIT_LCDSTAT);

    return;
}

void increase_ly( struct ppu *ppu) {

    ppu->ly++;

    memory_special_service_ly( ppu->ly);

    if ( ppu->ly == memory_read8( 0xFF45)) {

        memory_write8( 0XFF41, memory_read8( 0xFF41) | 0b00000100);

        if ( memory_read8( 0xFF41) & PPU_STAT_OAM)
            interrupt_request( INTERRUPT_BIT_LCDSTAT);
    } else
        memory_write8( 0XFF41, memory_read8( 0xFF41) & ~0b00000100);
}

extern void ppu_run( uint64_t cycles) {

    static uint64_t last_cycles = 0;
    uint8_t do_cycles = cycles - last_cycles;
    last_cycles = cycles;

    if ( !do_cycles)
        return;

    ppu.cycles += do_cycles;

    switch ( ppu.state) {

        case PPU_OAM:

            if ( ppu.cycles >= 80) {

                ppu.cycles -= 80;
                ppu.state = PPU_PT;
            }

            break;
        case PPU_PT:

            if ( ppu.cycles >= 172) {

                ppu.cycles -= 172;
                ppu.state = PPU_HBLANK;
                hblank();
            }

            break;
        case PPU_HBLANK:

            if ( ppu.cycles >= /*204*/ 42) {

                ppu.cycles -= /*204*/ 42;
                increase_ly( &ppu);

                if ( ppu.ly == 144) {

                    ppu.state = PPU_VBLANK;
                    vblank();
                } else {

                    ppu.state = PPU_OAM;
                    display_draw_line( ppu.ly);
                }
            }

            break;
        case PPU_VBLANK:

            if ( ppu.cycles < /*456*/ 20)
                return;

            ppu.cycles -= /*456*/ 20;
    
            increase_ly( &ppu);

            if ( ppu.ly >= 153) {

                ppu.ly = 0;
                ppu.state = PPU_OAM;
                real_time_wait();

                touche_get();

                if ( touche_appuyer( QUITTER) || touche_appuyer( ESCAPE))
                    exit( EXIT_SUCCESS);

                display_clear();
                display_draw_line( ppu.ly);

                #ifdef __DEBUG
                    display_try();
                #endif
            }

            break;
        #ifdef __DEBUG
        default:

            fprintf( stdout, "[FATAL] : ppu not in the good state !\n");
            exit( EXIT_FAILURE);
        #endif
    }
}