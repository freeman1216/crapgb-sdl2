/**
 * One snippet of this code is taken from PeanutGB https://github.com/deltabeard/Peanut-GB/ which is distrubuted with the
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
 *
 */
#pragma once
// CPU instructions code generation
// 16-bit instr

#include "crapstate.h"
#define LD_R16_N16(dest)                                                       \
  crapstate.cpu.dest = mem_read_word(crapstate.cpu.pc + 1);                    \
  crapstate.cpu.pc += 3;                                                       \
  crapstate.cpu.cycles += 12;                                                  \
  break

#define INC_R16(dest)                                                          \
  crapstate.cpu.dest++;                                                        \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 8;                                                   \
  break

#define DEC_R16(operand)                                                       \
  crapstate.cpu.operand--;                                                     \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 8;                                                   \
  break

#define PUSH_R16(rhigh, rlow)                                                  \
  crapstate.cpu.sp--;                                                          \
  mem_write_byte(crapstate.cpu.sp, crapstate.cpu.rhigh);                       \
  crapstate.cpu.sp--;                                                          \
  mem_write_byte(crapstate.cpu.sp, crapstate.cpu.rlow);                        \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 16;                                                  \
  break

#define POP_R16(operand)                                                       \
  uint8_t low = mem_read_byte(crapstate.cpu.sp++);                             \
  uint8_t high = mem_read_byte(crapstate.cpu.sp++);                            \
  crapstate.cpu.operand = (uint16_t)low | ((uint16_t)high << 8);               \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 12;                                                  \
  break
// 8-bit instr
// 8-bit loads
#define LD_R8_N8(dest)                                                         \
  crapstate.cpu.dest = mem_read_byte(crapstate.cpu.pc + 1);                    \
  crapstate.cpu.pc += 2;                                                       \
  crapstate.cpu.cycles += 8;                                                   \
  break

#define LD_R8_R8(dest, source)                                                 \
  crapstate.cpu.dest = crapstate.cpu.source;                                   \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define LD_R8_FROM_HL(dest)                                                    \
  crapstate.cpu.dest = mem_read_byte(crapstate.cpu.hl);                        \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 8;                                                   \
  break

#define LD_TO_HL_R8(source)                                                    \
  mem_write_byte(crapstate.cpu.hl, crapstate.cpu.source);                      \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 8;                                                   \
  break

#define LD_R8_SAME()                                                           \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break
// 8 bit arithmetic
#define INC_R8(operand)                                                        \
  crapstate.cpu.H = ((crapstate.cpu.operand & 0x0F) + 1) > 0x0F;               \
  crapstate.cpu.operand++;                                                     \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define DEC_R8(operand)                                                        \
  crapstate.cpu.H = !(crapstate.cpu.operand & 0x0F);                           \
  crapstate.cpu.operand--;                                                     \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.N = 1;                                                         \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define ADD_A_R8(addend)                                                       \
  uint8_t old_a = crapstate.cpu.a;                                             \
  crapstate.cpu.a += crapstate.cpu.addend;                                     \
  crapstate.cpu.Z = !crapstate.cpu.a;                                          \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.H = ((old_a & 0x0F) + (crapstate.cpu.addend & 0x0F)) > 0x0F;   \
  crapstate.cpu.C = old_a > crapstate.cpu.a;                                   \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define ADC_A_R8(addend)                                                       \
  uint8_t old_a = crapstate.cpu.a;                                             \
  uint16_t result = old_a + crapstate.cpu.addend + crapstate.cpu.C;            \
  crapstate.cpu.a = (uint8_t)result;                                           \
  crapstate.cpu.Z = (crapstate.cpu.a == 0);                                    \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.H =                                                            \
      ((old_a & 0xF) + (crapstate.cpu.addend & 0xF) + crapstate.cpu.C) > 0xF;  \
  crapstate.cpu.C = (result > 0xFF);                                           \
  crapstate.cpu.pc += 1;                                                       \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define SUB_A_R8(subtrahend)                                                   \
  crapstate.cpu.C = crapstate.cpu.subtrahend > crapstate.cpu.a;                \
  crapstate.cpu.H =                                                            \
      (crapstate.cpu.a & 0x0F) < (crapstate.cpu.subtrahend & 0x0F);            \
  crapstate.cpu.a = crapstate.cpu.a - crapstate.cpu.subtrahend;                \
  crapstate.cpu.Z = !crapstate.cpu.a;                                          \
  crapstate.cpu.N = 1;                                                         \
  crapstate.cpu.cycles += 4;                                                   \
  crapstate.cpu.pc += 1;                                                       \
  break;

// Taken from PeanutGB (MIT Licence)
#define SBC_A_R8(subtrahend)                                                   \
  uint16_t temp =                                                              \
      crapstate.cpu.a - (crapstate.cpu.subtrahend + crapstate.cpu.C);          \
  crapstate.cpu.C = (temp & 0xFF00) ? 1 : 0;                                   \
  crapstate.cpu.H =                                                            \
      ((crapstate.cpu.a ^ crapstate.cpu.subtrahend ^ temp) & 0x10) > 0;        \
  crapstate.cpu.N = 1;                                                         \
  crapstate.cpu.Z = ((temp & 0xFF) == 0x00);                                   \
  crapstate.cpu.a = (temp & 0xFF);                                             \
  crapstate.cpu.pc += 1;                                                       \
  crapstate.cpu.cycles += 4;                                                   \
  break

