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
    uint8_t draw; // In case of LCDC.7 off, next time it is turning on the current frame should not being displayed
    uint8_t enable;
    uint8_t draw_BG_window;
    uint8_t draw_sprite;

    uint8_t sstate; // used to save ppu context when LCDC.7 is turn off
    uint8_t sly;
    uint16_t scycles;
};

struct ppu ppu;

void ppu_destroy( void) {

    return;
}

extern void ppu_reset( void) {

    ppu.state = PPU_OAM;
    ppu.ly = 0;
    ppu.cycles = 0;
    ppu.draw = 1;
    ppu.enable = 1;
    ppu.draw_BG_window = 1;
    ppu.draw_sprite = 1;

    return;
}

extern void ppu_init( void) {

    atexit( ppu_destroy);
    
    ppu_reset();
    return;
}

extern void ppu_disable( void) {

    #ifdef __FAST // this should never occur
        if ( ppu.state != PPU_VBLANK) {

            #ifdef __DEBUG
                fprintf( stderr, "[FATAL] LCDC.7 on while ppu not in VBLANK state. A real gameboy might have serious issue.\n");
            #endif

            exit( EXIT_FAILURE);
        }
    #endif

    ppu.draw = 0;
    ppu.enable = 0;

    ppu.sstate = ppu.state; // save ppu context
    ppu.sly = ppu.ly;
    ppu.scycles = ppu.cycles;

    display_blank();
}

extern void ppu_enable( void) {

    ppu.state = ppu.sstate; // restore ppu context
    ppu.ly = ppu.sly;
    ppu.cycles = ppu.scycles;

    ppu.enable = 1;
}

extern void ppu_disable_BG_window( void) {

    ppu.draw_BG_window = 0;
    return;
}

extern void ppu_enable_BG_window( void) {

    ppu.draw_BG_window = 1;
    return;
}

extern void ppu_disable_sprite( void) {

    ppu.draw_sprite = 0;
    return;
}

extern void ppu_enable_sprite( void) {

    ppu.draw_sprite = 1;
    return;
}

void vblank( void) {

    if ( !(ppu.enable))
        return;

    interrupt_request( INTERRUPT_BIT_VBLANK);

    if ( memory_read8( 0xFF41) & PPU_STAT_VBLANK)
        interrupt_request( INTERRUPT_BIT_LCDSTAT);
    return;
}

void hblank( void) {

    if ( !(ppu.enable))
        return;

    if ( memory_read8( 0xFF41) & PPU_STAT_HBLANK)
        interrupt_request( INTERRUPT_BIT_LCDSTAT);

    return;
}

void oam( void) {

    if ( !(ppu.enable))
        return;

    if ( memory_read8( 0xFF41) & PPU_STAT_OAM)
        interrupt_request( INTERRUPT_BIT_LCDSTAT);

    return;
}

void increase_ly( struct ppu *ppu) {

    ppu->ly++;

    if ( !(ppu->enable))
        return;

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

            if ( ppu.cycles >= 204 /*42*/) {

                ppu.cycles -= 204 /*42*/;
                increase_ly( &ppu);

                if ( ppu.ly == 144) {

                    ppu.state = PPU_VBLANK;
                    vblank();

                    if ( !(ppu.enable))
                        return;

                    if ( ppu.draw)
                        display_draw_final();
                    else
                        ppu.draw = 1;
                } else {

                    ppu.state = PPU_OAM;

                    if ( !(ppu.enable))
                        return;
                    
                    if ( ppu.draw_BG_window)
                        display_draw_line_background( ppu.ly);

                    if ( ppu.draw_sprite)
                        display_draw_line_sprite( ppu.ly);
                }
            }

            break;
        case PPU_VBLANK:

            if ( ppu.cycles < 456 /*20*/)
                return;

            ppu.cycles -= 456 /*20*/;
    
            increase_ly( &ppu);

            if ( ppu.ly >= 153) {

                ppu.ly = 0;
                ppu.state = PPU_OAM;
                real_time_wait();

                touche_get();

                if ( touche_appuyer( QUITTER) || touche_appuyer( ESCAPE))
                    exit( EXIT_SUCCESS);

                if ( !(ppu.enable))
                    return;

                if ( ppu.draw_BG_window)
                    display_draw_line_background( ppu.ly);

                if ( ppu.draw_sprite)
                    display_draw_line_sprite( ppu.ly);

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