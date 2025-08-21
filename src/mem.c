#include "mem.h"
#include "defines.h"
#include <stdint.h>
#include <stdio.h>
#include "crapstate.h"
#include "ppu.h"

static void inline io_write(uint16_t addr, uint8_t val);
static uint8_t inline io_read(uint16_t addr);
void mem_init(FILE* rom){
    uint32_t res =fread(crapstate.mem.rom0,1,ROM0_SIZE,rom);
    res = fread(crapstate.mem.romx,1 , ROMX_SIZE, rom);
    crapstate.mem.rom = rom;
}

uint8_t mem_read_byte(uint16_t addr){
    if(addr<ROM0_SIZE){
        return crapstate.mem.rom0[addr-ROM0_START];
    }else if(addr>=ROMX_START && addr<ROMX_START+ROMX_SIZE){
        return crapstate.mem.romx[addr-ROMX_START];
    }else if(addr>=WRAM0_START&& addr< WRAMX_START+WRAMX_SIZE){
        return crapstate.mem.wram[addr-WRAM0_START];   
    }else if(addr>=VRAM_START && addr <VRAM_START+VRAM_SIZE){
        return crapstate.mem.vram[addr-VRAM_START];
    }else if(addr>=OAM_START && addr <OAM_START+OAM_SIZE){
        return crapstate.mem.oam[addr-OAM_START];
    }
    else if(addr>=IOREGS_START && addr <IOREGS_START+IOREGS_SIZE){
        return io_read(addr);
    }else if(addr>=HRAM_START && addr<HRAM_START+HRAM_SIZE){
        return crapstate.mem.hram[addr-HRAM_START];
    }
    else if(addr>=IE_REG_ADDR){
       return io_read(addr);
    } 
    return 0;
};

uint16_t mem_read_word(uint16_t addr) {
    uint8_t low = mem_read_byte(addr);
    uint8_t high = mem_read_byte(addr + 1);
    return (uint16_t)(low) | ((uint16_t)(high) << 8);
}

void handle_bank_switch(uint8_t value) {
    // Validate bank number (0 becomes 1)
    uint8_t bank = value & 0x1F;  // MBC1 uses 5 bits
    if(bank == 0) bank = 1;
    

    if(bank != crapstate.mem.currentromx) {
        if(fseek(crapstate.mem.rom, ROMX_SIZE * (bank), SEEK_SET) != 0) {
            printf("Bank seek failed!\n");
            return;
        }
        
        size_t read = fread(crapstate.mem.romx, 1, ROMX_SIZE, crapstate.mem.rom);
        if(read != ROMX_SIZE) {
            printf("Bank read incomplete! (%zu/%d bytes)\n", read, ROMX_SIZE);
        }
        
        crapstate.mem.currentromx = bank;
        printf("Switched to ROM bank %d\n", bank);
    }
}

void mem_write_byte(uint16_t addr, uint8_t value){
    if(addr<ROM0_SIZE){
        if(addr==0x2000){
            printf("switiching to bank %x\n",value);
            handle_bank_switch(value);
        }else{
            printf("wrote to rom wtf\n ");
        }
        return;
    }
    else if(addr>=WRAM0_START  && addr<WRAM0_START+WRAM0_SIZE){
        crapstate.mem.wram[addr-WRAM0_START] = value; 
        
        if(addr == 0xC000){
            printf("0xc000 %x changed\n",value);
        }
        return;
    }
    else if(addr>=WRAMX_START && addr<WRAMX_START+WRAMX_SIZE){
        crapstate.mem.wram[addr-WRAM0_START] = value;
        return;
    }
    else if(addr>=VRAM_START && addr <VRAM_START+VRAM_SIZE){
        crapstate.mem.vram[addr-VRAM_START] = value;
        return;
    }
    else if(addr>=OAM_START && addr <OAM_START+OAM_SIZE){
        crapstate.mem.oam[addr-OAM_START] = value;
        return;
    }
    else if(addr>=IOREGS_START && addr <IOREGS_START+IOREGS_SIZE){
        io_write(addr, value);
        printf("wrote to io %x at addr %x\n",value,addr);
        return;

    }else if(addr>=HRAM_START && addr<HRAM_START+HRAM_SIZE){
        crapstate.mem.hram[addr-HRAM_START] = value;
    }
    else if(addr>=IE_REG_ADDR){
        io_write(addr, value);
        printf("wrote to enable interrupt %x\n",value);
        return;
    }   
    
}
void mem_write_word(uint16_t addr, uint16_t value){
    
    mem_write_byte(addr, value & 0xFF);         // write low byte
    mem_write_byte(addr + 1, (value >> 8) & 0xFF); // write high byte

}



