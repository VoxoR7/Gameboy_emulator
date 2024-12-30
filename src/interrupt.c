#include "main.h"
#include "interrupt.h"

#include <stdlib.h>

#ifdef DEBUG
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

bool ime = true;

static void interrupt_destroy(void) {
}

void interrupt_init(void) {
    atexit(interrupt_destroy);
}

void interrupt_enable(void) {
    ime = true;
}

void interrupt_disable(void) {
    ime = false;
}

void interrupt_request(uint8_t interrupt) {
    memory_write8(INTERRUPT_REQUEST, memory_read8(INTERRUPT_REQUEST) | interrupt);
}

void interrupt_run(void) {
    if (!ime && !cpu_halted())
        return;

    uint8_t int_requested = memory_read8(INTERRUPT_REQUEST) & memory_read8(INTERRUPT_ENABLE);

    if (!ime && cpu_halted()) {
        if (int_requested) {
            cpu_wake_up();
        }

        return;
    }

    if (int_requested & INTERRUPT_BIT_VBLANK) {
        ime = false;
        memory_write8(INTERRUPT_REQUEST, memory_read8(INTERRUPT_REQUEST) & ~INTERRUPT_BIT_VBLANK);
        cpu_interrupt(INTERRUPT_ADDR_VBLANK);
        return;
    } else if (int_requested & INTERRUPT_BIT_LCDSTAT) {
        ime = false;
        memory_write8(INTERRUPT_REQUEST, memory_read8(INTERRUPT_REQUEST) & ~INTERRUPT_BIT_LCDSTAT);
        cpu_interrupt(INTERRUPT_ADDR_LCDSTAT);
        return;
    } else if (int_requested & INTERRUPT_BIT_TIMER) {
        ime = false;
        memory_write8(INTERRUPT_REQUEST, memory_read8(INTERRUPT_REQUEST) & ~INTERRUPT_BIT_TIMER);
        cpu_interrupt(INTERRUPT_ADDR_TIMER);
        return;
    } else if (int_requested & INTERRUPT_BIT_SERIAL) {
        ime = false;
        memory_write8(INTERRUPT_REQUEST, memory_read8(INTERRUPT_REQUEST) & ~INTERRUPT_BIT_SERIAL);
        cpu_interrupt(INTERRUPT_ADDR_SERIAL);
        return;
    } else if (int_requested & INTERRUPT_BIT_JOYPAD) {
        ime = false;
        memory_write8(INTERRUPT_REQUEST, memory_read8(INTERRUPT_REQUEST) & ~INTERRUPT_BIT_JOYPAD);
        cpu_interrupt(INTERRUPT_ADDR_JOYPAD);
        return;
    } else
        return;
}

#ifdef DEBUG
bool interrupt_ask(void) {
    return ime;
}
#endif