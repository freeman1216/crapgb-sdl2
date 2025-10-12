#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <unistd.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "crapstate.h"
#include "timer.h"
#include "cpu.h"
#include "ppu.h"
#include "defines.h"
#include "mem.h"


int main(int argc,char** argv ){
    if(argc>1){
        
        FILE* cartridge = fopen(argv[1],"rb");
        mem_init(cartridge);
        crapstate_init();
        
        assert(SDL_Init(SDL_INIT_VIDEO) == 0);
        
        SDL_Window *window = SDL_CreateWindow("gbcrapemu", SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED , PIXELS_PER_SCANLINE*2, VISIBLE_SCANLINES*2, SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
        assert(window);
        SDL_SetWindowMinimumSize(window, PIXELS_PER_SCANLINE  , VISIBLE_SCANLINES);

        
	    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        assert(renderer);
        
        assert(SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255) == 0);
        assert(SDL_RenderClear(renderer) == 0);
        
        
        SDL_RenderPresent(renderer);
        
        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB555, SDL_TEXTUREACCESS_STREAMING, PIXELS_PER_SCANLINE, VISIBLE_SCANLINES);
        assert(texture);
        
        crapstate.renderer.SDL_texture = texture;
        crapstate.renderer.SDL_renderer = renderer;
        SDL_Event event;
        while (1) {
            uint32_t milis_start = SDL_GetTicks();
            while(crapstate.display.vblank_counter<=60){
                
                
                update_cpu();
                update_timer(crapstate.cpu.cycles);
                update_ppu(crapstate.cpu.cycles);
                
                
                
                crapstate.cpu.cycles=0;
            }
            crapstate.display.vblank_counter = 0;
            uint32_t delta =  SDL_GetTicks() -  milis_start;
            if(delta < 1000 ){
                usleep((1000 - delta)*1000);
            }
        }
    }
    
    
    return 0;
}