static void  inline io_write(uint16_t addr, uint8_t val) {
    switch (addr) {
        // Joypad
        case 0xFF00: 
            crapstate.io.P1 = (crapstate.io.P1 & 0xCF) | (val & 0x30);
            break;
            
        // Serial
        case 0xFF01: crapstate.io.SB = val; break;
        case 0xFF02: crapstate.io.SC = val & 0x83; break; // Only bits 0-1,7 writable
        
        // Timer
        case 0xFF04: crapstate.io.DIV = 0; break; // Any write resets
        case 0xFF05: crapstate.io.TIMA = val; break;
        case 0xFF06: crapstate.io.TMA = val; break;
        case 0xFF07: crapstate.io.TAC = val & 0x07; break; // Only bits 0-2 matter
        
        // LCD
        case 0xFF40: {    
            uint8_t old_lcdc = crapstate.io.LCDC;
            crapstate.io.LCDC = val;

            if ((old_lcdc ^ val) & 0x80) {  // Only if enable bit changed
                if (val & 0x80) {
                    start_ppu();
                } else {
                    stop_ppu();
                }
            }
        }   break;
        case 0xFF41: crapstate.io.STAT = (crapstate.io.STAT & 0x03) | (val & 0xFC); break;
        case 0xFF42: crapstate.io.SCY = val; break;
        case 0xFF43: crapstate.io.SCX = val; break;
        case 0xFF45: crapstate.io.LYC = val; break;
        case 0xFF46: {
            uint16_t src = val << 8; // Source address (XX00-XX9F)
            
    
            // Unrolled copy (20% faster on ARM)
            for (int i = 0; i < 160; i ++) {
                crapstate.mem.oam[i]   = mem_read_byte(src + i);
            }
    
            crapstate.cpu.cycles += 640;

        } break; // Special DMA case
        case 0xFF47: crapstate.io.BGP = val; break;
        case 0xFF48: crapstate.io.OBP0 = val; break;
        case 0xFF49: crapstate.io.OBP1 = val; break;
        case 0xFF4A: crapstate.io.WY = val; break;
        case 0xFF4B: crapstate.io.WX = val; break;
        
        // Interrupts
        case 0xFF0F: crapstate.io.if_reg = val & 0x1F; break;
        case 0xFFFF: crapstate.io.ie = val & 0x1F; break;
            
        default: break; // Ignore writes to unused
    }
}
static inline uint8_t read_joypad() {
    uint8_t result = 0xF;  // Default: no buttons pressed
    
    // Check direction buttons (if P1.4=0)
    if (!(crapstate.io.P1 & 0x10)) {
        if (crapstate.buttons.right)  result &= ~(1 << 0);
        if (crapstate.buttons.left)   result &= ~(1 << 1);
        if (crapstate.buttons.up)     result &= ~(1 << 2);
        if (crapstate.buttons.down)   result &= ~(1 << 3);
    }
    
    // Check action buttons (if P1.5=0)
    if (!(crapstate.io.P1 & 0x20)) {
        if (crapstate.buttons.A)      result &= ~(1 << 0);
        if (crapstate.buttons.B)      result &= ~(1 << 1);
        if (crapstate.buttons.start)  result &= ~(1 << 2);
        if (crapstate.buttons.select) result &= ~(1 << 3);
    }
    
    return (crapstate.io.P1 & 0xF0) | result;
}

static uint8_t inline io_read(uint16_t addr) {
    switch (addr) {
        // Joypad
        case 0xFF00: return read_joypad();
            
        // Serial
        case 0xFF01: return crapstate.io.SB;
        case 0xFF02: return crapstate.io.SC | 0x7E; // Bit 7 unused
        
        // Timer
        case 0xFF04: return crapstate.io.DIV;
        case 0xFF05: return crapstate.io.TIMA;
        case 0xFF06: return crapstate.io.TMA;
        case 0xFF07: return crapstate.io.TAC | 0xF8; // Upper bits 1
        
        // LCD
        case 0xFF40: return crapstate.io.LCDC;
        case 0xFF41: return crapstate.io.STAT | 0x80; // Bit 7 always 1
        case 0xFF42: return crapstate.io.SCY;
        case 0xFF43: return crapstate.io.SCX;
        case 0xFF44: return crapstate.io.LY;
        case 0xFF45: return crapstate.io.LYC;
        case 0xFF47: return crapstate.io.BGP;
        case 0xFF48: return crapstate.io.OBP0;
        case 0xFF49: return crapstate.io.OBP1;
        case 0xFF4A: return crapstate.io.WY;
        case 0xFF4B: return crapstate.io.WX;
        case 0xFF50: return 0x1;
        // Interrupts
        case 0xFF0F: return crapstate.io.if_reg | 0xE0; // Upper bits 1
        case 0xFFFF: return crapstate.io.ie;
            
        default: return 0xFF; // Unused I/O
    }
}