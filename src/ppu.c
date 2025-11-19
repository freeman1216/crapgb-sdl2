#include <stdint.h>

#include "ppu.h"
#include "badstate.h"
#include "defines.h"


//internal function to draw a line of the bg to the fb
static inline void __render_bg(){
    uint8_t pix_y = badstate.io.LY + badstate.io.SCY;
    uint8_t tile_y = pix_y >> 3;
    uint8_t y_rem = pix_y & 0x07;
    uint16_t map_base;

    if((badstate.io.LCDC & LCDC_BG_TILE_MAP_MASK) == 0){
        map_base = TILEMAP1_ADDR - VRAM_START + tile_y * 32;
    }else{
        map_base = TILEMAP2_ADDR - VRAM_START + tile_y * 32;
    }
    
    uint8_t draw_x  = PIXELS_PER_SCANLINE - 1;
    uint8_t tile_x = draw_x + badstate.io.SCX;
    uint8_t x_rem = 7 - (tile_x & 0x07);
    
    uint8_t tile_map_index = badstate.mem.vram[map_base + (tile_x >> 3)] ;
    uint16_t tile_index;    
    
    if(badstate.io.LCDC & LCDC_TILESET_MASK){
        tile_index = tile_map_index * 16;
    }else{
        tile_index = TILESET_SIGNED - VRAM_START + (uint16_t)(((int8_t)tile_map_index)*16); 
    }
    
    tile_index += 2 * y_rem;
    uint8_t tb1 = badstate.mem.vram[tile_index];
    uint8_t tb2 = badstate.mem.vram[tile_index+1];
    tb1 >>= x_rem;
    tb2 >>= x_rem;

    while(draw_x!=0xFF)  {
        
        uint8_t palette_index = (tb1 & 0x1)|((tb2&1)<<1);
        uint8_t colour = badstate.display.BGP_indeces[palette_index];
        badstate.display.pixels[badstate.io.LY][draw_x]=badstate.display.palette[colour];
        tb1 >>= 1;
        tb2 >>= 1;            
        draw_x--;
        x_rem++;
        
        if(x_rem == 8){
            x_rem = 0;
            tile_x = draw_x + badstate.io.SCX;
            tile_map_index = badstate.mem.vram[map_base + (tile_x >> 3)] ;   
            
            if(badstate.io.LCDC & LCDC_TILESET_MASK){
                tile_index = tile_map_index * 16;
            }else{
                tile_index = TILESET_SIGNED - VRAM_START + (uint16_t)(((int8_t)tile_map_index)*16); 
            }
            
            tile_index += 2 * y_rem;
            tb1 = badstate.mem.vram[tile_index];
            tb2 = badstate.mem.vram[tile_index+1];
        }
    }
}
//internal function to draw a line of the window to the fb
static inline void __render_window(){
    uint16_t map_base;
    
    if((badstate.io.LCDC & LCDC_WINDOW_TILE_MAP_MASK) == 0){
        map_base = TILEMAP1_ADDR - VRAM_START + (badstate.display.window_curr_line >> 3) * 32;
    }else{
        map_base = TILEMAP2_ADDR - VRAM_START + (badstate.display.window_curr_line >> 3) * 32;
    }
    
    uint8_t draw_x  = PIXELS_PER_SCANLINE - 1;
    uint8_t tile_x = draw_x - badstate.io.WX + 7;
    uint8_t x_rem = 7 - (tile_x & 0x07);
    
    uint8_t tile_map_index = badstate.mem.vram[map_base + (tile_x >> 3)] ;
    uint16_t tile_index;    
    uint8_t y_rem = badstate.display.window_curr_line & 0x07;
    
    if(badstate.io.LCDC & LCDC_TILESET_MASK){
        tile_index = tile_map_index * 16;
    }else{
        tile_index = TILESET_SIGNED - VRAM_START + (uint16_t)(((int8_t)tile_map_index)*16); 
    }
    
    tile_index += 2 * y_rem;
    uint8_t tb1 = badstate.mem.vram[tile_index];
    uint8_t tb2 = badstate.mem.vram[tile_index+1];
    tb1 >>= x_rem;
    tb2 >>= x_rem;
    uint8_t x_last = (badstate.io.WX > 7 ? badstate.io.WX - 7 : 0)-1 ;
    
    while(draw_x!=x_last)  {
        
        uint8_t palette_index = (tb1 & 0x1)|((tb2&1)<<1);
        uint8_t colour = badstate.display.BGP_indeces[palette_index];
        badstate.display.pixels[badstate.io.LY][draw_x]=badstate.display.palette[colour];
        tb1 >>= 1;
        tb2 >>= 1;            
        draw_x--;
        x_rem++;
        
        if(x_rem == 8){
            x_rem = 0;
            tile_x = draw_x  - badstate.io.WX + 7;
            tile_map_index = badstate.mem.vram[map_base + (tile_x >> 3)] ;   
            
            if(badstate.io.LCDC & LCDC_TILESET_MASK){
                tile_index = tile_map_index * 16;
            }else{
                tile_index = TILESET_SIGNED - VRAM_START + (uint16_t)(((int8_t)tile_map_index)*16); 
            }
            
            tile_index += 2 * y_rem;
            tb1 = badstate.mem.vram[tile_index];
            tb2 = badstate.mem.vram[tile_index+1];
        }
    }
    
    badstate.display.window_curr_line++;
}

