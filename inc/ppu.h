#pragma once

#include <stdint.h>

//PPU defines
typedef enum {
    MODE0_HBLANK        = 0,
    MODE1_VBLANK        = 1,
    MODE2_OAM           = 2,
    MODE3_DRAW          = 3,
    MODE_FAKE_VBLANK    = 4, //occurs after bootrom and lasts 4 cycles
    MODE_DISABLED       = 5
} ppu_mode_t;

void update_ppu (uint16_t clocks);
void stop_ppu();
void start_ppu();