#ifndef __MEMORY__
#define __MEMORY__

#include <inttypes.h>

extern void memory_init( char *rom);

extern void memory_write8( uint16_t addr, uint8_t value);
extern uint8_t memory_read8( uint16_t addr);
extern void memory_write16( uint16_t addr, uint16_t value);
extern uint16_t memory_read16( uint16_t addr);

extern void memory_special_service_ly( uint16_t value);
extern void memory_special_service_div( uint8_t value);
extern uint8_t memory_special_service_read_joy( void);

#ifdef __STEP
    extern void prot_memory_write8( uint16_t addr, uint8_t value);
    extern uint8_t prot_memory_read8( uint16_t addr);
#endif

#endif