typedef struct{
    uint8_t y;
    uint8_t x;
    uint8_t tile_index;
    uint8_t attributes;
} sprite_data_t;

typedef struct{
    sprite_data_t *sprite_list[10];
    uint8_t sprite_count;
}sprite_list_t;


//internal function to find a max of 10 sprites to render
static inline void __insert_sprite(sprite_list_t* render_list, sprite_data_t* candidate){
    uint8_t pos = 0;

    for (; pos < render_list->sprite_count; pos++) {
        uint8_t px = render_list->sprite_list[pos]->x;

        if (px > candidate->x) {
            break;
        }

    }
    
    
    if(render_list->sprite_count<SPRITES_PER_SCANLINE){
        for(uint8_t i = render_list->sprite_count; i > pos; --i){
            render_list->sprite_list[i] = render_list->sprite_list[i-1];
        }
        render_list->sprite_list[pos] = candidate;
        render_list->sprite_count++;
    }else{
        if(render_list->sprite_list[render_list->sprite_count-1]->x <= candidate->x){
            return;
        }

        for(uint8_t i = SPRITES_PER_SCANLINE-1; i > pos; --i){
            render_list->sprite_list[i] = render_list->sprite_list[i-1];
        }
        render_list->sprite_list[pos] = candidate;
    }
    
    
    
}
// internal function to render 1 line of sprites to the fb
// finds 10 most prioritised sprites and renders them from least prioritised to most
static inline void __render_sprites(){ 
    sprite_data_t *sprite_view =(sprite_data_t *)badstate.mem.oam;
    sprite_list_t list ;
    list.sprite_count = 0;
    uint8_t added_y= (badstate.io.LCDC & LCDC_OBJ_SIZE_MASK ) ? 8 : 0;
    
    for(uint8_t i = 0; i < 40; i++){
        if(badstate.io.LY+16 - (sprite_view->y) < 8 + added_y && badstate.io.LY+16 >= sprite_view->y ){
            __insert_sprite(&list,sprite_view);
        }
        sprite_view++;
    }

    
    for( ; list.sprite_count != 0x0; list.sprite_count-- ){
        sprite_view = list.sprite_list[list.sprite_count-1];
        
        if(sprite_view->x ==0 || sprite_view->x >= PIXELS_PER_SCANLINE+8){
            continue;
        }
        
        uint8_t tile_index = sprite_view->tile_index & ~((badstate.io.LCDC & 0x04) >> 2);
        uint8_t py = badstate.io.LY+16 - (sprite_view->y);
        
        if(sprite_view->attributes & ATTR_YFLIP_MASK){ // if y flip just fetch the other side of the sprite
            py = (7+added_y) - py;
        }
        
        uint16_t tile = (tile_index * 16) + py*2;
        uint8_t tb1 = badstate.mem.vram[tile];
        uint8_t tb2 = badstate.mem.vram[tile+1];
        uint8_t draw_x;
        uint8_t x_rem = 0;
        uint8_t inc;
        uint8_t pixnum;
        uint8_t palette;
        if(sprite_view->attributes & ATTR_XFLIP_MASK){ // if x flip just render from left to right
            inc = 1;
            draw_x = sprite_view->x >= 8 ? sprite_view->x - 8 : 0;
            x_rem = 8 - sprite_view->x + draw_x;
            pixnum = PIXELS_PER_SCANLINE > draw_x+8 ? 8: PIXELS_PER_SCANLINE - draw_x+8 ; 
        }else{
            inc = -1;
            draw_x = (sprite_view->x >PIXELS_PER_SCANLINE? PIXELS_PER_SCANLINE : sprite_view->x) - 1;
            x_rem = sprite_view->x - (draw_x+1);
            pixnum = 8;
        }
        
        if(sprite_view->attributes & ATTR_PALLETE_MASK){
            palette = 1;
        }else{
            palette = 0;
        }
        
        tb1 >>= x_rem;
        tb2 >>= x_rem;

        if(sprite_view->attributes & ATTR_PRIORITY_MASK){
            uint16_t bg_colour = badstate.display.palette[0]; 
            for (; x_rem < pixnum ; x_rem++) {
                uint8_t palette_index = (tb1 & 1)|((tb2 & 1)<<1);
                if(badstate.display.pixels[badstate.io.LY][draw_x] == bg_colour && palette_index){
                    uint8_t colour = badstate.display.OBP_indeces[palette][palette_index];
                    badstate.display.pixels[badstate.io.LY][draw_x]=badstate.display.palette[colour];
                }
                tb1 >>= 1;
                tb2 >>= 1;            
                draw_x+=inc;   
            }
        }else {
            for (; x_rem < pixnum ; x_rem++) {
                uint8_t palette_index = (tb1 & 1)|((tb2 & 1)<<1);  
                if(palette_index){
                    uint8_t colour = badstate.display.OBP_indeces[palette][palette_index];
                    badstate.display.pixels[badstate.io.LY][draw_x]=badstate.display.palette[colour];
                }
                tb1 >>= 1;
                tb2 >>= 1;            
                draw_x+=inc;
            }
        }
    }
}

