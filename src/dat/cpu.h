// void cpu_execute( uint8_t opcode, uint8_t op8, uint16_t op16)

switch ( opcode) {

    case 0: // 0x00 NOP
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x00 NOP REVIEWED\n");
        #endif

        break;
    case 1: // 0x01 BC = XX XX
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x01 BC = XX XX REVIEWED\n");
        #endif

        registers.bc = op16;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : bc 0x%04x (%"PRIu16")\n", registers.bc, registers.bc);
        #endif

        break;
    case 2: // 0x02 *(BC) = A
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x02 *(BC) = A REVIEWED\n");
        #endif

        memory_write8( registers.bc, registers.a);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : bc 0x%04x; a 0x%02x (%"PRIu8")\n", registers.bc, registers.a, registers.a);
        #endif

        break;
    case 3: // 0x03 BC++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x03 BC++ REVIEWED\n");
        #endif

        (registers.bc)++;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : bc 0x%04x (%"PRIu16")\n", registers.bc, registers.bc);
        #endif

        break;
    case 4: // 0x04 B++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x04 B++ REVIEWED\n");
        #endif

        if ( (((registers.b & 0x0F) + (registers.b & 0x0F)) & 0x10) == 0x10 )
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.f &= ~FLAGS_N;

        (registers.b)++;

        if ( registers.b)
            registers.f &= ~FLAGS_Z;
        else
             registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 5: // 0x05 B--
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x05 B-- REVIEWED\n");
        #endif

        if ( registers.b & 0x0F)
            registers.f &= ~FLAGS_H;
        else
            registers.f |= FLAGS_H;

        registers.f |= FLAGS_N;

        (registers.b)--;

        if ( registers.b)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 6: // 0x06 B = XX
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x06 B = XX REVIEWED\n");
        #endif

        registers.b = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 7: // 0x07 ROTATE A LEFT
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x07 ROTATE A LEFT REVIEWED\n");
        #endif

        if ( registers.a & 0b10000000)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        registers.a = registers.a << 1;

        if ( registers.f == FLAGS_C)
            registers.a |= 0b00000001;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif
        
        break;
    case 8: // 0x08 *(XX XX) = SP
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x08 *(XX XX) = SP REVIEWED\n");
        #endif

        memory_write16( op16, registers.sp);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : op16 0x%02x; sp 0x%02x\n", op16, registers.sp);
        #endif

        break;
    case 9: // 0x09 HL += BC
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x09 HL += BC REVIEWED\n");
        #endif

        registers.f &= ~FLAGS_N;

        if ( (((registers.hl & 0x0FFF) + ( registers.bc & 0x0FFF)) & 0x1000) == 0x1000)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.hl += registers.bc;

        if ( registers.hl < registers.bc)
            registers.f |= FLAGS_C;
        else
            registers.f &= ~FLAGS_C;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : hl 0x%04x (%"PRIu16")\n", registers.hl, registers.hl);
        #endif

        break;
    case 10: // 0x0A A = *(BC)
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x0A A = *(BC) REVIEWED\n");
        #endif

        registers.a = memory_read8( registers.bc);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); bc 0x%04x\n", registers.a, registers.a, registers.bc);
        #endif

        break;
    case 11: // 0x0B BC--
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x0B BC-- REVIEWED\n");
        #endif

        (registers.bc)--;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : bc 0x%02x (%"PRIu8")\n", registers.bc, registers.bc);
        #endif

        break;
    case 12: // 0x0C C++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x0C C++ REVIEWED\n");
        #endif

        if ( (registers.c & 0x0F) == 0x0F)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.f &= ~FLAGS_N;

        (registers.c)++;

        if ( registers.c)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 13: // 0x0D C--
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x0D C-- REVIEWED\n");
        #endif

        if ( (registers.c & 0x0F) == 0x00)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.f |= FLAGS_N;

        (registers.c)--;

        if ( registers.c)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 14: // 0x0E C = XX
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x0E C = XX REVIEWED\n");
        #endif

        registers.c = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 15: // 0x0F ROTATE A RIGHT

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x0F ROTATE A RIGHT\n");
        #endif

        if ( registers.a & 0b00000001)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        registers.a = registers.a >> 1;

        if ( registers.f == FLAGS_C)
            registers.a |= 0b00000001;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif
        
        break;
    case 17: // 0x11 DE = XX XX
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x11 DE = XX XX REVIEWED\n");
        #endif

        registers.de = op16;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : de 0x%04x (%"PRIu16")\n", registers.de, registers.de);
        #endif

        break;
    case 18: // 0x12 *(DE) = A
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x12 *(DE) = A REVIEWED\n");
        #endif

        memory_write8( registers.de, registers.a);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : de 0x%02x; a 0x%02x (%"PRIu8")\n", registers.de, registers.a, registers.a);
        #endif

        break;
    case 19: // 0x13 DE++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x13 DE++ REVIEWED\n");
        #endif

        (registers.de)++;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : de 0x%04x (%"PRIu16")\n", registers.de, registers.de);
        #endif

        break;
    case 20: // 0x14 D++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x14 D++ REVIEWED\n");
        #endif

        if ( (registers.d & 0x0F) == 0x0F)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.f &= ~FLAGS_N;

        (registers.d)++;

        if ( registers.d)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 21: // 0x15 D--
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x15 D-- REVIEWED\n");
        #endif

        if ( (registers.d & 0x0F) == 0x00)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.f |= FLAGS_N;

        (registers.d)--;

        if ( registers.d)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 22: // 0x16 D = XX
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x16 D = XX REVIEWED\n");
        #endif

        registers.d = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 24: // 0x18 JMP pc + (signed) XX
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x18 JMP pc + (signed) XX REVIEWED\n");
        #endif

        registers.pc += (int8_t)op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%04x\n", registers.pc);
        #endif

        break;
    case 25: // 0x19 HL += DE
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x19 HL += DE REVIEWED\n");
        #endif

        registers.f &= ~FLAGS_N;

        if ( (((registers.hl & 0x0FFF) + ( registers.de & 0x0FFF)) & 0x1000) == 0x1000)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        registers.hl += registers.de;

        if ( registers.hl < registers.de)
            registers.f |= FLAGS_C;
        else
            registers.f &= ~FLAGS_C;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : hl 0x%02x (%"PRIu8")\n", registers.hl, registers.hl);
        #endif

        break;
    case 26: // 0x1A A = *(DE)
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x1A A = *(DE) REVIEWED\n");
        #endif

        registers.a = memory_read8( registers.de);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 27: // 0x1B DE--
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x1B DE-- REVIEWED\n");
        #endif

        (registers.de)--;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : de 0x%04x (%"PRIu16")\n", registers.de, registers.de);
        #endif

        break;
    case 28: // 0x1C E++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x1C E++ REVIEWED\n");
        #endif

        if ( (registers.e & 0x0F) == 0x0F)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        (registers.e)++;

        if ( registers.e)
            registers.f &= ~( FLAGS_Z | FLAGS_N);
        else {

            registers.f &= (~FLAGS_N);
            registers.f |= FLAGS_Z;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e 0x%02x (%"PRIu8")\n", registers.e, registers.e);
        #endif

        break;
    case 29: // 0x1D E--
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x1D E-- REVIEWED\n");
        #endif

        if ( (registers.e & 0x0F) == 0x00)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        (registers.e)--;

        if ( registers.e)
            registers.f &= ~( FLAGS_Z | FLAGS_N);
        else {

            registers.f &= (~FLAGS_N);
            registers.f |= FLAGS_Z;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e 0x%02x (%"PRIu8")\n", registers.e, registers.e);
        #endif

        break;
    case 30: // 0x1E E = XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x1E E = XX\n");
        #endif

        registers.e = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e 0x%02x (%"PRIu8")\n", registers.e, registers.e);
        #endif

        break;
    case 31: // 0x1F ROTATE A RIGHT TROUGH CARRY

        #ifdef __STEP
            if ( verbose)
                fprintf( stdout, "[INFO] : 0x1F ROTATE A RIGHT TROUGH CARRYn");
        #endif

        if ( registers.f & FLAGS_C)
            registers.f = FLAGS_N;
        else
            registers.f = 0;

        if ( registers.a & 0b00000001)
            registers.f |= FLAGS_C;

        registers.a = registers.a >> 1;

        if ( registers.f & FLAGS_N) {

            registers.a |= 0b10000000;
            registers.f &= ~FLAGS_N;
        }

        if ( !(registers.a))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stdout, "\t[INFO] : a = %d (0x%02x)\n", registers.a, registers.a);
        #endif

        break;
    case 32: // 0x20 JMP pc + (signed) XX if !FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x20 JMP pc + (signed) XX if !FLAGS_Z\n");
        #endif

        if ( !(registers.f & FLAGS_Z)) {

            registers.pc += (int8_t)op8;
            emul_time++;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif

        break;
    case 33: // 0x21 HL = XX XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x21 HL = XX\n");
        #endif

        registers.hl = op16;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : hl 0x%04x (%"PRIu16")\n", registers.hl, registers.hl);
        #endif
        break;
    case 34: // 0x22 *(HL) = A; HL++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x22 *(HL) = A; HL++\n");
        #endif

        memory_write8( registers.hl, registers.a);
        (registers.hl)++;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); hl (after increment) 0x%02x\n", registers.a, registers.a, registers.hl);
        #endif

        break;
    case 35: // 0x23 HL++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x23 HL++\n");
        #endif

        (registers.hl)++;

        break;
    case 36: // 0x24 H++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x24 H++\n");
        #endif

        (registers.h)++;

        registers.f &= ~FLAGS_N;

        if ( !(registers.h))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h 0x%02x (%"PRIu8")\n", registers.h, registers.h);
        #endif

        break;
    case 37: // 0x25 H--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x25 H--\n");
        #endif

        (registers.h)--;

        if ( registers.h)
            registers.f = FLAGS_N;
        else
            registers.f = FLAGS_Z | FLAGS_N;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h 0x%02x (%"PRIu8")\n", registers.h, registers.h);
        #endif

        break;
    case 38: // 0x26 H = XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x26 H = XX\n");
        #endif

        registers.hl = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : hl 0x%02x %"PRIu8"\n", registers.hl, registers.hl);
        #endif

        break;
    case 39: // 0x27 ADJUST A FOR BCD

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x27 ADJUST A FOR BCD\n");
        #endif

        if ( !(registers.f & FLAGS_N)) {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds

            if ( registers.f & FLAGS_C || registers.a > 0x99) { 
                registers.a += 0x60; 
                registers.f |= FLAGS_C; 
            }

            if ( registers.f & FLAGS_H || ( registers.a & 0x0F) > 0x09) 
                registers.a += 0x06;
        } else {  // after a subtraction, only adjust if (half-)carry occurred

            if (registers.f & FLAGS_C)
                registers.a -= 0x60;

            if (registers.f & FLAGS_H) 
                registers.a -= 0x06;
        }

        if ( registers.a)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        registers.f &= ~FLAGS_H;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x\n", registers.a);
        #endif

        break;
    case 40: // 0x28 JMP pc + (signed) XX if FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x28 JMP pc + (signed) XX if FLAGS_Z\n");
        #endif

        if ( registers.f & FLAGS_Z) {

            registers.pc += (int8_t)op8;
            emul_time++;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif
        
        break;
    case 41: // 0x29 HL += HL
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x29 HL += HL REVIEWED\n");
        #endif

        registers.f &= ~FLAGS_N;

        if ( (((registers.hl & 0x0FFF) + ( registers.hl & 0x0FFF)) & 0x1000) == 0x1000)
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        if ( registers.hl + registers.hl < registers.hl)
            registers.f |= FLAGS_C;
        else
            registers.f &= ~FLAGS_C;

        registers.hl += registers.hl;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : hl 0x%04x (%"PRIu16")\n", registers.hl, registers.hl);
        #endif

        break;
    case 42: // 0x2A A = *(HL); HL++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x2A A = *(HL); HL++\n");
        #endif

        registers.a = memory_read8( registers.hl);
        (registers.hl)++;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); hl (after increment) 0x%02x\n", registers.a, registers.a, registers.hl);
        #endif

        break;
    case 43: // 0x2B HL--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x2B HL--\n");
        #endif

        (registers.hl)--;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : de 0x%04x (%"PRIu16")\n", registers.hl, registers.hl);
        #endif

        break;
    case 44: // 0x2C L++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x2C L++\n");
        #endif

        (registers.l)++;

        if ( registers.l)
            registers.f &= ~( FLAGS_Z | FLAGS_N);
        else {

            registers.f &= (~FLAGS_N);
            registers.f |= FLAGS_Z;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l 0x%02x (%"PRIu8")\n", registers.l, registers.l);
        #endif

        break;
    case 45: // 0x2D L--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x2D L--\n");
        #endif

        (registers.l)--;

        if ( registers.l) {

            registers.f &= ~FLAGS_Z;
            registers.f |= FLAGS_N;
        } else
            registers.f |= FLAGS_Z | FLAGS_N;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l 0x%02x (%"PRIu8")\n", registers.l, registers.l);
        #endif

        break;
    case 46: // 0x2E L = XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x2E L = XX\n");
        #endif

        registers.l = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l 0x%02x (%"PRIu8")\n", registers.l, registers.l);
        #endif

        break;
    case 47: // 0x2F A = ~A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x2F A = ~A\n");
        #endif

        registers.a = ~registers.a;
        registers.f = FLAGS_N;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x\n", registers.a);
        #endif

        break;
    case 48: // 0x30 JMP pc + (signed) XX if !FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x30 JMP pc + (signed) XX if !FLAGS_C\n");
        #endif

        if ( !(registers.f & FLAGS_C)) {

            registers.pc += (int8_t)op8;
            emul_time++;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif

        break;
    case 49: // 0x31 SP = XX XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x31 SP = XX XX\n");
        #endif

        registers.sp = op16;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : sp = 0x%02x \n", registers.sp);
        #endif

        break;
    case 50: // 0x32 *(HL) = A; HL--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x32 *(HL) = A; HL--\n");
        #endif

        memory_write8( registers.hl, registers.a);
        (registers.hl)--;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : *(HL) = 0x%02x (%"PRIu8"); HL (after decrement) = 0x%02x (%"PRIu16")\n", memory_read8( registers.hl + 1), memory_read8( registers.hl + 1), registers.hl, registers.hl);
        #endif

        break;
    case 51: // 0x33 SP++
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x33 SP++ REVIEWED\n");
        #endif

        (registers.sp)++;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : sp 0x%04x (%"PRIu16")\n", registers.sp, registers.sp);
        #endif

        break;
    case 52: // 0x34 *(HL)++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] :0x34 *(HL)++\n");
        #endif

        memory_write8( registers.hl, memory_read8( registers.hl) + 1);

        registers.f &= ~FLAGS_N;

        if ( memory_read8( registers.hl))
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : *(HL) = 0x%02x (%"PRIu8")\n", memory_read8( registers.hl), memory_read8( registers.hl));
        #endif

        break;
    case 53: // 0x35 *(HL)--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x35 *(HL)--\n");
        #endif

        memory_write8( registers.hl, memory_read8( registers.hl) - 1);

        if ( memory_read8( registers.hl)) {

            registers.f &= ~FLAGS_Z;
            registers.f |= FLAGS_N;
        } else
            registers.f |= ( FLAGS_Z | FLAGS_N);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : *(HL) = 0x%02x (%"PRIu8")\n", memory_read8( registers.hl), memory_read8( registers.hl));
        #endif

        break;
    case 54: // 0x36 *(HL) = XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] :0x36 *(HL) = XX\n");
        #endif

        memory_write8( registers.hl, op8);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : *(HL) = 0x%02x (%"PRIu8"); HL = 0x%02x (%"PRIu16")\n", memory_read8( registers.hl), memory_read8( registers.hl), registers.hl, registers.hl);
        #endif

        break;
    case 56: // 0x38 JMP pc + (signed) XX IF FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x38 JMP pc + (signed) XX IF FLAGS_C\n");
        #endif

        if ( registers.f & FLAGS_C) {

            registers.pc += (int8_t)op8;
            emul_time++;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif

        break;
    case 58: // 0x3A A = *(HL); HL--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x32 *(HL) = A; HL--\n");
        #endif

        registers.a = memory_read8( registers.hl);
        (registers.hl)--;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8"); HL (after decrement) = 0x%02x\n", registers.a, registers.a, registers.hl);
        #endif
        
        break;
    case 60: // 0x3C A++

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x3C A++\n");
        #endif

        (registers.a)++;

        registers.f &= ~FLAGS_N;

        if ( registers.a)
            registers.f &= ~FLAGS_Z;
        else
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 61: // 0x3D A--

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x3D A--\n");
        #endif

        (registers.a)--;

        if ( registers.a)
            registers.f = FLAGS_N;
        else
            registers.f = FLAGS_Z | FLAGS_N;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 62: // 0x3E A = XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x3E A = XX\n");
        #endif

        registers.a = op8;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 64: // 0x40 B = B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x40 B = B\n");
        #endif

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b = 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 66: // 0x42 B = D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x42 B = D\n");
        #endif

        registers.b = registers.d;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b = 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 68: // 0x44 B = H

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x44 B = H\n");
        #endif

        registers.b = registers.h;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b = 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 70: // 0x46 B = *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x46 B = *(HL)\n");
        #endif

        registers.b = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b = 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 71: // 0x47 B = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x47 B = A\n");
        #endif

        registers.b = registers.a;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b = 0x%02x (%"PRIu8")\n", registers.b, registers.b);
        #endif

        break;
    case 75: // 0x4B C = E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x4B C = E\n");
        #endif

        registers.c = registers.e;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c = 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 77: // 0x4D C = L

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x4D C = L\n");
        #endif

        registers.c = registers.l;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c = 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 78: // 0x4E C = *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x4E C = *(HL)\n");
        #endif

        registers.c = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c = 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 79: // 0x4F C = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x4F C = A\n");
        #endif

        registers.c = registers.a;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c = 0x%02x (%"PRIu8")\n", registers.c, registers.c);
        #endif

        break;
    case 80: // 0x50 D = B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x50 D = B\n");
        #endif

        registers.d = registers.b;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d = 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 84: // 0x54 D = H

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x54 D = H\n");
        #endif

        registers.d = registers.h;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 86: // 0x56 D = *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x56 D = *(HL)\n");
        #endif

        registers.d = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d = 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 87: // 0x57 D = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x57 D = A\n");
        #endif

        registers.d = registers.a;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d 0x%02x (%"PRIu8")\n", registers.d, registers.d);
        #endif

        break;
    case 93: // 0x5D E = L

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x5D E = L\n");
        #endif

        registers.e = registers.l;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e 0x%02x (%"PRIu8")\n", registers.e, registers.e);
        #endif

        break;
    case 94: // 0x5E E = *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x5E E = *(HL)\n");
        #endif

        registers.e = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e = 0x%02x (%"PRIu8"); hl = 0x%02x\n", registers.e, registers.e, registers.hl);
        #endif

        break;
    case 95: // 0x5F E = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x5F E = A\n");
        #endif

        registers.e = registers.a;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e = 0x%02x (%"PRIu8")\n", registers.e, registers.e);
        #endif

        break;
    case 96: // 0x60 H = B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x60 H = B\n");
        #endif

        registers.h = registers.b;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h = 0x%02x (%"PRIu8")\n", registers.h, registers.h);
        #endif

        break;
    case 97: // 0x61 H = C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x61 H = C\n");
        #endif

        registers.h = registers.c;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h = 0x%02x (%"PRIu8")\n", registers.h, registers.h);
        #endif

        break;
    case 98: // 0x62 H = D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x62 H = D\n");
        #endif

        registers.h = registers.d;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h = 0x%02x (%"PRIu8")\n", registers.h, registers.h);
        #endif

        break;
    case 102: // 0x66 H = *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x5E E = *(HL)\n");
        #endif

        registers.h = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h = 0x%02x (%"PRIu8"); hl = 0x%02x\n", registers.h, registers.h, registers.hl);
        #endif

        break;
    case 103: // 0x67 H = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x67 H = A\n");
        #endif

        registers.h = registers.a;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : h = 0x%02x (%"PRIu8")\n", registers.h, registers.h);
        #endif

        break;
    case 105: // 0x69 L = C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x69 L = C\n");
        #endif

        registers.l = registers.c;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l = 0x%02x (%"PRIu8")\n", registers.l, registers.l);
        #endif

        break;
    case 107: // 0x6B L = E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x6B L = E\n");
        #endif

        registers.l = registers.e;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l = 0x%02x (%"PRIu8")\n", registers.l, registers.l);
        #endif

        break;
    case 110: // 0x6E L = (*HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x6E L = (*HL)\n");
        #endif

        registers.l = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l = 0x%02x (%"PRIu8"); hl = 0x%02x\n", registers.l, registers.l, registers.hl);
        #endif

        break;
    case 111: // 0x6F L = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x6F L = A\n");
        #endif

        registers.l = registers.a;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : l = 0x%02x (%"PRIu8")\n", registers.l, registers.l);
        #endif

        break;
    case 112: // 0x70 *(HL) = B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x70 *(HL) = B\n");
        #endif

        memory_write8( registers.hl, registers.b);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : b = 0x%02x; hl = 0x%02x\n", registers.b, registers.b);
        #endif

        break;
    case 113: // 0x71 *(HL) = C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x71 *(HL) = C\n");
        #endif

        memory_write8( registers.hl, registers.c);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : c = 0x%02x; hl = 0x%02x\n", registers.c, registers.c);
        #endif

        break;
    case 114: // 0x72 *(HL) = D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x72 *(HL) = D\n");
        #endif

        memory_write8( registers.hl, registers.d);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : d = 0x%02x; hl = 0x%02x\n", registers.d, registers.hl);
        #endif

        break;
    case 115: // 0x73 *(HL) = E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x73 *(HL) = E\n");
        #endif

        memory_write8( registers.hl, registers.e);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : e = 0x%02x; hl = 0x%02x\n", registers.e, registers.hl);
        #endif

        break;
    case 118: // 0x76 HALT

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x76 HALT\n");
        #endif

        (registers.pc)--;

        break;
    case 119: // 0x77 *(HL) = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x77 *(HL) = A\n");
        #endif

        memory_write8( registers.hl, registers.a);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x; hl = 0x%02x\n", registers.a, registers.hl);
        #endif

        break;
    case 120: // 0x78 A = B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x78 A = B\n");
        #endif

        registers.a = registers.b;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 121: // 0x79 A = C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x79 A = C\n");
        #endif

        registers.a = registers.c;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 122: // 0x7A A = D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x7A A = D\n");
        #endif

        registers.a = registers.d;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 123: // 0x7B A = E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x7B A = E\n");
        #endif

        registers.a = registers.e;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 124: // 0x7C A = H

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x7C A = H\n");
        #endif

        registers.a = registers.h;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 125: // 0x7D A = L

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x7D A = L\n");
        #endif

        registers.a = registers.l;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 126: // 0x7E A = *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x7E A = *(HL)\n");
        #endif

        registers.a = memory_read8( registers.hl);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8"); hl 0x%04x\n", registers.a, registers.a, registers.hl);
        #endif

        break;
    case 128: // 0x80 A += B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x80 A += B\n");
        #endif

        registers.a += registers.b;

        if ( registers.a < registers.b)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 129: // 0x81 A += C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x81 A += C\n");
        #endif

        registers.a += registers.c;

        if ( registers.a < registers.c)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 130: // 0x82 A += D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x82 A += D\n");
        #endif

        registers.a += registers.d;

        if ( registers.a < registers.d)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 131: // 0x83 A += E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x83 A += E\n");
        #endif

        registers.a += registers.e;

        if ( registers.a < registers.e)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 132: // 0x84 A += H

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x84 A += H\n");
        #endif

        registers.a += registers.h;

        if ( registers.a < registers.h)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 133: // 0x85 A += L

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x85 A += L\n");
        #endif

        registers.a += registers.l;

        if ( registers.a < registers.l)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 134: // 0x86 A += *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x86 A += *(HL)\n");
        #endif

        registers.a += memory_read8( registers.hl);

        if ( registers.a < memory_read8( registers.hl))
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8"); hl = 0x%02x; *(hl) = 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
        #endif

        break;
    case 135: // 0x87 A = A + A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x87 A = A + A\n");
        #endif

        if ( registers.a > 127)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        registers.a += registers.a;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        break;
    case 137: // 0x89 A += C + FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x89 A += C + FLAGS_C\n");
        #endif

        registers.a += registers.c;

        if ( registers.f & FLAGS_C)
            (registers.a)++;

        if ( registers.a < registers.c)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 142: // 0x8E A += *(HL) + FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x8E A += *(HL) + FLAGS_C\n");
        #endif

        registers.a += memory_read8( registers.hl);

        if ( registers.f & FLAGS_C)
            (registers.a)++;

        if ( registers.a < memory_read8( registers.hl))
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 144: // 0x90 A -= B
    // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x90 A -= B REVIEWED\n");
        #endif

        registers.f = FLAGS_N;
        
        if ( (((registers.a & 0x0F) - (registers.b & 0x0F)) & 0x10) == 0x10 )
            registers.f |= FLAGS_H;
        else
            registers.f &= ~FLAGS_H;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); b 0x%02x (%"PRIu8"); res a 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.b, registers.b, (uint8_t)(registers.a - registers.b), (uint8_t)(registers.a - registers.b));
        #endif

        if ( registers.a < registers.b)
            registers.f |= FLAGS_C;

        registers.a -= registers.b;

        if ( !(registers.a))
            registers.f |= FLAGS_Z;

        break;
    case 145: // 0x91 A -= C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x91 A -= C\n");
        #endif

        registers.f = FLAGS_N;

        if ( (((registers.a & 0x0F) - (registers.c & 0x0F)) & 0x10) == 0x10 )
            registers.f |= FLAGS_H;

        registers.a -= registers.c;

        if ( registers.a > registers.c)
            registers.f |= FLAGS_C;

        if ( !(registers.a))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 147: // 0x93 A -= E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x93 A -= E\n");
        #endif

        registers.f = FLAGS_N;

        if ( (((registers.a & 0x0F) - (registers.e & 0x0F)) & 0x10) == 0x10 )
            registers.f |= FLAGS_H;

        registers.a -= registers.e;

        if ( registers.a > registers.e)
            registers.f |= FLAGS_C;

        if ( !(registers.a))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 150: // 0x96 A -= *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x96 A -= *(HL)\n");
        #endif

        registers.a -= memory_read8( registers.hl);

        if ( registers.a > memory_read8( registers.hl))
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a))
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8"); hl 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
        #endif

        break;
    case 153: // 0x99 A -= (C + C_FLAG)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x99 A -= (C + C_FLAG)\n");
        #endif

        if ( registers.f & FLAGS_C) {

            registers.f = FLAGS_N;
            
            if ( (((registers.a & 0x0F) - (registers.c & 0x0F) - 1) & 0x10) == 0x10 )
                registers.f |= FLAGS_H;
            else
                registers.f &= ~FLAGS_H;

            #ifdef __STEP
                if ( verbose)
                    fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); c 0x%02x (%"PRIu8"); carry YES; res a 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.c, registers.c, (uint8_t)(registers.a - registers.c), (uint8_t)(registers.a - registers.c - 1));
            #endif

            if ( registers.a < ( registers.c + 1))
                registers.f |= FLAGS_C;

            registers.a -= ( registers.c + 1);

            if ( !(registers.a))
                registers.f |= FLAGS_Z;
        } else {

            registers.f = FLAGS_N;
            
            if ( (((registers.a & 0x0F) - (registers.c & 0x0F)) & 0x10) == 0x10 )
                registers.f |= FLAGS_H;
            else
                registers.f &= ~FLAGS_H;

            #ifdef __STEP
                if ( verbose)
                    fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); c 0x%02x (%"PRIu8"); carry NO; res a 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.c, registers.c, (uint8_t)(registers.a - registers.c), (uint8_t)(registers.a - registers.c));
            #endif

            if ( registers.a < registers.c)
                registers.f |= FLAGS_C;

            registers.a -= registers.c;

            if ( !(registers.a))
                registers.f |= FLAGS_Z;
        }

        break;
    case 160: // 0xA0 A &= B
    // reviewed

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xA0 A &= B REVIEWED\n");
        #endif

        registers.a &= registers.b;

        if ( registers.a)
            registers.f = FLAGS_H;
        else
            registers.f = FLAGS_Z | FLAGS_H;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 161: // 0xA1 A &= C
    // reviewed

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xA1 A &= C REVIEWED\n");
        #endif

        registers.a &= registers.c;

        if ( registers.a)
            registers.f = FLAGS_H;
        else
            registers.f = FLAGS_Z | FLAGS_H;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 167: // 0xA7 A &= A
    // reviewed

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xA7 A &= A REVIEWED\n");
        #endif

        if ( registers.a)
            registers.f = FLAGS_H;
        else
            registers.f = FLAGS_Z | FLAGS_H;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 168: // 0xA8 A ^= B
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xA8 A ^= B REVIEWED\n");
        #endif

        registers.a ^= registers.b;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 169: // 0xA9 A ^= C
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xA9 A ^= C REVIEWED\n");
        #endif

        registers.a ^= registers.c;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 170: // 0xAA A ^= E
        // reviewed OK

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xAA A ^= E REVIEWED\n");
        #endif

        registers.a ^= registers.e;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 174: // 0xAE A ^= *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] :  0xAE A ^= *(HL)\n");
        #endif

        registers.a = registers.a ^ memory_read8( registers.hl);

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : hl 0x%02x; *(hl) 0x%02x; a 0x%02x\n", registers.hl, memory_read8( registers.hl), registers.a);
        #endif

        break;
    case 175: // 0xAF A = A ^ A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xAF A = A ^ A\n");
        #endif

        registers.a = 0;
        registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 176: // 0xB0 A = A | B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB0 A = A | B\n");
        #endif

        registers.a |= registers.b;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 177: // 0xB1 A = A | C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB1 A = A | C\n");
        #endif

        registers.a |= registers.c;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 178: // 0xB2 A = A | D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB2 A = A | D\n");
        #endif

        registers.a |= registers.d;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 179: // 0xB3 A = A | E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB3 A = A | E\n");
        #endif

        registers.a |= registers.e;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 182: // 0xB6 A = A | *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB6 A = A | *(HL)\n");
        #endif

        registers.a |= memory_read8( registers.hl);

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); hl 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
        #endif

        break;
    case 183: // 0xB7 A = A | A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB2 A = A | A\n");
        #endif

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 184: // 0xB8 A == B

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB8 A == B\n");
        #endif

        if ( registers.a - registers.b > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a - registers.b))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 185: // 0xB9 A == C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xB9 A == C\n");
        #endif

        if ( registers.a - registers.c > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a - registers.c))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 186: // 0xBA A == D

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xBA A == D\n");
        #endif

        if ( registers.a - registers.d > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a - registers.d))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 187: // 0xBB A == E

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xBB A == E\n");
        #endif

        if ( registers.a - registers.e > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a - registers.e))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 188: // 0xBC A == H

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xBC A == H\n");
        #endif

        if ( registers.a - registers.h > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a - registers.h))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 189: // 0xBD A == L

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xBD A == L\n");
        #endif

        if ( registers.a - registers.l > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a - registers.l))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 190: // 0xBE *(HL) == A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xBE *(HL) == A\n");
        #endif

        if ( registers.a - memory_read8( registers.hl) > registers.a)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( registers.a == memory_read8( registers.hl))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); hl 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.a, registers.a, registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
        #endif

        break;
    case 191: // 0xBF A == A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xBF A == A\n");
        #endif

        registers.f = FLAGS_N | FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8")\n", registers.a, registers.a);
        #endif

        break;
    case 192: // 0xC0 RET IF !FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC0 RET IF !FLAGS_Z\n");
        #endif

        if ( !( registers.f & FLAGS_Z)) {

            registers.pc = memory_read16( registers.sp);
            registers.sp += 2;
            emul_time += 3;
        }

        break;
    case 193: // 0xC1 POP BC

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC1 POP BC\n");
        #endif

        registers.bc = memory_read16( registers.sp);
        registers.sp += 2;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : bc 0x%02x (%"PRIu16")\n", registers.bc, registers.bc);
        #endif

        break;
    case 194: // 0xC2 JMP XX XX IF LAST RESULT !FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC2 JMP XX XX IF LAST RESULT !FLAGS_Z\n");
        #endif

        if ( !(registers.f & FLAGS_Z)) {

            registers.pc = op16;
            emul_time++;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif

        break;
    case 195: // 0xC3 JMP XX XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC3 JMP XX XX\n");
        #endif

        registers.pc = op16;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif

        break;
    case 196: // 0xC4 CALL XX XX IF !FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC4 CALL XX XX IF !FLAGS_Z\n");
        #endif

        if ( !(registers.f & FLAGS_Z)) {

            registers.sp -= 2;
            memory_write16( registers.sp, registers.pc);
            registers.pc = op16;

            emul_time += 3;
        }

        break;
    case 197: // 0xC5 PUSH BC

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC5 PUSH BC\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.bc);

        break;
    case 198: // 0xC6 A += XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC6 A += XX\n");
        #endif

        registers.a += op8;

        if ( registers.a < op8)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !(registers.a))
            registers.f |= FLAGS_Z;

        break;
    case 200: // 0xC8 RET IF FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC8 RET IF FLAGS_Z\n");
        #endif

        if ( registers.f & FLAGS_Z) {

            registers.pc = memory_read16( registers.sp);
            registers.sp += 2;
            emul_time += 3;
        }

        break;
    case 201: // 0xC9 RET

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xC9 RET\n");
        #endif

        registers.pc = memory_read16( registers.sp);
        registers.sp += 2;

        break;
    case 202: // 0xCA JMP XX XX IF FLAGS_Z

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xCA JMP XX XX IF FLAGS_Z\n");
        #endif

        if ( registers.f & FLAGS_Z) {

            registers.pc = op16;
            emul_time++;
        }

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : pc 0x%02x\n", registers.pc);
        #endif

        break;
    case 203: // 0xCB : EXTENDED OPERATION

        #ifdef __STEP
            if ( verbose)
                fprintf( stdout, "[INFO] {0xCB EXTENDED OPERATION}\n");
        #endif

        #ifdef __STEP
            if ( verbose)
                fprintf( stdout, "[INFO]\t execute ext. op. %d (0x%02x)\n", op8, op8);
        #endif

        switch ( op8) {

            case 18: // 0x12 ROTATE D LEFT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x12 ROTATE D LEFT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.d & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.d = registers.d >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.d |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.d))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.d = %d (0x%02x)\n", registers.d, registers.d);
                #endif

                break;
            case 19: // 0x13 ROTATE E LEFT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x13 ROTATE E LEFT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.e & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.e = registers.e >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.e |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.e))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.e = %d (0x%02x)\n", registers.e, registers.e);
                #endif

                break;
            case 25: // 0x19 ROTATE C RIGHT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x19 ROTATE C RIGHT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.c & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.c = registers.c >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.c |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.c))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.c = %d (0x%02x)\n", registers.c, registers.c);
                #endif

                break;
            case 26: // 0x1A ROTATE D RIGHT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x1A ROTATE D RIGHT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.d & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.d = registers.d >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.d |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.d))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.d = %d (0x%02x)\n", registers.d, registers.d);
                #endif

                break;
            case 27: // 0x1B ROTATE E RIGHT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x1B ROTATE E RIGHT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.e & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.e = registers.e >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.e |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.e))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.e = %d (0x%02x)\n", registers.e, registers.e);
                #endif

                break;
            case 28: // 0x1C ROTATE H RIGHT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x1C ROTATE H RIGHT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.h & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.h = registers.h >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.h |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.h))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.h = %d (0x%02x)\n", registers.h, registers.h);
                #endif

                break;
            case 29: // 0x1D ROTATE L RIGHT TROUGH CARRY

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x1D ROTATE L RIGHT TROUGH CARRY}\n");
                #endif

                if ( registers.f & FLAGS_C)
                    registers.f = FLAGS_N;
                else
                    registers.f = 0;

                if ( registers.l & 0b00000001)
                    registers.f |= FLAGS_C;

                registers.l = registers.l >> 1;

                if ( registers.f & FLAGS_N) {

                    registers.l |= 0b10000000;
                    registers.f &= ~FLAGS_N;
                }

                if ( !(registers.l))
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.l = %d (0x%02x)\n", registers.l, registers.l);
                #endif

                break;
            case 33: // 0x21 SHIFT C LEFT PRESERVING SIGN

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x21 SHIFT C LEFT PRESERVING SIGN}\n");
                #endif

                if ( registers.c & 0b01000000)
                    registers.f = FLAGS_C;
                else
                    registers.f = 0;
            
                registers.c |= (registers.a & 0b10000000) >> 1;
                registers.c = registers.c << 1;

                if ( !registers.c)
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.c = %d (0x%02x)\n", registers.c, registers.c);
                #endif

                break;
            case 39: // 0x27 SHIFT A LEFT PRESERVING SIGN

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x27 SHIFT A LEFT PRESERVING SIGN}\n");
                #endif

                if ( registers.a & 0b01000000)
                    registers.f = FLAGS_C;
                else
                    registers.f = 0;
            
                registers.a |= (registers.a & 0b10000000) >> 1;
                registers.a = registers.a << 1;

                if ( !registers.a)
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.a = %d (0x%02x)\n", registers.a, registers.a);
                #endif

                break;
            case 51: // 0x33 : SWAP NIBBLES IN E

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x33 : SWAP NIBBLES IN E}\n");
                #endif
            
                registers.e = (registers.e & 0x0F) << 4 | (registers.e & 0xF0) >> 4 ;

                if ( registers.e)
                    registers.f = 0;
                else
                    registers.f = FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.e = %d (0x%02x)\n", registers.e, registers.e);
                #endif

                break;
            case 55: // 0x37 : SWAP NIBBLES IN A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x37 SWAP NIBBLES IN A}\n");
                #endif
            
                registers.a = (registers.a & 0x0F) << 4 | (registers.a & 0xF0) >> 4 ;

                if ( registers.a)
                    registers.f = 0;
                else
                    registers.f = FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.a = %d (0x%02x)\n", registers.a, registers.a);
                #endif

                break;
            case 56: // 0x38 SHIFT B RIGHT

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x38 SHIFT B RIGHT}\n");
                #endif

                if ( registers.b & 0b00000001)
                    registers.f = FLAGS_C;
                else
                    registers.f = 0;
            
                registers.b = registers.b >> 1;

                if ( !registers.b)
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.b = %d (0x%02x)\n", registers.b, registers.b);
                #endif

                break;
            case 63: // 0x3F SHIFT A RIGHT

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x3F SHIFT A RIGHT}\n");
                #endif

                if ( registers.a & 0b00000001)
                    registers.f = FLAGS_C;
                else
                    registers.f = 0;
            
                registers.a = registers.a >> 1;

                if ( !registers.a)
                    registers.f |= FLAGS_Z;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.a = %d (0x%02x)\n", registers.a, registers.a);
                #endif

                break;
            case 64: // 0x40 : TEST BIT 0 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x40 : TEST BIT 0 OF B}\n");
                #endif
            
                if ( registers.b & 0b00000001)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 0 of b = %"PRIu8"\n", registers.b & 0b00000001);
                #endif

                break;
            case 65: // 0x41 : TEST BIT 0 OF C

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x41 : TEST BIT 0 OF C}\n");
                #endif
            
                if ( registers.c & 0b00000001)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 0 of c = %"PRIu8"\n", registers.c & 0b00000001);
                #endif

                break;
            case 66: // 0x42 : TEST BIT 4 OF *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x42 : TEST BIT 4 OF *(HL)}\n");
                #endif
            
                if ( memory_read8( registers.hl) & 0b00010000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 3;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 0 of *(HL) = %"PRIu8"\n", memory_read8( registers.hl) & 0b00000001);
                #endif

                break;
            case 71: // 0x47 : TEST BIT 0 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x47 : TEST BIT 0 OF A}\n");
                #endif
            
                if ( registers.a & 0b00000001)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 0 of a = %"PRIu8"\n", registers.a & 0b00000001);
                #endif

                break;
            case 74: // 0x4A : TEST BIT 1 OF D

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x4A : TEST BIT 1 OF D}\n");
                #endif
            
                if ( registers.d & 0b00000010)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 1 of d = %"PRIu8"\n", registers.d & 0b00000010);
                #endif

                break;
            case 79: // 0x4F : TEST BIT 1 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x4F : TEST BIT 1 OF A}\n");
                #endif
            
                if ( registers.a & 0b00000010)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 1 of a = %"PRIu8"\n", registers.a & 0b00000010);
                #endif

                break;
            case 72: // 0x48 : TEST BIT 1 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x48 : TEST BIT 1 OF B}\n");
                #endif
            
                if ( registers.b & 0b00000010)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 1 of b = %"PRIu8"\n", registers.b & 0b00000010);
                #endif

                break;
            case 80: // 0x50 : TEST BIT 2 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x50 : TEST BIT 2 OF B}\n");
                #endif
            
                if ( registers.b & 0b00000100)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 2 of b = %"PRIu8"\n", registers.b & 0b00000100);
                #endif

                break;
            case 87: // 0x57 : TEST BIT 2 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x57 : TEST BIT 2 OF A}\n");
                #endif
            
                if ( registers.a & 0b00000100)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 2 of a = %"PRIu8"\n", registers.a & 0b00000100);
                #endif

                break;
            case 88: // 0x58 : TEST BIT 3 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x58 : TEST BIT 3 OF B}\n");
                #endif
            
                if ( registers.b & 0b00001000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 2 of b = %"PRIu8"\n", registers.b & 0b00001000);
                #endif

                break;
            case 95: // 0x5F : TEST BIT 3 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x5F : TEST BIT 3 OF A}\n");
                #endif
            
                if ( registers.a & 0b00001000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 3 of a = %"PRIu8"\n", registers.a & 0b00001000);
                #endif

                break;
            case 96: // 0x60 : TEST BIT 4 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x60 : TEST BIT 4 OF B}\n");
                #endif
            
                if ( registers.b & 0b00010000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 2 of b = %"PRIu8"\n", registers.b & 0b00010000);
                #endif

                break;
            case 97: // 0x61 : TEST BIT 4 OF C

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x61 : TEST BIT 4 OF C}\n");
                #endif
            
                if ( registers.c & 0b00010000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 4 of c = %"PRIu8"\n", registers.c & 0b00010000);
                #endif

                break;
            case 104: // 0x68 : TEST BIT 5 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x68 : TEST BIT 5 OF B}\n");
                #endif
            
                if ( registers.b & 0b00100000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 5 of b = %"PRIu8"\n", registers.b & 0b00100000);
                #endif

                break;
            case 105: // 0x69 : TEST BIT 5 OF C

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x69 : TEST BIT 5 OF C}\n");
                #endif
            
                if ( registers.c & 0b00100000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 5 of c = %"PRIu8"\n", registers.c & 0b00100000);
                #endif

                break;
            case 111: // 0x6F : TEST BIT 5 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x6F : TEST BIT 5 OF A}\n");
                #endif
            
                if ( registers.a & 0b00100000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 5 of a = %"PRIu8"\n", registers.a & 0b00100000);
                #endif

                break;
            case 112: // 0x70 : TEST BIT 6 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x70 : TEST BIT 6 OF B}\n");
                #endif
            
                if ( registers.b & 0b01000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 5 of b = %"PRIu8"\n", registers.b & 0b01000000);
                #endif

                break;
            case 113: // 0x71 : TEST BIT 6 OF C

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x71 : TEST BIT 6 OF C}\n");
                #endif
            
                if ( registers.c & 0b01000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 6 of c = %"PRIu8"\n", registers.c & 0b01000000);
                #endif

                break;
            case 119: // 0x77 : TEST BIT 6 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x77 : TEST BIT 6 OF A}\n");
                #endif
            
                if ( registers.a & 0b01000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 6 of a = %"PRIu8"\n", registers.a & 0b01000000);
                #endif

                break;
            case 120: // 0x78 : TEST BIT 7 OF B

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x78 : TEST BIT 7 OF B}\n");
                #endif
            
                if ( registers.b & 0b10000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 5 of b = %"PRIu8"\n", registers.b & 0b10000000);
                #endif

                break;
            case 121: // 0x79 : TEST BIT 7 OF C

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x79 : TEST BIT 7 OF C}\n");
                #endif
            
                if ( registers.c & 0b10000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 7 of c = %"PRIu8"\n", registers.c & 0b10000000);
                #endif

                break;
            case 123: // 0x7B : TEST BIT 7 OF E

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x7B : TEST BIT 7 OF E}\n");
                #endif
            
                if ( registers.e & 0b10000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 7 of e = %"PRIu8"\n", registers.e & 0b10000000);
                #endif

                break;
            case 124: // 0x7C : TEST BIT 7 OF H

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x7C : TEST BIT 7 OF H}\n");
                #endif

                registers.f &= ~(FLAGS_N);
            
                if ( registers.h & 0b10000000)
                    registers.f = FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 7 of h = %"PRIu8"\n", registers.h & 0b10000000);
                #endif

                break;
            case 126: // 0x7E : TEST BIT 7 OF *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x58 : TEST BIT 3 OF B}\n");
                #endif
            
                if ( memory_read8( registers.hl) & 0b10000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 3;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 7 of hl = %"PRIu8"\n", memory_read8( registers.hl) & 0b10000000);
                #endif

                break;
            case 127: // 0x7F : TEST BIT 7 OF A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x7F : TEST BIT 7 OF A}\n");
                #endif
            
                if ( registers.a & 0b10000000)
                    registers.f = FLAGS_N | FLAGS_H;
                else
                    registers.f = FLAGS_Z | FLAGS_N | FLAGS_H;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tbit 7 of a = %"PRIu8"\n", registers.a & 0b10000000);
                #endif

                break;
            case 134: // 0x86 : CLEAR BIT 0 IN *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x86 : CLEAR BIT 0 IN *(HL)}\n");
                #endif
            
                memory_write8( registers.hl, memory_read8( registers.hl) & 0b11111110);

                emul_time += 4;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.hl = 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
                #endif

                break;
            case 135: // 0x87 : CLEAR BIT 0 IN A

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x87 : CLEAR BIT 0 IN A}\n");
                #endif
            
                registers.a = registers.a & 0b11111110;

                emul_time += 2;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.a = %d (0x%02x)\n", registers.a, registers.a);
                #endif

                break;
            case 158: // 0x9E : RESET BIT 3 IN *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0x9E : RESET BIT 3 IN *(HL)}\n");
                #endif
            
                memory_write8( registers.hl, memory_read8( registers.hl) & ~0b00001000);

                emul_time += 4;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.hl = 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
                #endif

                break;
            case 190: // 0xBE : RESET BIT 7 IN *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0xBE : RESET BIT 7 IN *(HL)}\n");
                #endif
            
                memory_write8( registers.hl, memory_read8( registers.hl) & ~0b10000000);

                emul_time += 4;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.hl = 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
                #endif

                break;
            case 222: // 0xDE : SET BIT 3 IN *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0xDE : SET BIT 3 IN *(HL)}\n");
                #endif
            
                memory_write8( registers.hl, memory_read8( registers.hl) | 0b00001000);

                emul_time += 4;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.hl = 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
                #endif

                break;
            case 254: // 0xFE : SET BIT 7 IN *(HL)

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "[INFO] {0xFE : SET BIT 7 IN *(HL)}\n");
                #endif
            
                memory_write8( registers.hl, memory_read8( registers.hl) | 0b10000000);

                emul_time += 4;

                #ifdef __STEP
                    if ( verbose)
                        fprintf( stdout, "\tregisters.hl = 0x%02x; *(hl) 0x%02x (%"PRIu8")\n", registers.hl, memory_read8( registers.hl), memory_read8( registers.hl));
                #endif

                break;
            default:

                #ifdef __DEBUG
                    fprintf( stderr, "[FATAL] : extended instruction 0x%02x (%"PRIu8") not implemented. crashed after %"PRIu64" instructions\n", op8, op8, registers.ex);
                    fprintf( stderr, "\t[TRACE] : pc 0x%02x <> sp %d (0x%02x)\n", registers.pc, registers.sp, registers.sp);
                    fprintf( stderr, "\t[TRACE] : a %d (0x%02x) <> f %d (0x%02x) <> af %d (0x%02x)\n", registers.a, registers.a, registers.f, registers.f, registers.af, registers.af);
                #endif

                exit( EXIT_FAILURE);
                break;
        }
    
        break;
    case 205: // 0xCD CALL XX XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xCD CALL XX XX\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.pc);
        registers.pc = op16;

        break;
    case 206: // 0xCE A += XX + FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xCE A += XX + FLAGS_C\n");
        #endif

        registers.a += op8;

        if ( registers.f & FLAGS_C)
            (registers.a)++;

        if ( registers.a < op8)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        if ( !registers.a)
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a = 0x%02x (%"PRIu8"); op8 0x%02x (%"PRIu8")\n", registers.a, registers.a, op8, op8);
        #endif

        break;
    case 208: // 0xD0 RET IF !FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xD0 RET IF !FLAGS_C\n");
        #endif

        if ( !( registers.f & FLAGS_C)) {

            registers.pc = memory_read16( registers.sp);
            registers.sp += 2;
            emul_time += 3;
        }

        break;
    case 209: // 0xD1 POP DE

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xD1 POP DE\n");
        #endif

        registers.de = memory_read16( registers.sp);
        registers.sp += 2;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : de 0x%02x (%"PRIu16")\n", registers.de, registers.de);
        #endif

        break;
    case 213: // 0xD5 PUSH DE

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xD5 PUSH DE\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.de);

        break;
    case 214: // 0xD6 A -= XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xD6 A -= XX\n");
        #endif

        registers.a -= op8;

        if ( registers.a > op8)
            registers.f = FLAGS_C | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( !(registers.a))
            registers.f |= FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : XX 0x%02x (%"PRIu8"); a 0x%02x (%"PRIu8")\n", op8, op8, registers.a, registers.a);
        #endif

        break;
    case 216: // 0xD8 RET IF FLAGS_C

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xD8 RET IF FLAGS_C\n");
        #endif

        if ( registers.f & FLAGS_C) {

            registers.pc = memory_read16( registers.sp);
            registers.sp += 2;
            emul_time += 3;
        }

        break;
    case 217: // 0xD9 RETI

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xD9 RETI\n");
        #endif

        interrupt_enable();

        registers.pc = memory_read16( registers.sp);
        registers.sp += 2;

        break;
    case 222: // 0xDE A -= (XX + C_FLAG)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xDE A -= (XX + C_FLAG)\n");
        #endif

        if ( registers.f & FLAGS_C) {

            registers.f = FLAGS_N;
            
            if ( (((registers.a & 0x0F) - (op8 & 0x0F) - 1) & 0x10) == 0x10 )
                registers.f |= FLAGS_H;
            else
                registers.f &= ~FLAGS_H;

            #ifdef __STEP
                if ( verbose)
                    fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); XX 0x%02x (%"PRIu8"); carry YES; res a 0x%02x (%"PRIu8")\n", registers.a, registers.a, op8, op8, (uint8_t)(registers.a - op8), (uint8_t)(registers.a - op8 - 1));
            #endif

            if ( registers.a < ( op8 + 1))
                registers.f |= FLAGS_C;

            registers.a -= ( op8 + 1);

            if ( !(registers.a))
                registers.f |= FLAGS_Z;
        } else {

            registers.f = FLAGS_N;
            
            if ( (((registers.a & 0x0F) - (op8 & 0x0F)) & 0x10) == 0x10 )
                registers.f |= FLAGS_H;
            else
                registers.f &= ~FLAGS_H;

            #ifdef __STEP
                if ( verbose)
                    fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); XX 0x%02x (%"PRIu8"); carry NO; res a 0x%02x (%"PRIu8")\n", registers.a, registers.a, op8, op8, (uint8_t)(registers.a - op8), (uint8_t)(registers.a - op8));
            #endif

            if ( registers.a < op8)
                registers.f |= FLAGS_C;

            registers.a -= op8;

            if ( !(registers.a))
                registers.f |= FLAGS_Z;
        }

        break;
    case 224: // 0xE0 *(0xFF00 + XX) = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE0 *(0xFF00 + XX) = A\n");
        #endif

        memory_write8( 0xFF00 + op8, registers.a);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : *(0xFF00 + XX -> 0x%02x) = 0x%02x\n", op8, memory_read8( 0xFF00 + op8));
        #endif

        break;
    case 225: // 0xE1 POP HL

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE1 POP HL\n");
        #endif

        registers.hl = memory_read16( registers.sp);
        registers.sp += 2;

        break;
    case 226: // 0xE2 *(0xFF00 + C) = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE2 *(0xFF00 + C) = A\n");
        #endif

        memory_write8( 0xFF00 + registers.c, registers.a);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : *(0xFF00 + C -> 0x%02x) = 0x%02x\n", registers.c, memory_read8( 0xFF00 + registers.c));
        #endif

        break;
    case 229: // 0xE5 PUSH HL

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE5 PUSH HL\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.hl);

        break;
    case 230: // 0xE6 A = A & XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE6 A = A & XX\n");
        #endif

        registers.a &= op8;
        
        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); immmediate : 0x%02x\n", registers.a, registers.a, op8);
        #endif

        break;
    case 232: // 0xE8 SP += XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE8 SP += (signed)XX\n");
        #endif

        if ( (int8_t)(op8) > 0) {

            if ( registers.sp += (int8_t)(op8) < registers.sp)
                registers.f = FLAGS_C;
            else
                registers.f = 0;
        } else {

            if ( registers.sp += (int8_t)(op8) > registers.sp)
                registers.f = FLAGS_C;
            else
                registers.f = 0;
        }

        registers.sp += (int8_t)(op8);

        // TODO FLAGS_H + REVIEW FLAGS_C

        break;
    case 233: // 0xE9 JMP *(HL)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xE9 JMP *(HL)\n");
        #endif

        registers.pc = registers.hl;

        break;
    case 234: // 0xEA *(XX XX) = A

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] :0xEA *(XX XX) = A\n");
        #endif

        memory_write8( op16, registers.a);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0x%02x = 0x%02x (%"PRIu8")\n", op16, registers.a, registers.a);
        #endif

        break;
    case 238: // 0xEE A ^= XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] :  0xEE A ^= XX\n");
        #endif

        registers.a = registers.a ^ op8;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : value = 0x%02x; a 0x%02x\n", op8, registers.a);
        #endif

        break;
    case 239: // 0xEF RST 0x28

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xEF RST 0x28\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.pc);
        registers.pc = 0x28;

        break;
    case 240: // 0xF0  A = *(0xFF00 + XX)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF0  A = *(0xFF00 + XX)\n");
        #endif

        registers.a = memory_read8( 0xFF00 + op8);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x; XX -> 0x%02x\n", registers.a, op8);
        #endif

        break;
    case 241: // 0xF1 POP AF

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF1 POP AF\n");
        #endif

        registers.af = memory_read16( registers.sp);
        registers.sp += 2;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : af 0x%02x (%"PRIu16")\n", registers.af, registers.af);
        #endif

        break;
    case 243: // 0xF3 disable interrupt

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF3 disable interrupt\n");
        #endif

        interrupt_disable();

        break;
    case 245: // 0xF5 PUSH AF

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF5 PUSH AF\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.af);

        break;
    case 246: // 0xF6 A |= XX

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF6 A |= XX\n");
        #endif

        registers.a |= op8;

        if ( registers.a)
            registers.f = 0;
        else
            registers.f = FLAGS_Z;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); XX = 0x%02x\n", registers.a, registers.a, op8);
        #endif

        break;
    case 248: // 0xF8 HL = SP + (signed)op8

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF8 HL = SP + (signed)op8\n");
        #endif

        registers.hl = registers.sp + (int8_t)op8;

        if ( (int8_t)op8 < 0 && registers.hl > registers.sp)
            registers.f = FLAGS_C;
        else if ( (int8_t)op8 > 0 && registers.hl < registers.sp)
            registers.f = FLAGS_C;
        else
            registers.f = 0;

        break;
    case 249: // 0xF9 SP = HL

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xF9 SP = HL\n");
        #endif

        registers.sp = registers.hl;

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : sp 0x%04x\n", registers.sp);
        #endif

        break;
    case 250: // 0xFA A = *(XX XX)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xFA A = *(XX XX)\n");
        #endif

        registers.a = memory_read8( op16);

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : a 0x%02x (%"PRIu8"); XX XX -> 0x%04x\n", registers.a, registers.a, op16);
        #endif

        break;
    case 251: // 0xFB enable interrupt

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xFB enable interrupt\n");
        #endif

        interrupt_enable();

        break;
    // case 252: 0xFC BAD INSTRUCTION
    // case 253: 0xFD BAD INSTRUCTION
    case 254: // 0xFE A == XX (compare A - XX)

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xFE A == XX (compare A - XX -> 0x%02x)\n", op8);
        #endif

        if ( registers.a == op8)
            registers.f = FLAGS_Z | FLAGS_N;
        else
            registers.f = FLAGS_N;

        if ( registers.a < op8)
            registers.f |= FLAGS_C;

        break;
    case 255: // 0xFF RST 0x38

        #ifdef __STEP
            if ( verbose)
                fprintf( stderr, "[INFO] : 0xFF RST 0x38\n");
        #endif

        registers.sp -= 2;
        memory_write16( registers.sp, registers.pc);
        registers.pc = 0x38;

        break;
    default:

        #ifdef __DEBUG
            fprintf( stderr, "[FATAL] : instruction %d (0x%02x) not implemented. crashed after %"PRIu64" instructions and %"PRIu64" time\n", opcode, opcode, registers.ex, emul_time);
            fprintf( stderr, "\t[TRACE] : pc 0x%02x <> sp %d (0x%02x)\n", registers.pc, registers.sp, registers.sp);
            fprintf( stderr, "\t[TRACE] : a %"PRIu8" (0x%02x) <> f %"PRIu8" (0x%02x) <> af %"PRIu16" (0x%02x)\n", registers.a, registers.a, registers.f, registers.f, registers.af, registers.af);
            fprintf( stderr, "\t[TRACE] : b %"PRIu8" (0x%02x) <> c %"PRIu8" (0x%02x) <> bc %"PRIu16" (0x%02x)\n", registers.b, registers.b, registers.c, registers.c, registers.bc, registers.bc);
            fprintf( stderr, "\t[TRACE] : d %"PRIu8" (0x%02x) <> e %"PRIu8" (0x%02x) <> de %"PRIu16" (0x%02x)\n", registers.d, registers.d, registers.e, registers.e, registers.de, registers.de);
            fprintf( stderr, "\t[TRACE] : h %"PRIu8" (0x%02x) <> l %"PRIu8" (0x%02x) <> hl %"PRIu16" (0x%02x)\n", registers.h, registers.h, registers.l, registers.l, registers.hl, registers.hl);
        #endif

        exit( EXIT_FAILURE);
        break;
}