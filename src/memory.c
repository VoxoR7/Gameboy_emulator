#include "main.h"
#include "memory.h"

#include "cpu.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define REALITY 0
#define MEMORY_SIZE 0x10000

#define MBC_TYPE_0 0
#define MBC_TYPE_1 1
#define MBC_TYPE_2 2
#define MBC_TYPE_5 5
#define MBC_TYPE_ERROR 255


/*
0000-3FFF 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
4000-7FFF 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
8000-9FFF 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
A000-BFFF 8KB External RAM (in cartridge, switchable bank, if any)
C000-CFFF 4KB Work RAM Bank 0 (WRAM)
D000-DFFF 4KB Work RAM Bank 1 (WRAM) (switchable bank 1-7 in CGB Mode)
E000-FDFF Same as C000-FDFF (ECHO) (typically not used)
FE00-FE9F Sprite Attribute Table (OAM)
FEA0-FEFF Not Usable
FF00-FF7F I/O Ports
FF80-FFFE High RAM (HRAM)
FFFF Interrupt Enable Register
*/

uint8_t *emul_cartridge;
uint8_t emul_mbctype;

uint16_t emul_total_rom_bank;
uint16_t emul_current_rom_bank;

uint8_t emul_current_ram_bak;

uint8_t memory[MEMORY_SIZE];
uint8_t cart_number_ram_bank; // each ram bank is 0x2000 size, maximum 4 ram bank

void memory_destroy() {

    free( emul_cartridge);
}

