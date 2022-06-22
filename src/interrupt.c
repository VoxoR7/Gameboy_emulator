#include "main.h"
#include "interrupt.h"

#include <stdlib.h>

#ifdef __DEBUG
    #include <stdio.h>
#endif

#include "memory.h"
#include "cpu.h"

#define INTERRUPT_ADDR_VBLANK 0x40
#define INTERRUPT_ADDR_LCDSTAT 0x48
#define INTERRUPT_ADDR_TIMER 0x50
#define INTERRUPT_ADDR_SERIAL 0x58
#define INTERRUPT_ADDR_JOYPAD 0x60

#define INTERRUPT_REQUEST 0xFF0F
#define INTERRUPT_ENABLE 0xFFFF

uint8_t ime = 1;

void interrupt_destroy( void) {

    
}

extern void interrupt_init( void) {

    atexit( interrupt_destroy);
}

extern void interrupt_enable( void) {

    ime = 1;
}

extern void interrupt_disable( void) {

    ime = 0;
}

extern void interrupt_request( uint8_t interrupt) {

    memory_write8( INTERRUPT_REQUEST, memory_read8( INTERRUPT_REQUEST) | interrupt);
}

extern void interrupt_run() {

    /*#ifdef __DEBUG
    if ( memory_read8( INTERRUPT_REQUEST) & INTERRUPT_BIT_VBLANK && !ime)
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_VBLANK, BUT IME IS NOT ALLOWING\n");
    #endif*/

    if ( !ime)
        return;

    uint8_t int_requested = memory_read8( INTERRUPT_REQUEST) & memory_read8( INTERRUPT_ENABLE);

    /*#ifdef __DEBUG
    if ( int_requested & INTERRUPT_BIT_VBLANK)
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_VBLANK, BUT INTERRUPT_ENABLE IS NOT ALLOWING\n");
    #endif*/

    if ( int_requested & INTERRUPT_BIT_VBLANK) {

        /*#ifdef __DEBUG
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_VBLANK\n");
        #endif*/

        ime = 0;
        memory_write8( INTERRUPT_REQUEST, memory_read8( INTERRUPT_REQUEST) & ~INTERRUPT_BIT_VBLANK);
        cpu_interrupt( INTERRUPT_ADDR_VBLANK);
        return;
    } else if ( int_requested & INTERRUPT_BIT_LCDSTAT) {

        /*#ifdef __DEBUG
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_LCDSTAT\n");
        #endif*/

        ime = 0;
        memory_write8( INTERRUPT_REQUEST, memory_read8( INTERRUPT_REQUEST) & ~INTERRUPT_BIT_LCDSTAT);
        cpu_interrupt( INTERRUPT_ADDR_LCDSTAT);
        return;
    } else if ( int_requested & INTERRUPT_BIT_TIMER) {

        /*#ifdef __DEBUG
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_TIMER\n");
        #endif*/

        ime = 0;
        memory_write8( INTERRUPT_REQUEST, memory_read8( INTERRUPT_REQUEST) & ~INTERRUPT_BIT_TIMER);
        cpu_interrupt( INTERRUPT_ADDR_TIMER);
        return;
    } else if ( int_requested & INTERRUPT_BIT_SERIAL) {

        /*#ifdef __DEBUG
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_SERIAL\n");
        #endif*/

        ime = 0;
        memory_write8( INTERRUPT_REQUEST, memory_read8( INTERRUPT_REQUEST) & ~INTERRUPT_BIT_SERIAL);
        cpu_interrupt( INTERRUPT_ADDR_SERIAL);
        return;
    } else if ( int_requested & INTERRUPT_BIT_JOYPAD) {

        /*#ifdef __DEBUG
            fprintf( stdout, "[INFO] : INTERRUPT REQUESTED, INTERRUPT_BIT_JOYPAD\n");
        #endif*/

        ime = 0;
        memory_write8( INTERRUPT_REQUEST, memory_read8( INTERRUPT_REQUEST) & ~INTERRUPT_BIT_JOYPAD);
        cpu_interrupt( INTERRUPT_ADDR_JOYPAD);
        return;
    } else
        return;
}

#ifdef __DEBUG
    extern uint8_t interrupt_ask( void) {

        return ime;
    }
#endif