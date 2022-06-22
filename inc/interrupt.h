#ifndef __INTERRUPT__
#define __INTERRUPT__

#include <inttypes.h>

#define INTERRUPT_BIT_VBLANK 0b00000001
#define INTERRUPT_BIT_LCDSTAT 0b00000010
#define INTERRUPT_BIT_TIMER 0b00000100
#define INTERRUPT_BIT_SERIAL 0b00001000
#define INTERRUPT_BIT_JOYPAD 0b00010000

extern void interrupt_init( void);
extern void interrupt_enable( void);
extern void interrupt_disable( void);
extern void interrupt_request( uint8_t interrupt);
extern void interrupt_run();

#ifdef __DEBUG
    extern uint8_t interrupt_ask( void);
#endif

#endif