extern void memory_init( char *rom) {

    #if __REALITY

        srand( time( NULL) * getpid());
        for ( uint32_t i = 0; i < MEMORY_SIZE; i++ )
            memory[i] = rand() % 256;
    #else
        for ( uint32_t i = 0; i < MEMORY_SIZE; i++ )
            memory[i] = 0;
    #endif

    memory[0xFF05] = 0x00;
    memory[0xFF06] = 0x00;
    memory[0xFF07] = 0x00;
    memory[0xFF10] = 0x80;
    memory[0xFF11] = 0xBF;
    memory[0xFF12] = 0xF3;
    memory[0xFF14] = 0xBF;
    memory[0xFF16] = 0x3F;
    memory[0xFF17] = 0x00;
    memory[0xFF19] = 0xBF;
    memory[0xFF1A] = 0x7F;
    memory[0xFF1B] = 0xFF;
    memory[0xFF1C] = 0x9F;
    memory[0xFF1E] = 0xBF;
    memory[0xFF20] = 0xFF;
    memory[0xFF21] = 0x00;
    memory[0xFF22] = 0x00;
    memory[0xFF23] = 0xBF;
    memory[0xFF24] = 0x77;
    memory[0xFF25] = 0xF3;
    memory[0xFF26] = 0xF1;
    memory[0xFF40] = 0x91;
    memory[0xFF42] = 0x00;
    memory[0xFF43] = 0x00;
    memory[0xFF45] = 0x00;
    memory[0xFF47] = 0xFC;
    memory[0xFF48] = 0xFF;
    memory[0xFF49] = 0xFF;
    memory[0xFF4A] = 0x00;
    memory[0xFF4B] = 0x00;
    memory[0xFFFF] = 0x00;

    memory[0xFF44] = 0x90;

    atexit( memory_destroy);

    FILE *f = NULL;
    f = fopen( rom, "r");
    if ( f == NULL) {

        fprintf( stderr, "[FATAL] : the path %s for the rom is wrong !\n", rom);
        exit( EXIT_FAILURE);
    }

    fseek( f, 0, SEEK_END);
    uint64_t f_size = ftell( f);
    rewind( f);
    if ( f_size > 16777216) {

        fprintf( stderr, "[FATAL] : the file is too big to be loaded : %"PRIu64" !\n", f_size);
        exit( EXIT_FAILURE);
    }

    emul_cartridge = NULL;
    emul_cartridge = malloc( sizeof( uint8_t) * f_size);
    if ( emul_cartridge == NULL) {

        fprintf( stderr, "[FATAL] : the file is too big to be loaded : %"PRIu64" !\n", f_size);
        exit( EXIT_FAILURE);
    }

    #ifdef __DEBUG
        fprintf( stdout, "[INFO] : rom size : %"PRIu64"\n", f_size);
    #endif

    if ( fread( emul_cartridge, 1, f_size, f) != f_size ) {

        #ifdef __WIN
            fprintf( stdout, "[CRITICAL] : ---------------------------------------------------------------------------------------------------\n");
            fprintf( stdout, "             |                                             WARNING                                             |\n");
            fprintf( stdout, "             |                                                                                                 |\n");
            fprintf( stdout, "             |                             fread on windows seems to be broken !                               |\n");
            fprintf( stdout, "             |                        the value returned by it is wrong but it's likely                        |\n");
            fprintf( stdout, "             |                        only a windows internal bug without consequences.                        |\n");
            fprintf( stdout, "             |                         Here's the dump of the given rom, please check                          |\n");
            fprintf( stdout, "             |                                   that every bytes are here !                                   |\n");
            fprintf( stdout, "             |                                                                                                 |\n");
            fprintf( stdout, "             |-------------------------------------------------------------------------------------------------|\n");

            /*for ( uint32_t i = 0; i < f_size; i++) {

                if ( i % 16 == 0)
                    fprintf( stdout, "\n 0x%05x0 | ", i / 16);

                fprintf( stdout, "%02x ", emul_cartridge[i]);
            }*/

            fprintf( stdout, "\n\n");

            fprintf( stdout, "[CRITICAL] : ---------------------------------------------------------------------------------------------------\n");
            fprintf( stdout, "             |                                             WARNING                                             |\n");
            fprintf( stdout, "             |                                                                                                 |\n");
            fprintf( stdout, "             |                             fread on windows seems to be broken !                               |\n");
            fprintf( stdout, "             |                        the value returned by it is wrong but it's likely                        |\n");
            fprintf( stdout, "             |                        only a windows internal bug without consequences.                        |\n");
            fprintf( stdout, "             |                         Here's the dump of the given rom, please check                          |\n");
            fprintf( stdout, "             |                                   that every bytes are here !                                   |\n");
            fprintf( stdout, "             |                                                                                                 |\n");
            fprintf( stdout, "             |-------------------------------------------------------------------------------------------------|\n");
        #else

            fprintf( stdout, "[FATAL] : fread issue\n");
            exit( EXIT_FAILURE);
        #endif
    }

    fclose( f);

    #ifdef __DEBUG
        fprintf( stdout, "[INFO] : rom name : %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", emul_cartridge[0x134], emul_cartridge[0x135], emul_cartridge[0x136], emul_cartridge[0x137], emul_cartridge[0x138], emul_cartridge[0x139], emul_cartridge[0x13a], emul_cartridge[0x13b], emul_cartridge[0x13c], emul_cartridge[0x13d], emul_cartridge[0x13e], emul_cartridge[0x13f], emul_cartridge[0x140], emul_cartridge[0x141], emul_cartridge[0x142], emul_cartridge[0x143]);
    #endif

    switch ( emul_cartridge[0x147]) {

        case 0x00:

            emul_mbctype = MBC_TYPE_0;
            emul_current_ram_bak = -1;
            break;
        case 0x01:

            emul_mbctype = MBC_TYPE_1;
            emul_current_ram_bak = -1;
            break;
        case 0x02:
        case 0x03:
        
            emul_mbctype = MBC_TYPE_1;
            emul_current_ram_bak = 0;
            break;
        case 0x05:
        case 0x06:

            emul_mbctype = MBC_TYPE_2;
            emul_current_ram_bak = -1;
            break;
        case 0x1A:
        case 0x1B:

            emul_mbctype = MBC_TYPE_5;
            emul_current_ram_bak = 0;
            break;
        default:

            #ifdef __DEBUG
                fprintf( stdout, "[FATAL] : unable to read the rom mbc type (0x%02x)\n", emul_cartridge[0x147]);
            #endif
            exit( EXIT_FAILURE);
    }

    #ifdef __DEBUG
    if ( emul_mbctype == MBC_TYPE_0)
        fprintf( stdout, "[INFO] : the rom MBC type is 0\n");
    else if ( emul_mbctype == MBC_TYPE_1)
        fprintf( stdout, "[INFO] : the rom MBC type is 1\n[WARNING] : not supported\n");
    else if ( emul_mbctype == MBC_TYPE_2)
        fprintf( stdout, "[INFO] : the rom MBC type is 2\n[WARNING] : not supported\n");
    else if ( emul_mbctype == MBC_TYPE_5)
        fprintf( stdout, "[INFO] : the rom MBC type is 5\n[WARNING] : not supported\n");
    #endif

    emul_current_rom_bank = 1;
    
    switch ( emul_cartridge[0x148]) {

        case 0x00:

            emul_total_rom_bank = 2;
            break;
        case 0x01:

            emul_total_rom_bank = 4;
            break;
        case 0x02:

            emul_total_rom_bank = 8;
            break;
        case 0x03:

            emul_total_rom_bank = 16;
            break;
        case 0x04:

            emul_total_rom_bank = 32;
            break;
        case 0x05:

            emul_total_rom_bank = 64;
            break;
        case 0x06:

            emul_total_rom_bank = 128;
            break;
        case 0x07:

            emul_total_rom_bank = 256;
            break;
        case 0x08:

            emul_total_rom_bank = 512;
            break;
        case 0x52:
        case 0x53:
        case 0x54:

            #ifdef __DEBUG
                fprintf( stdout, "[FATAL] the rom size of your cartridge is unoficial and not supported by this emulator\n");
            #endif

            exit( EXIT_FAILURE);
        default:

            #ifdef __DEBUG
                fprintf( stdout, "[FATAL] unable to read the total number of rom bank\n");
            #endif 

            exit( EXIT_FAILURE);
    }

    #ifdef __DEBUG
        fprintf( stdout, "[INFO] cartridge rom size : %"PRIu16" KiB\n", 16 * emul_total_rom_bank);
    #endif

    if ( emul_current_ram_bak != -1) {

        switch ( emul_cartridge[0x149]) {

            case 0:

                emul_total_rom_bank = 0;
                break;
            case 1:

                emul_total_rom_bank = -1;
                break;
            case 2:

                emul_total_rom_bank = 1;
                break;
            case 3:

                emul_total_rom_bank = 4;
                break;
            case 4:

                emul_total_rom_bank = 16;
                break;
            case 5:

                emul_total_rom_bank = 8;
                break;
            default:

                #ifdef __DEBUG
                    fprintf( stdout, "[FATAL] unable to read the total number of ram bank\n");
                #endif 

                exit( EXIT_FAILURE);
        }
    }

    emul_current_ram_bak = 0;

    #ifdef __DEBUG
        if ( emul_total_rom_bank == 0 || emul_total_rom_bank == -1) 
            fprintf( stdout, "[INFO] the cartridge has no ram bank\n");
        else
            fprintf( stdout, "[INFO] the cartridge has %"PRIu8" KiB of ram\n", emul_total_rom_bank * 8);
    #endif

    #ifdef __DEBUG
        if ( emul_cartridge[0x14A] == 0)
            fprintf( stdout, "[INFO] the cartridge is supposed to be sold in japan\n");
        else if ( emul_cartridge[0x14A] == 1)
            fprintf( stdout, "[INFO] the cartridge is supposed to be sold in non-japan country\n");
        else
            fprintf( stdout, "[WARNING] unable to read the sold destination of this cartridge\n");
    #endif

    for ( uint32_t i = 0; i < 0x4000; i++)
        memory[i] = emul_cartridge[i];

    for ( uint32_t i = 0x4000; i < 0x8000; i++)
        memory[i] = emul_cartridge[i];
}

