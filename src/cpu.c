#include "main.h"
#include "cpu.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "memory.h"
#include "interrupt.h"

#ifdef DEBUG
    #include <string.h>
#elifdef STEP
    #include <string.h>
#endif

#ifdef STEP
void cpu_debugger(void);
#endif

#define FLAGS_Z 0b10000000
#define FLAGS_N 0b01000000
#define FLAGS_H 0b00100000
#define FLAGS_C 0b00010000
#define FLAGS_ALL (FLAGS_Z | FLAGS_N | FLAGS_H | FLAGS_C)

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
    0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, // D
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, // E
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, // F
};

/*  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
const uint8_t instruction_time[256] = {
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1, // 0 OK
    0, 3, 2, 2, 1, 1, 2, 0, 3, 2, 2, 2, 1, 1, 2, 2, // 1
    2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1, // 2
    2, 3, 3, 2, 3, 3, 3, 0, 2, 0, 2, 0, 1, 1, 2, 0, // 3
    1, 0, 1, 1, 1, 0, 2, 1, 0, 1, 0, 1, 0, 1, 2, 1, // 4
    1, 0, 0, 0, 1, 0, 2, 1, 0, 0, 0, 0, 0, 1, 2, 1, // 5
    1, 1, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 2, 1, // 6
    2, 2, 2, 2, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 2, 0, // 7
    1, 1, 1, 0, 1, 1, 2, 1, 0, 1, 0, 0, 0, 0, 2, 0, // 8
    1, 1, 0, 1, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, // 9
    1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 2, 1, // A
    1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // B
    2, 3, 3, 4, 3, 4, 2, 0, 2, 4, 3, 0, 0, 6, 2, 8, // C
    2, 3, 3, 0, 0, 4, 2, 0, 2, 4, 0, 0, 0, 0, 2, 8, // D
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

    bool halted;

    #ifdef DEBUG
        uint64_t ex;
    #endif
};

#if STEP
    uint8_t verbose = 1;
#endif

struct registers registers;
uint64_t emul_time = 0;
#ifdef CPU_LOG
FILE *f;
#endif

static void cpu_destroy(void) {
    #ifdef DEBUG
        LOG_MESG(LOG_DEBUG, "executed: %"PRIu64"", registers.ex);
    #endif

    return;
}

void cpu_pc(void) {
    LOG_MESG(LOG_INFO, "PC : 0x%04x", registers.pc);
}

void cpu_init(void) {
    atexit(cpu_destroy);

    #ifdef DEBUG
        uint16_t inst = 1; // 0xCB

        for (uint16_t i = 0; i < 256; i++)
            if (instruction_time[i])
                inst++;

        LOG_MESG(LOG_DEBUG, "%"PRIu16"/243 (%2.2f%%) instructions fully implemented (0xCB counting)", inst, (inst/243.0)*100.0);
    #endif

    registers.pc = 0x0100;
    registers.af = 0x0100;
    registers.f = FLAGS_Z | FLAGS_H | FLAGS_C;
    registers.bc = 0x0013;
    registers.de = 0x00D8;
    registers.hl = 0x014D;
    registers.sp = 0xFFFE;

    registers.halted = false;

    #ifdef DEBUG
        registers.ex = 0;
    #endif

    emul_time = 0;

    #ifdef CPU_LOG
    f = fopen("./cpu.log", "w");
    if (f == NULL) {
        LOG_MESG(LOG_FATAL, "Couldn't open f");
        exit(EXIT_FAILURE);
    }

    char buf[512];
    snprintf(
        buf,
        sizeof(buf),
        "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
        registers.a,
        registers.f,
        registers.b,
        registers.c,
        registers.d,
        registers.e,
        registers.h,
        registers.l,
        registers.sp,
        registers.pc,
        0x00,
        0xC3,
        0x13,
        0x02
    );
    buf[sizeof(buf) - 1] = '\0';
    fwrite(buf, strlen(buf), 1, f);
    fflush(f);
    #endif
}

static void cpu_execute(uint8_t opcode, uint8_t op8, uint16_t op16) {

    #ifdef __linux__
        #include "dat/cpu.h"
    #elifdef _WIN32
        #include "dat\cpu.h"
    #else
        #error "unknow target"
    #endif
}

void cpu_interrupt(uint16_t addr) {
    registers.halted = false;
    registers.sp -= 2;
    memory_write16(registers.sp, registers.pc);
    registers.pc = addr;
}

void cpu_wake_up(void) {
    assert(registers.halted);

    registers.halted = false;
}

bool cpu_halted(void) {
    return registers.halted;
}

uint64_t cpu_run(void) {
    if (registers.halted) {
        emul_time += 1;
        return emul_time;
    }

    #ifdef STEP
    cpu_debugger();
    #endif

    uint8_t inst = memory_read8(registers.pc);

    #ifdef STEP
        if (verbose)
            LOG_MESG(LOG_DEBUG, "execute at \t0x%02x, instruction \t0x%02x \t(%d)\t\ttime = %"PRIu64"", registers.pc, inst, inst, emul_time);
    #endif

    (registers.pc)++;

    if (instruction_operand[inst] == 0)
        cpu_execute(inst, 0, 0);
    else if (instruction_operand[inst] == 1) {
        (registers.pc)++;
        cpu_execute(inst, memory_read8(registers.pc - 1), 0);
    } else {
        registers.pc += 2;
        cpu_execute(inst, 0, memory_read16(registers.pc - 2));
    }

    #ifdef CPU_LOG
    char buf[512];
    snprintf(
        buf,
        sizeof(buf),
        "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
        registers.a,
        registers.f,
        registers.b,
        registers.c,
        registers.d,
        registers.e,
        registers.h,
        registers.l,
        registers.sp,
        registers.pc,
        memory_read8(registers.pc),
        memory_read8(registers.pc + 1),
        memory_read8(registers.pc + 2),
        memory_read8(registers.pc + 3)
    );
    buf[sizeof(buf) - 1] = '\0';
    fwrite(buf, strlen(buf), 1, f);
    fflush(f);
    #endif

    emul_time += instruction_time[inst];

    #ifdef DEBUG
        if (instruction_time[inst] == 0 && inst != 0xCB)
            LOG_MESG(LOG_WARN, "Instruction 0x%02x (%"PRIu8") does not have time", inst, inst);
    #endif

    #ifdef DEBUG
        (registers.ex)++;
    #endif

    return emul_time;
}

#ifdef STEP
static void cpu_debugger(void) {
    #define MAX_CMD 500
    #define MAX_BREAKPOINTS 500

    char cmd[MAX_CMD];
    static char last[MAX_CMD];
    static int64_t toexecute = 0;
    static int32_t addr, breakpoints[MAX_BREAKPOINTS];
    static int8_t init = 1, display_register = 0;
    uint16_t breakpoint = MAX_BREAKPOINTS, tmp16;

    if (init == 1) {
        for (uint16_t i = 0; i < MAX_BREAKPOINTS; i++)
            breakpoints[i] = -1;

        init = 0;
    }

    for (tmp16 = 0; tmp16 < MAX_BREAKPOINTS && toexecute; tmp16++) {
        if (registers.pc == breakpoints[tmp16]) {
            breakpoint = tmp16;
            toexecute = 0;
        }
    }

    if (tmp16 == MAX_BREAKPOINTS)
        breakpoint = MAX_BREAKPOINTS;

    if (toexecute == 0) {
        if (display_register) {
            fprintf(stdout, "debugger> --------DISPLAY REGISTER-------\n");
            fprintf(stdout, "debugger> af    %d (0x%02x)\n", registers.af, registers.af);
            fprintf(stdout, "debugger> bc    %d (0x%02x)\n", registers.bc, registers.bc);
            fprintf(stdout, "debugger> de    %d (0x%02x)\n", registers.de, registers.de);
            fprintf(stdout, "debugger> hl    %d (0x%02x)\n", registers.hl, registers.hl);
            fprintf(stdout, "debugger> sp    %d (0x%02x)\n", registers.sp, registers.sp);
            fprintf(stdout, "debugger> pc    0x%02x\n", registers.pc);

            fprintf(stdout, "debugger> flags ");
            if (registers.f & FLAGS_Z)
                fprintf(stdout, "FLAG_Z ");
            if (registers.f & FLAGS_N)
                fprintf(stdout, "FLAG_N ");
            if (registers.f & FLAGS_H)
                fprintf(stdout, "FLAG_H ");
            if (registers.f & FLAGS_C)
                fprintf(stdout, "FLAG_C");
            fprintf(stdout, "\n");
        }

        if (breakpoint != MAX_BREAKPOINTS)
            fprintf(stdout, "debugger> breakpoint %d hit ! 0x%02x\n", breakpoint, registers.pc);

        do {
            fprintf(stdout, "cmd (help)>");
            fgets(cmd, MAX_CMD - 1, stdin);
            cmd[MAX_CMD - 1] = '\0';

            if (cmd[0] == '\n')
                strncpy(cmd, last, MAX_CMD);
            else {
                cmd[strlen(cmd) - 1] = '\0';
                strncpy(last, cmd, MAX_CMD);
            }

            if (!strncmp(cmd, "help", 4)) {
                fprintf(stdout, "debugger> verbose\ndebugger>\tturn on/off verbose option\n");
                fprintf(stdout, "debugger> continue 0xVALUE\ndebugger>\tcontinue execution for VALUE instructions. 0 as VALUE continue instruction forever\n");
                fprintf(stdout, "debugger> print [REGISTER|0xVALUE]\ndebugger>\tprint register REGISTER value or ram at adresse VALUE\n");
                fprintf(stdout, "debugger> step\ndebugger>\texecute 1 instruction\n");
                fprintf(stdout, "debugger> breakpoint [VALUE]\ndebugger>\tplace breakpoint at adresse VALUE\n");
                fprintf(stdout, "debugger> delete [VALUE]\ndebugger>\tdelete breakpoint at adresse VALUE\n");
                fprintf(stdout, "debugger> info breakpoints\ndebugger>\tlist all active breakpoints\n");
                fprintf(stdout, "debugger> info registers\ndebugger>\tprint all register\n");

            } else if (!strncmp(cmd, "print ", 6)) {
                if (!strcmp(cmd + 6, "a"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.a, registers.a);
                else if (!strcmp(cmd + 6, "f"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.f, registers.f);
                else if (!strcmp(cmd + 6, "af"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.af, registers.af);
                else if (!strcmp(cmd + 6, "b"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.b, registers.b);
                else if (!strcmp(cmd + 6, "c"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.c, registers.c);
                else if (!strcmp(cmd + 6, "bc"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.bc, registers.bc);
                else if (!strcmp(cmd + 6, "d"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.d, registers.d);
                else if (!strcmp(cmd + 6, "e"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.e, registers.e);
                else if (!strcmp(cmd + 6, "de"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.de, registers.de);
                else if (!strcmp(cmd + 6, "h"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.h, registers.h);
                else if (!strcmp(cmd + 6, "l"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.l, registers.l);
                else if (!strcmp(cmd + 6, "hl"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.hl, registers.hl);
                else if (!strcmp(cmd + 6, "sp"))
                    fprintf(stdout, "debugger> %d (0x%02x)\n", registers.sp, registers.sp);
                else if (!strcmp(cmd + 6, "pc"))
                    fprintf(stdout, "debugger> 0x%02x\n", registers.pc);
                else if (!strcmp(cmd + 6, "flags")) {
                    fprintf(stdout, "debugger> ");

                    if (registers.f & FLAGS_Z)
                        fprintf(stdout, "FLAG_Z ");
                    if (registers.f & FLAGS_N)
                        fprintf(stdout, "FLAG_N ");
                    if (registers.f & FLAGS_H)
                        fprintf(stdout, "FLAG_H ");
                    if (registers.f & FLAGS_C)
                        fprintf(stdout, "FLAG_C");

                    fprintf(stdout, "\n");
                } else if (!strncmp(cmd + 6, "ime", 3)) {
                    if (interrupt_ask())
                        fprintf(stdout, "debugger> TRUE\n");
                    else
                        fprintf(stdout, "debugger> FALSE\n");
                
                } else if (!strncmp(cmd + 6, "0x", 2)) {
                
                    if ((cmd[8] < '0' || cmd[8] > '9') && cmd[8] != 'a' && cmd[8] != 'A' && cmd[8] != 'b' && cmd[8] != 'B' && cmd[8] != 'c' && cmd[8] != 'C' && cmd[8] != 'd' && cmd[8] != 'D' && cmd[8] != 'e' && cmd[8] != 'E' && cmd[8] != 'f' && cmd[8] != 'F') {
                        fprintf(stdout, "debugger> error adresse malformed\n");
                        cmd[0] = '!';
                    } else {
                        sscanf(cmd + 8, "%04x", &addr);
                        fprintf(stdout, "debugger> %"PRIu8"\n", prot_memory_read8(addr));
                    }
                } else
                    fprintf(stdout, "debugger> unknow register or ram adresse\n");
            } else if (!strncmp(cmd, "continue ", 9)) {
                if (cmd[9] < '0' || cmd[9] > '9') {
                    fprintf(stdout, "debugger> error command malformed\n");
                } else {
                    toexecute = atoi(cmd + 9);

                    if (toexecute == 0)
                        toexecute = -1;
                }
            } else if (!strncmp(cmd, "verbose ", 7)) {
                if (verbose) {
                    fprintf(stdout, "debugger> switching verbose OFF\n");
                    verbose = 0;
                } else {
                    fprintf(stdout, "debugger> switching verbose ON\n");
                    verbose = 1;
                }
            } else if (!strncmp(cmd, "breakpoint ", 11)) {
                if (strncmp(cmd + 11, "0x", 2) || ((!(cmd[13] > '0' || cmd[13] < '9')) && cmd[13] != 'a' && cmd[13] != 'A' && cmd[13] != 'b' && cmd[13] != 'B' && cmd[13] != 'c' && cmd[13] != 'C' && cmd[13] != 'd' && cmd[13] != 'D' && cmd[13] != 'e' && cmd[13] != 'E' && cmd[13] != 'f' && cmd[13] != 'F') )
                    fprintf(stdout, "debugger> error adresse malformed\n");
                else {
                    uint16_t i = 0;

                    for (; breakpoints[i] != -1 && i < MAX_BREAKPOINTS; i++);

                    if (i == MAX_BREAKPOINTS)
                        fprintf(stdout, "debugger> no more breakpoints avalaible\n");
                    else {
                        cmd[17] = '\0';
                        sscanf(cmd + 13, "%x", &(breakpoints[i]));
                        fprintf(stdout, "debugger> adding breakpoint %d at 0x%02x\n", i, breakpoints[i]);
                    }
                }
            } else if (!strncmp(cmd, "delete ", 7)) {
                if (strncmp(cmd + 7, "0x", 2) || ((!(cmd[9] > '0' || cmd[9] < '9')) && cmd[9] != 'a' && cmd[9] != 'A' && cmd[9] != 'b' && cmd[9] != 'B' && cmd[9] != 'c' && cmd[9] != 'C' && cmd[9] != 'd' && cmd[9] != 'D' && cmd[9] != 'e' && cmd[9] != 'E' && cmd[9] != 'f' && cmd[9] != 'F') )
                    fprintf(stdout, "debugger> error adresse malformed\n");
                else {
                    uint16_t i = 0, br;

                    cmd[13] = '\0';
                    sscanf(cmd + 9, "%hx", &br);

                    for (; breakpoints[i] != br && i < MAX_BREAKPOINTS; i++);

                    if (i == MAX_BREAKPOINTS)
                        fprintf(stdout, "debugger> no breakpoints at adresse %02x\n", br);
                    else {

                        fprintf(stdout, "debugger> delete breakpoint %d at 0x%02x\n", i, breakpoints[i]);
                        breakpoints[i] = -1;
                    }
                }
            } else if (!strncmp(cmd, "info breakpoints", 16)) {
                for (uint16_t i = 0; i < MAX_BREAKPOINTS; i++)
                    if (breakpoints[i] != -1)
                        fprintf(stdout, "debugger>\tbreakpoints %d at 0x%02x\n", i, breakpoints[i]);

            } else if (!strncmp(cmd, "info registers", 14)) {
                fprintf(stdout, "debugger> a     %d (0x%02x)\n", registers.a, registers.a);
                fprintf(stdout, "debugger> f     %d (0x%02x)\n", registers.f, registers.f);
                fprintf(stdout, "debugger> af    %d (0x%02x)\n", registers.af, registers.af);
                fprintf(stdout, "debugger> b     %d (0x%02x)\n", registers.b, registers.b);
                fprintf(stdout, "debugger> c     %d (0x%02x)\n", registers.c, registers.c);
                fprintf(stdout, "debugger> bc    %d (0x%02x)\n", registers.bc, registers.bc);
                fprintf(stdout, "debugger> d     %d (0x%02x)\n", registers.d, registers.d);
                fprintf(stdout, "debugger> e     %d (0x%02x)\n", registers.e, registers.e);
                fprintf(stdout, "debugger> de    %d (0x%02x)\n", registers.de, registers.de);
                fprintf(stdout, "debugger> h     %d (0x%02x)\n", registers.h, registers.h);
                fprintf(stdout, "debugger> l     %d (0x%02x)\n", registers.l, registers.l);
                fprintf(stdout, "debugger> hl    %d (0x%02x)\n", registers.hl, registers.hl);
                fprintf(stdout, "debugger> sp    %d (0x%02x)\n", registers.sp, registers.sp);
                fprintf(stdout, "debugger> pc    0x%02x\n", registers.pc);

                fprintf(stdout, "debugger> flags ");
                if (registers.f & FLAGS_Z)
                    fprintf(stdout, "FLAG_Z ");
                if (registers.f & FLAGS_N)
                    fprintf(stdout, "FLAG_N ");
                if (registers.f & FLAGS_H)
                    fprintf(stdout, "FLAG_H ");
                if (registers.f & FLAGS_C)
                    fprintf(stdout, "FLAG_C");
                fprintf(stdout, "\n");

            } else if (!strncmp(cmd, "display registers", 17)) {
                if (display_register) {
                    fprintf(stdout, "debugger> switching display register OFF\n");
                    display_register = 0;
                } else {
                    fprintf(stdout, "debugger> switching display register ON\n");
                    display_register = 1;
                }
            }else if (strncmp(cmd, "step", 4))
                fprintf(stdout, "debugger> unknow command\n");
        } while (strncmp(cmd, "continue", 8) && strncmp(cmd, "step", 4));
    } else if (toexecute != -1)
        toexecute--;
}
#endif
