#ifndef __GET_ROM__
#define __GET_ROM__

#include <inttypes.h>

#define NEXT 0
#define BACK 1

extern void get_rom_init( void);

extern uint16_t get_rom_number( void);
extern void get_rom_next( char **rom_path, char **rom_name, uint8_t order);

#endif