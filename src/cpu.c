/*
Three parts of this code are taken from Sameboy emulator by Lior Harpor https://github.com/LIJI32/SameBoy
  

Expat License

Copyright (c) 2015-2025 Lior Halphon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.




Everything else you can use however you
*/

#include "cpu.h"
#include "defines.h"
#include "mem.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include "crapstate.h"

static inline void opcodes_cb(uint8_t postfix){
    switch (postfix) {
        case 0x00:  { //RLC B
            crapstate.cpu.b = (crapstate.cpu.b << 1) | (crapstate.cpu.b >> 7);
            crapstate.cpu.C = crapstate.cpu.b & 1;
            crapstate.cpu.Z = !crapstate.cpu.b;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate b left  %x,c %x\n", crapstate.cpu.b, crapstate.cpu.C);
            break;
        }

        case 0x01: { //RLC C
            crapstate.cpu.c = (crapstate.cpu.c << 1) | (crapstate.cpu.c >> 7);
            crapstate.cpu.C = crapstate.cpu.c & 1;
            crapstate.cpu.Z = !crapstate.cpu.c;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate c left  %x,c %x\n", crapstate.cpu.c, crapstate.cpu.C);
            break;
        }

        case 0x02: { //RLC D
            crapstate.cpu.d = (crapstate.cpu.d << 1) | (crapstate.cpu.d >> 7);
            crapstate.cpu.C = crapstate.cpu.d & 1;
            crapstate.cpu.Z = !crapstate.cpu.d;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate d left  %x,c %x\n", crapstate.cpu.d, crapstate.cpu.C);
            break;
        }

        case 0x03:  { //RLC E
            crapstate.cpu.e = (crapstate.cpu.e << 1) | (crapstate.cpu.e >> 7);
            crapstate.cpu.C = crapstate.cpu.e & 1;
            crapstate.cpu.Z = !crapstate.cpu.e;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate e left  %x,c %x\n", crapstate.cpu.e, crapstate.cpu.C);
            break;
        }

        case 0x04:  { //RLC H
            crapstate.cpu.h = (crapstate.cpu.h << 1) | (crapstate.cpu.h >> 7);
            crapstate.cpu.C = crapstate.cpu.h & 1;
            crapstate.cpu.Z = !crapstate.cpu.h;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate b left  %x,c %x\n", crapstate.cpu.h, crapstate.cpu.C);
            break;
        }

        case 0x05: { //RLC L
            crapstate.cpu.l = (crapstate.cpu.l << 1) | (crapstate.cpu.l >> 7);
            crapstate.cpu.C = crapstate.cpu.l & 1;
            crapstate.cpu.Z = !crapstate.cpu.l;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate l left  %x,c %x\n", crapstate.cpu.l, crapstate.cpu.C);
            break;
        }

        case 0x06:  { //RLC [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            byte = (byte << 1) | (byte >> 7);
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.C = byte & 1;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.cycles+=8;
            printf("rotate byte at hl%x left \n %x",byte, crapstate.cpu.C);
            break;
        }

        case 0x07:  { //RLC A
            crapstate.cpu.a = (crapstate.cpu.a << 1) | (crapstate.cpu.a >> 7);
            crapstate.cpu.C = crapstate.cpu.a & 1;
            crapstate.cpu.Z = !crapstate.cpu.a;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate a left  %x,c %x\n", crapstate.cpu.a, crapstate.cpu.C);
            break;
        }

        case 0x08:  { //RRC B
            crapstate.cpu.C = crapstate.cpu.b & 0x1;
            crapstate.cpu.b = (crapstate.cpu.b >>1) | (crapstate.cpu.b << 7 );
            crapstate.cpu.Z = !crapstate.cpu.b;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate b right  %x,c %x\n", crapstate.cpu.b, crapstate.cpu.C);
            break;
        }

        case 0x09:  { //RRC C
            crapstate.cpu.C = crapstate.cpu.c & 0x1;
            crapstate.cpu.c = (crapstate.cpu.c >>1) | (crapstate.cpu.c << 7 );
            crapstate.cpu.Z = !crapstate.cpu.c;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate c right  %x,c %x\n", crapstate.cpu.c, crapstate.cpu.C);
            break;
        }

        case 0x0A:  { //RRC D
            crapstate.cpu.C = crapstate.cpu.d & 0x1;
            crapstate.cpu.d = (crapstate.cpu.d >>1) | (crapstate.cpu.d << 7 );
            crapstate.cpu.Z = !crapstate.cpu.d;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate d right  %x,c %x\n", crapstate.cpu.d, crapstate.cpu.C);
            break;
        }

        case 0x0B:  {  //RRC E
            crapstate.cpu.C = crapstate.cpu.e & 0x1;
            crapstate.cpu.e = (crapstate.cpu.e >>1) | (crapstate.cpu.e << 7 );
            crapstate.cpu.Z = !crapstate.cpu.e;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate e right  %x,c %x\n", crapstate.cpu.e, crapstate.cpu.C);
            break;
        }

        case 0x0C:  { //RRC H
            crapstate.cpu.C = crapstate.cpu.h & 0x1;
            crapstate.cpu.h = (crapstate.cpu.h >>1) | (crapstate.cpu.h << 7 );
            crapstate.cpu.Z = !crapstate.cpu.h;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate h right  %x,c %x\n", crapstate.cpu.h, crapstate.cpu.C);
            break;
        }

        case 0x0D:  { //RRC L
            crapstate.cpu.C = crapstate.cpu.l & 0x1;
            crapstate.cpu.l = (crapstate.cpu.l >>1) | (crapstate.cpu.l << 7 );
            crapstate.cpu.Z = !crapstate.cpu.l;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate b right  %x,c %x\n", crapstate.cpu.l, crapstate.cpu.C);
            break;
        }

        case 0x0E:  { //RRC [HL] 
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.C = byte & 0x1;
            byte = (byte >>1) | (byte << 7 );
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.Z = !byte;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.cycles+=8;
            printf("rotate byte at hl %x right  %x,c %x\n",crapstate.cpu.hl, byte, crapstate.cpu.C);
            break;
        }

        case 0x0F:  { //RRC A
            crapstate.cpu.C = crapstate.cpu.a & 0x1;
            crapstate.cpu.a = (crapstate.cpu.a >>1) | (crapstate.cpu.a << 7 );
            crapstate.cpu.Z = !crapstate.cpu.a;  
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate b right  %x,c %x\n", crapstate.cpu.a, crapstate.cpu.C);
            break;
        }

        case 0x10:  { //RL B
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.b) & 0x80 ? 1 : 0 ;
            crapstate.cpu.b <<= 1;
            crapstate.cpu.b |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !crapstate.cpu.b;
            crapstate.cpu.N = 0;
            printf("rotated b through carry res %x c %x",crapstate.cpu.b, crapstate.cpu.C);
            break;
        }

        case 0x11:  { //RL C
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.c) & 0x80 ? 1 : 0 ;
            crapstate.cpu.c <<= 1;
            crapstate.cpu.c |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !crapstate.cpu.c;
            crapstate.cpu.N = 0;
            printf("rotated c through carry res %x c %x",crapstate.cpu.c, crapstate.cpu.C);
            break;
        }

        case 0x12:  { //RL D
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.d) & 0x80 ? 1 : 0 ;
            crapstate.cpu.d <<= 1;
            crapstate.cpu.d |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !crapstate.cpu.d;
            crapstate.cpu.N = 0;
            printf("rotated d through carry res %x c %x",crapstate.cpu.d, crapstate.cpu.C);
            break;
        }

        case 0x13:  { //RL E
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.e) & 0x80 ? 1 : 0 ;
            crapstate.cpu.e <<= 1;
            crapstate.cpu.e |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !crapstate.cpu.e;
            crapstate.cpu.N = 0;
            printf("rotated e through carry res %x c %x",crapstate.cpu.e, crapstate.cpu.C);
            break;
        }

        case 0x14:   { //RL H
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.h) & 0x80 ? 1 : 0 ;
            crapstate.cpu.h <<= 1;
            crapstate.cpu.h |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !crapstate.cpu.h;
            crapstate.cpu.N = 0;
            printf("rotated h through carry res %x c %x",crapstate.cpu.h, crapstate.cpu.C);
            break;
        }

        case 0x15:  { //RL L
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.l) & 0x80 ? 1 : 0 ;
            crapstate.cpu.l <<= 1;
            crapstate.cpu.l |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !crapstate.cpu.l;
            crapstate.cpu.N = 0;
            printf("rotated l through carry res %x c %x",crapstate.cpu.l, crapstate.cpu.C);
            break;
        }

        case 0x16:  { //RL [HL]
            uint8_t old_carry = crapstate.cpu.C;
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.C = (byte) & 0x80 ? 1 : 0 ;
            byte <<= 1;
            byte |= old_carry;
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = !byte;
            crapstate.cpu.N = 0;
            crapstate.cpu.cycles+=8;
            printf("rotated b through carry res %x c %x",byte, crapstate.cpu.C);
            break;
        }

        case 0x19:  {//RR C
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C =crapstate.cpu.c & 1;
            crapstate.cpu.c  >>=1; 
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.c |= (old_carry<<7) ;
            crapstate.cpu.Z = !crapstate.cpu.c;
            printf("rotated through carry c %x z %x c %x\n",crapstate.cpu.c,crapstate.cpu.Z,crapstate.cpu.C);
            break;
        }
        
        case 0x1A:  {//RR D
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C =crapstate.cpu.d & 1;
            crapstate.cpu.d >>=1; 
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.d |= (old_carry<<7) ;
            crapstate.cpu.Z = !crapstate.cpu.d;
            printf("rotated through carry  d %x z %x c %x\n",crapstate.cpu.d,crapstate.cpu.Z,crapstate.cpu.C);
            break;
        }
        
        case 0x1B: { //RR E
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C =crapstate.cpu.e & 1;
            crapstate.cpu.e >>=1; 
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.e |= (old_carry<<7);
            crapstate.cpu.Z = !crapstate.cpu.e;
            printf("rotated through carry  e %x z %x c %x\n",crapstate.cpu.l,crapstate.cpu.Z,crapstate.cpu.C);
            break;        
        }

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
        
        case 0x37:{ //SWAP A
            crapstate.cpu.a = (crapstate.cpu.a<<4) | (crapstate.cpu.a>>4);
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("swapped a = %x\n",crapstate.cpu.a);
            break;
        }

        case 0x38:  {  //SRL B
            crapstate.cpu.C = (crapstate.cpu.b & 0x01);
            crapstate.cpu.b >>=1;
            crapstate.cpu.Z = !crapstate.cpu.b;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("shifted b logicaly to right by one %x b %x c %x z \n",crapstate.cpu.b,crapstate.cpu.C,crapstate.cpu.Z);
            break;
        }

        case 0x3F:  { //SRL A
            crapstate.cpu.C = (crapstate.cpu.a & 0x01);
            crapstate.cpu.a >>=1;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("shifted a logicaly to right by one %x a %x c %x z \n",crapstate.cpu.a,crapstate.cpu.C,crapstate.cpu.Z);
            break;
        }

        case 0x47:{// BIT 0 A
            crapstate.cpu.Z = !(crapstate.cpu.a & 1);
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 0 in a res %x\n",crapstate.cpu.Z);
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
            printf("tested bit 3 in a res %x\n",crapstate.cpu.Z);
            break;
            
        }

        case 0x67: {//BIT 2 A
            crapstate.cpu.Z = !(crapstate.cpu.a & (1 << 2));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 2 in a res %x\n",crapstate.cpu.Z);
            break;
        }

        case 0x6F:{// BIT 5 A
            crapstate.cpu.Z = !(crapstate.cpu.a & (1 << 5));
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 1;
            printf("tested bit 5 in a res %x\n",crapstate.cpu.Z);
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

        case 0xDF:  { //SET 3 , A 
            crapstate.cpu.a |= (1<<3);
            printf("set bit 3 in a now %x \n",crapstate.cpu.a);
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
            FILE* sav = fopen("sav.bin","wb");
            fwrite(&crapstate, sizeof(crapstate), 1, sav);
            fclose(sav);
            while(1);
        }
    }
} 

