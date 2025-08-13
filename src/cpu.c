#include "cpu.h"
#include "defines.h"
#include "mem.h"
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include "crapstate.h"

static inline void opcodes_cb(uint8_t postfix){
    switch (postfix) {
        case 0x21:{ //SLA C
            crapstate.cpu.C = (crapstate.cpu.c & 0x80)!=0;
            crapstate.cpu.c <<=1;
            crapstate.cpu.Z = !crapstate.cpu.c;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("shifted c arithemicaly to left by one %x c %x c %x z \n",crapstate.cpu.c,crapstate.cpu.C,crapstate.cpu.Z);
            break;
        }

        case 0x22:{ //SLA D
            crapstate.cpu.C = (crapstate.cpu.d & 0x80)!=0;
            crapstate.cpu.d <<=1;
            crapstate.cpu.Z = !crapstate.cpu.d;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("shifted d arithemicaly to left by one %x d %x c %x z \n",crapstate.cpu.d,crapstate.cpu.C,crapstate.cpu.Z);
            break;
        }
        
        case 0x27:{ //SLA A
            crapstate.cpu.C = (crapstate.cpu.a & 0x80)!=0;
            crapstate.cpu.a <<=1;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("shifted a arithemicaly to left by one %x d %x c %x z \n",crapstate.cpu.a,crapstate.cpu.C,crapstate.cpu.Z);
            break;
        }
        

        case 0x3F:{ //SRL A
            crapstate.cpu.C = (crapstate.cpu.a & 0x01);
            crapstate.cpu.a >>=1;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("shifted a logicaly to right by one %x a %x c %x z \n",crapstate.cpu.a,crapstate.cpu.C,crapstate.cpu.Z);
            break;
        }

        case 0x4F:{// BIT 1 A
            crapstate.cpu.Z = !(crapstate.cpu.a & (1 << 1));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 1 in a res %x\n",crapstate.cpu.Z);
            break;
            
        }

        case 0x5F:{// BIT 3 A
            crapstate.cpu.Z = !(crapstate.cpu.a & (1 << 3));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 1 in a res %x\n",crapstate.cpu.Z);
            break;
            
        }

        case 0x5E:{ //BIT 3 [HL]
            uint8_t byte  = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.Z = !(byte & (1 << 3));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 3 in at addr %x res %x\n",crapstate.cpu.hl,crapstate.cpu.Z);
            crapstate.cpu.cycles+=4;
            break;
            
        }

        case 0x77: { //BIT 6 A
            crapstate.cpu.Z = !(crapstate.cpu.a & (1 << 6));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 7 in a res %x\n",crapstate.cpu.Z);
            break;
        }

        case 0x7F:{ //BIT 7 A
            crapstate.cpu.Z = !(crapstate.cpu.a & (1 << 7));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 7 in a res %x\n",crapstate.cpu.Z);
            break;
            
        }

        case 0xBE:{ //RES 7 , [HL]
            uint8_t byte  = mem_read_byte(crapstate.cpu.hl);
            byte &=0x7f;
            mem_write_byte(crapstate.cpu.hl, byte);
            printf("unset bit 7 in byte at addr %x now %x\n",crapstate.cpu.hl,byte);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0xFE:  { //SET 5, [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            byte |= (1<<5);
            mem_write_byte(crapstate.cpu.hl, byte);
            printf("set bit 5 in byte at addr %x now%x \n",crapstate.cpu.hl,byte);
            crapstate.cpu.cycles+=8;
            break;

        }

        default:{
            
            printf("unhandled cb postcode %x\n",postfix);
            while(1);
        }
    }
} 

static void opcodes(uint8_t opcode){
    switch (opcode) {
        case 0x00:{
            //nop
            printf("got nop\n");
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }
        case 0x01:{
            //ld_bc_n16
            crapstate.cpu.bc = mem_read_word(crapstate.cpu.pc+1);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=12;
            printf("loaded %x to bc\n",crapstate.cpu.bc);
            break;
        }
        case 0x02: { // LD [BC], A
            mem_write_byte(crapstate.cpu.bc,crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded a %x to memory addr %x from bc\n",crapstate.cpu.a,crapstate.cpu.bc);
            break;  
        }
        case 0x03:{
            //inc_bc
            crapstate.cpu.bc++;
            crapstate.cpu.pc++;
            printf("inc bc now %x\n",crapstate.cpu.bc);
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x05:  { // DEC B
            crapstate.cpu.H = !(crapstate.cpu.b & 0x0F) ;
            crapstate.cpu.b--;
            crapstate.cpu.Z  = !crapstate.cpu.b;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec b %x z %x h %x \n",crapstate.cpu.b, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x06:  { //LD B, n8
            crapstate.cpu.b = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to b\n", crapstate.cpu.b);
            break;
        }

        case 0x07: {  // RLCA
            uint8_t old_A = crapstate.cpu.a;
            crapstate.cpu.a = (old_A << 1) | (old_A >> 7);
            crapstate.cpu.C = (old_A >> 7) & 1;
            crapstate.cpu.Z = 0;  // Unlike RLC A, Z is always 0 in RLCA!
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate a  a %x,c %x\n", crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }

        case 0x09:{ // ADD HL, BC
            uint16_t old_hl = crapstate.cpu.hl;
            crapstate.cpu.hl += crapstate.cpu.bc;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_hl & 0x0FFF) + (crapstate.cpu.bc & 0x0FFF)) > 0x0FFF;
            crapstate.cpu.C = ((uint32_t)old_hl + (uint32_t)crapstate.cpu.bc) > 0xFFFF;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            printf("added bc %x to hl %x res %x flags  h%x c%x\n", crapstate.cpu.bc,old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        case 0x0A: { // LD A, (BC)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.bc);
            printf("read %x byte to a pointed by bc at addr %x\n", crapstate.cpu.a, crapstate.cpu.bc);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        case 0x0B:{
            //dec_bc
            crapstate.cpu.bc--;
            crapstate.cpu.pc++;
            printf("decremented bc now %x\n",crapstate.cpu.bc);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0x0C:  { //INC C
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.c & 0x0F) + 1) > 0x0F;
            crapstate.cpu.c++;
            crapstate.cpu.Z = !crapstate.cpu.c;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc c %x h %x z%x\n",crapstate.cpu.c,crapstate.cpu.H,crapstate.cpu.Z);
            break;
        }

        case 0x0D:  { // DEC C
            crapstate.cpu.H = !(crapstate.cpu.c & 0x0F) ;
            crapstate.cpu.c--;
            crapstate.cpu.Z  = !crapstate.cpu.c;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec c %x z %x h %x \n",crapstate.cpu.c, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x0E: { // LD C, #
            crapstate.cpu.c = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to c\n", crapstate.cpu.c);
            break;
        }

        case 0x11:{
            //ld_de_n16
            crapstate.cpu.de = mem_read_word(crapstate.cpu.pc+1);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=12;
            printf("loaded %x to de\n",crapstate.cpu.de);
            break;
        }
        case 0x12:{
            //ld_de_a
            mem_write_byte(crapstate.cpu.de, crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("wrote %x from a to byte %x pointed by de\n",crapstate.cpu.a,crapstate.cpu.de );
            break;
        }
        case 0x13:{
            //inc_de
            crapstate.cpu.de++;
            crapstate.cpu.pc++;
            printf("inc de now %x\n",crapstate.cpu.de);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0x15:  { // DEC D
            crapstate.cpu.H = !(crapstate.cpu.d & 0x0F) ;
            crapstate.cpu.d--;
            crapstate.cpu.Z  = !crapstate.cpu.d;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec d %x z %x h %x \n",crapstate.cpu.d, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x16:{
            //ld_d_n8
            crapstate.cpu.d = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=8;
            printf("loaded %x to d\n",crapstate.cpu.d);
            break;
        }
        case 0x18:{
            //jr
            int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
            printf("jumping relatively to %x\n",crapstate.cpu.pc);
            crapstate.cpu.cycles+=12;
            break;
        }

        case 0x19:{ // ADD HL, DE
            uint16_t old_hl = crapstate.cpu.hl;
            crapstate.cpu.hl += crapstate.cpu.de;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_hl & 0x0FFF) + (crapstate.cpu.de & 0x0FFF)) > 0x0FFF;
            crapstate.cpu.C = ((uint32_t)old_hl + (uint32_t)crapstate.cpu.de) > 0xFFFF;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            printf("added de %x to hl %x res %x flags  h%x c%x\n", crapstate.cpu.de,old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x1A: { // LD A, (DE)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.de);
            printf("read %x byte to a pointed by de at addr %x\n", crapstate.cpu.a, crapstate.cpu.de);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x1C:  { //INC E
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.e & 0x0F) + 1) > 0x0F;
            crapstate.cpu.e++;
            crapstate.cpu.Z = !crapstate.cpu.e;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc e %x h %x z%x\n",crapstate.cpu.e,crapstate.cpu.H,crapstate.cpu.Z);
            break;
        }

        case 0x20:{
            //jr_nz
            if(!crapstate.cpu.Z){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                printf("Z flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=12;
            }else{
                crapstate.cpu.pc+=2;
                crapstate.cpu.cycles+=8;
            }
            break;
        }
        case 0x21:{
            //ld_hl_n16
            crapstate.cpu.hl = mem_read_word(crapstate.cpu.pc+1);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=12;
            printf("loaded %x to hl\n",crapstate.cpu.hl);
            break;
        }

        case 0x22:{
            //ld_hli_a
            mem_write_byte(crapstate.cpu.hl, crapstate.cpu.a);
            crapstate.cpu.hl++;
            crapstate.cpu.pc++;
            printf("loaded from a to hl %x and incremented %x\n",crapstate.cpu.a,crapstate.cpu.hl);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0x23:{ //INC HL
            crapstate.cpu.hl++;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("incremented hl now %x\n",crapstate.cpu.hl);
            break;
        }
        
        case 0x24:{ //inc H
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.h & 0x0F) + 1) > 0x0F;
            crapstate.cpu.h++;
            crapstate.cpu.Z = !crapstate.cpu.h;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc c %x h %x z%x\n",crapstate.cpu.h,crapstate.cpu.H,crapstate.cpu.Z);
            break;
        }

        case 0x26: { // LD H, #
            crapstate.cpu.h = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to h\n", crapstate.cpu.h);
            break;
        }

        case 0x28:{ // JR Z
            if(crapstate.cpu.Z){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                printf("Z flag set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=12;
            }else{
                crapstate.cpu.pc+=2;
                crapstate.cpu.cycles+=8;
            }
            break;
        }

        case 0x29: { // ADD HL, HL
            uint16_t old_hl = crapstate.cpu.hl;
            crapstate.cpu.hl += crapstate.cpu.hl;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_hl & 0x0FFF) + (old_hl & 0x0FFF)) > 0x0FFF;
            crapstate.cpu.C = ((uint32_t)old_hl + (uint32_t)old_hl) > 0xFFFF;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            printf("added hl %x to hl res %x flags  h%x c%x\n", old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x2A: { // LD A, (HL+)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to a pointed by hl at addr and incremented hl %x\n", crapstate.cpu.a, crapstate.cpu.hl);
            crapstate.cpu.hl++;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0x2B:{ //DEC HL
            crapstate.cpu.hl--;
            crapstate.cpu.pc++;
            printf("decremented hl now %x\n",crapstate.cpu.hl);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0x2F: {//CPL
            crapstate.cpu.a = ~crapstate.cpu.a;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("negated a %x\n",crapstate.cpu.a);
            break;
        }

        case 0x30:{
            //JR NC
            if(!crapstate.cpu.C){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                printf("C flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=12;
            }else{
                crapstate.cpu.pc+=2;
                crapstate.cpu.cycles+=8;
            }
            break;
        }

        case 0x31: { // LD SP, nn
            crapstate.cpu.sp = mem_read_word(crapstate.cpu.pc + 1);
            printf("loaded %x to sp\n", crapstate.cpu.sp);
            crapstate.cpu.pc += 3;
            crapstate.cpu.cycles += 12;
            break;
        }

        case 0x32:{// LD HLD, A
            mem_write_byte(crapstate.cpu.hl, crapstate.cpu.a);
            crapstate.cpu.hl--;
            crapstate.cpu.pc++;
            printf("loaded from a to hl %x and decremented %x\n",crapstate.cpu.a,crapstate.cpu.hl);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0x34: { // INC [HL]
            uint8_t val = mem_read_byte(crapstate.cpu.hl);
            uint8_t result = val + 1;
            mem_write_byte(crapstate.cpu.hl, result);

            crapstate.cpu.Z = !result;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((val & 0xF) + 1 > 0xF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 12;
            printf("incremented a byte at hl %x res %x\n",crapstate.cpu.hl,result);
            
            break;
        }

        case 0x38:{ // JR C
            if(crapstate.cpu.C){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                printf("C flag set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=12;
            }else{
                crapstate.cpu.pc+=2;
                crapstate.cpu.cycles+=8;
            }
            break;
        }

        case 0x3D:  { // DEC A
            crapstate.cpu.H = !(crapstate.cpu.a & 0x0F) ;
            crapstate.cpu.a--;
            crapstate.cpu.Z  = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec a %x z %x h %x \n",crapstate.cpu.a, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x3C:  { //INC A
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) + 1) > 0x0F;
            crapstate.cpu.a++;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc a %x h %x z%x\n",crapstate.cpu.a,crapstate.cpu.H,crapstate.cpu.Z);
            break;
        }

        case 0x3E: { // LD A, #
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to a\n", crapstate.cpu.a);
            break;
        }

        case 0x3F:  {//CCF
            crapstate.cpu.C = !crapstate.cpu.C;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            printf("inverted carry flag %x\n", crapstate.cpu.C);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x44: { // LD B, H
            crapstate.cpu.b = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from b to h\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }
        
        case 0x46: { // LD B, (HL)
            crapstate.cpu.b = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to b pointed by hl at addr %x\n", crapstate.cpu.b, crapstate.cpu.hl);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x47: { // LD B, A
            crapstate.cpu.b = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to b\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x4D: { // LD C, L
            crapstate.cpu.c = crapstate.cpu.l;
            crapstate.cpu.pc++;
            printf("loaded %x from l to c\n", crapstate.cpu.c);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x4E: { // LD C, (HL)
            crapstate.cpu.c = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to c pointed by hl at addr %x\n", crapstate.cpu.c, crapstate.cpu.hl);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x4F: { // LD C, A
            crapstate.cpu.c = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to c\n", crapstate.cpu.c);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x57: { // LD D, A
            crapstate.cpu.d = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x5F: { // LD E, A
            crapstate.cpu.e = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to e\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }
        
        case 0x60: { // LD H, B
            crapstate.cpu.h = crapstate.cpu.b;
            crapstate.cpu.pc++;
            printf("loaded %x from h to b\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x62: { // LD H, D
            crapstate.cpu.h = crapstate.cpu.d;
            crapstate.cpu.pc++;
            printf("loaded %x from d to h\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x66:{ //LD L,[HL]
            crapstate.cpu.l = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to l pointed by hl at addr hl %x\n", crapstate.cpu.l, crapstate.cpu.hl);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x67:{ //LD H, A
            crapstate.cpu.h = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to h\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x69: { // LD L, C
            crapstate.cpu.l = crapstate.cpu.c;
            crapstate.cpu.pc++;
            printf("loaded %x from l to c\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x6B: { // LD L, E
            crapstate.cpu.l = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from l to e\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x6E: { // LD L, (HL)
            crapstate.cpu.l = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to l pointed by hl at addr %x\n", crapstate.cpu.l, crapstate.cpu.hl);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x6F: { // LD L, A
            crapstate.cpu.l = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to l\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x70: { // LD [HL], B
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.b);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded b %x to memory addr %x from hl\n",crapstate.cpu.c,crapstate.cpu.hl);
            break;  
        }

        case 0x71: { // LD [HL], C
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.c);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded c %x to memory addr %x from hl\n",crapstate.cpu.c,crapstate.cpu.hl);
            break;  
        }

        case 0x72: { // LD[HL], D
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.d);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded d %x to memory addr %x from hl\n",crapstate.cpu.d,crapstate.cpu.hl);
            break;
        }

        case 0x76: {// HALT
            if((crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_VBLANK) |(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_STAT)){
                crapstate.cpu.cycles = crapstate.ppu.mode_cycles; // i dunno if it needs full proper calculation of cycles until interrupt 
                                                                  // this will do for now
            }

            static const uint16_t tac_treshhold[4] = {1024, 16, 64, 256};
            if(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_TIMER ){
                
                uint16_t tima_cycles = (256 - crapstate.io.TIMA) * tac_treshhold[ crapstate.io.TAC & 0x03];
                if(crapstate.cpu.cycles> tima_cycles){
                    crapstate.cpu.cycles = tima_cycles;
                }
            }
            printf("halted\n");
            break;
        }

        case 0x77:{ //LD [HL], A
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded a %x to memory addr %x from hl\n",crapstate.cpu.a,crapstate.cpu.hl);
            break;
        }

        case 0x78: { // LD A, B
            crapstate.cpu.a = crapstate.cpu.b;
            crapstate.cpu.pc++;
            printf("loaded %x from b to a\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x79: { // LD A, C
            crapstate.cpu.a = crapstate.cpu.c;
            crapstate.cpu.pc++;
            printf("loaded %x from c to a\n", crapstate.cpu.c);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x7A: { // LD A, D
            crapstate.cpu.a = crapstate.cpu.d;
            crapstate.cpu.pc++;
            printf("loaded %x from d to a\n", crapstate.cpu.a);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x7B: { // LD A, E
            crapstate.cpu.a = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from e to a\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x7C: { // LD A, H
            crapstate.cpu.a = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from h to a\n", crapstate.cpu.a);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x7D: { // LD A, L
            crapstate.cpu.a = crapstate.cpu.l;
            crapstate.cpu.pc++;
            printf("loaded %x from l to a\n", crapstate.cpu.a);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x7E: { // LD A, (HL)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to a pointed by hl at addr %x\n", crapstate.cpu.a, crapstate.cpu.hl);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        case 0x80: { // ADD A, B
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.b;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.b & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added b %x to a %x res %x flags z%x h%x c%x\n", crapstate.cpu.b, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x85: { // ADD A, L
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.l;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.l & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added l %x to a %x res %x flags z%x h%x c%x\n", crapstate.cpu.l, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x87: { // ADD A, A
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.a;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (old_a & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added a to a %x res %x flags z%x h%x c%x\n", old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0xAF: { // XOR A
            crapstate.cpu.a = 0;
            crapstate.cpu.Z = 1;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a\n");
            break;
        }

        case 0xA7: { // AND A, A
            crapstate.cpu.a &= crapstate.cpu.a;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and a res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xB0: { // OR A, B
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.b;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and b result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB1: { // OR A, C
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.c;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and c result %x\n", crapstate.cpu.a);
            break;
        }
        
        case 0xB7: {
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.a;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and a result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xC0: { // RET NZ
            if (!crapstate.cpu.Z) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                printf("returning to %x\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 20;
            } else {
                crapstate.cpu.pc++;
                crapstate.cpu.cycles += 8;
            }
            break;
        }

        case 0xC1: { //POP BC
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.bc = (uint16_t)low | ((uint16_t)high << 8);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=12;
            printf("popped bc = %x\n",crapstate.cpu.bc);
            break;
        }

        case 0xC2:{
            //JP NZ
            if(!crapstate.cpu.Z){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                printf("Z flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=16;
            }else{
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles+=12;
            }
            break;
        }

        case 0xC3: { // JP nn
            crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
            printf("jump to %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xC5: { // PUSH BC
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.bc) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.bc) & 0xFF); // Low byte
            printf("pushed %x%x\n", (crapstate.cpu.bc) >> 8, crapstate.cpu.bc & 0xFF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xC7:{ // RST 00H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x00;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        

        }

        case 0xC8: { // RET Z
            if (crapstate.cpu.Z) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                printf("returning to %x\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 20;
            } else {
                crapstate.cpu.pc++;
                crapstate.cpu.cycles += 8;
            }
            break;
        }

        case 0xC9: { // RET
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
            printf("returning to %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }
        case 0xCB: {// CB postcodes
            opcodes_cb(mem_read_byte(crapstate.cpu.pc+1));
            crapstate.cpu.pc +=2;
            crapstate.cpu.cycles+=8;
            break;
        }
        case 0xCC: { // CALL Z
            if (crapstate.cpu.Z) {
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) >> 8); // High byte
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) & 0xFF); // Low byte
                printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                printf("calling to %x because Z\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
            break;
        }

        case 0xCD: { // CALL nn
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
            crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
            printf("calling to %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 24;
            break;
        }

        case 0xCF:{ // RST 08H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x08;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;

        }


        case 0xD0: { // RET NC
            if (!crapstate.cpu.C) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                printf("returning to %x\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 20;
            } else {
                crapstate.cpu.pc++;
                crapstate.cpu.cycles += 8;
            }
            break;
        }

        case 0xD1: { //POP DE
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.de = (uint16_t)low | ((uint16_t)high << 8);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=12;
            printf("popped de = %x\n",crapstate.cpu.de);
            break;
        }

        case 0xD5: { // PUSH DE
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.de) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.de) & 0xFF); // Low byte
            printf("pushed de %x%x\n", (crapstate.cpu.de) >> 8, crapstate.cpu.de & 0xFF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xD6: { //SUB A N8
            uint8_t subval = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.C = subval > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (subval & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - subval;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=8;
            crapstate.cpu.pc+=2;
            printf("sub a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,subval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xD7: { // RST 10H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x10;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xD9:  { // RETI
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
            printf("returning to %x and enabling interrupts\n", crapstate.cpu.pc);
            crapstate.cpu.ime = 1;
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xDE : { //SBC A, n8
            uint8_t subval = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.C = subval + crapstate.cpu.C > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < ((subval +crapstate.cpu.C) & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - (subval+crapstate.cpu.C);
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=8;
            crapstate.cpu.pc+=2;
            printf("sub a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,subval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break; 

        }

        case 0xE0: { // LDH (n), A
            uint16_t addr = 0xFF00 + mem_read_byte(crapstate.cpu.pc + 1);
            mem_write_byte(addr, crapstate.cpu.a);
            printf("wrote a = %x to mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 12;
            break;
        }

        case 0xE1: { //POP HL
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.hl = (uint16_t)low | ((uint16_t)high << 8);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=12;
            printf("popped hl = %x\n",crapstate.cpu.hl);
            break;
        }

        case 0xE2: { //LDH [C],A
            uint16_t addr = 0xFF00 + crapstate.cpu.c;
            mem_write_byte(addr, crapstate.cpu.a);
            printf("wrote a = %x to mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 12;
            break;
        }

        case 0xE5: { // PUSH HL
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.hl) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.hl) & 0xFF); // Low byte
            printf("pushed hl %x%x\n", (crapstate.cpu.hl) >> 8, crapstate.cpu.hl & 0xFF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xE6: { // AND A, n
            uint8_t val = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.a &= val;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and %x res %x\n", val, crapstate.cpu.a);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0xEA: { // LD (nn), A
            uint16_t addr = mem_read_word(crapstate.cpu.pc + 1);
            mem_write_byte(addr, crapstate.cpu.a);
            printf("wrote %x from a to %x \n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 3;
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xEF: { // RST 28H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x28;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xE9: {
            crapstate.cpu.pc = crapstate.cpu.hl;
            printf("jump to hl %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xF0: { // LDH A, (n)
            uint16_t addr = 0xFF00 + mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.a = mem_read_byte(addr);
            printf("wrote to a = %x from mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 12;
            break;
        }
        
        case 0xF1: { //POP AF
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.af = (uint16_t)low | ((uint16_t)high << 8);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=12;
            printf("popped af = %x\n",crapstate.cpu.af);
            break;
        }

        case 0xF3: { // DI
            crapstate.cpu.ime = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("disabled interrupts\n");
            break;
        }

        case 0xF5: { // PUSH AF
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.af) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.af) & 0xFF); // Low byte
            printf("pushed af %x%x\n", (crapstate.cpu.af) >> 8, crapstate.cpu.af & 0xFF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xF7:{ // RST 30H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x30;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xFA:{ //LD A a16
            uint16_t addr = mem_read_word(crapstate.cpu.pc+1);
            crapstate.cpu.a = mem_read_byte(addr);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=16;
            printf("wrote a = %x from addr %x\n",crapstate.cpu.a,addr);
            break;
        }

        case 0xFB: { // EI
            crapstate.cpu.ime_pending = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("enabled interrupts\n");
            break;
        }

        case 0xFE: { //CP A N8
            uint8_t cpval = mem_read_byte(crapstate.cpu.pc+1);
            uint8_t res = crapstate.cpu.a - cpval;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = cpval > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (cpval & 0x0F);
            crapstate.cpu.cycles+=8;
            crapstate.cpu.pc+=2;
            printf("cp a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,cpval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xFF: { // RST 38H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x38;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        default:{
            printf("Unhandled opcode: %X\n", opcode);
            while(1);
            break;
        }
    }   
}

static inline void handle_interrupt() {
    if (crapstate.cpu.ime && (crapstate.io.if_reg & crapstate.io.ie & 0x1F)) {
        
        crapstate.cpu.sp--;
        mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc ) >> 8); // High byte
        crapstate.cpu.sp--;
        mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc ) & 0xFF);
        printf("went to interrupt\n");
        crapstate.cpu.ime = 0;
        
        if(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_VBLANK ){
            crapstate.cpu.pc = VBLANK_VECTOR;
            crapstate.io.if_reg ^= INTERRUPT_VBLANK;
        }else if(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_STAT ){
            crapstate.cpu.pc = STAT_VECTOR;
            crapstate.io.if_reg ^= INTERRUPT_STAT;
        }else if(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_TIMER ){
            crapstate.cpu.pc = TIMER_VECTOR;
            crapstate.io.if_reg ^= INTERRUPT_TIMER;
        }else if(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_SERIAL ){
            crapstate.cpu.pc = SERIAL_VECTOR;
            crapstate.io.if_reg ^= INTERRUPT_SERIAL;
        }else if(crapstate.io.ie&crapstate.io.if_reg & INTERRUPT_JOYPAD ){
            crapstate.cpu.pc = JOYPAD_VECTOR;
            crapstate.io.if_reg ^= INTERRUPT_JOYPAD;
        }
        
    } 
}

void update_cpu(){
    handle_interrupt();

    if(crapstate.cpu.ime_pending==1){
        crapstate.cpu.ime_pending =0;
        crapstate.cpu.ime=1;
    }
    opcodes(mem_read_byte(crapstate.cpu.pc));

}
void cpu_start(){
      
}