void handle_banking( uint16_t addr, uint8_t value) {

    if ( emul_mbctype == 0)
        return;

    if ( addr >= 0x2000 && addr < 0x4000) {

        emul_current_rom_bank = ( value & 0b00011111);

        if ( emul_current_rom_bank == 0)
            emul_current_rom_bank = 1;

        #ifdef __DEBUG
            if ( emul_current_rom_bank > emul_total_rom_bank)
                NO_IMPL
        #endif

        return;
    }

    #ifdef __DEBUG
        NO_IMPL
        fprintf( stdout, "(handle_banking) 0x%04x, value 0x%02x\n", addr, value);
    #endif

    return;
}

void dma( uint8_t start_adress) {

    uint16_t addr = start_adress << 8;

    for ( uint16_t i = 0; i < 0xA0; i++)
        memory[0xFE00 + i] = memory[addr + i];

    return;
}

extern void memory_write8( uint16_t addr, uint8_t value) {

    if (addr < 0x8000)
        handle_banking( addr, value);

    else if ( addr >= 0x8000 && addr < 0xA000) // 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
        if ( addr == 0x9800) {
            //fprintf( stdout, "WRITING 0x9800 value -> 0x%02x\n", value);
            memory[addr] = value;
        } else
            memory[addr] = value;

    else if ( addr >= 0xC000 && addr < 0xE000) // 4KB Work RAM Bank 0 (WRAM)
        memory[addr] = value;

    else if ( addr >= 0xFE00 && addr < 0xFEA0) // Sprite attribute table (OAM)
        memory[addr] = value;

    else if ( addr >= 0xFEA0 && addr < 0xFF00); // Not Usable - Nintendo says use of this area is prohibited
        // memory[addr] = value;

    else if ( addr == 0xFF00) { // P1/JOYP - Joypad (R/W)

        // fprintf( stdout, "[INFO] writing 0xFF00 value -> 0x%02x\n", value);
        memory[addr] = value;
    }

    else if ( addr == 0xFF01) // SB - Serial transfer data (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF02) // SC - Serial Transfer Control (R/W)
        memory[addr] = value;

    else if ( addr == 0xFF04) // DIV - Divider Register (R/W)
        memory[addr] = 0;
    else if ( addr == 0xFF05) // TIMA - Timer counter (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF06) // TMA - Timer Modulo (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF07) // TAC - Timer Control (R/W)
        memory[addr] = value;
    
    else if ( addr == 0xFF0F) // IF - Interrupt flag (R/W)
        memory[addr] = value;

    else if ( addr == 0xFF10) // NR10 - Channel 1 Sweep register (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF11) // NR11 - Channel 1 Sound length/Wave pattern duty (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF12) // NR12 - Channel 1 Volume Envelope (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF13) // NR13 - Channel 1 Frequency lo (W)
        memory[addr] = value;
    else if ( addr == 0xFF14) // NR14 - Channel 1 Frequency hi (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF16) // NR21 - Channel 2 Sound Length/Wave Pattern Duty (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF17) // NR22 - Channel 2 Volume Envelope (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF18) // NR23 - Channel 2 Frequency lo data (W)
        memory[addr] = value;
    else if ( addr == 0xFF19) // NR24 - Channel 2 Frequency hi data (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF1A) // NR30 - Channel 3 Sound on/off (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF1B) // NR31 - Channel 3 Sound Length (W)
        memory[addr] = value;
    else if ( addr == 0xFF1C) // NR32 - Channel 3 Select output level (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF1D) // NR33 - Channel 3 Frequency’s lower data (W)
        memory[addr] = value;
    else if ( addr == 0xFF1E) // NR34 - Channel 3 Frequency’s higher data (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF20) // NR41 - Channel 4 Sound Length (W)
        memory[addr] = value;
    else if ( addr == 0xFF21) // NR42 - Channel 4 Volume Envelope (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF22) // NR43 - Channel 4 Polynomial Counter (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF23) // NR44 - Channel 4 Counter/consecutive; Inital (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF24) // NR50 - Channel control / ON-OFF / Volume (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF25) // NR51 - Selection of Sound output terminal (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF26) // NR52 - Sound on/off (R/W)
        memory[addr] = value;

    else if ( addr == 0xFF40) // LCDC - (LCD Control) (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF41) // STAT - (LCD Status) (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF42) // SCY - Scroll Y (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF43) // SCX - Scroll X (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF45) // LYC - (LY Compare) (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF46) // DMA Transfer and Start Address (W)
        dma( value);
    else if ( addr == 0xFF47) // BGP (BG Palette Data) (R/W) - Non CGB Mode Only
        memory[addr] = value;
    else if ( addr == 0xFF48) // OBP0 (OBJ Palette 0 Data) (R/W) - Non CGB Mode Only
        memory[addr] = value;
    else if ( addr == 0xFF49) // OBP1 (OBJ Palette 1 Data) (R/W) - Non CGB Mode Only
        memory[addr] = value;
    else if ( addr == 0xFF4A) // WY (Window Y Position) (R/W)
        memory[addr] = value;
    else if ( addr == 0xFF4B) // WX (Window X Position) (R/W)
        memory[addr] = value;

    else if ( addr >= 0xFF80 && addr < 0xFFFF) { // High RAM (HRAM)

        memory[addr] = value;
        
        /*if ( addr == 0xFF80) {

            fprintf( stderr, "WRITING at 0xFF80 ...0x%02x\n", value);
            cpu_pc();
        }*/
        

        return;

    } else if ( addr == 0xFFFF) // IE - Interrupt Enable (R/W)
        memory[addr] = value;
    #ifdef __DEBUG
    else {

        NO_IMPL
        fprintf( stdout, "(write8) 0x%02x, value 0x%02x\n", addr, value);
    }
    #endif

    return;
}

