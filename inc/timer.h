#ifndef __TIMER__
#define __TIMER__

#include <inttypes.h>

extern void timer_init( void);
extern void timer_tac( uint8_t value);
extern void timer_run( uint64_t cycles);

#endif