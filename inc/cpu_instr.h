/**
 * One snippet of this code is taken from PeanutGB
 * https://github.com/deltabeard/Peanut-GB/ which is distrubuted with the
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
#ifndef BADCPU_INSTR_H
#define BADCPU_INSTR_H
// CPU instructions code generation
// 16-bit instr

#define LD_R16_N16(dest)                                                       \
  badstate.cpu.dest = mem_read_word(badstate.cpu.pc + 1);                    \
  badstate.cpu.pc += 3;                                                       \
  badstate.cpu.cycles += 12;                                                  \
  break

#define INC_R16(dest)                                                          \
  badstate.cpu.dest++;                                                        \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 8;                                                   \
  break

#define DEC_R16(operand)                                                       \
  badstate.cpu.operand--;                                                     \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 8;                                                   \
  break

#define PUSH_R16(rhigh, rlow)                                                  \
  badstate.cpu.sp--;                                                          \
  mem_write_byte(badstate.cpu.sp, badstate.cpu.rhigh);                       \
  badstate.cpu.sp--;                                                          \
  mem_write_byte(badstate.cpu.sp, badstate.cpu.rlow);                        \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 16;                                                  \
  break

#define POP_R16(operand)                                                       \
  uint8_t low = mem_read_byte(badstate.cpu.sp++);                             \
  uint8_t high = mem_read_byte(badstate.cpu.sp++);                            \
  badstate.cpu.operand = (uint16_t)low | ((uint16_t)high << 8);               \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 12;                                                  \
  break
// 8-bit instr
// 8-bit loads
#define LD_R8_N8(dest)                                                         \
  badstate.cpu.dest = mem_read_byte(badstate.cpu.pc + 1);                    \
  badstate.cpu.pc += 2;                                                       \
  badstate.cpu.cycles += 8;                                                   \
  break

#define LD_R8_R8(dest, source)                                                 \
  badstate.cpu.dest = badstate.cpu.source;                                   \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break

#define LD_R8_FROM_HL(dest)                                                    \
  badstate.cpu.dest = mem_read_byte(badstate.cpu.hl);                        \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 8;                                                   \
  break

#define LD_TO_HL_R8(source)                                                    \
  mem_write_byte(badstate.cpu.hl, badstate.cpu.source);                      \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 8;                                                   \
  break

#define LD_R8_SAME()                                                           \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break
// 8 bit arithmetic
#define INC_R8(operand)                                                        \
  badstate.cpu.H = ((badstate.cpu.operand & 0x0F) + 1) > 0x0F;               \
  badstate.cpu.operand++;                                                     \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  badstate.cpu.N = 0;                                                         \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break

#define DEC_R8(operand)                                                        \
  badstate.cpu.H = !(badstate.cpu.operand & 0x0F);                           \
  badstate.cpu.operand--;                                                     \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  badstate.cpu.N = 1;                                                         \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break

#define ADD_A_R8(addend)                                                       \
  uint8_t old_a = badstate.cpu.a;                                             \
  badstate.cpu.a += badstate.cpu.addend;                                     \
  badstate.cpu.Z = !badstate.cpu.a;                                          \
  badstate.cpu.N = 0;                                                         \
  badstate.cpu.H = ((old_a & 0x0F) + (badstate.cpu.addend & 0x0F)) > 0x0F;   \
  badstate.cpu.C = old_a > badstate.cpu.a;                                   \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break

#define ADC_A_R8(addend)                                                       \
  uint8_t old_a = badstate.cpu.a;                                             \
  uint16_t result = old_a + badstate.cpu.addend + badstate.cpu.C;            \
  badstate.cpu.a = (uint8_t)result;                                           \
  badstate.cpu.Z = (badstate.cpu.a == 0);                                    \
  badstate.cpu.N = 0;                                                         \
  badstate.cpu.H =                                                            \
      ((old_a & 0xF) + (badstate.cpu.addend & 0xF) + badstate.cpu.C) > 0xF;  \
  badstate.cpu.C = (result > 0xFF);                                           \
  badstate.cpu.pc += 1;                                                       \
  badstate.cpu.cycles += 4;                                                   \
  break

#define SUB_A_R8(subtrahend)                                                   \
  badstate.cpu.C = badstate.cpu.subtrahend > badstate.cpu.a;                \
  badstate.cpu.H =                                                            \
      (badstate.cpu.a & 0x0F) < (badstate.cpu.subtrahend & 0x0F);            \
  badstate.cpu.a = badstate.cpu.a - badstate.cpu.subtrahend;                \
  badstate.cpu.Z = !badstate.cpu.a;                                          \
  badstate.cpu.N = 1;                                                         \
  badstate.cpu.cycles += 4;                                                   \
  badstate.cpu.pc += 1;                                                       \
  break;

// Taken from PeanutGB (MIT Licence)
#define SBC_A_R8(subtrahend)                                                   \
  uint16_t temp =                                                              \
      badstate.cpu.a - (badstate.cpu.subtrahend + badstate.cpu.C);          \
  badstate.cpu.C = (temp & 0xFF00) ? 1 : 0;                                   \
  badstate.cpu.H =                                                            \
      ((badstate.cpu.a ^ badstate.cpu.subtrahend ^ temp) & 0x10) > 0;        \
  badstate.cpu.N = 1;                                                         \
  badstate.cpu.Z = ((temp & 0xFF) == 0x00);                                   \
  badstate.cpu.a = (temp & 0xFF);                                             \
  badstate.cpu.pc += 1;                                                       \
  badstate.cpu.cycles += 4;                                                   \
  break

// 8-bit bitwise operations
#define AND_A_R8(operand)                                                      \
  badstate.cpu.a &= badstate.cpu.operand;                                    \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.H = 1;                                                         \
  badstate.cpu.Z = !badstate.cpu.a;                                          \
  badstate.cpu.pc += 1;                                                       \
  badstate.cpu.cycles += 4;                                                   \
  break

#define XOR_A_R8(operand)                                                      \
  badstate.cpu.a ^= badstate.cpu.operand;                                    \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.Z = !badstate.cpu.a;                                          \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break

#define OR_A_R8(operand)                                                       \
  badstate.cpu.a = badstate.cpu.a | badstate.cpu.operand;                   \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.Z = !badstate.cpu.a;                                          \
  badstate.cpu.pc++;                                                          \
  badstate.cpu.cycles += 4;                                                   \
  break

#define CP_A_R8(operand)                                                       \
  uint8_t res = badstate.cpu.a - badstate.cpu.operand;                       \
  badstate.cpu.Z = !res;                                                      \
  badstate.cpu.N = 1;                                                         \
  badstate.cpu.C = badstate.cpu.operand > badstate.cpu.a;                   \
  badstate.cpu.H = (badstate.cpu.a & 0x0F) < (badstate.cpu.operand & 0x0F); \
  badstate.cpu.cycles += 4;                                                   \
  badstate.cpu.pc += 1;                                                       \
  break
// Misc instr
#define RST(addr)                                                              \
  badstate.cpu.sp--;                                                          \
  mem_write_byte(badstate.cpu.sp, (badstate.cpu.pc + 1) >> 8);               \
  badstate.cpu.sp--;                                                          \
  mem_write_byte(badstate.cpu.sp, (badstate.cpu.pc + 1) & 0xFF);             \
  badstate.cpu.pc = addr;                                                     \
  badstate.cpu.cycles += 16;                                                  \
  break

// CB instr
#define RLC_R8(operand)                                                        \
  badstate.cpu.operand =                                                      \
      (badstate.cpu.operand << 1) | (badstate.cpu.operand >> 7);             \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = badstate.cpu.operand & 0x1;                               \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define RRC_R8(operand)                                                        \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = badstate.cpu.operand & 0x1;                               \
  badstate.cpu.operand =                                                      \
      (badstate.cpu.operand >> 1) | (badstate.cpu.operand << 7);             \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define RL_R8(operand)                                                         \
  uint8_t old_carry = badstate.cpu.C;                                         \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = (badstate.cpu.operand) & 0x80 ? 1 : 0;                    \
  badstate.cpu.operand <<= 1;                                                 \
  badstate.cpu.operand |= old_carry;                                          \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define RR_R8(operand)                                                         \
  uint8_t old_carry = badstate.cpu.C;                                         \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = badstate.cpu.operand & 1;                                 \
  badstate.cpu.operand >>= 1;                                                 \
  badstate.cpu.operand |= (old_carry << 7);                                   \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define SLA_R8(operand)                                                        \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = (badstate.cpu.operand & 0x80) != 0;                       \
  badstate.cpu.operand <<= 1;                                                 \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define SRA_R8(operand)                                                        \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = badstate.cpu.operand & 0x1;                               \
  badstate.cpu.operand =                                                      \
      (badstate.cpu.operand >> 1) | (badstate.cpu.operand & 0x80);           \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define SWAP_R8(operand)                                                       \
  badstate.cpu.operand =                                                      \
      (badstate.cpu.operand << 4) | (badstate.cpu.operand >> 4);             \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define SRL_R8(operand)                                                        \
  badstate.cpu.f = 0;                                                         \
  badstate.cpu.C = (badstate.cpu.operand & 0x01);                            \
  badstate.cpu.operand >>= 1;                                                 \
  badstate.cpu.Z = !badstate.cpu.operand;                                    \
  break

#define BIT_N_R8(bit, operand)                                                 \
  badstate.cpu.Z = !(badstate.cpu.operand & (1 << bit));                     \
  badstate.cpu.N = 0;                                                         \
  badstate.cpu.H = 1;                                                         \
  break

#define BIT_N_AT_HL(bit)                                                       \
  uint8_t byte = mem_read_byte(badstate.cpu.hl);                              \
  badstate.cpu.Z = !(byte & (1 << bit));                                      \
  badstate.cpu.N = 0;                                                         \
  badstate.cpu.H = 1;                                                         \
  badstate.cpu.cycles += 4;                                                   \
  break
#define RES_N_R8(bit, operand)                                                 \
  badstate.cpu.operand &= (uint8_t)~(1 << bit);                               \
  break

#define RES_N_AT_HL(bit)                                                       \
  uint8_t byte = mem_read_byte(badstate.cpu.hl);                              \
  byte &= (uint8_t)~(1 << bit);                                                \
  mem_write_byte(badstate.cpu.hl, byte);                                      \
  badstate.cpu.cycles += 8;                                                   \
  break

#define SET_N_R8(bit, operand)                                                 \
  badstate.cpu.operand |= (uint8_t)(1 << bit);                                \
  break

#define SET_N_AT_HL(bit)                                                       \
  uint8_t byte = mem_read_byte(badstate.cpu.hl);                              \
  byte |= (1 << bit);                                                          \
  mem_write_byte(badstate.cpu.hl, byte);                                      \
  badstate.cpu.cycles += 8;                                                   \
  break

#define UNHANDLED_OPCODE()                                                     \
  BADLOG("UNHANDLED_OPCODE");                                                 \
  while (1)                                                                    
    
#endif