extern uint8_t memory_read8( uint16_t addr) {

    if ( addr < 0x4000) // 8KB ROM Bank 00 (in cartridge, fixed at bank 00)
        return memory[addr];
    else if ( addr >= 0x4000 && addr < 0x8000 && emul_current_rom_bank == 1) // 8KB ROM Bank 01
        return emul_cartridge[addr + (( emul_current_rom_bank - 1) * 0x4000)];

    else if ( addr >= 0x8000 && addr < 0xA000) // 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
        return memory[addr];
    
    else if ( addr >= 0xC000 && addr < 0xE000) // 4KB Work RAM Bank 0 (WRAM)
        return memory[addr];

    else if ( addr >= 0xFE00 && addr < 0xFEA0) // Sprite attribute table (OAM)
        return memory[addr];

    else if ( addr >= 0xFEA0 && addr < 0xFF00) // Not Usable - Nintendo says use of this area is prohibited
        return 0;

    else if ( addr == 0xFF00) // P1/JOYP - Joypad (R/W)
        return memory[addr];

    else if ( addr == 0xFF01) // SB - Serial transfer data (R/W)
        return memory[addr];
    else if ( addr == 0xFF02) // SC - Serial Transfer Control (R/W)
        return memory[addr];

    else if ( addr == 0xFF04) // DIV - Divider Register (R/W)
        return memory[addr];
    else if ( addr == 0xFF05) // TIMA - Timer counter (R/W)
        return memory[addr];
    else if ( addr == 0xFF06) // TMA - Timer Modulo (R/W)
        return memory[addr];
    else if ( addr == 0xFF07) // TAC - Timer Control (R/W)
        return memory[addr];

    else if ( addr == 0xFF10) // NR10 - Channel 1 Sweep register (R/W)
        return memory[addr];
    else if ( addr == 0xFF11) // NR11 - Channel 1 Sound length/Wave pattern duty (R/W)
        return memory[addr];    
    else if ( addr == 0xFF12) // NR12 - Channel 1 Volume Envelope (R/W)
        return memory[addr];
    else if ( addr == 0xFF14) // NR14 - Channel 1 Frequency hi (R/W)
        return memory[addr];
    else if ( addr == 0xFF16) // NR21 - Channel 2 Sound Length/Wave Pattern Duty (R/W)
        return memory[addr];
    else if ( addr == 0xFF17) // NR22 - Channel 2 Volume Envelope (R/W)
        return memory[addr];
    else if ( addr == 0xFF19) // NR24 - Channel 2 Frequency hi data (R/W)
        return memory[addr];
    else if ( addr == 0xFF1A) // NR30 - Channel 3 Sound on/off (R/W)
        return memory[addr];
    else if ( addr == 0xFF1C) // NR32 - Channel 3 Select output level (R/W)
        return memory[addr];
    else if ( addr == 0xFF1E) // NR34 - Channel 3 Frequency’s higher data (R/W)
        return memory[addr];
    else if ( addr == 0xFF21) // NR42 - Channel 4 Volume Envelope (R/W)
        return memory[addr];
    else if ( addr == 0xFF22) // NR43 - Channel 4 Polynomial Counter (R/W)
        return memory[addr];
    else if ( addr == 0xFF23) // NR44 - Channel 4 Counter/consecutive; Inital (R/W)
        return memory[addr];
    else if ( addr == 0xFF24) // NR50 - Channel control / ON-OFF / Volume (R/W)
        return memory[addr];
    else if ( addr == 0xFF25) // NR51 - Selection of Sound output terminal (R/W)
        return memory[addr];
    else if ( addr == 0xFF26) // NR52 - Sound on/off (R/W)
        return memory[addr];
    
    else if ( addr == 0xFF0F) // IF - Interrupt flag (R/W)
        return memory[addr];
    else if ( addr == 0xFF40) // LCDC - (LCD Control) (R/W)
        return memory[addr];
    else if ( addr == 0xFF41) // STAT - (LCD Status) (R/W)
        return memory[addr];
    else if ( addr == 0xFF42) // SCY - Scroll Y (R/W)
        return memory[addr];
    else if ( addr == 0xFF43) // SCX - Scroll X (R/W)
        return memory[addr];
    else if ( addr == 0xFF44) // LY - (LCD Y Coordinate) (R)
        return memory[addr];
    else if ( addr == 0xFF45) // LYC - (LY Compare) (R/W)
        return memory[addr];
    else if ( addr == 0xFF47) // BGP (BG Palette Data) (R/W) - Non CGB Mode Only
        return memory[addr];
    else if ( addr == 0xFF48) // OBP0 (OBJ Palette 0 Data) (R/W) - Non CGB Mode Only
        return memory[addr];
    else if ( addr == 0xFF49) // OBP1 (OBJ Palette 1 Data) (R/W) - Non CGB Mode Only
        return memory[addr];
    else if ( addr == 0xFF4A) // WY (Window Y Position) (R/W)
        return memory[addr];
    else if ( addr == 0xFF4B) // WX (Window X Position) (R/W)
        return memory[addr];

    if ( addr >= 0xFF80 && addr < 0xFFFF) // High RAM (HRAM)
        return memory[addr];
    
    else if ( addr == 0xFFFF) // IE - Interrupt Enable (R/W)
        return memory[addr];

    #ifdef __DEBUG
        NO_IMPL
        fprintf( stdout, "(read8) 0x%02x\n", addr);
    #endif
    return 0;
}