//renders the line to the framebuffer
//midscanline effects doesnt work because of the nature of per scanline rendering
static inline void render_line(){
    
    if(badstate.io.LCDC & LCDC_BG_ENABLE_MASK){
        __render_bg();
    }
    
    if((badstate.io.LCDC & BG_WIN_ENABLE_MASK) == BG_WIN_ENABLE_MASK  && badstate.io.WY <= badstate.io.LY && badstate.io.WX <= 166 ){
        
        __render_window();
    }
    
    if(badstate.io.LCDC & LCDC_OBJ_ENABLE_MASK){
        __render_sprites();
    }
    
}

static inline void check_ly_lyc() {
    if (badstate.io.LY == badstate.io.LYC) {
        badstate.io.STAT |= STAT_LYC_MASK; 
        if (badstate.io.STAT & STAT_LYC_INT_ENABLE_MASK) {
            REQUEST_INTERRUPT(INTERRUPT_STAT); 
        }
    } else {
        badstate.io.STAT &= ~STAT_LYC_MASK; // Clear coincidence flag
    }
}

//function used to report the internal ppu state to actual registers
//dont call this on fake vblank mode and disabled mode
static inline void set_lcd_mode(ppu_mode_t mode) {
    badstate.io.STAT = (badstate.io.STAT & ~0x03) | mode;
    badstate.ppu.mode = mode;
    
    // Trigger STAT interrupt if enabled for this mode
    if (mode != MODE3_DRAW) {
        if (badstate.io.STAT & (1 << (mode + 3))) {
            REQUEST_INTERRUPT(INTERRUPT_STAT); 
        }
    }
}

