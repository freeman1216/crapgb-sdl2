#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "badstate.h"
#include "timer.h"
#include "cpu.h"
#include "ppu.h"
#include "defines.h"
#include "mem.h"

int main(int argc,char** argv ){
    if(argc<1){
        return -1;
    }
    FILE* cartridge = fopen(argv[1],"rb");
    if(cartridge == NULL){
        BADLOG("Cannot open cartridge\n");
        return -1;
    }
    if(mem_init(cartridge)){
        return -1;
    }

    badstate_init();
    
    assert(SDL_Init(SDL_INIT_VIDEO) == 0);
    
    SDL_Window *window = SDL_CreateWindow("gbbademu", SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED , PIXELS_PER_SCANLINE*2, VISIBLE_SCANLINES*2, SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
    assert(window);
    SDL_SetWindowMinimumSize(window, PIXELS_PER_SCANLINE  , VISIBLE_SCANLINES);
    
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    assert(renderer);
    
    assert(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) == 0);
    assert(SDL_RenderClear(renderer) == 0);
    
    
    SDL_RenderPresent(renderer);
    
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB555, SDL_TEXTUREACCESS_STREAMING, PIXELS_PER_SCANLINE, VISIBLE_SCANLINES);
    assert(texture);
    
    SDL_Event event;

    SDL_RenderSetLogicalSize(renderer, PIXELS_PER_SCANLINE, VISIBLE_SCANLINES);
	SDL_RenderSetIntegerScale(renderer, 1);
    
    while (1) {
    
    
        uint8_t vblank_counter = 0;
        uint32_t milis_start = SDL_GetTicks();
        while (vblank_counter<=60) {
        
        
            while(SDL_PollEvent(&event)){
                switch (event.type) {
                    case SDL_QUIT:{

                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_DestroyTexture(texture);
                        SDL_Quit();

                        return 0;
                        break;
                    }
                    case SDL_KEYUP: {
                        switch (event.key.keysym.sym) {
                            case SDLK_RETURN:{
                                badstate.buttons.start = 0;
                                break;
                            }
                            case SDLK_TAB:{
                                badstate.buttons.select = 0;
                                break;
                            }
                            case SDLK_z:{
                                badstate.buttons.A = 0;
                                break;
                            }
                            case SDLK_x:{
                                badstate.buttons.B = 0;
                                break;
                            }
                            case SDLK_a:{
                                badstate.buttons.left = 0;
                                break;
                            }
                            case SDLK_s:{
                                badstate.buttons.down = 0;
                                break;
                            }
                            case SDLK_d:{
                                badstate.buttons.right = 0;
                                break;
                            }
                            case SDLK_w:{
                                badstate.buttons.up = 0;
                                break;
                            }
                        }
                        break;
                    }
                    case SDL_KEYDOWN:{
                        switch (event.key.keysym.sym) {
                            case SDLK_RETURN:{
                                badstate.buttons.start = 1;
                                flag_joypad_interrupt_buttons();
                                break;
                            }
                            case SDLK_TAB:{
                                badstate.buttons.select = 1;
                                flag_joypad_interrupt_buttons();
                                break;
                            }
                            case SDLK_z:{
                                badstate.buttons.A = 1;
                                flag_joypad_interrupt_buttons();
                                break;
                            }
                            case SDLK_x:{
                                badstate.buttons.B = 1;
                                flag_joypad_interrupt_buttons();
                                break;
                            }
                            case SDLK_a:{
                                badstate.buttons.left = 1;
                                flag_joypad_interrupt_dpad();
                                break;
                            }
                            case SDLK_s:{
                                badstate.buttons.down = 1;
                                flag_joypad_interrupt_dpad();
                                break;
                            }
                            case SDLK_d:{
                                badstate.buttons.right = 1;
                                flag_joypad_interrupt_dpad();
                                break;
                            }
                            case SDLK_w:{
                                badstate.buttons.up = 1;
                                flag_joypad_interrupt_dpad();
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            while (badstate.display.frame_finished!=1) {
                
                update_cpu();
                update_timer(badstate.cpu.cycles);
                update_ppu(badstate.cpu.cycles);
                badstate.cpu.cycles=0;
                
            }
            assert(SDL_UpdateTexture(texture, NULL, &badstate.display.pixels, PIXELS_PER_SCANLINE * sizeof(uint16_t))==0);
            
            assert( SDL_RenderClear(renderer)==0);
            assert(SDL_RenderCopy(renderer, texture, NULL, NULL)==0);
            SDL_RenderPresent(renderer);

            vblank_counter++;
            badstate.display.frame_finished = 0;
        }
        vblank_counter = 0;
        uint32_t delta =  SDL_GetTicks() -  milis_start;
        if(delta < 1000 ){
            usleep((1000 - delta)*1000);
        }
    }
    return 0;
}