extern void memory_write16( uint16_t addr, uint16_t value) {

    if ( addr >= 0xC000 && addr < 0xDFFF) { // 4KB Work RAM Bank 0 (WRAM)
        memory[addr + 1] = (uint8_t)(value >> 8);
        memory[addr] = (uint8_t)(value & 0xFF);
    }
    #ifdef __DEBUG
    else {

        NO_IMPL
        fprintf( stdout, "\t(write16) 0x%04x; value 0x%02x\n", addr, value);
    }
    #endif
    return;
}

extern uint16_t memory_read16( uint16_t addr) {

    if ( addr < 0x3FFF) // 8KB ROM Bank 00 (in cartridge, fixed at bank 00)
        return (uint16_t)(memory[addr] + (memory[addr + 1] << 8));
    else if ( addr >= 0x4000 && addr < 0x7FFF && emul_current_rom_bank == 1) // 8KB ROM Bank 01
        return (uint16_t)(memory[addr] + (memory[addr + 1] << 8));
    else if ( addr >= 0xC000 && addr < 0xDFFF)
        return (uint16_t)(memory[addr] + (memory[addr + 1] << 8));

    #ifdef __DEBUG
        NO_IMPL
        fprintf( stdout, "\taddr 0x%04x\n", addr);
    #endif
    return 0;
}

extern void memory_special_service_ly( uint16_t value) {

    memory[0xFF44] = value;
}

extern void memory_special_service_div( uint8_t value) {

    memory[0xFF04] = value;
}

#ifdef __STEP
    extern void prot_memory_write8( uint16_t addr, uint8_t value) {

        memory[addr] = value;
        return;
    }

    extern uint8_t prot_memory_read8( uint16_t addr) {

        return memory[addr];
    }
#endif