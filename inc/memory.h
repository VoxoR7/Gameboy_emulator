#ifndef MEMORY
#define MEMORY

#include <inttypes.h>

void memory_init(char *rom);

void memory_write8(uint16_t addr, uint8_t value);
uint8_t memory_read8(uint16_t addr);
void memory_write16(uint16_t addr, uint16_t value);
uint16_t memory_read16(uint16_t addr);

void memory_special_service_ly(uint16_t value);
void memory_special_service_div(uint8_t value);
uint8_t memory_special_service_read_joy(void);

#ifdef STEP
void prot_memory_write8(uint16_t addr, uint8_t value);
uint8_t prot_memory_read8(uint16_t addr);
#endif

#endif