#include "main.h"
#include "cpu.h"

#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "interrupt.h"

#ifdef __DEBUG
    #include <string.h>
#elif __STEP
    #include <string.h>
#endif

#define FLAGS_Z 0b10000000
#define FLAGS_N 0b01000000
#define FLAGS_H 0b00100000
#define FLAGS_C 0b00010000

#define INTERRUPT_BIT_VBLANK    0b00000001
#define INTERRUPT_BIT_LCD_STAT  0b00000010
#define INTERRUPT_BIT_TIMER     0b00000100
#define INTERRUPT_BIT_SERIAL    0b00001000
#define INTERRUPT_BIT_JOYPAD    0b00010000

#define INTERRUPT_ADDR_VBLANK   0x40
#define INTERRUPT_ADDR_LCD_STAT 0x48
#define INTERRUPT_ADDR_TIMER    0x50
#define INTERRUPT_ADDR_SERIAL   0x58
#define INTERRUPT_ADDR_JOYPAD   0x60

/*  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
const uint8_t instruction_operand[256] = {
    0, 2, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 1, 0, // 0 OK
    0, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, // 1
    1, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, // 2
    1, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
    0, 0, 2, 2, 2, 0, 1, 0, 0, 0, 2, 1, 0, 2, 1, 0, // C
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, // D
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, // E
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, // F
};

/*  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
const uint8_t instruction_time[256] = {
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1, // 0 OK
    0, 3, 2, 2, 1, 1, 2, 0, 3, 2, 2, 2, 1, 1, 2, 2, // 1
    2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1, // 2
    2, 3, 3, 2, 3, 3, 3, 0, 2, 0, 2, 0, 1, 1, 2, 0, // 3
    1, 0, 1, 0, 1, 0, 2, 1, 0, 1, 0, 1, 0, 1, 2, 1, // 4
    1, 0, 0, 0, 1, 0, 2, 1, 0, 0, 0, 0, 0, 1, 2, 1, // 5
    1, 1, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 2, 1, // 6
    2, 2, 2, 2, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 2, 0, // 7
    1, 1, 1, 0, 1, 1, 2, 1, 0, 1, 0, 0, 0, 0, 2, 0, // 8
    1, 1, 0, 1, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, // 9
    1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 2, 1, // A
    1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // B
    2, 3, 3, 4, 3, 4, 2, 0, 2, 4, 3, 0, 0, 6, 2, 0, // C
    2, 3, 0, 0, 0, 4, 2, 0, 2, 4, 0, 0, 0, 0, 2, 0, // D
    3, 3, 2, 0, 0, 4, 2, 0, 4, 1, 4, 0, 0, 0, 2, 4, // E
    3, 3, 0, 1, 0, 4, 2, 0, 3, 2, 4, 1, 0, 0, 2, 4, // F
};

struct registers {
	struct {
		union {
			struct {
				uint8_t f;
				uint8_t a;
			};
			uint16_t af;
		};
	};
	
	struct {
		union {
			struct {
				uint8_t c;
				uint8_t b;
			};
			uint16_t bc;
		};
	};
	
	struct {
		union {
			struct {
				uint8_t e;
				uint8_t d;
			};
			uint16_t de;
		};
	};
	
	struct {
		union {
			struct {
				uint8_t l;
				uint8_t h;
			};
			uint16_t hl;
		};
	};
	
	uint16_t sp;
	uint16_t pc;

    #ifdef __DEBUG
        uint64_t ex;
    #endif
};

#if __STEP
    uint8_t verbose = 1;
#endif

struct registers registers;
uint64_t emul_time = 0;

void cpu_destroy( void) {

    #ifdef __DEBUG
        fprintf( stderr, "executed : %"PRIu64"\n", registers.ex);
    #endif

    return;
}

extern void cpu_pc() {

    fprintf( stderr, "PC : 0x%04x\n", registers.pc);
}

extern void cpu_init() {

    atexit( cpu_destroy);

    #ifdef __DEBUG

        uint16_t inst = 1; // 0xCB

        for ( uint16_t i = 0; i < 256; i++)
            if ( instruction_time[i])
                inst++;

        fprintf( stdout, "[INFO] %"PRIu16"/243 (%2.2f%%) instructions fully implemented (0xCB counting)\n", inst, (inst/243.0)*100.0);
    #endif

    registers.pc = 0x0100;
    /*registers.af = 0x01B0;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014d;
    registers.sp = 0x0FFE;*/

    registers.af = 0x1100;
    registers.f = FLAGS_Z | FLAGS_H | FLAGS_C;
    registers.bc = 0x0000;
    registers.de = 0xFF56;
    registers.hl = 0x000D;
    registers.sp = 0xFFFE;

    #ifdef __DEBUG
        registers.ex = 0;
    #endif

    emul_time = 0;
}

void cpu_execute( uint8_t opcode, uint8_t op8, uint16_t op16) {

    #ifdef __WIN
        #include "dat\cpu.h"
    #elif __LIN
        #include "dat/cpu.h"
    #endif
}

#ifdef __WIN
    #include "dat\cpu_debug.h"
#elif __LIN
    #include "dat/cpu_debug.h"
#endif

extern void cpu_interrupt( uint16_t addr) {

    registers.sp -= 2;
    memory_write16( registers.sp, registers.pc);
    registers.pc = addr;
}

extern uint64_t cpu_run() {

    uint8_t inst;

    #ifdef __STEP
        cpu_debugger();

        // SIMULATE A PRESS
        /*if ( !(memory_read8( 0xFF00) & 0b00100000))
            memory_write8( 0xFF00, 0b00100001);*/
    #endif

    inst = memory_read8( registers.pc);

    #ifdef __STEP
        if ( verbose)
            fprintf( stdout, "[INFO] : execute at \t0x%02x, instruction \t0x%02x \t(%d)\t\ttime = %"PRIu64"\n", registers.pc, inst, inst, emul_time);
    #endif

    (registers.pc)++;

    if ( instruction_operand[inst] == 0)
        cpu_execute( inst, 0, 0);
    else if ( instruction_operand[inst] == 1) {

        (registers.pc)++;
        cpu_execute( inst, memory_read8( registers.pc - 1), 0);
    } else {

        registers.pc += 2;
        cpu_execute( inst, 0, memory_read16( registers.pc - 2));
    }

    emul_time += instruction_time[inst];

    #ifdef __DEBUG
        if ( !(instruction_time[inst]) && inst != 0xCB)
            fprintf( stdout, "[WARNING] instruction 0x%02x (%"PRIu8") does not have time\n", inst, inst);
    #endif

    #ifdef __DEBUG
        (registers.ex)++;
    #endif

    return emul_time;
}