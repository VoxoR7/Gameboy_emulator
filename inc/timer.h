#ifndef TIMER
#define TIMER

#include <inttypes.h>

void timer_init(void);
void timer_tac(uint8_t value);
void timer_run(uint64_t cycles);

#endif