#pragma once
#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#include "stdio.h"

uint8_t mem_read_byte(uint16_t);
uint16_t mem_read_word(uint16_t);
void mem_write_byte(uint16_t,uint8_t);
void mem_write_word(uint16_t,uint16_t);
uint8_t mem_init(FILE*);

typedef enum{
    JOYP_DPAD = 0x10,
    JOYP_BUTTONS = 0x20,
}joypad_part_t;

void flag_joypad_interrupt_buttons();

void flag_joypad_interrupt_dpad();

#endif