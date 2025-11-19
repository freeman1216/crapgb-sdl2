#include <stdint.h>
#include <stdio.h>

#include "mem.h"
#include "defines.h"
#include "badstate.h"
#include "ppu.h"

static void inline io_write(uint16_t addr, uint8_t val);
static uint8_t inline io_read(uint16_t addr);

uint8_t mem_init(FILE* rom){
    uint64_t res =fread(badstate.mem.rom0,1,ROM0_SIZE,rom);
    if(res != ROM0_SIZE) {
        BADLOG("Bank 0 read incomplete! (%zu/%d bytes)\n", res, ROMX_SIZE);
        return -1;
    }
    res = fread(badstate.mem.romx,1 , ROMX_SIZE, rom);
    if(res != ROMX_SIZE) {
        BADLOG("Bank 1 read incomplete! (%zu/%d bytes)\n", res, ROMX_SIZE);
        return -1;
    }
    badstate.mem.rom = rom;
    return 0;
}

uint8_t mem_read_byte(uint16_t addr){

    switch (addr>>12) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            return badstate.mem.rom0[addr];
            break;
        
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            return badstate.mem.romx[addr - ROMX_START];
            break;
        
        case 0x8:
        case 0x9:
            return badstate.mem.vram[addr - VRAM_START];
            break;
        
        case 0xA:
        case 0xB:
            //unsupported
            break;
        case 0xC:
        case 0xD:
            return  badstate.mem.wram[addr - WRAM0_START];
            break;
        case 0xE:
            return badstate.mem.wram[addr - ECHO_START];
            break;

        case 0xF:
            if(addr < OAM_START){
                return badstate.mem.wram[addr - ECHO_START];
            }

            if(addr < UNUSED_START){
                return badstate.mem.oam[addr - OAM_START];
            }

            if(addr < IOREGS_START){
                return 0xFF;
            }

            if (addr < HRAM_START) {
                return io_read(addr);
            }

            if(addr < IE_REG_ADDR){
                return badstate.mem.hram[addr - HRAM_START];
            }

            if(addr == IE_REG_ADDR){
                return badstate.io.ie;
            }
            break;
    }
    return 0;
};
//TODO get rid of this 
uint16_t mem_read_word(uint16_t addr) {
    uint8_t low = mem_read_byte(addr);
    uint8_t high = mem_read_byte(addr + 1);
    return (uint16_t)(low) | ((uint16_t)(high) << 8);
}

void handle_bank_switch(uint8_t value) {
    
    uint8_t bank = value & 0x1F;  
    if(bank == 0) bank = 1;
    

    if(bank != badstate.mem.currentromx) {
        if(fseek(badstate.mem.rom, ROMX_SIZE * (bank), SEEK_SET) != 0) {
            BADLOG("Bank seek failed!\n");
            return;
        }
        
        size_t read = fread(badstate.mem.romx, 1, ROMX_SIZE, badstate.mem.rom);
        if(read != ROMX_SIZE) {
            BADLOG("Bank read incomplete! (%zu/%d bytes)\n", read, ROMX_SIZE);
        }
        
        badstate.mem.currentromx = bank;
       // badLOG("Switched to ROM bank %d\n", bank);
    }
}

void mem_write_byte(uint16_t addr, uint8_t val){
  switch (addr>>12) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            handle_bank_switch(val);
            break;
        
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            //bad place to be
            break;
        
        case 0x8:
        case 0x9:
            badstate.mem.vram[addr - VRAM_START] = val;
            break;
        case 0xA:
        case 0xB:
            //unsupported
            break;

        case 0xC:
        case 0xD:
            badstate.mem.wram[addr - WRAM0_START] = val;
            break;

        case 0xE:
            badstate.mem.wram[addr - ECHO_START] = val; 
            break;

        case 0xF:
            if(addr < OAM_START ){
                badstate.mem.wram[addr - ECHO_START] = val; 
            }

            if(addr < UNUSED_START){
                badstate.mem.oam[addr - OAM_START] = val;
            }

            if(addr < IOREGS_START){
                break;
            }

            if (addr < HRAM_START) {
                io_write(addr,val);
                break;
            }

            if(addr < IE_REG_ADDR){
                badstate.mem.hram[addr - HRAM_START] = val;
                break;
            }

            if(addr == IE_REG_ADDR){
                badstate.io.ie = val & 0x1F; 
                break;
            }

    }   
    
}
//TODO get rid of this 
void mem_write_word(uint16_t addr, uint16_t value){
    
    mem_write_byte(addr, value & 0xFF);         // write low byte
    mem_write_byte(addr + 1, (value >> 8) & 0xFF); // write high byte

}


