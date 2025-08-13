#include "crapstate.h"

#include "defines.h"
crapstate_t crapstate;
void crapstate_init() {
    // 1. CPU Registers
    crapstate.cpu.a = 0x01;  // Actual post-boot values observed
    crapstate.cpu.Z = 1;
    crapstate.cpu.N = 0;
    crapstate.cpu.H = 1;
    crapstate.cpu.C = 1;
    crapstate.cpu.bc = 0x0013;
    crapstate.cpu.de = 0x00D8;
    crapstate.cpu.hl = 0x014D;
    crapstate.cpu.sp = 0xFFFE;
    crapstate.cpu.pc = 0x100;  // Start here
    crapstate.cpu.cycles = 0;
    
    // 2. Memory State
    crapstate.io.TIMA = 0x00;   // TIMA
    crapstate.io.TMA = 0x00;   // TMA
    crapstate.io.TAC = 0xF8;   // TAC (timer disabled)
    crapstate.io.DIV = 0xAB;   // DIV (~166,912 cycles elapsed)
    crapstate.io.if_reg= 0xE1;   // IF (some interrupts flagged)
    crapstate.io.ie = 0x00;   // IE (interrupts disabled)
    crapstate.mem.currentromx = 1;
    crapstate.io.OBP0 = 0xFF;
    crapstate.io.OBP1 = 0xFF;
    crapstate.io.WX = 0x00;
    crapstate.io.WY = 0x00;
    crapstate.io.LY = 0x00;
    crapstate.io.LYC =0x00;
    crapstate.ppu.mode = MODE_FAKE_VBLANK;
    crapstate.ppu.mode_cycles = FAKE_VBLANK_CYCLES;
    // 3. Hardware Flags
    crapstate.io.LCDC = 0x91;   // LCDC (LCD on, BG enabled)
    crapstate.io.SCY = 0x00;   // SCY
    crapstate.io.SCX = 0x00;   // SCX
    crapstate.io.STAT = 0x85;   // STAT (mode flag set)
    crapstate.io.LYC = 0x00;   // LYC
    crapstate.io.BGP = 0xFC;   // BGP (default palette)
}