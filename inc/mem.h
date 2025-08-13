#pragma once


#include <stdint.h>

#include "stdio.h"

uint8_t mem_read_byte(uint16_t);
uint16_t mem_read_word(uint16_t);
void mem_write_byte(uint16_t,uint8_t);
void mem_write_word(uint16_t,uint16_t);
void mem_init(FILE*);