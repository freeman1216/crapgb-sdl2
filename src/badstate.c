#include "badstate.h"
#include "defines.h"

badstate_t badstate;

void badstate_init() {
    // 1. CPU Registers
    badstate.cpu.a = 0x01; // Actual post-boot values observed
    badstate.cpu.Z = 1;
    badstate.cpu.N = 0;
    badstate.cpu.H = 1;
    badstate.cpu.C = 1;
    badstate.cpu.bc = 0x0013;
    badstate.cpu.de = 0x00D8;
    badstate.cpu.hl = 0x014D;
    badstate.cpu.sp = 0xFFFE;
    badstate.cpu.pc = 0x100;
    badstate.cpu.cycles = 0;
    
    // 2. Memory State
    badstate.io.TIMA = 0x00;
    badstate.io.TMA = 0x00;
    badstate.io.TAC = 0xF8;
    badstate.io.DIV = 0xAB; // DIV (~166,912 cycles elapsed)
    badstate.io.if_reg = 0xE1;
    badstate.io.ie = 0x00;
    badstate.mem.currentromx = 1;
    badstate.io.OBP0 = 0xFF;
    badstate.io.OBP1 = 0xFF;
    badstate.io.WX = 0x00;
    badstate.io.WY = 0x00;
    badstate.io.LY = 0x00;
    badstate.io.LYC = 0x00;
    badstate.ppu.mode = MODE_FAKE_VBLANK;
    badstate.ppu.mode_cycles = FAKE_VBLANK_CYCLES;
    
    // 3. Hardware Flags
    badstate.io.LCDC = 0x91; // LCDC (LCD on, BG enabled)
    badstate.io.SCY = 0x00;  // SCY
    badstate.io.SCX = 0x00;  // SCX
    badstate.io.STAT = 0x85; // STAT (mode flag set)
    badstate.io.BGP = 0xFC;  // BGP (default palette)
    
    //4. Internal rendrerer state
    badstate.display.OBP_indeces[0][0] = 3;
    badstate.display.OBP_indeces[0][1] = 3;
    badstate.display.OBP_indeces[0][2] = 3;
    badstate.display.OBP_indeces[0][3] = 3;
    
    badstate.display.OBP_indeces[1][0] = 3;
    badstate.display.OBP_indeces[1][1] = 3;
    badstate.display.OBP_indeces[1][2] = 3;
    badstate.display.OBP_indeces[1][3] = 3;
    
    badstate.display.BGP_indeces[0] = 0;
    badstate.display.BGP_indeces[1] = 3;
    badstate.display.BGP_indeces[2] = 3;
    badstate.display.BGP_indeces[3] = 3;
    
    badstate.display.window_curr_line = 0;
    badstate.display.frame_finished = 0;

    badstate.display.palette[0] = 0x7FFF;
    badstate.display.palette[1] = 0x7E10;
    badstate.display.palette[2] = 0x48E7;
    badstate.display.palette[3] = 0x0000;
}