// 8-bit bitwise operations
#define AND_A_R8(operand)                                                      \
  crapstate.cpu.a &= crapstate.cpu.operand;                                    \
  crapstate.cpu.H = 1;                                                         \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.C = 0;                                                         \
  crapstate.cpu.Z = !crapstate.cpu.a;                                          \
  crapstate.cpu.pc += 1;                                                       \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define XOR_A_R8(operand)                                                      \
  crapstate.cpu.a ^= crapstate.cpu.operand;                                    \
  crapstate.cpu.Z = !crapstate.cpu.a;                                          \
  crapstate.cpu.C = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define OR_A_R8(operand)                                                       \
  crapstate.cpu.a = crapstate.cpu.a | crapstate.cpu.operand;                   \
  crapstate.cpu.Z = !crapstate.cpu.a;                                          \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.C = 0;                                                         \
  crapstate.cpu.pc++;                                                          \
  crapstate.cpu.cycles += 4;                                                   \
  break

#define CP_A_R8(operand)                                                       \
  uint8_t res = crapstate.cpu.a - crapstate.cpu.operand;                       \
  crapstate.cpu.Z = !res;                                                      \
  crapstate.cpu.N = 1;                                                         \
  crapstate.cpu.C = crapstate.cpu.operand > crapstate.cpu.a;                   \
  crapstate.cpu.H = (crapstate.cpu.a & 0x0F) < (crapstate.cpu.operand & 0x0F); \
  crapstate.cpu.cycles += 4;                                                   \
  crapstate.cpu.pc += 1;                                                       \
  break
// Misc instr
#define RST(addr)                                                              \
  crapstate.cpu.sp--;                                                          \
  mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) >> 8);               \
  crapstate.cpu.sp--;                                                          \
  mem_write_byte(crapstate.cpu.sp, (crapstate.cpu.pc + 1) & 0xFF);             \
  crapstate.cpu.pc = addr;                                                     \
  crapstate.cpu.cycles += 16;                                                  \
  break

// CB instr
#define RLC_R8(operand)                                                        \
  crapstate.cpu.operand =                                                      \
      (crapstate.cpu.operand << 1) | (crapstate.cpu.operand >> 7);             \
  crapstate.cpu.C = crapstate.cpu.operand & 0x1;                               \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  break

#define RRC_R8(operand)                                                        \
  crapstate.cpu.C = crapstate.cpu.operand & 0x1;                               \
  crapstate.cpu.operand =                                                      \
      (crapstate.cpu.operand >> 1) | (crapstate.cpu.operand << 7);             \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  break

#define RL_R8(operand)                                                         \
  uint8_t old_carry = crapstate.cpu.C;                                         \
  crapstate.cpu.C = (crapstate.cpu.operand) & 0x80 ? 1 : 0;                    \
  crapstate.cpu.operand <<= 1;                                                 \
  crapstate.cpu.operand |= old_carry;                                          \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.N = 0;                                                         \
  break

#define RR_R8(operand)                                                         \
  uint8_t old_carry = crapstate.cpu.C;                                         \
  crapstate.cpu.C = crapstate.cpu.operand & 1;                                 \
  crapstate.cpu.operand >>= 1;                                                 \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.operand |= (old_carry << 7);                                   \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  break

#define SLA_R8(operand)                                                        \
  crapstate.cpu.C = (crapstate.cpu.operand & 0x80) != 0;                       \
  crapstate.cpu.operand <<= 1;                                                 \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  break

#define SRA_R8(operand)                                                        \
  crapstate.cpu.C = crapstate.cpu.operand & 0x1;                               \
  crapstate.cpu.operand =                                                      \
      (crapstate.cpu.operand >> 1) | (crapstate.cpu.operand & 0x80);           \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.H = 0;                                                         \
  break

#define SWAP_R8(operand)                                                       \
  crapstate.cpu.operand =                                                      \
      (crapstate.cpu.operand << 4) | (crapstate.cpu.operand >> 4);             \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.C = 0;                                                         \
  break
#define SRL_R8(operand)                                                        \
  crapstate.cpu.C = (crapstate.cpu.operand & 0x01);                            \
  crapstate.cpu.operand >>= 1;                                                 \
  crapstate.cpu.Z = !crapstate.cpu.operand;                                    \
  crapstate.cpu.H = 0;                                                         \
  crapstate.cpu.N = 0;                                                         \
  break

#define BIT_N_R8(bit, operand)                                                 \
  crapstate.cpu.Z = !(crapstate.cpu.operand & (1 << bit));                     \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.H = 1;                                                         \
  break

#define BIT_N_AT_HL(bit)                                                       \
  uint8_t byte = mem_read_byte(crapstate.cpu.hl);                              \
  crapstate.cpu.Z = !(byte & (1 << bit));                                      \
  crapstate.cpu.N = 0;                                                         \
  crapstate.cpu.H = 1;                                                         \
  crapstate.cpu.cycles += 4;                                                   \
  break
#define RES_N_R8(bit, operand)                                                 \
  crapstate.cpu.operand &= (uint8_t)~(1 << bit);                               \
  break

#define RES_N_AT_HL(bit)                                                       \
  uint8_t byte = mem_read_byte(crapstate.cpu.hl);                              \
  byte &= (uint8_t)~(1 << bit);                                                \
  mem_write_byte(crapstate.cpu.hl, byte);                                      \
  crapstate.cpu.cycles += 8;                                                   \
  break

#define SET_N_R8(bit, operand)                                                 \
  crapstate.cpu.operand |= (uint8_t)(1 << bit);                                \
  break

#define SET_N_AT_HL(bit)                                                       \
  uint8_t byte = mem_read_byte(crapstate.cpu.hl);                              \
  byte |= (1 << bit);                                                          \
  mem_write_byte(crapstate.cpu.hl, byte);                                      \
  crapstate.cpu.cycles += 8;                                                   \
  break