void stop_ppu(){
    badstate.ppu.mode = MODE_DISABLED;
    badstate.io.LY = 0;
    badstate.io.STAT &= ~(STAT_MODE_MASK | STAT_LYC_MASK);  // Clear mode and coincidence
}

void start_ppu(){
    badstate.ppu.mode = MODE2_OAM;
    badstate.ppu.mode_cycles = MODE2_OAM_CYCLES;
    set_lcd_mode(MODE2_OAM);
    check_ly_lyc();  // Need to check because LY was reset
}

static void hblank_handler(){
    if(++badstate.io.LY!=VISIBLE_SCANLINES){
        badstate.ppu.mode = MODE2_OAM;
        badstate.ppu.mode_cycles = MODE2_OAM_CYCLES;
        set_lcd_mode(MODE2_OAM);
    }else{
        badstate.ppu.mode = MODE1_VBLANK;
        badstate.ppu.mode_cycles = CYCLES_PER_SCANLINE;
        set_lcd_mode(MODE1_VBLANK);
        REQUEST_INTERRUPT(INTERRUPT_VBLANK);
        badstate.display.frame_finished=1;
    }
    check_ly_lyc();
   
}

static void vblank_handler(){
    if(++badstate.io.LY==TOTAL_SCANLINES){
        badstate.io.LY = 0;
        badstate.display.window_curr_line = 0;
        badstate.ppu.mode = MODE2_OAM;
        badstate.ppu.mode_cycles = MODE2_OAM_CYCLES;
        set_lcd_mode(MODE2_OAM);
    }else{
        
        badstate.ppu.mode_cycles = CYCLES_PER_SCANLINE;
    }
    check_ly_lyc();
}

//mode in which ppu is after bootrom
static void fake_vblank_hander(){
    badstate.ppu.mode = MODE2_OAM;
    badstate.ppu.mode_cycles = MODE2_OAM_CYCLES-FAKE_VBLANK_CYCLES;
    check_ly_lyc(); //can fire right after bootrom
    set_lcd_mode(MODE2_OAM);
}

static void draw_handler(){
    badstate.ppu.mode = MODE0_HBLANK;
    badstate.ppu.mode_cycles = MODE0_HBLANK_CYCLES;
    set_lcd_mode(MODE0_HBLANK);
}

static void oam_search_hander(){
    badstate.ppu.mode = MODE3_DRAW;
    badstate.ppu.mode_cycles = MODE3_DRAW_CYCLES;
    set_lcd_mode(MODE3_DRAW);
    render_line();
}

static void (* const mode_change_handlers[])  (void) = {
    [MODE0_HBLANK] = hblank_handler,
    [MODE1_VBLANK] = vblank_handler,
    [MODE2_OAM] = oam_search_hander,
    [MODE3_DRAW] = draw_handler, 
    [MODE_FAKE_VBLANK] = fake_vblank_hander
};

//public stepping function 
void update_ppu(uint16_t clocks){

    if(badstate.ppu.mode == MODE_DISABLED){
        return;
    }
    
    while(clocks != 0){
        
        uint16_t step = (clocks > badstate.ppu.mode_cycles) ?  badstate.ppu.mode_cycles : clocks; 
        badstate.ppu.mode_cycles-= step;
        clocks -= step;
        
        if(badstate.ppu.mode_cycles==0){
            mode_change_handlers[badstate.ppu.mode]();
        }
    }
    
}