static void  inline io_write(uint16_t addr, uint8_t val) {
    switch (addr) {
        // Joypad
        case 0xFF00: 
            badstate.io.P1 = (badstate.io.P1 & 0xCF) | (val & 0x30); //bits 4 and 5 writable
            break;
            
        // Serial (for testing with roms that write to it)
        case 0xFF01: {
            badstate.io.SB = val;
            BADLOG("%c",val);
            break;
        }
        case 0xFF02: badstate.io.SC = val & 0x83; break; // Only bits 0-1,7 writable
        
        // Timer
        case 0xFF04: badstate.io.DIV = 0; break; // Any write resets
        case 0xFF05: badstate.io.TIMA = val; break;
        case 0xFF06: badstate.io.TMA = val; break;
        case 0xFF07: badstate.io.TAC = val & 0x07; break; // Only bits 0-2 matter
        
        // LCD
        case 0xFF40: {    
            uint8_t old_lcdc = badstate.io.LCDC;
            badstate.io.LCDC = val;
            if ((old_lcdc ^ val) & 0x80) {  
                if (val & 0x80) {
                    start_ppu();
                } else {
                    stop_ppu();
                }
            }
            break;
        }   
            
        case 0xFF41: badstate.io.STAT = (badstate.io.STAT & 0x03) | (val & 0xFC); break;
        case 0xFF42: badstate.io.SCY = val; break;
        case 0xFF43: badstate.io.SCX = val; break;
        case 0xFF45: badstate.io.LYC = val; break;
        case 0xFF46: {
            uint16_t src = val << 8; 
            
            
            for (int i = 0; i < 160; i ++) {
                badstate.mem.oam[i]   = mem_read_byte(src + i);
            }
    
            badstate.cpu.cycles += 640;//160 transfers * 4 ticks
            break;
        }
          
        case 0xFF47:{ 
            badstate.io.BGP = val;
            badstate.display.BGP_indeces[0] = val & 0x3;
            badstate.display.BGP_indeces[1] = (val>>2) & 0x3;
            badstate.display.BGP_indeces[2] = (val>>4) & 0x3;
            badstate.display.BGP_indeces[3] = (val>>6) & 0x3;
            break;
        }   
        case 0xFF48:{ 
            badstate.io.OBP0 = val; 
            badstate.display.OBP_indeces[0][0] = val & 0x3;
            badstate.display.OBP_indeces[0][1] = (val>>2) & 0x3;
            badstate.display.OBP_indeces[0][2] = (val>>4) & 0x3;
            badstate.display.OBP_indeces[0][3] = (val>>6) & 0x3;
            break;
        }
        case 0xFF49:{ 
            badstate.io.OBP1 = val;
            badstate.display.OBP_indeces[1][0] = val & 0x3;
            badstate.display.OBP_indeces[1][1] = (val>>2) & 0x3;
            badstate.display.OBP_indeces[1][2] = (val>>4) & 0x3;
            badstate.display.OBP_indeces[1][3] = (val>>6) & 0x3;
            break;
        }
        case 0xFF4A: badstate.io.WY = val; break;
        case 0xFF4B: badstate.io.WX = val; break;
        
        // Interrupts
        case 0xFF0F: badstate.io.if_reg = val & 0x1F; break;
            
        default: break; 
    }
}
static inline uint8_t read_joypad() {
    uint8_t result = 0xF;  // Default: no buttons pressed
    
    // Check direction buttons (if P1.4=0)
    if ((badstate.io.P1 & JOYP_DPAD) ^ JOYP_DPAD) {
        if (badstate.buttons.right)  result &= ~(1 << 0);
        if (badstate.buttons.left)   result &= ~(1 << 1);
        if (badstate.buttons.up)     result &= ~(1 << 2);
        if (badstate.buttons.down)   result &= ~(1 << 3);
    }
    
    // Check action buttons (if P1.5=0)
    if ((badstate.io.P1 & JOYP_BUTTONS) ^ JOYP_BUTTONS) {
        if (badstate.buttons.A)      result &= ~(1 << 0);
        if (badstate.buttons.B)      result &= ~(1 << 1);
        if (badstate.buttons.start)  result &= ~(1 << 2);
        if (badstate.buttons.select) result &= ~(1 << 3);
    }
    
    return (badstate.io.P1 & 0xF0) | result;
}


void flag_joypad_interrupt_buttons(){
    if((badstate.io.P1 & JOYP_BUTTONS) ^ JOYP_BUTTONS){
        REQUEST_INTERRUPT(INTERRUPT_JOYPAD);
    }
}

void flag_joypad_interrupt_dpad(){
    if((badstate.io.P1 & JOYP_DPAD) ^ JOYP_DPAD){
        REQUEST_INTERRUPT(INTERRUPT_JOYPAD);
    }
}

static uint8_t inline io_read(uint16_t addr) {
    switch (addr) {
        // Joypad
        case 0xFF00: return read_joypad();
            
        // Serial
        case 0xFF01: return badstate.io.SB;
        case 0xFF02: return badstate.io.SC | 0x7E; // Bit 7 unused
        
        // Timer
        case 0xFF04: return badstate.io.DIV;
        case 0xFF05: return badstate.io.TIMA;
        case 0xFF06: return badstate.io.TMA;
        case 0xFF07: return badstate.io.TAC | 0xF8; // Upper bits 1
        
        // LCD
        case 0xFF40: return badstate.io.LCDC;
        case 0xFF41: return badstate.io.STAT | 0x80; // Bit 7 always 1
        case 0xFF42: return badstate.io.SCY;
        case 0xFF43: return badstate.io.SCX;
        case 0xFF44: return badstate.io.LY;
        case 0xFF45: return badstate.io.LYC;
        case 0xFF47: return badstate.io.BGP;
        case 0xFF48: return badstate.io.OBP0;
        case 0xFF49: return badstate.io.OBP1;
        case 0xFF4A: return badstate.io.WY;
        case 0xFF4B: return badstate.io.WX;
        case 0xFF50: return 0x1;
        // Interrupts
        case 0xFF0F: return badstate.io.if_reg | 0xE0; // Upper bits 1
            
        default: return 0xFF; // Unused I/O
    }
}