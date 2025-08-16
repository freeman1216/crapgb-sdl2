#include "ppu.h"
#include "crapstate.h"
#include "defines.h"
#include <stdint.h>

#define REQUEST_INTERRUPT(bit)  crapstate.io.if_reg |= (1 << bit) // IF register
    
static inline void check_ly_lyc() {
    if (crapstate.io.LY == crapstate.io.LYC) {
        crapstate.io.STAT |= (1 << 2); // Set coincidence flag
        if (crapstate.io.STAT & (1 << 6)) {
            REQUEST_INTERRUPT(STAT_IF_BIT); // STAT interrupt
        }
    } else {
        crapstate.io.STAT &= ~(1 << 2); // Clear coincidence flag
    }
}

static inline void set_lcd_mode(uint8_t mode) {
    crapstate.io.STAT = (crapstate.io.STAT & ~0x03) | mode;
    crapstate.ppu.mode = mode;
    
    // Trigger STAT interrupt if enabled for this mode
    if (mode <= 2) {
        if (crapstate.io.STAT & (1 << (mode + 3))) {
            REQUEST_INTERRUPT(STAT_IF_BIT); // STAT interrupt
        }
    }
}

void stop_ppu(){
    crapstate.ppu.mode = MODE_DISABLED;
    crapstate.io.LY = 0;
    crapstate.io.STAT &= ~(0x03 | (1 << 2));  // Clear mode and coincidence
}

void start_ppu(){
    crapstate.ppu.mode = 2;
    crapstate.ppu.mode_cycles = MODE2_OAM_CYCLES;
    crapstate.io.LY = 0;
    check_ly_lyc();  // Need to check because LY was reset
}

static void hblank_handler(){
    if(++crapstate.io.LY!=VBLANK_START_LINE){
        crapstate.ppu.mode = MODE2_OAM;
        crapstate.ppu.mode_cycles = MODE2_OAM_CYCLES;
        set_lcd_mode(MODE2_OAM);
    }else {
        crapstate.ppu.mode = MODE1_VBLANK;
        crapstate.ppu.mode_cycles = CYCLES_PER_SCANLINE;
        set_lcd_mode(MODE1_VBLANK);
        
    }
    check_ly_lyc();
}

static void vblank_handler(){
    if(++crapstate.io.LY==TOTAL_SCANLINES){
        crapstate.io.LY = 0;
        crapstate.ppu.mode = MODE2_OAM;
        crapstate.ppu.mode_cycles = MODE2_OAM_CYCLES;
        crapstate.io.LY = 0;
        REQUEST_INTERRUPT(VBLANK_IF_BIT);
        set_lcd_mode(MODE2_OAM);
    }else{
        crapstate.ppu.mode_cycles = CYCLES_PER_SCANLINE;
    }
    check_ly_lyc();
}

//mode in which ppu is after bootrom
static void fake_vblank_hander(){

    crapstate.ppu.mode= MODE2_OAM;
    crapstate.ppu.mode_cycles =MODE2_OAM_CYCLES;
    check_ly_lyc(); //can fire right after bootrom
    set_lcd_mode(MODE2_OAM);

}

static void draw_handler(){
    crapstate.ppu.mode = MODE0_HBLANK;
    crapstate.ppu.mode_cycles = MODE0_HBLANK_CYCLES;
    set_lcd_mode(MODE0_HBLANK);

}

static void oam_search_hander(){
    crapstate.ppu.mode = MODE3_DRAW;
    crapstate.ppu.mode_cycles = MODE3_DRAW_CYCLES;
    set_lcd_mode(MODE3_DRAW);

}

static void(*mode_change_handlers[]) (void) = {
    [MODE0_HBLANK] = hblank_handler,
    [MODE1_VBLANK] = vblank_handler,
    [MODE2_OAM] = oam_search_hander,
    [MODE3_DRAW] = draw_handler, 
    [MODE_FAKE_VBLANK] = fake_vblank_hander
};



void update_ppu(uint16_t clocks){
    if(crapstate.ppu.mode == MODE_DISABLED){
        return;
    }
    
    while(clocks != 0){
    
        uint16_t step = (clocks > crapstate.ppu.mode_cycles) ?  crapstate.ppu.mode_cycles : clocks; 
        crapstate.ppu.mode_cycles-= step;
        clocks -= step;

        if(crapstate.ppu.mode_cycles==0){
            mode_change_handlers[crapstate.ppu.mode]();
        }
    }
    
}