#include "main.h"
#include "timer.h"

#include <stdlib.h>

#include "memory.h"
#include "interrupt.h"

// 4096 Hz
// 16384 Hz
// 65536 Hz
// 262144 Hz

#define TIMER_FREQ_DIV 256
#define TIMER_CLOCK_DIV (CLOCK_SPEED / TIMER_FREQ_DIV)

#define TIMER_FREQ_0 4096
#define TIMER_FREQ_1 16384
#define TIMER_FREQ_2 65536
#define TIMER_FREQ_3 262144

#define TIMER_DIV 0xFF04
#define TIMER_TIMA 0xFF05
#define TIMER_TMA 0xFF06
#define TIMER_TAC 0xFF07

uint16_t inc_freq = (CLOCK_SPEED / TIMER_FREQ_0);

static void timer_destroy(void) {
    return;
}

void timer_init(void) {
    atexit(timer_destroy);
}

void timer_tac(uint8_t value) { // timer control
    if ((value & 0b00000011) == 0b00)
        inc_freq = (CLOCK_SPEED / 64);
    else if ((value & 0b00000011) == 0b01)
        inc_freq = (CLOCK_SPEED / 4096);
    else if ((value & 0b00000011) == 0b10)
        inc_freq = (CLOCK_SPEED / 1024);
    else if ((value & 0b00000011) == 0b11)
        inc_freq = (CLOCK_SPEED / 256);
}

void timer_run(uint64_t cycles) {
    static uint64_t last_cycles = 0;
    uint64_t m_cycles_elapsed = last_cycles - cycles;
    last_cycles = cycles;

    static uint16_t div_current_m_cycles = 0, tac_current_m_cycles = 0;
    div_current_m_cycles += m_cycles_elapsed;

    if (div_current_m_cycles >= TIMER_CLOCK_DIV) {
        div_current_m_cycles -= TIMER_CLOCK_DIV;

        memory_special_service_div(memory_read8(TIMER_DIV) + 1);
    }

    if (memory_read8(TIMER_TAC) & 0b00000100) {
        tac_current_m_cycles += m_cycles_elapsed;

        if (tac_current_m_cycles >= inc_freq) {
            tac_current_m_cycles -= inc_freq;

            if (memory_read8(TIMER_TIMA) == 255) {
                memory_write8(TIMER_TIMA, memory_read8(TIMER_TMA));
                interrupt_request(INTERRUPT_BIT_TIMER);
            } else
                memory_write8(TIMER_TIMA, memory_read8(TIMER_TIMA) + 1);
        }
    }
}