static void opcodes(uint8_t opcode){
    switch (opcode) {
        case 0x00:{// NOP
            printf("got nop\n");
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }
        case 0x01:{ //LD BC, n16
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
        case 0x03:{ // INC BC
            crapstate.cpu.bc++;
            crapstate.cpu.pc++;
            printf("inc bc now %x\n",crapstate.cpu.bc);
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x04:  { //INC B
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.b & 0x0F) + 1) > 0x0F;
            crapstate.cpu.b++;
            crapstate.cpu.Z = !crapstate.cpu.b;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc b %x h %x z%x\n",crapstate.cpu.b,crapstate.cpu.H,crapstate.cpu.Z);
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
            crapstate.cpu.C = crapstate.cpu.a & 1;
            crapstate.cpu.Z = 0;  // Unlike RLC A, Z is always 0 in RLCA!
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            printf("rotate a left a %x,c %x\n", crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }

        case 0x08:  { // LD [n16], SP
            uint16_t addr = mem_read_word(crapstate.cpu.pc+1);
            mem_write_word(addr,crapstate.cpu.sp);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=20;
            printf("loaded sp %x to %x\n",crapstate.cpu.sp,addr);
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

        case 0x0F:  { //RRCA
            uint8_t old_A = crapstate.cpu.a;
            crapstate.cpu.a = (old_A >> 1) | (old_A << 7);
            crapstate.cpu.C = old_A & 1;
            crapstate.cpu.Z = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("rotated a right a %x, c%x \n",crapstate.cpu.a,crapstate.cpu.C);
            break;
        }

        case 0x11:{//LD DE, n16
            crapstate.cpu.de = mem_read_word(crapstate.cpu.pc+1);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=12;
            printf("loaded %x to de\n",crapstate.cpu.de);
            break;
        }

        case 0x12:{ // LD [DE],a
            mem_write_byte(crapstate.cpu.de, crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("wrote %x from a to byte %x pointed by de\n",crapstate.cpu.a,crapstate.cpu.de );
            break;
        }

        case 0x13:{ //INC DE
            crapstate.cpu.de++;
            crapstate.cpu.pc++;
            printf("inc de now %x\n",crapstate.cpu.de);
            crapstate.cpu.cycles+=8;
            break;
        }

        case 0x14:  { //INC D
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.d & 0x0F) + 1) > 0x0F;
            crapstate.cpu.d++;
            crapstate.cpu.Z = !crapstate.cpu.d;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc d %x h %x z%x\n",crapstate.cpu.d,crapstate.cpu.H,crapstate.cpu.Z);
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

        case 0x16:  { // LD D, n8
            crapstate.cpu.d = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=8;
            printf("loaded %x to d\n",crapstate.cpu.d);
            break;
        }

        case 0x17:  { // RLA            
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.a) & 0x80 ? 1 : 0 ;
            crapstate.cpu.a <<= 1;
            crapstate.cpu.a |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = 0;
            crapstate.cpu.N = 0;
            printf("rotated a through carry res %x c %x",crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=4;
            break;
        }

        case 0x18:{ // JR
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

        case 0x1B:{ //DEC DE
            crapstate.cpu.de--;
            crapstate.cpu.pc++;
            printf("decremented de now %x\n",crapstate.cpu.de);
            crapstate.cpu.cycles+=8;
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

        case 0x1D:  { // DEC E
            crapstate.cpu.H = !(crapstate.cpu.e & 0x0F) ;
            crapstate.cpu.e--;
            crapstate.cpu.Z  = !crapstate.cpu.e;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec e %x z %x h %x \n",crapstate.cpu.e, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x1E:  { //LD E, n8
            crapstate.cpu.e = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to a\n", crapstate.cpu.e);
            break;

        }

        case 0x1F: { //RRA
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.a) & 0x1 ;
            crapstate.cpu.a <<= 1;
            crapstate.cpu.a |= (old_carry<<7);
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = 0;
            crapstate.cpu.N = 0;
            printf("rotated a right through carry res %x c %x",crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=4;
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

        case 0x25:  { // DEC H
            crapstate.cpu.H = !(crapstate.cpu.h & 0x0F) ;
            crapstate.cpu.h--;
            crapstate.cpu.Z  = !crapstate.cpu.h;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec h %x z %x h %x \n",crapstate.cpu.h, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x26: { // LD H, n8
            crapstate.cpu.h = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to h\n", crapstate.cpu.h);
            break;
        }

        case 0x27:  {  // DAA
 		/* The following is from SameBoy. MIT License. */
            int16_t a = crapstate.cpu.a;
            if(crapstate.cpu.af==0xFFF0){
                printf("HIT\n");
            }
            if (crapstate.cpu.N) {
                if (crapstate.cpu.H){
                    a = (a - 0x06) & 0xFF;
                }

                if (crapstate.cpu.C){
                    a -= 0x60;
                }

            } else {
                if (crapstate.cpu.H || (a & 0x0F) > 9){
                    a += 0x06;
                }

                if (crapstate.cpu.C|| a > 0x9F){
                    a += 0x60;
                }
            }

            if ((a & 0x100) == 0x100){
                crapstate.cpu.C= 1;
            }
            printf("daa a%x res %x \n",crapstate.cpu.a,a);
            crapstate.cpu.a = a;
            crapstate.cpu.Z = (crapstate.cpu.a == 0);
            crapstate.cpu.H = 0;
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=4;

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

        case 0x2C:  { //INC L
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((crapstate.cpu.l & 0x0F) + 1) > 0x0F;
            crapstate.cpu.l++;
            crapstate.cpu.Z = !crapstate.cpu.l;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
          
            printf("inc l %x h %x z%x\n",crapstate.cpu.l,crapstate.cpu.H,crapstate.cpu.Z);
            break;
        }

        case 0x2D:  { // DEC L
            crapstate.cpu.H = !(crapstate.cpu.l & 0x0F) ;
            crapstate.cpu.l--;
            crapstate.cpu.Z  = !crapstate.cpu.l;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("dec b %x z %x h %x \n",crapstate.cpu.l, crapstate.cpu.Z,crapstate.cpu.H);
            break;

        }

        case 0x2E: { //LD L,n8
            crapstate.cpu.l = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            printf("loaded %x to l\n", crapstate.cpu.a);
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

        case 0x33: {//INC SP
            crapstate.cpu.sp++;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("incremented sp now %x\n",crapstate.cpu.hl);
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

        case 0x35: { // DEC [HL]
            uint8_t val = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.H = !(crapstate.cpu.a & 0x0F);
            uint8_t result = val - 1;
            mem_write_byte(crapstate.cpu.hl, result);

            crapstate.cpu.Z = !result;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 12;
            printf("dec a byte at hl %x res %x\n",crapstate.cpu.hl,result);
            
            break;
        }

        case 0x36: {// LD HL,n8
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=12;
            printf("wrote byte %x to addr%x\n",byte,crapstate.cpu.hl);
            break;
        }

        case 0x37:  { //SCF
            crapstate.cpu.C =1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("set carry flag\n");
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

        case 0x39:{ // ADD HL, SP
            uint16_t old_hl = crapstate.cpu.hl;
            crapstate.cpu.hl += crapstate.cpu.sp;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_hl & 0x0FFF) + (crapstate.cpu.sp & 0x0FFF)) > 0x0FFF;
            crapstate.cpu.C = ((uint32_t)old_hl + (uint32_t)crapstate.cpu.sp) > 0xFFFF;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            printf("added sp %x to hl %x res %x flags  h%x c%x\n", crapstate.cpu.sp,old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x3A: { // LD A, HLD
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to a pointed by hl at addr and decremented hl %x\n", crapstate.cpu.a, crapstate.cpu.hl);
            crapstate.cpu.hl--;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x3B: { //DEC SP
            crapstate.cpu.sp--;
            crapstate.cpu.pc++;
            printf("decremented sp now %x\n",crapstate.cpu.sp);
            crapstate.cpu.cycles+=8;
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

        case 0x3E: { // LD A, n8
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

        case 0x40: { //LD B,B
            printf("nop essentially");
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }

        case 0x41: { //LD B,C
            crapstate.cpu.b = crapstate.cpu.c;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("loaded %x from b to c\n",crapstate.cpu.b);
            break;
        }

        case 0x42: { // LD B, D
            crapstate.cpu.b = crapstate.cpu.d;
            crapstate.cpu.pc++;
            printf("loaded %x from b to d\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x43: { // LD B, E
            crapstate.cpu.b = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from b to e\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x44: { // LD B, H
            crapstate.cpu.b = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from h to b\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }
        
        case 0x45: { // LD B, L
            crapstate.cpu.b = crapstate.cpu.l;
            crapstate.cpu.pc++;
            printf("loaded %x from l to b\n", crapstate.cpu.b);
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

        case 0x48: { // LD C, B
            crapstate.cpu.c = crapstate.cpu.b;
            crapstate.cpu.pc++;
            printf("loaded %x from b to c\n", crapstate.cpu.c);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x49:  { // LD C, C
            printf("nop essentially");
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }

        case 0x4A:  { // LD C, D
            crapstate.cpu.c = crapstate.cpu.d;
            crapstate.cpu.pc++;
            printf("loaded %x from d to c\n", crapstate.cpu.c);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x4B:  { // LD C, E
            crapstate.cpu.c = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from e to c\n", crapstate.cpu.c);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x4C:  { //LD C,H
            crapstate.cpu.c = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from h to c\n", crapstate.cpu.c);
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

        case 0x50: { // LD D, B
            crapstate.cpu.d = crapstate.cpu.b;
            crapstate.cpu.pc++;
            printf("loaded %x from b to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x51: { // LD D, C
            crapstate.cpu.d = crapstate.cpu.c;
            crapstate.cpu.pc++;
            printf("loaded %x from c to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x52: { // LD D, D
            crapstate.cpu.pc++;
            printf("loaded %x from d to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x53: { // LD D, E
            crapstate.cpu.d = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from e to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }
  
        case 0x54: { // LD D, H
            crapstate.cpu.d = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from h to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x55: { // LD D, L
            crapstate.cpu.d = crapstate.cpu.l;
            crapstate.cpu.pc++;
            printf("loaded %x from l to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x56: { // LD D, [HL]
            crapstate.cpu.d = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.pc++;
            printf("loaded %x from byte from hl to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 8;
            break;
        }

        case 0x57: { // LD D, A
            crapstate.cpu.d = crapstate.cpu.a;
            crapstate.cpu.pc++;
            printf("loaded %x from a to d\n", crapstate.cpu.d);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x58: {// LD E, B
            crapstate.cpu.e = crapstate.cpu.b;
            crapstate.cpu.pc++;
            printf("loaded %x from b to e\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x59: { // LD E, C
            crapstate.cpu.e = crapstate.cpu.c;
            crapstate.cpu.pc++;
            printf("loaded %x from c to e\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x5A: { //LD E, D
            crapstate.cpu.e = crapstate.cpu.d;
            crapstate.cpu.pc++;
            printf("loaded %x from d to e\n", crapstate.cpu.b);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x5B: { //LD E, E
            
            crapstate.cpu.pc++;
            printf("loaded %x from e to e\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }

        
        case 0x5C: { //LD E, H
            crapstate.cpu.e = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from d to e\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x5D: { // LD E, L
            crapstate.cpu.e = crapstate.cpu.l;
            crapstate.cpu.pc++;
            printf("loaded %x from l to e\n", crapstate.cpu.e);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x5E: { // LD E, [HL]
            crapstate.cpu.e = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.pc++;
            printf("loaded %x from byte from hl to e\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 8;
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
            printf("loaded %x from b to h\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x61: { // LD H, C
            crapstate.cpu.h = crapstate.cpu.c;
            crapstate.cpu.pc++;
            printf("loaded %x from c to h\n", crapstate.cpu.h);
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

        case 0x63: { // LD H, E
            crapstate.cpu.h = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from e to h\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x64: { // LD H, H
            crapstate.cpu.pc++;
            printf("loaded %x from h to h\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x65: { // LD H, L
            crapstate.cpu.h = crapstate.cpu.l;
            crapstate.cpu.pc++;
            printf("loaded %x from l to h\n", crapstate.cpu.h);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x66:{ //LD H,[HL]
            crapstate.cpu.h = mem_read_byte(crapstate.cpu.hl);
            printf("read %x byte to h pointed by hl at addr hl %x\n", crapstate.cpu.h, crapstate.cpu.hl);
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

        case 0x68: { // LD L, B
            crapstate.cpu.l = crapstate.cpu.b;
            crapstate.cpu.pc++;
            printf("loaded %x from b to l\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x69: { // LD L, C
            crapstate.cpu.l = crapstate.cpu.c;
            crapstate.cpu.pc++;
            printf("loaded %x from c to l\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x6A: { // LD L, D
            crapstate.cpu.l = crapstate.cpu.d;
            crapstate.cpu.pc++;
            printf("loaded %x from d to l\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x6B: { // LD L, E
            crapstate.cpu.l = crapstate.cpu.e;
            crapstate.cpu.pc++;
            printf("loaded %x from e to l\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x6C: { // LD L, H
            crapstate.cpu.l = crapstate.cpu.h;
            crapstate.cpu.pc++;
            printf("loaded %x from h to l\n", crapstate.cpu.l);
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0x6D: { // LD L, L
            crapstate.cpu.pc++;
            printf("loaded %x from l to l\n", crapstate.cpu.l);
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
            printf("loaded b %x to memory addr %x from hl\n",crapstate.cpu.b,crapstate.cpu.hl);
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

        case 0x73: {//LD [HL], E
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.e);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded e %x to memory addr %x from hl\n",crapstate.cpu.e,crapstate.cpu.hl);
            break;
        }

        case 0x74: { // LD [HL], H
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.h);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded h %x to memory addr %x from hl\n",crapstate.cpu.h,crapstate.cpu.hl);
            break;  
        }

        case 0x75: { // LD [HL], L
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.l);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            printf("loaded l %x to memory addr %x from hl\n",crapstate.cpu.l,crapstate.cpu.hl);
            break;  
        }

        case 0x76: {// HALT
            crapstate.cpu.cycles = UINT16_MAX;
            crapstate.cpu.halted = 1;
            if((crapstate.io.ie&crapstate.io.ie & INTERRUPT_VBLANK) |(crapstate.io.ie&crapstate.io.ie & INTERRUPT_STAT)){
                crapstate.cpu.cycles = crapstate.ppu.mode_cycles; // i dunno if it needs full proper calculation of cycles until interrupt 
                                                                  // this will do for now
            }

            static const uint16_t tac_treshhold[4] = {1024, 16, 64, 256};
            if(crapstate.io.ie & crapstate.io.TAC & INTERRUPT_TIMER ){
                
                uint16_t tima_cycles = (256 - crapstate.io.TIMA) * tac_treshhold[ crapstate.io.TAC & 0x03];
                if((crapstate.cpu.cycles> tima_cycles) && tima_cycles!= 0){ // i have to deal with this some other way dont know how yet
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

        case 0x7F: {
            crapstate.cpu.pc++;
            printf("loaded %x from a to a\n", crapstate.cpu.a);
            crapstate.cpu.cycles += 4;
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

        case 0x81: { // ADD A, C
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.c;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.b & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added c %x to a %x res %x flags z%x h%x c%x\n", crapstate.cpu.c, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x82:  { //ADD A, D
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.d;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.d & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added l %x to a %x res %x flags z%x h%x c%x\n", crapstate.cpu.d, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x83:  { //ADD A, E
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.e;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.e & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added l %x to a %x res %x flags z%x h%x c%x\n", crapstate.cpu.e, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x84:  { //ADD A, H
            uint8_t old_a = crapstate.cpu.a;
            crapstate.cpu.a += crapstate.cpu.h;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.h & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("added l %x to a %x res %x flags z%x h%x c%x\n", crapstate.cpu.h, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
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

        case 0x86: { // ADD A,[HL] 
            uint8_t old_a = crapstate.cpu.a;
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.a += byte;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (byte & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 8;
            printf("added byte %x at addr %x to a %x res %x flags z%x h%x c%x\n", byte, crapstate.cpu.hl , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
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

        case 0x88: { //ADC A, B
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.b + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0);
            crapstate.cpu.N = 0;                   
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.b & 0xF) + crapstate.cpu.C) > 0xF; 
            crapstate.cpu.C = (result > 0xFF);      
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added b%x with carry %x  to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.b,crapstate.cpu.C  , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;

        }

        case 0x89:  { //ADC A, C
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.c + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                     
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.c & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added c%x with carry %x  to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.c,crapstate.cpu.C  , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x8A:  { //ADC A, D
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.d + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                     
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.d & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added d%x with carry %x  to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.d,crapstate.cpu.C , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x8B:  { //ADC A, E
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.e + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                     
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.e & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added e%x with carry %x to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.e,crapstate.cpu.C , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x8C:  { //ADC A, H
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.h + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                     
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.h & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added h%x with carry %x to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.h,crapstate.cpu.C  , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x8D:  { //ADC A, L
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.l + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                     
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.l & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added b%x with carry %x at addr %x to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.l,crapstate.cpu.C ,crapstate.cpu.hl , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x8E: { //ADC A,[HL]

            uint8_t old_a = crapstate.cpu.a;          
            uint8_t value = mem_read_byte(crapstate.cpu.hl);  
            uint16_t result = old_a + value + crapstate.cpu.C; 

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                      
            crapstate.cpu.H = ((old_a & 0xF) + (value & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 8;
            printf("added byte %x with carry %x at addr %x to a %x res %x flags z%x h%x c%x\n",value,crapstate.cpu.C ,crapstate.cpu.hl , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;

        }

        case 0x8F:  {  //ADC A,A
            uint8_t old_a = crapstate.cpu.a;          
            uint16_t result = old_a + crapstate.cpu.a + crapstate.cpu.C;  

            crapstate.cpu.a = (uint8_t)result;        
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                     
            crapstate.cpu.H = ((old_a & 0xF) + (crapstate.cpu.a & 0xF) + crapstate.cpu.C) > 0xF;  
            crapstate.cpu.C = (result > 0xFF);        
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 4;
            printf("added b%x with carry %x  to a %x res %x flags z%x h%x c%x\n",crapstate.cpu.a,crapstate.cpu.C , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }

        case 0x90: {//SUB A,B
            crapstate.cpu.C = crapstate.cpu.b > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.b & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.b;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a %x to b ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x91: {//SUB A, C
            crapstate.cpu.C = crapstate.cpu.c > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.c & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.c;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a %x to c ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x92:  { //SUB A, D
            crapstate.cpu.C = crapstate.cpu.d > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.d & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.d;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a %x to d ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x93:  { //SUB A, E
            crapstate.cpu.C = crapstate.cpu.e > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.e & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.e;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a %x to e ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x94:  { //SUB A, H
            crapstate.cpu.C = crapstate.cpu.h > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.h & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.h;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a %x to h ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x95:  { //SUB A, L
            crapstate.cpu.C = crapstate.cpu.l > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.l & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.l;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a %x to l ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x96: { //SUB A [HL]
            uint8_t subval = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.C = subval > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (subval & 0x0F);
            crapstate.cpu.a = crapstate.cpu.a - subval;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=8;
            crapstate.cpu.pc+=1;
            printf("sub a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,subval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0x97:  { //SUB A, A
            crapstate.cpu.C = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.a = 0;
            crapstate.cpu.Z = 1;
            crapstate.cpu.N = 1;
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("sub a a\n");
            break;
        }

        case 0x98:  { //SBC A, B
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.b - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.b & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc a  b%x, res %x c%x z%x h%x\n",crapstate.cpu.b,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x99:  { //SBC A, C
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.c - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.c & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc a  c%x, res %x c%x z%x h%x\n",crapstate.cpu.c,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x9A:  { //SBC A, D
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.d - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.d & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc a  d%x, res %x c%x z%x h%x\n",crapstate.cpu.d,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x9B:  { //SBC A, E
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.e - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.e & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc a  e%x, res %x c%x z%x h%x\n",crapstate.cpu.e,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x9C:  { //SBC A, H
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.h - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.h & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc a  h%x, res %x c%x z%x h%x\n",crapstate.cpu.h,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x9D:  { //SBC A, L
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.l - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.l & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc a  l%x, res %x c%x z%x h%x\n",crapstate.cpu.l,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x9E:  { //SBC A,[HL]
            uint8_t byte  = mem_read_byte(crapstate.cpu.hl);
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - byte - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (byte & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=8;
            printf("sbc a [hl]%x=%x, res %x c%x z%x h%x\n",crapstate.cpu.hl,byte,crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0x9F:  {
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - crapstate.cpu.a - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (crapstate.cpu.a & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=1;
            crapstate.cpu.cycles+=4;
            printf("sbc aa, res %x c%x z%x h%x\n",crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }

        case 0xA0:  { //ADD A, B
            crapstate.cpu.a &= crapstate.cpu.b;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and b res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xA1:  { //AND A, C
            crapstate.cpu.a &= crapstate.cpu.c;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and c res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xA2:  { //AND A, D
            crapstate.cpu.a &= crapstate.cpu.d;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and d res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xA3:  { //AND A, E
            crapstate.cpu.a &= crapstate.cpu.e;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and e res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xA4:  { //AND A, H
            crapstate.cpu.a &= crapstate.cpu.h;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and h res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xA5:  { //AND A, L
            crapstate.cpu.a &= crapstate.cpu.l;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and l res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }

        case 0xA6: { // AND A, [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.a &= byte;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            printf("anded a and byte [hl]%X res %x\n",byte, crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 8;
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

        case 0xA8:  { //XOR A, B
            crapstate.cpu.a ^= crapstate.cpu.b;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with b %x res%x \n",crapstate.cpu.b,crapstate.cpu.a);
            break;
        }

        case 0xA9: { // XOR A, C
            crapstate.cpu.a ^= crapstate.cpu.c;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with c %x res%x \n",crapstate.cpu.c,crapstate.cpu.a);
            break;
        }

        case 0xAA:  { //XOR A, D
            crapstate.cpu.a ^= crapstate.cpu.d;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with d %x res%x \n",crapstate.cpu.d,crapstate.cpu.a);
            break;
        }

        case 0xAB:  { //XOR A, E
            crapstate.cpu.a ^= crapstate.cpu.e;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with e %x res%x \n",crapstate.cpu.e,crapstate.cpu.a);
            break;
        }

        case 0xAC:  { //XOR A, H
            crapstate.cpu.a ^= crapstate.cpu.h;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with c %x res%x \n",crapstate.cpu.h,crapstate.cpu.a);
            break;
        }

        case 0xAD: { // XOR A, L
            crapstate.cpu.a ^= crapstate.cpu.l;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with l %x res%x \n",crapstate.cpu.l,crapstate.cpu.a);
            break;
        }        

        case 0xAE : { //XOR A, [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.a ^= byte;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.C = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("xored a  with byte at hl %x res%x \n",byte ,crapstate.cpu.a);
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

        case 0xB0: { // OR A, B
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.b;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and b result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB1: { // OR A, C
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.c;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and c result %x\n", crapstate.cpu.a);
            break;
        }
        
        case 0xB2:  { //OR A, D
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.d;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and d result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB3:  {//OR A, E
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.e;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and e result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB4:  {//OR A, H
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.h;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and h result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB5:  {// OR A, L
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.l;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and l result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB6: { // OR A, n8
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.a = crapstate.cpu.a | byte;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 8;
            printf("ored a and byte [hl] %X result %x\n",byte, crapstate.cpu.a);
            break;
        }

        case 0xB7: {// OR A,A
            crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.a;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            printf("ored a and a result %x\n", crapstate.cpu.a);
            break;
        }

        case 0xB8:  {// CP A, B
            uint8_t res = crapstate.cpu.a - crapstate.cpu.b;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = crapstate.cpu.b > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.b & 0x0F);
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("cp a b %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.b,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xB9:  { //CP A, C
            uint8_t res = crapstate.cpu.a - crapstate.cpu.c;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = crapstate.cpu.c > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.c & 0x0F);
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("cp a c %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.c,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xBA:  { //CP A, D
            uint8_t res = crapstate.cpu.a - crapstate.cpu.d;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = crapstate.cpu.d > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.d & 0x0F);
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("cp a hl %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.d,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xBB:  {// CP A, E
            uint8_t res = crapstate.cpu.a - crapstate.cpu.e;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = crapstate.cpu.e > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.e & 0x0F);
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("cp a e %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.e,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xBC:  {//CP A, H
            uint8_t res = crapstate.cpu.a - crapstate.cpu.h;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = crapstate.cpu.h > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.h & 0x0F);
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("cp a h %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.h,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xBD:  {// CP A, L
            uint8_t res = crapstate.cpu.a - crapstate.cpu.e;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = crapstate.cpu.e > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.e & 0x0F);
            crapstate.cpu.cycles+=4;
            crapstate.cpu.pc+=1;
            printf("cp a e %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,crapstate.cpu.e,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xBE:  { // CP A, [HL]
            uint8_t cpval = mem_read_byte(crapstate.cpu.hl);
            uint8_t res = crapstate.cpu.a - cpval;
            crapstate.cpu.Z = !res;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = cpval > crapstate.cpu.a;
            crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (cpval & 0x0F);
            crapstate.cpu.cycles+=8;
            crapstate.cpu.pc+=1;
            printf("cp a hl %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,cpval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }

        case 0xBF:  {// CP A, A
            crapstate.cpu.Z = 1;
            crapstate.cpu.N = 1;
            crapstate.cpu.C = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            printf("cp a ,a");
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

        case 0xC4: { // CALL NZ
            if (!crapstate.cpu.Z) {
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) >> 8); // High byte
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) & 0xFF); // Low byte
                printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                printf("calling to %x because NZ\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
            break;
        }

        case 0xC5: { // PUSH BC
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.bc) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.bc) & 0xFF); // Low byte
            printf("pushed bc:%x%x\n", (crapstate.cpu.bc) >> 8, crapstate.cpu.bc & 0xFF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xC6: { // ADD A, n8
            uint8_t old_a = crapstate.cpu.a;
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.a += byte;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = ((old_a & 0x0F) + (byte & 0x0F)) > 0x0F;
            crapstate.cpu.C = old_a > crapstate.cpu.a;
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles += 8;
            printf("added byte %x to a %x res %x flags z%x h%x c%x\n", byte, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
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

        case 0xCA:  { //JP Z
            if(crapstate.cpu.Z){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                printf("Z flag set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=16;
            }else{
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles+=12;
            }
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

        case 0xCE: { //ADC A,n8

            uint8_t old_a = crapstate.cpu.a;          // 0x00
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            uint16_t result = old_a + byte + crapstate.cpu.C;  // 0x00 + 0xFF + 1 = 0x0100

            crapstate.cpu.a = (uint8_t)result;        // 0x00
            crapstate.cpu.Z = (crapstate.cpu.a == 0); // 1 (since A=0x00)
            crapstate.cpu.N = 0;                      // Always 0 for ADC
            crapstate.cpu.H = ((old_a & 0xF) + (byte & 0xF) + crapstate.cpu.C) > 0xF;  // 1
            crapstate.cpu.C = (result > 0xFF);        // 1
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles += 8;
            printf("added byte %x with carry  %x to a %x res %x flags z%x h%x c%x\n",byte,crapstate.cpu.C , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
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

        case 0xD2:{
            //JP NC
            if(!crapstate.cpu.C){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                printf("c flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=16;
            }else{
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles+=12;
            }
            break;
        }

        case 0xD4: { // CALL NC
            if (!crapstate.cpu.C) {
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) >> 8); // High byte
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) & 0xFF); // Low byte
                printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                printf("calling to %x because NC\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
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

        case 0xD8: { // RET C
            if (crapstate.cpu.C) {
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

        case 0xD9:  { // RETI
            uint8_t low = mem_read_byte(crapstate.cpu.sp++);
            uint8_t high = mem_read_byte(crapstate.cpu.sp++);
            crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
            printf("returning to %x and enabling interrupts\n", crapstate.cpu.pc);
            crapstate.cpu.ime = 1;
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xDA:  {
            //JP C
            if(crapstate.cpu.C){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                printf("C flag set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=16;
            }else{
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles+=12;
            }
            break;
        }

        case 0xDC:  {//CALL C
            if (crapstate.cpu.C) {
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) >> 8); // High byte
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) & 0xFF); // Low byte
                printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                printf("calling to %x because C\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
            break;
        }

        case 0xDF:  {//RST 18H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x18;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xDE:  { //SBC A, n8
            uint8_t value  = mem_read_byte(crapstate.cpu.pc+1);
            uint8_t borrow = !crapstate.cpu.C; 
            uint16_t result  = crapstate.cpu.a - value - borrow;
            crapstate.cpu.C = result > 0xFF;
            crapstate.cpu.H = ((crapstate.cpu.a & 0x0F) - (value & 0x0F) - borrow) < 0;
            crapstate.cpu.a = (uint8_t) result;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=2;
            crapstate.cpu.cycles+=8;
            printf("sbc a n8, res %x c%x z%x h%x\n",crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
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
            crapstate.cpu.cycles += 8;
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

        case 0xE7:  {// RST 20H
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF); // Low byte
            printf("stored %x%x\n", (crapstate.cpu.pc + 1) >> 8, (crapstate.cpu.pc + 1) & 0xFF);
            crapstate.cpu.pc = 0x20;
            printf("calling to vector %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }

        case 0xE8:  {//ADD SP, s8
		    //Sameboy code MIT licence
            int8_t offset = (int8_t) mem_read_byte( crapstate.cpu.pc+1);
		    crapstate.cpu.Z = 0;
            crapstate.cpu.N = 0;
		    crapstate.cpu.H = ((crapstate.cpu.sp & 0xF) + (offset & 0xF) > 0xF) ? 1 : 0;
		    crapstate.cpu.C = ((crapstate.cpu.sp & 0xFF) + (offset & 0xFF) > 0xFF) ? 1 : 0;
		    crapstate.cpu.sp += offset;
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=12;
            printf("added sp %x+offset %x (%x) ,H%x, C%x\n",crapstate.cpu.sp,offset,crapstate.cpu.sp,crapstate.cpu.H,crapstate.cpu.C);
            
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

        case 0xEE:  { //XOR A, n8
            uint8_t byte  = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.a ^= byte;
            crapstate.cpu.Z = !crapstate.cpu.a; 
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.Z = 0;
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=8;
            printf("xored a with %x res %x\n",byte,crapstate.cpu.a);
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
            crapstate.cpu.af = (uint16_t)(low & 0xf0) | ((uint16_t)high << 8);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=12;
            printf("popped af = %x\n",crapstate.cpu.af);
            break;
        }

        case 0xF2: { //LDH A, [C]
            uint16_t addr = 0xFF00 + crapstate.cpu.c;
            crapstate.cpu.a = mem_read_byte(addr);
            printf("wrote a = %x from mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 8;
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

        case 0xF6: { // OR A, n8
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.a = crapstate.cpu.a | byte;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles += 8;
            printf("ored a and byte %X result %x\n",byte, crapstate.cpu.a);
            break;
        }

        case 0xF7:  { // RST 30H
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

        case 0xF8: { // LD HL, SP+e8
          // Sameboy code MIT licence
          int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc + 1);
          crapstate.cpu.hl = crapstate.cpu.sp + offset;
          crapstate.cpu.Z = 0;
          crapstate.cpu.N = 0;
          crapstate.cpu.H =
              ((crapstate.cpu.sp & 0xF) + (offset & 0xF) > 0xF) ? 1 : 0;
          crapstate.cpu.C =
              ((crapstate.cpu.sp & 0xFF) + (offset & 0xFF) > 0xFF) ? 1 : 0;
          crapstate.cpu.pc += 2;
          crapstate.cpu.cycles += 12;
          printf("loaded sp %x+offset %x (%x) to hl,H%x, C%x\n",
                 crapstate.cpu.sp, offset, crapstate.cpu.hl, crapstate.cpu.H,
                 crapstate.cpu.C);

          break;
        }

        case 0xF9: { //LD SP,HL
            crapstate.cpu.sp = crapstate.cpu.hl;
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=8;
            printf("loaded hl to sp %x\n",crapstate.cpu.sp);
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
            FILE* sav = fopen("sav.bin","wb");
            fwrite(&crapstate, sizeof(crapstate), 1, sav);
            fclose(sav);
            while(1);
            break;
        }
    }   
}

static inline void handle_interrupt() {
    if ((crapstate.cpu.ime |crapstate.cpu.halted) && (crapstate.io.if_reg & crapstate.io.ie & 0x1F)) {
        if(crapstate.cpu.halted && crapstate.cpu.ime ==0 ){
            crapstate.cpu.pc ++;
            crapstate.cpu.halted=0;
            return;
        }
        crapstate.cpu.halted = 0;
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

    uint8_t op =mem_read_byte(crapstate.cpu.pc);
    printf("OP:%X PC:%X ",op,crapstate.cpu.pc); 
    opcodes(mem_read_byte(crapstate.cpu.pc));

}
void cpu_start(){
      
}