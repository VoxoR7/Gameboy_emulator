#ifdef __STEP

    void cpu_debugger() {

        #define MAX_CMD 500
        #define MAX_BREAKPOINTS 500

        char cmd[MAX_CMD];
        static char last[MAX_CMD];
        static int64_t toexecute = 0;
        static int32_t addr, breakpoints[MAX_BREAKPOINTS];
        static int8_t init = 1, display_register = 0;
        uint16_t breakpoint = MAX_BREAKPOINTS, tmp16;

        if ( init == 1) {

            for ( uint16_t i = 0; i < MAX_BREAKPOINTS; i++)
                breakpoints[i] = -1;

            init = 0;
        }

        for ( tmp16 = 0; tmp16 < MAX_BREAKPOINTS && toexecute; tmp16++) {
            if ( registers.pc == breakpoints[tmp16]) {

                breakpoint = tmp16;
                toexecute = 0;
            }
        }

        if ( tmp16 == MAX_BREAKPOINTS)
            breakpoint = MAX_BREAKPOINTS;

        if ( toexecute == 0) {

            if ( display_register) {

                fprintf( stdout, "debugger> --------DISPLAY REGISTER-------\n");
                fprintf( stdout, "debugger> af    %d (0x%02x)\n", registers.af, registers.af);
                fprintf( stdout, "debugger> bc    %d (0x%02x)\n", registers.bc, registers.bc);
                fprintf( stdout, "debugger> de    %d (0x%02x)\n", registers.de, registers.de);
                fprintf( stdout, "debugger> hl    %d (0x%02x)\n", registers.hl, registers.hl);
                fprintf( stdout, "debugger> sp    %d (0x%02x)\n", registers.sp, registers.sp);
                fprintf( stdout, "debugger> pc    0x%02x\n", registers.pc);

                fprintf( stdout, "debugger> flags ");
                if ( registers.f & FLAGS_Z)
                    fprintf( stdout, "FLAG_Z ");
                if ( registers.f & FLAGS_N)
                    fprintf( stdout, "FLAG_N ");
                if ( registers.f & FLAGS_H)
                    fprintf( stdout, "FLAG_H ");
                if ( registers.f & FLAGS_C)
                    fprintf( stdout, "FLAG_C");
                fprintf( stdout, "\n");
            }

            if ( breakpoint != MAX_BREAKPOINTS)
                fprintf( stdout, "debugger> breakpoint %d hit ! 0x%02x\n", breakpoint, registers.pc);

            do {

                fprintf( stdout, "cmd (help)>");
                if ( fgets(cmd, MAX_CMD - 1, stdin) == NULL) {

                    #ifdef __DEBUG
                        fprintf( stdout, "fgets issue\n");
                    #endif
                }
                cmd[MAX_CMD - 1] = '\0';

                if ( cmd[0] == '\n')
                    strncpy( cmd, last, MAX_CMD);
                else {

                    cmd[strlen( cmd) - 1] = '\0';
                    strncpy( last, cmd, MAX_CMD);
                }

                if ( !strncmp( cmd, "help", 4)) {

                    fprintf( stdout, "debugger> verbose\ndebugger>\tturn on/off verbose option\n");
                    fprintf( stdout, "debugger> continue 0xVALUE\ndebugger>\tcontinue execution for VALUE instructions. 0 as VALUE continue instruction forever\n");
                    fprintf( stdout, "debugger> print [REGISTER|0xVALUE]\ndebugger>\tprint register REGISTER value or ram at adresse VALUE\n");
                    fprintf( stdout, "debugger> step\ndebugger>\texecute 1 instruction\n");
                    fprintf( stdout, "debugger> breakpoint [VALUE]\ndebugger>\tplace breakpoint at adresse VALUE\n");
                    fprintf( stdout, "debugger> delete [VALUE]\ndebugger>\tdelete breakpoint at adresse VALUE\n");
                    fprintf( stdout, "debugger> info breakpoints\ndebugger>\tlist all active breakpoints\n");
                    fprintf( stdout, "debugger> info registers\ndebugger>\tprint all register\n");

                } else if ( !strncmp( cmd, "print ", 6)) {

                    if ( !strcmp( cmd + 6, "a"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.a, registers.a);
                    else if ( !strcmp( cmd + 6, "f"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.f, registers.f);
                    else if ( !strcmp( cmd + 6, "af"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.af, registers.af);
                    else if ( !strcmp( cmd + 6, "b"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.b, registers.b);
                    else if ( !strcmp( cmd + 6, "c"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.c, registers.c);
                    else if ( !strcmp( cmd + 6, "bc"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.bc, registers.bc);
                    else if ( !strcmp( cmd + 6, "d"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.d, registers.d);
                    else if ( !strcmp( cmd + 6, "e"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.e, registers.e);
                    else if ( !strcmp( cmd + 6, "de"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.de, registers.de);
                    else if ( !strcmp( cmd + 6, "h"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.h, registers.h);
                    else if ( !strcmp( cmd + 6, "l"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.l, registers.l);
                    else if ( !strcmp( cmd + 6, "hl"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.hl, registers.hl);
                    else if ( !strcmp( cmd + 6, "sp"))
                        fprintf( stdout, "debugger> %d (0x%02x)\n", registers.sp, registers.sp);
                    else if ( !strcmp( cmd + 6, "pc"))
                        fprintf( stdout, "debugger> 0x%02x\n", registers.pc);
                    else if ( !strcmp( cmd + 6, "flags")) {

                        fprintf( stdout, "debugger> ");

                        if ( registers.f & FLAGS_Z)
                            fprintf( stdout, "FLAG_Z ");
                        if ( registers.f & FLAGS_N)
                            fprintf( stdout, "FLAG_N ");
                        if ( registers.f & FLAGS_H)
                            fprintf( stdout, "FLAG_H ");
                        if ( registers.f & FLAGS_C)
                            fprintf( stdout, "FLAG_C");

                        fprintf( stdout, "\n");
                    } else if ( !strncmp( cmd + 6, "ime", 3)) {

                        if ( interrupt_ask())
                            fprintf( stdout, "debugger> TRUE\n");
                        else
                            fprintf( stdout, "debugger> FALSE\n");
                    
                    } else if ( !strncmp( cmd + 6, "0x", 2)) {
                    
                        if (( cmd[8] < '0' || cmd[8] > '9') && cmd[8] != 'a' && cmd[8] != 'A' && cmd[8] != 'b' && cmd[8] != 'B' && cmd[8] != 'c' && cmd[8] != 'C' && cmd[8] != 'd' && cmd[8] != 'D' && cmd[8] != 'e' && cmd[8] != 'E' && cmd[8] != 'f' && cmd[8] != 'F') {

                            fprintf( stdout, "debugger> error adresse malformed\n");
                            cmd[0] = '!';
                        } else {

                        sscanf( cmd + 8, "%04x", &addr);
                        fprintf( stdout, "debugger> %"PRIu8"\n", prot_memory_read8( addr));
                        }
                    } else
                        fprintf( stdout, "debugger> unknow register or ram adresse\n");
                } else if ( !strncmp( cmd, "continue ", 9)) {

                    if ( cmd[9] < '0' || cmd[9] > '9') {

                        fprintf( stdout, "debugger> error command malformed\n");
                    } else {

                        toexecute = atoi( cmd + 9);

                        if ( toexecute == 0)
                            toexecute = -1;
                    }
                } else if ( !strncmp( cmd, "verbose ", 7)) {
                
                    if ( verbose) {

                        fprintf( stdout, "debugger> switching verbose OFF\n");
                        verbose = 0;
                    } else {

                        fprintf( stdout, "debugger> switching verbose ON\n");
                        verbose = 1;
                    }
                } else if ( !strncmp( cmd, "breakpoint ", 11)) {
                
                    if ( strncmp( cmd + 11, "0x", 2) || ((!(cmd[13] > '0' || cmd[13] < '9')) && cmd[13] != 'a' && cmd[13] != 'A' && cmd[13] != 'b' && cmd[13] != 'B' && cmd[13] != 'c' && cmd[13] != 'C' && cmd[13] != 'd' && cmd[13] != 'D' && cmd[13] != 'e' && cmd[13] != 'E' && cmd[13] != 'f' && cmd[13] != 'F') )
                        fprintf( stdout, "debugger> error adresse malformed\n");
                    else {

                        uint16_t i = 0;

                        for ( ; breakpoints[i] != -1 && i < MAX_BREAKPOINTS; i++);

                        if ( i == MAX_BREAKPOINTS)
                            fprintf( stdout, "debugger> no more breakpoints avalaible\n");
                        else {

                            cmd[17] = '\0';
                            sscanf( cmd + 13, "%x", &(breakpoints[i]));
                            fprintf( stdout, "debugger> adding breakpoint %d at 0x%02x\n", i, breakpoints[i]);
                        }
                    }
                } else if ( !strncmp( cmd, "delete ", 7)) {
                
                    if ( strncmp( cmd + 7, "0x", 2) || ((!(cmd[9] > '0' || cmd[9] < '9')) && cmd[9] != 'a' && cmd[9] != 'A' && cmd[9] != 'b' && cmd[9] != 'B' && cmd[9] != 'c' && cmd[9] != 'C' && cmd[9] != 'd' && cmd[9] != 'D' && cmd[9] != 'e' && cmd[9] != 'E' && cmd[9] != 'f' && cmd[9] != 'F') )
                        fprintf( stdout, "debugger> error adresse malformed\n");
                    else {

                        uint16_t i = 0, br;

                        cmd[13] = '\0';
                        sscanf( cmd + 9, "%hx", &br);

                        for ( ; breakpoints[i] != br && i < MAX_BREAKPOINTS; i++);

                        if ( i == MAX_BREAKPOINTS)
                            fprintf( stdout, "debugger> no breakpoints at adresse %02x\n", br);
                        else {

                            fprintf( stdout, "debugger> delete breakpoint %d at 0x%02x\n", i, breakpoints[i]);
                            breakpoints[i] = -1;
                        }
                    }
                } else if ( !strncmp( cmd, "info breakpoints", 16)) {
                    
                    for ( uint16_t i = 0; i < MAX_BREAKPOINTS; i++)
                        if ( breakpoints[i] != -1)
                            fprintf( stdout, "debugger>\tbreakpoints %d at 0x%02x\n", i, breakpoints[i]);

                } else if ( !strncmp( cmd, "info registers", 14)) {
                    
                    fprintf( stdout, "debugger> a     %d (0x%02x)\n", registers.a, registers.a);
                    fprintf( stdout, "debugger> f     %d (0x%02x)\n", registers.f, registers.f);
                    fprintf( stdout, "debugger> af    %d (0x%02x)\n", registers.af, registers.af);
                    fprintf( stdout, "debugger> b     %d (0x%02x)\n", registers.b, registers.b);
                    fprintf( stdout, "debugger> c     %d (0x%02x)\n", registers.c, registers.c);
                    fprintf( stdout, "debugger> bc    %d (0x%02x)\n", registers.bc, registers.bc);
                    fprintf( stdout, "debugger> d     %d (0x%02x)\n", registers.d, registers.d);
                    fprintf( stdout, "debugger> e     %d (0x%02x)\n", registers.e, registers.e);
                    fprintf( stdout, "debugger> de    %d (0x%02x)\n", registers.de, registers.de);
                    fprintf( stdout, "debugger> h     %d (0x%02x)\n", registers.h, registers.h);
                    fprintf( stdout, "debugger> l     %d (0x%02x)\n", registers.l, registers.l);
                    fprintf( stdout, "debugger> hl    %d (0x%02x)\n", registers.hl, registers.hl);
                    fprintf( stdout, "debugger> sp    %d (0x%02x)\n", registers.sp, registers.sp);
                    fprintf( stdout, "debugger> pc    0x%02x\n", registers.pc);

                    fprintf( stdout, "debugger> flags ");
                    if ( registers.f & FLAGS_Z)
                        fprintf( stdout, "FLAG_Z ");
                    if ( registers.f & FLAGS_N)
                        fprintf( stdout, "FLAG_N ");
                    if ( registers.f & FLAGS_H)
                        fprintf( stdout, "FLAG_H ");
                    if ( registers.f & FLAGS_C)
                        fprintf( stdout, "FLAG_C");
                    fprintf( stdout, "\n");

                } else if ( !strncmp( cmd, "display registers", 17)) {
                    
                    if ( display_register) {

                        fprintf( stdout, "debugger> switching display register OFF\n");
                        display_register = 0;
                    } else {

                        fprintf( stdout, "debugger> switching display register ON\n");
                        display_register = 1;
                    }
                }else if ( strncmp( cmd, "step", 4))
                    fprintf( stdout, "debugger> unknow command\n");
            } while ( strncmp( cmd, "continue", 8) && strncmp( cmd, "step", 4));
        } else if ( toexecute != -1)
            toexecute--;
    }

#endif