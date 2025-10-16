#pragma once
#include <stdint.h>
#include "defines.h"
#include "ppu.h"
#include "stdio.h"

#ifdef ENABLE_GRAPHICS
#include <SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#endif

typedef struct {
    // CPU Core
    struct {
        // Main registers
        union{
            struct{
                union{
                    struct{ 
                        uint8_t unused : 4; // lower 4 bits are always zero in F
                        uint8_t C : 1;      // Carry flag (bit 4)
                        uint8_t H : 1;      // Half Carry flag (bit 5)
                        uint8_t N : 1;      // Subtract flag (bit 6)
                        uint8_t Z : 1;
                    };
                uint8_t f;
                };
            uint8_t a;
            };
        uint16_t af;
        };
        union { uint16_t bc; struct { uint8_t c, b; }; };
        union { uint16_t de; struct { uint8_t e, d; }; };
        union { uint16_t hl; struct { uint8_t l, h; }; };
        
        // Control registers
        uint16_t pc;
        uint16_t sp;
        
        // CPU flags
        uint8_t halted : 1;
        uint8_t ime : 1;       // Interrupt Master Enable
        uint8_t ime_pending : 1; // For EI delay
        uint8_t _reserved : 5;
        uint16_t cycles;
    } cpu;

    // Memory
    struct {
        // ROM/RAM 
        uint8_t rom0[ROM0_SIZE];
        uint8_t romx[ROMX_SIZE];
        uint8_t currentromx;
        FILE* rom ;         
        uint8_t wram[WRAM0_SIZE+WRAMX_SIZE]; // 8KB Work RAM
        uint8_t hram[HRAM_SIZE];    // High RAM
        
        
        // Graphics
        uint8_t vram[VRAM_SIZE]; // VRAM
        
        uint8_t oam[OAM_SIZE] __attribute__((aligned(4)));     // Sprite RAM
        
       
    } mem;
        // I/O Ports (mapped to 0xFF00-0xFF7F)
    struct {
        uint8_t P1;   // FF00
        uint8_t SB;   // FF01
        uint8_t SC;   // FF02
        uint8_t DIV;  // FF04
        uint8_t TIMA; // FF05
        uint8_t TMA;  // FF06
        uint8_t TAC;  // FF07
        uint8_t LCDC; // FF40
        uint8_t STAT; // FF41
        uint8_t SCY;  // FF42
        uint8_t SCX;  // FF43
        uint8_t LY;   // FF44
        uint8_t LYC;  // FF45
        uint8_t BGP;  // FF47
        uint8_t OBP0; // FF48
        uint8_t OBP1; // FF49
        uint8_t WY;   // FF4A
        uint8_t WX;   // FF4B
        uint8_t ie;          // 0xFFFF (Interrupt Enable)
        uint8_t if_reg;       // 0xFF0F (Interrupt Flags)
    } io;
    // Timing
    struct {
        uint16_t globalcycles;
        uint16_t div_cycles;
        uint16_t tima_cycles;
    } timing;

    // PPU State
    struct {
        ppu_mode_t mode;        // 0-5 (HBlank, VBlank, etc.) not the actual reported value in the register 
        uint16_t mode_cycles; // Cycles in current mode
    } ppu;

    struct{
        uint16_t pixels[VISIBLE_SCANLINES][PIXELS_PER_SCANLINE];
        uint16_t palette[4];
        uint8_t OBP_indeces[2][4];
        uint8_t BGP_indeces[4] ;
        uint8_t window_curr_line;
        uint8_t vblank_counter;

    } display;

#ifdef ENABLE_GRAPHICS
    struct{
        SDL_Renderer* SDL_renderer;
        SDL_Texture*  SDL_texture;
       
    }renderer;
#endif

    struct {
        uint8_t right:1, left:1, up:1, down:1;
        uint8_t A:1, B:1, start:1, select:1;
    } buttons;
} crapstate_t;

extern crapstate_t crapstate;
void crapstate_init();
// Global instance
