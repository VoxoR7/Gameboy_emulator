#ifndef GET_ROM
#define GET_ROM

#include <inttypes.h>

#define GET_ROM_NEXT 0
#define GET_ROM_BACK 1

void get_rom_init(void);

uint16_t get_rom_number(void);
void get_rom_next(char **rom_path, char **rom_name, uint8_t order);

#endif