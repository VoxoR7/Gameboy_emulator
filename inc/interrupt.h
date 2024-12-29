#ifndef INTERRUPT
#define INTERRUPT

#include <inttypes.h>

#define INTERRUPT_BIT_VBLANK 0b00000001
#define INTERRUPT_BIT_LCDSTAT 0b00000010
#define INTERRUPT_BIT_TIMER 0b00000100
#define INTERRUPT_BIT_SERIAL 0b00001000
#define INTERRUPT_BIT_JOYPAD 0b00010000

void interrupt_init(void);
void interrupt_enable(void);
void interrupt_disable(void);
void interrupt_request(uint8_t interrupt);
void interrupt_run(void);

#ifdef STEP
    bool interrupt_ask(void);
#endif

#endif