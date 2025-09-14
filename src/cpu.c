/*
 *Three parts of this code are taken from Sameboy emulator by Lior Halphon https://github.com/LIJI32/SameBoy
 *
 *
 * Expat License
 *
 * Copyright (c) 2015-2025 Lior Halphon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * Two snippets of this code is taken from PeanutGB https://github.com/deltabeard/Peanut-GB/ which is distrubuted with the
 * licence provided under 
 *
 * MIT License
 *
 * Copyright (c) 2018-2023 Mahyar Koshkouei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 * Everything else you can use however you see fit
 */

#include "cpu.h"
#include "defines.h"
#include "mem.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include "crapstate.h"
#include "cpu_instr.h"

static inline void opcodes_cb(uint8_t postfix){
    switch (postfix) {
        case 0x00:  { //RLC B
            RLC_R8(b);
        }
        
        case 0x01: { //RLC C
            RLC_R8(c);
        }
        
        case 0x02: { //RLC D
            RLC_R8(d);
        }
        
        case 0x03:  { //RLC E
            RLC_R8(e);
        }
        
        case 0x04:  { //RLC H
            RLC_R8(h);
        }
        
        case 0x05: { //RLC L
            RLC_R8(l);
        }
        
        case 0x06:  { //RLC [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            byte = (byte << 1) | (byte >> 7);
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.f = 0;
            crapstate.cpu.C = byte & 1;
            crapstate.cpu.Z = !byte;

            crapstate.cpu.cycles+=8;
            //printf("rotate byte at hl%x left \n %x",byte, crapstate.cpu.C);
            break;
        }
        
        case 0x07:  { //RLC A
            RLC_R8(a);
        }
        
        case 0x08:  { //RRC B
            RRC_R8(b);
        }
        
        case 0x09:  { //RRC C
            RRC_R8(c);
        }
        
        case 0x0A:  { //RRC D
            RRC_R8(d);
        }
        
        case 0x0B:  {  //RRC E
            RRC_R8(e);
        }
        
        case 0x0C:  { //RRC H
            RRC_R8(h);
        }
        
        case 0x0D:  { //RRC L
            RRC_R8(l);
        }
        
        case 0x0E:  { //RRC [HL] 
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.f = 0;
            crapstate.cpu.C = byte & 0x1;
            byte = (byte >>1) | (byte << 7 );
            crapstate.cpu.Z = !byte;  
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.cycles+=8;
            //printf("rotate byte at hl %x right  %x,c %x\n",crapstate.cpu.hl, byte, crapstate.cpu.C);
            break;
        }
        
        case 0x0F:  { //RRC A
            RRC_R8(a);
        }
        
        case 0x10:  { //RL B
            RL_R8(b);
        }
        
        case 0x11:  { //RL C
            RL_R8(c);
        }
        
        case 0x12:  { //RL D
            RL_R8(d);
        }
        
        case 0x13:  { //RL E
            RL_R8(e);
        }
        
        case 0x14:   { //RL H
            RL_R8(h);
        }
        
        case 0x15:  { //RL L
            RL_R8(l);
        }
        
        case 0x16:  { //RL [HL]
            uint8_t old_carry = crapstate.cpu.C;
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.f = 0;
            crapstate.cpu.C = (byte) & 0x80 ? 1 : 0 ;
            byte <<= 1;
            byte |= old_carry;
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.Z = !byte;
            crapstate.cpu.cycles+=8;
            //printf("rotated b through carry res %x c %x",byte, crapstate.cpu.C);
            break;
        }
        
        case 0x17:  { //RL A
            RL_R8(a);
        }
        
        case 0x18:  { //RR B
            RR_R8(b);
        }
        
        case 0x19:  {//RR C
            RR_R8(c);
        }
        
        case 0x1A:  {//RR D
            RR_R8(d);
        }
        
        case 0x1B:  { //RR E
            RR_R8(e);
        }
        
        case 0x1C:  { //RR H
            RR_R8(h);
        }
        
        case 0x1D:  { //RR L
            RR_R8(l);
        }
        
        case 0x1E:  { //RR [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.f = 0;
            crapstate.cpu.C = byte & 1;
            byte  >>=1; 
            byte |= (old_carry<<7);
            crapstate.cpu.Z = !byte;
            mem_write_byte(crapstate.cpu.hl, byte);
            //printf("rotated byte %x at addr %x right through carry  z %x c %x\n",byte,crapstate.cpu.hl,crapstate.cpu.Z,crapstate.cpu.C);
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x1F:  { //RR A
            RR_R8(a);
        }
        
        case 0x20:  { //SLA B
            SLA_R8(b);
        }
        
        case 0x21:  { //SLA C
            SLA_R8(c);
        }
        
        case 0x22:  { //SLA D
            SLA_R8(d);
        }
        
        case 0x23:  { //SLA E
            SLA_R8(e);
        }
        
        case 0x24:  { //SLA H
            SLA_R8(h);
        }
        
        case 0x25:  { //SLA L
            SLA_R8(l);
        }
        
        case 0x26:  { //SLA [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.f = 0;                           
            crapstate.cpu.C = (byte & 0x80) != 0;                       
            byte <<= 1;                                                 
            mem_write_byte(crapstate.cpu.hl, byte);                                    
            crapstate.cpu.Z = !byte;
            crapstate.cpu.cycles+=8;                            
            break;
        }
        
        case 0x27:  { //SLA A
            SLA_R8(a);
        }
        
        case 0x28:  { //SRA B
            SRA_R8(b);
        }
        
        case 0x29:  { //SRA C
            SRA_R8(c); 
        }
        
        case 0x2A:  { //SRA D
            SRA_R8(d);
        }
        
        case 0x2B:  { //SRA E
            SRA_R8(e);
        }
        
        case 0x2C:  { //SRA H
            SRA_R8(h);
        }
        
        case 0x2D:  { //SRA L
            SRA_R8(l);
        }
        
        case 0x2E:  { //SRA [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.f = 0;
            crapstate.cpu.C = byte & 0x1;                               
            byte = (byte>> 1) | (byte & 0x80);               
            crapstate.cpu.Z = !byte;   
            mem_write_byte(crapstate.cpu.hl, byte);                                    
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x2F:  { //SRA A
            SRA_R8(a);
        }
        
        case 0x30:  { //SWAP B
            SWAP_R8(b);
        }
        
        case 0x31:  { //SWAP C
            SWAP_R8(c);
        }
        
        case 0x32:  { //SWAP D
            SWAP_R8(d);
        }
        
        case 0x33:  { //SWAP E
            SWAP_R8(e);
        }
        
        case 0x34:  { //SWAP H
            SWAP_R8(h);
        }
        
        case 0x35:  { //SWAP L
            SWAP_R8(l);
        }
        
        case 0x36:  { //SWAP [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            byte = (byte << 4) | (byte >> 4);             
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = !byte;                                    
            mem_write_byte(crapstate.cpu.hl,byte);
                                                       
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x37:{ //SWAP A
            SWAP_R8(a);
        }
        
        case 0x38:  { //SRL B
            SRL_R8(b);
        }
        
        case 0x39:  { //SRL C
            SRL_R8(c);
        }
        
        case 0x3A:  { //SRL D
            SRL_R8(d);
        }
        
        case 0x3B:  { //SRL E
            SRL_R8(e);
        }
        
        case 0x3C:  { //SRL H
            SRL_R8(h);
        }
        
        case 0x3D:  { //SRL L
            SRL_R8(l);
        }
        
        case 0x3E:  { //SRL [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.f = 0;
            crapstate.cpu.C = (byte & 0x01);
            byte >>= 1;                                                 
            crapstate.cpu.Z = !byte;
            mem_write_byte(crapstate.cpu.hl, byte);                                    
            crapstate.cpu.cycles+=8;                                                         
            break;
        }
        
        case 0x3F:  { //SRL A
            SRL_R8(a);
        }
        
        case 0x40:  { //BIT 0 B
            BIT_N_R8(0, b);
        }
        
        case 0x41:  { //BIT 0 C
            BIT_N_R8(0, c);
        }
        
        case 0x42:  { //BIT 0 D
            BIT_N_R8(0, d);
        }
        
        case 0x43:  { //BIT 0 E
            BIT_N_R8(0, e);
        }
        
        case 0x44:  { //BIT 0 H
            BIT_N_R8(0, h);
        }
        
        case 0x45:  { //BIT 0 L
            BIT_N_R8(0, l);
        }
        
        case 0x46:  { //BIT [HL]
            BIT_N_AT_HL(0);
        }
        
        case 0x47:{// BIT 0 A
            BIT_N_R8(0, a);         
        }
        
        case 0x48:  {//BIT 1 B
            BIT_N_R8(1, b);
        }
        
        case 0x49:  { //BIT 1 C
            BIT_N_R8(1, c);
        }
        
        case 0x4A:  {//BIT 1 D
            BIT_N_R8(1, d);
        }
        
        case 0x4B:  {//BIT 1 E
            BIT_N_R8(1, e);
        }
        
        case 0x4C:  {//BIT 1 H
            BIT_N_R8(1, h);
        }
        
        case 0x4D:  {//BIT 1 L
            BIT_N_R8(1, l);
        }
        
        case 0x4E:  {// BIT 1 [HL] 
            BIT_N_AT_HL(1);
        }
        
        case 0x4F:  {// BIT 1 A
            BIT_N_R8(1, a);
        }
        
        case 0x50:  {// BIT 2 B
            BIT_N_R8(2, b);
        }
        
        case 0x51:  {// BIT 2 C
            BIT_N_R8(2, c);
        }
        
        case 0x52:  {// BIT 2 D
            BIT_N_R8(2, d);
        }
        
        case 0x53:  {// BIT 2 E
            BIT_N_R8(2, e);
        }
        
        case 0x54:  {// BIT 2 H
            BIT_N_R8(2, h);
        }
        
        case 0x55:  {// BIT 2 L
            BIT_N_R8(2, l);
        }
        
        case 0x56:  {// BIT 2 [HL]
            BIT_N_AT_HL(2);
        }
        
        case 0x57:  {// BIT 2 A
            BIT_N_R8(2, a);
        }
        
        case 0x58:  {//BIT 3 B
            BIT_N_R8(3, b);
        }
        
        case 0x59:  { //BIT 3 C
            BIT_N_R8(3, c);
        }
        
        case 0x5A:  {//BIT 3 D
            BIT_N_R8(3, d);
        }
        
        case 0x5B:  {//BIT 3 E
            BIT_N_R8(3, e);
        }
        
        case 0x5C:  {//BIT 3 H
            BIT_N_R8(3, h);
        }
        
        case 0x5D:  {//BIT 3 L
            BIT_N_R8(3, l);
        }
        
        case 0x5E:  {// BIT 3 [HL] 
            BIT_N_AT_HL(3);
        }
        
        case 0x5F:  { //BIT 3 A
            BIT_N_R8(3, a);
        }
        
        case 0x60:  {// BIT 4 B
            BIT_N_R8(4, b);
        }
        
        case 0x61:  {// BIT 4 C
            BIT_N_R8(4, c);
        }
        
        case 0x62:  {// BIT 4 D
            BIT_N_R8(4, d);
        }
        
        case 0x63:  {// BIT 4 E
            BIT_N_R8(4, e);
        }
        
        case 0x64:  {// BIT 4 H
            BIT_N_R8(4, h);
        }
        
        case 0x65:  {// BIT 4 L
            BIT_N_R8(4, l);
        }
        
        case 0x66:  {// BIT 4 [HL]
            BIT_N_AT_HL(4);
        }
        
        case 0x67:  {// BIT 4 A
            BIT_N_R8(4, a);
        }
        
        case 0x68:  {//BIT 5 B
            BIT_N_R8(5, b);
        }
        
        case 0x69:  { //BIT 5 C
            BIT_N_R8(5, c);
        }
        
        case 0x6A:  {//BIT 5 D
            BIT_N_R8(5, d);
        }
        
        case 0x6B:  {//BIT 5 E
            BIT_N_R8(5, e);
        }
        
        case 0x6C:  {//BIT 5 H
            BIT_N_R8(5, h);
        }
        
        case 0x6D:  {//BIT 5 L
            BIT_N_R8(5, l);
        }
        
        case 0x6E:  {// BIT 5 [HL] 
            BIT_N_AT_HL(5);
        }
        
        case 0x6F:  { //BIT 5 A
            BIT_N_R8(5, a);
        }
        
        case 0x70:  {// BIT 6 B
            BIT_N_R8(6, b);
        }
        
        case 0x71:  {// BIT 6 C
            BIT_N_R8(6, c);
        }
        
        case 0x72:  {// BIT 6 D
            BIT_N_R8(6, d);
        }
        
        case 0x73:  {// BIT 6 E
            BIT_N_R8(6, e);
        }
        
        case 0x74:  {// BIT 6 H
            BIT_N_R8(6, h);
        }
        
        case 0x75:  {// BIT 6 L
            BIT_N_R8(6, l);
        }
        
        case 0x76:  {// BIT 6 [HL]
            BIT_N_AT_HL(6);
        }
        
        case 0x77:  {// BIT 6 A
            BIT_N_R8(6, a);
        }
        
        
        case 0x78:  {//BIT 7 B
            BIT_N_R8(7, b);
        }
        
        case 0x79:  { //BIT 7 C
            BIT_N_R8(7, c);
        }
        
        case 0x7A:  {//BIT 7 D
            BIT_N_R8(7, d);
        }
        
        case 0x7B:  {//BIT 7 E
            BIT_N_R8(7, e);
        }
        
        case 0x7C:  {//BIT 7 H
            BIT_N_R8(7, h);
        }
        
        case 0x7D:  {//BIT 7 L
            BIT_N_R8(7, l);
        }
        
        case 0x7E:  {// BIT 7 [HL] 
            BIT_N_AT_HL(7);
        }
        
        case 0x7F:  { //BIT 7 A
            BIT_N_R8(7, a);
        }
        
        case 0x80:  { //RES 0 B
            RES_N_R8(0,b);
        }
        
        case 0x81:  { //RES 0 C
            RES_N_R8(0,c);
        }
        
        case 0x82:  { //RES 0 D
            RES_N_R8(0,d);
        }
        
        case 0x83:  { //RES 0 E
            RES_N_R8(0,e);
        }
        
        case 0x84:  { //RES 0 H
            RES_N_R8(0,h);
        }
        
        case 0x85:  { //RES 0 L
            RES_N_R8(0,l);
        }
        
        case 0x86:  { //RES 0 [HL]
            RES_N_AT_HL(0);
        }
        
        case 0x87:  { //RES 0 A
            RES_N_R8(0, a);
        }
        
        case 0x88:  {//RES 1 B
            RES_N_R8(1, b);
        }
        
        case 0x89:  { //RES 1 C
            RES_N_R8(1, c);
        }
        
        case 0x8A:  {//RES 1 D
            RES_N_R8(1, d);
        }
        
        case 0x8B:  {//RES 1 E
            RES_N_R8(1, e);
        }
        
        case 0x8C:  {//RES 1 H
            RES_N_R8(1, h);
        }
        
        case 0x8D:  {//RES 1 L
            RES_N_R8(1, l);
        }
        
        case 0x8E:  {// RES 1 [HL] 
            RES_N_AT_HL(1);
        }
        
        case 0x8F:  { //RES 1 A
            RES_N_R8(1, a);
        }
        
        case 0x90:  { //RES 2 B
            RES_N_R8(2,b);
        }
        
        case 0x91:  { //RES 2 C
            RES_N_R8(2,c);
        }
        
        case 0x92:  { //RES 2 D
            RES_N_R8(2,d);
        }
        
        case 0x93:  { //RES 2 E
            RES_N_R8(2,e);
        }
        
        case 0x94:  { //RES 2 H
            RES_N_R8(2,h);
        }
        
        case 0x95:  { //RES 2 L
            RES_N_R8(2,l);
        }
        
        case 0x96:  { //RES 2 [HL]
            RES_N_AT_HL(2);
        }
        
        case 0x97:  { //RES 2 A
            RES_N_R8(2, a);
        }
        
        case 0x98:  {//RES 3 B
            RES_N_R8(3, b);
        }
        
        case 0x99:  { //RES 3 C
            RES_N_R8(3, c);
        }
        
        case 0x9A:  {//RES 3 D
            RES_N_R8(3, d);
        }
        
        case 0x9B:  {//RES 3 E
            RES_N_R8(3, e);
        }
        
        case 0x9C:  {//RES 3 H
            RES_N_R8(3, h);
        }
        
        case 0x9D:  {//RES 3 L
            RES_N_R8(3, l);
        }
        
        case 0x9E:  {// RES 3 [HL] 
            RES_N_AT_HL(3);
        }
        
        case 0x9F:  { //RES 3 A
            RES_N_R8(3, a);
        }  
        
        case 0xA0:  { //RES 4 B
            RES_N_R8(4,b);
        }
        
        case 0xA1:  { //RES 4 C
            RES_N_R8(4,c);
        }
        
        case 0xA2:  { //RES 4 D
            RES_N_R8(4,d);
        }
        
        case 0xA3:  { //RES 4 E
            RES_N_R8(4,e);
        }
        
        case 0xA4:  { //RES 4 H
            RES_N_R8(4,h);
        }
        
        case 0xA5:  { //RES 4 L
            RES_N_R8(4,l);
        }
        
        case 0xA6:  { //RES 4 [HL]
            RES_N_AT_HL(4);
        }
        
        case 0xA7:  { //RES 4 A
            RES_N_R8(4, a);
        }
        
        case 0xA8:  {//RES 5 B
            RES_N_R8(5, b);
        }
        
        case 0xA9:  { //RES 5 C
            RES_N_R8(5, c);
        }
        
        case 0xAA:  {//RES 5 D
            RES_N_R8(5, d);
        }
        
        case 0xAB:  {//RES 5 E
            RES_N_R8(5, e);
        }
        
        case 0xAC:  {//RES 5 H
            RES_N_R8(5, h);
        }
        
        case 0xAD:  {//RES 5 L
            RES_N_R8(5, l);
        }
        
        case 0xAE:  {// RES 5 [HL] 
            RES_N_AT_HL(5);
        }
        
        case 0xAF:  { //RES 5 A
            RES_N_R8(5, a);
        }  
        
        case 0xB0:  { //RES 6 B
            RES_N_R8(6,b);
        }
        
        case 0xB1:  { //RES 6 C
            RES_N_R8(6,c);
        }
        
        case 0xB2:  { //RES 6 D
            RES_N_R8(6,d);
        }
        
        case 0xB3:  { //RES 6 E
            RES_N_R8(6,e);
        }
        
        case 0xB4:  { //RES 6 H
            RES_N_R8(6,h);
        }
        
        case 0xB5:  { //RES 6 L
            RES_N_R8(6,l);
        }
        
        case 0xB6:  { //RES 6 [HL]
            RES_N_AT_HL(6);
        }
        
        case 0xB7:  { //RES 6 A
            RES_N_R8(6, a);
        }
        
        case 0xB8:  {//RES 7 B
            RES_N_R8(7, b);
        }
        
        case 0xB9:  { //RES 7 C
            RES_N_R8(7, c);
        }
        
        case 0xBA:  {//RES 7 D
            RES_N_R8(7, d);
        }
        
        case 0xBB:  {//RES 7 E
            RES_N_R8(7, e);
        }
        
        case 0xBC:  {//RES 7 H
            RES_N_R8(7, h);
        }
        
        case 0xBD:  {//RES 7 L
            RES_N_R8(7, l);
        }
        
        case 0xBE:  {// RES 7 [HL] 
            RES_N_AT_HL(7);
        }
        
        case 0xBF:  { //RES 7 A
            RES_N_R8(7, a);
        }  
        
        case 0xC0:  { //SET 0 B
            SET_N_R8(0,b);
        }
        
        case 0xC1:  { //SET 0 C
            SET_N_R8(0,c);
        }
        
        case 0xC2:  { //SET 0 D
            SET_N_R8(0,d);
        }
        
        case 0xC3:  { //SET 0 E
            SET_N_R8(0,e);
        }
        
        case 0xC4:  { //SET 0 H
            SET_N_R8(0,h);
        }
        
        case 0xC5:  { //SET 0 L
            SET_N_R8(0,l);
        }
        
        case 0xC6:  { //SET 0 [HL]
            SET_N_AT_HL(0);
        }
        
        case 0xC7:  { //SET 0 A
            SET_N_R8(0, a);
        }
        
        case 0xC8:  {//SET 1 B
            SET_N_R8(1, b);
        }
        
        case 0xC9:  { //SET 1 C
            SET_N_R8(1, c);
        }
        
        case 0xCA:  {//SET 1 D
            SET_N_R8(1, d);
        }
        
        case 0xCB:  {//SET 1 E
            SET_N_R8(1, e);
        }
        
        case 0xCC:  {//SET 1 H
            SET_N_R8(1, h);
        }
        
        case 0xCD:  {//SET 1 L
            SET_N_R8(1, l);
        }
        
        case 0xCE:  {// SET 1 [HL] 
            SET_N_AT_HL(1);
        }
        
        case 0xCF:  { //SET 1 A
            SET_N_R8(1, a);
        }  
        
        case 0xD0:  { //SET 2 B
            SET_N_R8(2,b);
        }
        
        case 0xD1:  { //SET 2 C
            SET_N_R8(2,c);
        }
        
        case 0xD2:  { //SET 2 D
            SET_N_R8(2,d);
        }
        
        case 0xD3:  { //SET 2 E
            SET_N_R8(2,e);
        }
        
        case 0xD4:  { //SET 2 H
            SET_N_R8(2,h);
        }
        
        case 0xD5:  { //SET 2 L
            SET_N_R8(2,l);
        }
        
        case 0xD6:  { //SET 2 [HL]
            SET_N_AT_HL(2);
        }
        
        case 0xD7:  { //SET 2 A
            SET_N_R8(2, a);
        }
        
        case 0xD8:  {//SET 3 B
            SET_N_R8(3, b);
        }
        
        case 0xD9:  { //SET 3 C
            SET_N_R8(3, c);
        }
        
        case 0xDA:  {//SET 3 D
            SET_N_R8(3, d);
        }
        
        case 0xDB:  {//SET 3 E
            SET_N_R8(3, e);
        }
        
        case 0xDC:  {//SET 3 H
            SET_N_R8(3, h);
        }
        
        case 0xDD:  {//SET 3 L
            SET_N_R8(3, l);
        }
        
        case 0xDE:  {// SET 3 [HL] 
            SET_N_AT_HL(3);
        }
        
        case 0xDF:  { //SET 3 A
            SET_N_R8(3, a);
        }
        
        case 0xE0:  { //SET 4 B
            SET_N_R8(4,b);
        }
        
        case 0xE1:  { //SET 4 C
            SET_N_R8(4,c);
        }
        
        case 0xE2:  { //SET 4 D
            SET_N_R8(4,d);
        }
        
        case 0xE3:  { //SET 4 E
            SET_N_R8(4,e);
        }
        
        case 0xE4:  { //SET 4 H
            SET_N_R8(4,h);
        }
        
        case 0xE5:  { //SET 4 L
            SET_N_R8(4,l);
        }
        
        case 0xE6:  { //SET 4 [HL]
            SET_N_AT_HL(4);
        }
        
        case 0xE7:  { //SET 4 A
            SET_N_R8(4, a);
        }
        
        case 0xE8:  {//SET 5 B
            SET_N_R8(5, b);
        }
        
        case 0xE9:  { //SET 5 C
            SET_N_R8(5, c);
        }
        
        case 0xEA:  {//SET 5 D
            SET_N_R8(5, d);
        }
        
        case 0xEB:  {//SET 5 E
            SET_N_R8(5, e);
        }
        
        case 0xEC:  {//SET 5 H
            SET_N_R8(5, h);
        }
        
        case 0xED:  {//SET 5 L
            SET_N_R8(5, l);
        }
        
        case 0xEE:  {// SET 5 [HL] 
            SET_N_AT_HL(5);
        }
        
        case 0xEF:  { //SET 5 A
            SET_N_R8(5, a);
        }
        
        case 0xF0:  { //SET 6 B
            SET_N_R8(6,b);
        }
        
        case 0xF1:  { //SET 6 C
            SET_N_R8(6,c);
        }
        
        case 0xF2:  { //SET 6 D
            SET_N_R8(6,d);
        }
        
        case 0xF3:  { //SET 6 E
            SET_N_R8(6,e);
        }
        
        case 0xF4:  { //SET 6 H
            SET_N_R8(6,h);
        }
        
        case 0xF5:  { //SET 6 L
            SET_N_R8(6,l);
        }
        
        case 0xF6:  { //SET 6 [HL]
            SET_N_AT_HL(6);
        }
        
        case 0xF7:  { //SET 6 A
            SET_N_R8(6, a);
        }
        
        case 0xF8:  {//SET 7 B
            SET_N_R8(7, b);
        }
        
        case 0xF9:  { //SET 7 C
            SET_N_R8(7, c);
        }
        
        case 0xFA:  {//SET 7 D
            SET_N_R8(7, d);
        }
        
        case 0xFB:  {//SET 7 E
            SET_N_R8(7, e);
        }
        
        case 0xFC:  {//SET 7 H
            SET_N_R8(7, h);
        }
        
        case 0xFD:  {//SET 7 L
            SET_N_R8(7, l);
        }
        
        case 0xFE:  {// SET 7 [HL] 
            SET_N_AT_HL(7);
        }
        
        case 0xFF:  { //SET 7 A
            SET_N_R8(7, a);
        }
        
        default:{
            
            printf("unhandled cb postcode %x\n",postfix);
            while(1);
        }
    }
} 

static void opcodes(uint8_t opcode){
    switch (opcode) {
        case 0x00:{// NOP
            //printf("got nop\n");
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }
        case 0x01:{ //LD BC, n16
            LD_R16_N16(bc);
            
        }
        case 0x02: { // LD [BC], A
            mem_write_byte(crapstate.cpu.bc,crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            //printf("loaded a %x to memory addr %x from bc\n",crapstate.cpu.a,crapstate.cpu.bc);
            break;  
        }
        case 0x03:{ // INC BC
            INC_R16(bc);
        }
        
        case 0x04:  { //INC B
            INC_R8(b);
        }
        
        case 0x05:  { // DEC B
            DEC_R8(b);
        }
        
        case 0x06:  { //LD B, n8
            LD_R8_N8(b);
        }
        
        case 0x07: {  // RLCA
            uint8_t old_A = crapstate.cpu.a;
            crapstate.cpu.a = (old_A << 1) | (old_A >> 7);
            crapstate.cpu.C = crapstate.cpu.a & 1;
            crapstate.cpu.Z = 0;  // Unlike RLC A, Z is always 0 in RLCA!
            crapstate.cpu.H = 0;
            crapstate.cpu.N = 0;
            //printf("rotate a left a %x,c %x\n", crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            break;
        }
        
        case 0x08:  { // LD [n16], SP
            uint16_t addr = mem_read_word(crapstate.cpu.pc+1);
            mem_write_word(addr,crapstate.cpu.sp);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=20;
            //printf("loaded sp %x to %x\n",crapstate.cpu.sp,addr);
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
            //printf("added bc %x to hl %x res %x flags  h%x c%x\n", crapstate.cpu.bc,old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        
        case 0x0A: { // LD A, (BC)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.bc);
            //printf("read %x byte to a pointed by bc at addr %x\n", crapstate.cpu.a, crapstate.cpu.bc);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        case 0x0B:{ //DEC BC
            DEC_R16(bc);
        }
        
        case 0x0C:  { //INC C
            INC_R8(c);
        }
        
        case 0x0D:  { // DEC C
            DEC_R8(c);
        }
        
        case 0x0E: { // LD C, n8
            LD_R8_N8(c);
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
            //printf("rotated a right a %x, c%x \n",crapstate.cpu.a,crapstate.cpu.C);
            break;
        }
        
        case 0x11:{//LD DE, n16
            LD_R16_N16(de);
        }
        
        case 0x12:{ // LD [DE],a
            mem_write_byte(crapstate.cpu.de, crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            //printf("wrote %x from a to byte %x pointed by de\n",crapstate.cpu.a,crapstate.cpu.de );
            break;
        }
        
        case 0x13:{ //INC DE
            INC_R16(de);
        }
        
        case 0x14:  { //INC D
            INC_R8(d);
        }
        
        case 0x15:  { // DEC D
            DEC_R8(d);
        }
        
        case 0x16:  { // LD D, n8
            LD_R8_N8(d);
        }
        
        case 0x17:  { // RLA            
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.C = (crapstate.cpu.a) & 0x80 ? 1 : 0 ;
            crapstate.cpu.a <<= 1;
            crapstate.cpu.a |= old_carry;
            crapstate.cpu.H = 0;
            crapstate.cpu.Z = 0;
            crapstate.cpu.N = 0;
            //printf("rotated a through carry res %x c %x",crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=4;
            break;
        }
        
        case 0x18:{ // JR
            int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
            //printf("jumping relatively to %x\n",crapstate.cpu.pc);
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
            //printf("added de %x to hl %x res %x flags  h%x c%x\n", crapstate.cpu.de,old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        
        case 0x1A: { // LD A, (DE)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.de);
            //printf("read %x byte to a pointed by de at addr %x\n", crapstate.cpu.a, crapstate.cpu.de);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0x1B:{ //DEC DE
            DEC_R16(de);
        }
        
        case 0x1C:  { //INC E
            INC_R8(e);
        }
        
        case 0x1D:  { // DEC E
            DEC_R8(e);
        }
        
        case 0x1E:  { //LD E, n8
            LD_R8_N8(e);
        }
        
        case 0x1F: { //RRA
            uint8_t old_carry = crapstate.cpu.C;
            crapstate.cpu.f = 0;
            crapstate.cpu.C = (crapstate.cpu.a) & 0x1 ;
            crapstate.cpu.a >>= 1;
            crapstate.cpu.a |= (old_carry<<7);
            //printf("rotated a right through carry res %x c %x\n",crapstate.cpu.a, crapstate.cpu.C);
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=4;
            break;
        }
        
        case 0x20:{
            //jr_nz
            if(!crapstate.cpu.Z){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                //printf("Z flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=12;
            }else{
                crapstate.cpu.pc+=2;
                crapstate.cpu.cycles+=8;
            }
            break;
        }
        case 0x21:{ //LD HL, n16
            LD_R16_N16(hl);
        }
        
        case 0x22:{ //LD [HLI], A
            mem_write_byte(crapstate.cpu.hl, crapstate.cpu.a);
            crapstate.cpu.hl++;
            crapstate.cpu.pc++;
            //printf("loaded from a to hl %x and incremented %x\n",crapstate.cpu.a,crapstate.cpu.hl);
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x23:{ //INC HL
            INC_R16(hl);
        }
        
        case 0x24:{ //INC H
            INC_R8(h);
        }
        
        case 0x25:  { // DEC H
            DEC_R8(h);
        }
        
        case 0x26: { // LD H, n8
            LD_R8_N8(h);
        }
        
        case 0x27:  {  // DAA
            /* The following is from SameBoy. MIT License. */
            int16_t a = crapstate.cpu.a;
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
            //printf("daa a%x res %x \n",crapstate.cpu.a,a);
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
                //printf("Z flag set jumping to %x\n",crapstate.cpu.pc);
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
            //printf("added hl %x to hl res %x flags  h%x c%x\n", old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        
        case 0x2A: { // LD A, (HL+)
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.hl);
            //printf("read %x byte to a pointed by hl at addr and incremented hl %x\n", crapstate.cpu.a, crapstate.cpu.hl);
            crapstate.cpu.hl++;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0x2B:{ //DEC HL
            DEC_R16(hl);
        }
        
        case 0x2C:  { //INC L
            INC_R8(l);
        }
        
        case 0x2D:  { // DEC L
            DEC_R8(l);
        }
        
        case 0x2E: { //LD L,n8
            LD_R8_N8(l);
        }
        
        case 0x2F: {//CPL
            crapstate.cpu.a = ~crapstate.cpu.a;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            //printf("negated a %x\n",crapstate.cpu.a);
            break;
        }
        
        case 0x30:{
            //JR NC
            if(!crapstate.cpu.C){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                //printf("C flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=12;
            }else{
                crapstate.cpu.pc+=2;
                crapstate.cpu.cycles+=8;
            }
            break;
        }
        
        case 0x31: { // LD SP, n16
            LD_R16_N16(sp);
        }
        
        case 0x32:{// LD HLD, A
            mem_write_byte(crapstate.cpu.hl, crapstate.cpu.a);
            crapstate.cpu.hl--;
            crapstate.cpu.pc++;
            //printf("loaded from a to hl %x and decremented %x\n",crapstate.cpu.a,crapstate.cpu.hl);
            crapstate.cpu.cycles+=8;
            break;
        }
        
        case 0x33: {//INC SP
            INC_R16(sp);
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
            //printf("incremented a byte at hl %x res %x\n",crapstate.cpu.hl,result);
            
            break;
        }
        
        case 0x35: { // DEC [HL]
            uint8_t val = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.H = !(val & 0x0F);
            uint8_t result = val - 1;
            mem_write_byte(crapstate.cpu.hl, result);
            
            crapstate.cpu.Z = !result;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 12;
            //printf("dec a byte at hl %x res %x\n",crapstate.cpu.hl,result);
            
            break;
        }
        
        case 0x36: {// LD [HL],n8
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            mem_write_byte(crapstate.cpu.hl, byte);
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=12;
            //printf("wrote byte %x to addr%x\n",byte,crapstate.cpu.hl);
            break;
        }
        
        case 0x37:  { //SCF
            crapstate.cpu.C = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            //printf("set carry flag\n");
            break;
        }
        
        case 0x38:{ // JR C
            if(crapstate.cpu.C){
                int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc+1);
                crapstate.cpu.pc = crapstate.cpu.pc+2+offset;
                //printf("C flag set jumping to %x\n",crapstate.cpu.pc);
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
            //printf("added sp %x to hl %x res %x flags  h%x c%x\n", crapstate.cpu.sp,old_hl, crapstate.cpu.hl, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        
        case 0x3A: { // LD A, HLD
            crapstate.cpu.a = mem_read_byte(crapstate.cpu.hl);
            //printf("read %x byte to a pointed by hl at addr and decremented hl %x\n", crapstate.cpu.a, crapstate.cpu.hl);
            crapstate.cpu.hl--;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0x3B: { //DEC SP
            DEC_R16(sp);
        }
        
        case 0x3D:  { // DEC A
            DEC_R8(a);
        }
        
        case 0x3C:  { //INC A
            INC_R8(a);
        }
        
        case 0x3E: { // LD A, n8
            LD_R8_N8(a);
        }
        
        case 0x3F:  {//CCF
            crapstate.cpu.C = !crapstate.cpu.C;
            crapstate.cpu.N = 0;
            crapstate.cpu.H = 0;
            crapstate.cpu.pc++;
            //printf("inverted carry flag %x\n", crapstate.cpu.C);
            crapstate.cpu.cycles += 4;
            break;
        }
        
        case 0x40: { //LD B, B
            LD_R8_SAME();
        }
        
        case 0x41: { //LD B, C
            LD_R8_R8(b, c);
        }
        
        case 0x42: { // LD B, D
            LD_R8_R8(b, d);
        }
        
        case 0x43: { // LD B, E
            LD_R8_R8(b, e);
        }
        
        case 0x44: { // LD B, H
            LD_R8_R8(b, h);
        }
        
        case 0x45: { // LD B, L
            LD_R8_R8(b, l);
        }
        
        case 0x46: { // LD B, (HL)
            LD_R8_FROM_HL(b);
        }
        
        case 0x47: { // LD B, A
            LD_R8_R8(b, a);
        }
        
        case 0x48: { // LD C, B
            LD_R8_R8(c, b);
        }
        
        case 0x49:  { // LD C, C
            LD_R8_SAME();
        }
        
        case 0x4A:  { // LD C, D
            LD_R8_R8(c, d);
        }
        
        case 0x4B:  { // LD C, E
            LD_R8_R8(c, e);
        }
        
        case 0x4C:  { //LD C,H
            LD_R8_R8(c, h);
        }
        
        case 0x4D: { // LD C, L
            LD_R8_R8(c, l);
        }
        
        case 0x4E: { // LD C, [HL]
            LD_R8_FROM_HL(c);
        }
        
        case 0x4F: { // LD C, A
            LD_R8_R8(c, a);
        }
        
        case 0x50: { // LD D, B
            LD_R8_R8(d, b);
        }
        
        case 0x51: { // LD D, C
            LD_R8_R8(d, c);
        }
        
        case 0x52: { // LD D, D
            LD_R8_SAME();
        }
        
        case 0x53: { // LD D, E
            LD_R8_R8(d, e);
        }
        
        case 0x54: { // LD D, H
            LD_R8_R8(d, h);
        }
        
        case 0x55: { // LD D, L
            LD_R8_R8(d, l);
        }
        
        case 0x56: { // LD D, [HL]
            LD_R8_FROM_HL(d);
        }
        
        case 0x57: { // LD D, A
            LD_R8_R8(d, a);
        }
        
        case 0x58: {// LD E, B
            LD_R8_R8(e, b);
        }
        
        case 0x59: { // LD E, C
            LD_R8_R8(e, c);
        }
        
        case 0x5A: { //LD E, D
            LD_R8_R8(e, d);
        }
        
        case 0x5B: { //LD E, E
            LD_R8_SAME();
        }
        
        
        case 0x5C: { //LD E, H
            LD_R8_R8(e, h);
        }
        
        case 0x5D: { // LD E, L
            LD_R8_R8(e, l);
        }
        
        case 0x5E: { // LD E, [HL]
            LD_R8_FROM_HL(e);
        }
        
        case 0x5F: { // LD E, A
            LD_R8_R8(e, a);
        }
        
        case 0x60: { // LD H, B
            LD_R8_R8(h, b);
        }
        
        case 0x61: { // LD H, C
            LD_R8_R8(h, c);
        }
        
        
        case 0x62: { // LD H, D
            LD_R8_R8(h, d);
        }
        
        case 0x63: { // LD H, E
            LD_R8_R8(h, e);
        }
        
        case 0x64: { // LD H, H
            LD_R8_SAME();
        }
        
        case 0x65: { // LD H, L
            LD_R8_R8(h, l);
        }
        
        case 0x66:{ //LD H,[HL]
            LD_R8_FROM_HL(h);
        }
        
        case 0x67:{ //LD H, A
            LD_R8_R8(h, a);
        }
        
        case 0x68: { // LD L, B
            LD_R8_R8(l, b);
        }
        
        case 0x69: { // LD L, C
            LD_R8_R8(l, c);
        }
        
        case 0x6A: { // LD L, D
            LD_R8_R8(l, d);
        }
        
        case 0x6B: { // LD L, E
            LD_R8_R8(l, e);
        }
        
        case 0x6C: { // LD L, H
            LD_R8_R8(l, h);
        }
        
        case 0x6D: { // LD L, L
            LD_R8_SAME();
        }
        
        case 0x6E: { // LD L, [HL]
            LD_R8_FROM_HL(l);
        }
        
        case 0x6F: { // LD L, A
            LD_R8_R8(l, a);
        }
        
        case 0x70: { // LD [HL], B
            LD_TO_HL_R8(b);
        }
        
        case 0x71: { // LD [HL], C
            LD_TO_HL_R8(c);
        }
        
        case 0x72: { // LD[HL], D
            LD_TO_HL_R8(d);
        }
        
        case 0x73: {//LD [HL], E
            LD_TO_HL_R8(e);
        }
        
        case 0x74: { // LD [HL], H
            LD_TO_HL_R8(h);
        }
        
        case 0x75: { // LD [HL], L
            LD_TO_HL_R8(l);
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
            //printf("halted\n");
            break;
        }
        
        case 0x77:{ //LD [HL], A
            mem_write_byte(crapstate.cpu.hl,crapstate.cpu.a);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=8;
            //printf("loaded a %x to memory addr %x from hl\n",crapstate.cpu.a,crapstate.cpu.hl);
            break;
        }
        
        case 0x78: { // LD A, B
            LD_R8_R8(a, b);
        }
        
        case 0x79: { // LD A, C
            LD_R8_R8(a, c);
        }
        
        case 0x7A: { // LD A, D
            LD_R8_R8(a, d);
        }
        
        case 0x7B: { // LD A, E
            LD_R8_R8(a, e);
        }
        
        case 0x7C: { // LD A, H
            LD_R8_R8(a, h);
        }
        
        case 0x7D: { // LD A, L
            LD_R8_R8(a, l);
        }
        
        case 0x7E: { // LD A, [HL]
            LD_R8_FROM_HL(a);
        }
        
        case 0x7F: { // LD A, A
            LD_R8_SAME();
        }
        
        case 0x80: { // ADD A, B
            ADD_A_R8(b);
        }
        
        case 0x81: { // ADD A, C
            ADD_A_R8(c);
        }
        
        case 0x82:  { //ADD A, D
            ADD_A_R8(d);
        }
        
        case 0x83:  { //ADD A, E
            ADD_A_R8(e);
        }
        
        case 0x84:  { //ADD A, H
            ADD_A_R8(h);
        }
        
        case 0x85: { // ADD A, L
            ADD_A_R8(l);
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
            //printf("added byte %x at addr %x to a %x res %x flags z%x h%x c%x\n", byte, crapstate.cpu.hl , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        
        case 0x87: { // ADD A, A
            ADD_A_R8(a);
        }
        
        case 0x88: { //ADC A, B
            ADC_A_R8(b);
        }
        
        case 0x89:  { //ADC A, C
            ADC_A_R8(c);
        }
        
        case 0x8A:  { //ADC A, D
            ADC_A_R8(d);
        }
        
        case 0x8B:  { //ADC A, E
            ADC_A_R8(e);
        }
        
        case 0x8C:  { //ADC A, H
            ADC_A_R8(h);
        }
        
        case 0x8D:  { //ADC A, L
            ADC_A_R8(l);
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
            //printf("added byte %x with carry %x at addr %x to a %x res %x flags z%x h%x c%x\n",value,crapstate.cpu.C ,crapstate.cpu.hl , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
            
        }
        
        case 0x8F:  {  //ADC A,A
            ADC_A_R8(a);
        }
        
        case 0x90: { //SUB A,B
            SUB_A_R8(b);
        }
        
        case 0x91: { //SUB A, C
            SUB_A_R8(c);
        }
        
        case 0x92:  { //SUB A, D
            SUB_A_R8(d);
        }
        
        case 0x93:  { //SUB A, E
            SUB_A_R8(e);
        }
        
        case 0x94:  { //SUB A, H
            SUB_A_R8(h);
        }
        
        case 0x95:  { //SUB A, L
            SUB_A_R8(l);
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
            //printf("sub a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,subval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
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
            //printf("sub a a\n");
            break;
        }
        
        case 0x98:  { //SBC A, B
            SBC_A_R8(b);
        }
        
        case 0x99:  { //SBC A, C
            SBC_A_R8(c);
        }
        
        
        case 0x9A:  { //SBC A, D
            SBC_A_R8(d);
        }
        
        case 0x9B:  { //SBC A, E
            SBC_A_R8(e);
        }
        
        case 0x9C:  { //SBC A, H
            SBC_A_R8(h);
        }
        
        case 0x9D:  { //SBC A, L
            SBC_A_R8(l);
        }
        
        case 0x9E:  { //SBC A,[HL]
            /* PeanutGB code MIT licence*/
            uint8_t byte  = mem_read_byte(crapstate.cpu.hl);
            uint16_t temp =  crapstate.cpu.a - (byte + crapstate.cpu.C);          
            crapstate.cpu.C = (temp & 0xFF00) ? 1 : 0;                                   
            crapstate.cpu.H = ((crapstate.cpu.a ^ byte ^ temp) & 0x10) > 0;        
            crapstate.cpu.N = 1;                                                         
            crapstate.cpu.Z = ((temp & 0xFF) == 0x00);                                   
            crapstate.cpu.a = (temp & 0xFF);                                             
            crapstate.cpu.pc += 1;                                                       
            crapstate.cpu.cycles += 8;                                                   
            break;
        }
        
        case 0x9F:  {//SBC A, A 
            SBC_A_R8(a);
        }
        
        case 0xA0:  { //AND A, B
            AND_A_R8(b);
        }
        
        case 0xA1:  { //AND A, C
            AND_A_R8(c);
        }
        
        case 0xA2:  { //AND A, D
            AND_A_R8(d);
        }
        
        case 0xA3:  { //AND A, E
            AND_A_R8(e);
        }
        
        case 0xA4:  { //AND A, H
            AND_A_R8(h);
        }
        
        case 0xA5:  { //AND A, L
            AND_A_R8(l);
        }
        
        case 0xA6: { // AND A, [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.a &= byte;
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            //printf("anded a and byte [hl]%X res %x\n",byte, crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0xA7: { // AND A, A
            crapstate.cpu.H = 1;
            crapstate.cpu.N = 0;
            crapstate.cpu.C = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            //printf("anded a and a res %x\n", crapstate.cpu.a);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 4;
            break;
        }
        
        case 0xA8:  { //XOR A, B
            XOR_A_R8(b);
        }
        
        case 0xA9: { // XOR A, C
            XOR_A_R8(c);
        }
        
        case 0xAA:  { //XOR A, D
            XOR_A_R8(d);
        }
        
        case 0xAB:  { //XOR A, E
            XOR_A_R8(e);
        }
        
        case 0xAC:  { //XOR A, H
            XOR_A_R8(h);
        }
        
        case 0xAD: { // XOR A, L
            XOR_A_R8(l);
        }        
        
        case 0xAE : { //XOR A, [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.a ^= byte;
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;

            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 8;
            //printf("xored a  with byte at hl %x res%x \n",byte ,crapstate.cpu.a);
            break;
        }
        
        case 0xAF: { // XOR A
            crapstate.cpu.a = 0;
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            //printf("xored a\n");
            break;
        }
        
        case 0xB0: { // OR A, B
            OR_A_R8(b);
        }
        
        case 0xB1: { // OR A, C
            OR_A_R8(c);
        }
        
        case 0xB2:  { //OR A, D
            OR_A_R8(d);
        }
        
        case 0xB3:  {//OR A, E
            OR_A_R8(e);
        }
        
        case 0xB4:  {//OR A, H
            OR_A_R8(h);
        }
        
        case 0xB5:  {// OR A, L
            OR_A_R8(l);
        }
        
        case 0xB6: { // OR A, [HL]
            uint8_t byte = mem_read_byte(crapstate.cpu.hl);
            crapstate.cpu.a = crapstate.cpu.a | byte;
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles += 8;
            //printf("ored a and byte [hl] %X result %x\n",byte, crapstate.cpu.a);
            break;
        }
        
        case 0xB7: {// OR A,A
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            //printf("ored a and a result %x\n", crapstate.cpu.a);
            break;
        }
        
        case 0xB8:  {// CP A, B
            CP_A_R8(b);
        }
        
        case 0xB9:  { //CP A, C
            CP_A_R8(c);
        }
        
        case 0xBA:  { //CP A, D
            CP_A_R8(d);
        }
        
        case 0xBB:  {// CP A, E
            CP_A_R8(e);
        }
        
        case 0xBC:  {//CP A, H
            CP_A_R8(h);
        }
        
        case 0xBD:  {// CP A, L
            CP_A_R8(l);
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
            //printf("cp a hl %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,cpval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }
        
        case 0xBF:  {// CP A, A
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = 1;
            crapstate.cpu.N = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles+=4;
            //printf("cp a ,a\n");
            break;
        }
        
        case 0xC0: { // RET NZ
            if (!crapstate.cpu.Z) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                //printf("returning to %x\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 20;
            } else {
                crapstate.cpu.pc++;
                crapstate.cpu.cycles += 8;
            }
            break;
        }
        
        case 0xC1: { //POP BC
            POP_R16(bc);
        }
        
        case 0xC2:{
            //JP NZ
            if(!crapstate.cpu.Z){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                //printf("Z flag not set jumping to %x\n",crapstate.cpu.pc);
                crapstate.cpu.cycles+=16;
            }else{
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles+=12;
            }
            break;
        }
        
        case 0xC3: { // JP nn
            crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
            //printf("jump to %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xC4: { // CALL NZ
            if (!crapstate.cpu.Z) {
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) >> 8); // High byte
                crapstate.cpu.sp--;
                mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 3) & 0xFF); // Low byte
                //printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                //printf("calling to %x because NZ\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
            break;
        }
        
        case 0xC5: { // PUSH BC
            PUSH_R16(b, c);
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
            //printf("added byte %x to a %x res %x flags z%x h%x c%x\n", byte, old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
        }
        
        case 0xC7:{ // RST 00H
            RST(0x00);
        }
        
        case 0xC8: { // RET Z
            if (crapstate.cpu.Z) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                //printf("returning to %x\n", crapstate.cpu.pc);
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
            //printf("returning to %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xCA:  { //JP Z
            if(crapstate.cpu.Z){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                //printf("Z flag set jumping to %x\n",crapstate.cpu.pc);
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
                //printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                //printf("calling to %x because Z\n", crapstate.cpu.pc);
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
            //printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
            crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
            //printf("calling to %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 24;
            break;
        }
        
        case 0xCE: { //ADC A,n8
            
            uint8_t old_a = crapstate.cpu.a;          // 0x00
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            uint16_t result = old_a + byte + crapstate.cpu.C;  // 0x00 + 0xFF + 1 = 0x0100
            
            crapstate.cpu.a = (uint8_t)result;        // 0x00
            crapstate.cpu.Z = (crapstate.cpu.a == 0); 
            crapstate.cpu.N = 0;                      // Always 0 for ADC
            crapstate.cpu.H = ((old_a & 0xF) + (byte & 0xF) + crapstate.cpu.C) > 0xF;  // 1
            crapstate.cpu.C = (result > 0xFF);        // 1
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles += 8;
            //printf("added byte %x with carry  %x to a %x res %x flags z%x h%x c%x\n",byte,crapstate.cpu.C , old_a, crapstate.cpu.a, crapstate.cpu.Z, crapstate.cpu.H, crapstate.cpu.C);
            break;
            
        }
        
        case 0xCF:{ // RST 08H
            RST(0x08);
        }
        
        
        case 0xD0: { // RET NC
            if (!crapstate.cpu.C) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                //printf("returning to %x\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 20;
            } else {
                crapstate.cpu.pc++;
                crapstate.cpu.cycles += 8;
            }
            break;
        }
        
        case 0xD1: { //POP DE
            POP_R16(de);
        }
        
        case 0xD2:{
            //JP NC
            if(!crapstate.cpu.C){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                //printf("c flag not set jumping to %x\n",crapstate.cpu.pc);
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
                //printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                //printf("calling to %x because NC\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
            break;
        }
        
        
        case 0xD5: { // PUSH DE
            PUSH_R16(d, e);
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
            //printf("sub a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,subval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }
        
        case 0xD7: { // RST 10H
            RST(0x10);
        }
        
        case 0xD8: { // RET C
            if (crapstate.cpu.C) {
                uint8_t low = mem_read_byte(crapstate.cpu.sp++);
                uint8_t high = mem_read_byte(crapstate.cpu.sp++);
                crapstate.cpu.pc = (uint16_t)low | ((uint16_t)high << 8);
                //printf("returning to %x\n", crapstate.cpu.pc);
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
            //printf("returning to %x and enabling interrupts\n", crapstate.cpu.pc);
            crapstate.cpu.ime = 1;
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xDA:  {
            //JP C
            if(crapstate.cpu.C){
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc+1);
                //printf("C flag set jumping to %x\n",crapstate.cpu.pc);
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
                //printf("stored %x%x\n", (crapstate.cpu.pc + 3) >> 8, (crapstate.cpu.pc + 3) & 0xFF);
                crapstate.cpu.pc = mem_read_word(crapstate.cpu.pc + 1);
                //printf("calling to %x because C\n", crapstate.cpu.pc);
                crapstate.cpu.cycles += 24;
            } else {
                crapstate.cpu.pc+=3;
                crapstate.cpu.cycles += 12;
            }
            break;
        }
        
        case 0xDF:  {//RST 18H
            RST(0x18);
        }
        
        case 0xDE:  { //SBC A, n8
            /*PeanutGB code MIT licence*/
            uint8_t byte  = mem_read_byte(crapstate.cpu.pc+1);
            uint16_t temp =  crapstate.cpu.a - (byte + crapstate.cpu.C);          
            crapstate.cpu.C = (temp & 0xFF00) ? 1 : 0;                                   
            crapstate.cpu.H = ((crapstate.cpu.a ^ byte ^ temp) & 0x10) > 0;        
            crapstate.cpu.N = 1;                                                         
            crapstate.cpu.Z = ((temp & 0xFF) == 0x00);                                   
            crapstate.cpu.a = (temp & 0xFF);     
            crapstate.cpu.N = 1;
            crapstate.cpu.pc +=2;
            crapstate.cpu.cycles+=8;
            //printf("sbc a n8, res %x c%x z%x h%x\n",crapstate.cpu.a, crapstate.cpu.C,crapstate.cpu.Z,crapstate.cpu.H);
            break;
        }
        
        case 0xE0: { // LDH (n), A
            uint16_t addr = 0xFF00 + mem_read_byte(crapstate.cpu.pc + 1);
            mem_write_byte(addr, crapstate.cpu.a);
            //printf("wrote a = %x to mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 12;
            break;
        }
        
        case 0xE1: { //POP HL
            POP_R16(hl);
        }
        
        case 0xE2: { //LDH [C],A
            uint16_t addr = 0xFF00 + crapstate.cpu.c;
            mem_write_byte(addr, crapstate.cpu.a);
            //printf("wrote a = %x to mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0xE5: { // PUSH HL
            PUSH_R16(h, l);
        }
        
        case 0xE6: { // AND A, n
            uint8_t val = mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.a &= val;
            crapstate.cpu.f = 0;
            crapstate.cpu.H = 1;
            crapstate.cpu.Z = !crapstate.cpu.a;
            //printf("anded a and %x res %x\n", val, crapstate.cpu.a);
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0xE7:  {// RST 20H
            RST(0x20);
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
            crapstate.cpu.cycles+=16;
            //printf("added sp %x+offset %x (%x) ,H%x, C%x\n",crapstate.cpu.sp,offset,crapstate.cpu.sp,crapstate.cpu.H,crapstate.cpu.C);
            break;
        }
        
        case 0xEA: { // LD (nn), A
            uint16_t addr = mem_read_word(crapstate.cpu.pc + 1);
            mem_write_byte(addr, crapstate.cpu.a);
            //printf("wrote %x from a to %x \n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 3;
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xEE:  { //XOR A, n8
            uint8_t byte  = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.a ^= byte;
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = !crapstate.cpu.a; 
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles+=8;
            //printf("xored a with %x res %x\n",byte,crapstate.cpu.a);
            break;
        }
        
        case 0xEF: { // RST 28H
            RST(0x28);
        }
        
        case 0xE9: { // JP HL
            crapstate.cpu.pc = crapstate.cpu.hl;
            //printf("jump to hl %x\n", crapstate.cpu.pc);
            crapstate.cpu.cycles += 4;
            break;
        }
        
        case 0xF0: { // LDH A, (n)
            uint16_t addr = 0xFF00 + mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.a = mem_read_byte(addr);
            //printf("wrote to a = %x from mem %x\n", crapstate.cpu.a, addr);
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
            //printf("popped af = %x\n",crapstate.cpu.af);
            break;
        }
        
        case 0xF2: { //LDH A, [C]
            uint16_t addr = 0xFF00 + crapstate.cpu.c;
            crapstate.cpu.a = mem_read_byte(addr);
            //printf("wrote a = %x from mem %x\n", crapstate.cpu.a, addr);
            crapstate.cpu.pc += 1;
            crapstate.cpu.cycles += 8;
            break;
        }
        
        case 0xF3: { // DI
            crapstate.cpu.ime = 0;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            //printf("disabled interrupts\n");
            break;
        }
        
        case 0xF5: { // PUSH AF
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.af) >> 8); // High byte
            crapstate.cpu.sp--;
            mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.af) & 0xFF); // Low byte
            //printf("pushed af %x%x\n", (crapstate.cpu.af) >> 8, crapstate.cpu.af & 0xFF);
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 16;
            break;
        }
        
        case 0xF6: { // OR A, n8
            uint8_t byte = mem_read_byte(crapstate.cpu.pc+1);
            crapstate.cpu.a = crapstate.cpu.a | byte;
            crapstate.cpu.f = 0;
            crapstate.cpu.Z = !crapstate.cpu.a;
            crapstate.cpu.pc+=2;
            crapstate.cpu.cycles += 8;
            //printf("ored a and byte %X result %x\n",byte, crapstate.cpu.a);
            break;
        }
        
        case 0xF7:  { // RST 30H
            RST(0x30);
        }
        
        case 0xF8: { // LD HL, SP+e8
            // Sameboy code MIT licence
            int8_t offset = (int8_t)mem_read_byte(crapstate.cpu.pc + 1);
            crapstate.cpu.hl = crapstate.cpu.sp + offset;
            crapstate.cpu.f = 0;
            crapstate.cpu.H =
            ((crapstate.cpu.sp & 0xF) + (offset & 0xF) > 0xF) ? 1 : 0;
            crapstate.cpu.C =
            ((crapstate.cpu.sp & 0xFF) + (offset & 0xFF) > 0xFF) ? 1 : 0;
            crapstate.cpu.pc += 2;
            crapstate.cpu.cycles += 12;
            //printf("loaded sp %x+offset %x (%x) to hl,H%x, C%x\n",
            //crapstate.cpu.sp, offset, crapstate.cpu.hl, crapstate.cpu.H,
            //crapstate.cpu.C);
            
            break;
        }
        
        case 0xF9: { //LD SP,HL
            crapstate.cpu.sp = crapstate.cpu.hl;
            crapstate.cpu.pc+=1;
            crapstate.cpu.cycles+=8;
            //printf("loaded hl to sp %x\n",crapstate.cpu.sp);
            break;
        }
        
        case 0xFA:{ //LD A, a16
            uint16_t addr = mem_read_word(crapstate.cpu.pc+1);
            crapstate.cpu.a = mem_read_byte(addr);
            crapstate.cpu.pc+=3;
            crapstate.cpu.cycles+=16;
            //printf("wrote a = %x from addr %x\n",crapstate.cpu.a,addr);
            break;
        }
        
        case 0xFB: { // EI
            crapstate.cpu.ime_pending = 1;
            crapstate.cpu.pc++;
            crapstate.cpu.cycles += 4;
            //printf("enabled interrupts\n");
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
            //printf("cp a %x to %x ,res z%x c%x, h%x\n",crapstate.cpu.a,cpval,crapstate.cpu.Z,crapstate.cpu.C,crapstate.cpu.H);
            break;
        }
        
        case 0xFF: { // RST 38H
            RST(0x38);
        }
        
        default:{
            printf("Unhandled opcode: %X\n", opcode);
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
        //printf("went to interrupt\n");
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
    opcodes(mem_read_byte(crapstate.cpu.pc));
    
}
void cpu_start(){
    ;
}