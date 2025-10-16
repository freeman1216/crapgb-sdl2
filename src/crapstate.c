#include "crapstate.h"
#include "defines.h"

crapstate_t crapstate;

void crapstate_init() {
    // 1. CPU Registers
    crapstate.cpu.a = 0x01; // Actual post-boot values observed
    crapstate.cpu.Z = 1;
    crapstate.cpu.N = 0;
    crapstate.cpu.H = 1;
    crapstate.cpu.C = 1;
    crapstate.cpu.bc = 0x0013;
    crapstate.cpu.de = 0x00D8;
    crapstate.cpu.hl = 0x014D;
    crapstate.cpu.sp = 0xFFFE;
    crapstate.cpu.pc = 0x100;
    crapstate.cpu.cycles = 0;
    
    // 2. Memory State
    crapstate.io.TIMA = 0x00;
    crapstate.io.TMA = 0x00;
    crapstate.io.TAC = 0xF8;
    crapstate.io.DIV = 0xAB; // DIV (~166,912 cycles elapsed)
    crapstate.io.if_reg = 0xE1;
    crapstate.io.ie = 0x00;
    crapstate.mem.currentromx = 1;
    crapstate.io.OBP0 = 0xFF;
    crapstate.io.OBP1 = 0xFF;
    crapstate.io.WX = 0x00;
    crapstate.io.WY = 0x00;
    crapstate.io.LY = 0x00;
    crapstate.io.LYC = 0x00;
    crapstate.ppu.mode = MODE_FAKE_VBLANK;
    crapstate.ppu.mode_cycles = FAKE_VBLANK_CYCLES;
    
    // 3. Hardware Flags
    crapstate.io.LCDC = 0x91; // LCDC (LCD on, BG enabled)
    crapstate.io.SCY = 0x00;  // SCY
    crapstate.io.SCX = 0x00;  // SCX
    crapstate.io.STAT = 0x85; // STAT (mode flag set)
    crapstate.io.BGP = 0xFC;  // BGP (default palette)
    
    //4. Internal rendrerer state
    crapstate.display.OBP_indeces[0][0] = 3;
    crapstate.display.OBP_indeces[0][1] = 3;
    crapstate.display.OBP_indeces[0][2] = 3;
    crapstate.display.OBP_indeces[0][3] = 3;
    
    crapstate.display.OBP_indeces[1][0] = 3;
    crapstate.display.OBP_indeces[1][1] = 3;
    crapstate.display.OBP_indeces[1][2] = 3;
    crapstate.display.OBP_indeces[1][3] = 3;
    
    crapstate.display.BGP_indeces[0] = 0;
    crapstate.display.BGP_indeces[1] = 3;
    crapstate.display.BGP_indeces[2] = 3;
    crapstate.display.BGP_indeces[3] = 3;
    
    crapstate.display.window_curr_line = 0;
    crapstate.display.frame_finished = 0;

    crapstate.display.palette[0] = 0x7FFF;
    crapstate.display.palette[1] = 0x7E10;
    crapstate.display.palette[2] = 0x48E7;
    crapstate.display.palette[3] = 0x0000;
}