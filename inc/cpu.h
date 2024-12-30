#ifndef CPU
#define CPU

#include <inttypes.h>

void cpu_init(void);
void cpu_interrupt(uint16_t addr);
bool cpu_halted(void);
void cpu_wake_up(void);
uint64_t cpu_run(void);

void cpu_pc(void);

#endif