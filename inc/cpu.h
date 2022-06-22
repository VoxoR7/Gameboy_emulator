#ifndef __CPU__
#define __CPU__

#include <inttypes.h>

extern void cpu_init();
extern void cpu_interrupt( uint16_t addr);
extern uint64_t cpu_run();

extern void cpu_pc();

#endif