#pragma once
#ifndef PPU_H
#define PPU_H

#include <stdint.h>

//PPU defines
typedef enum {
    MODE0_HBLANK        = 0,
    MODE1_VBLANK        = 1,
    MODE2_OAM           = 2,
    MODE3_DRAW          = 3,
    MODE_FAKE_VBLANK    = 4, //occurs after bootrom and lasts 4 cycles, not a real mode just a hardware quirk
    MODE_DISABLED       = 5  //not a real mode,used to not have a headache when tracking state when ppu is disabled
} ppu_mode_t;

void update_ppu (uint16_t clocks);
void stop_ppu();
void start_ppu();

#endif