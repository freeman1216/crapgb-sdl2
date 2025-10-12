#include "renderer.h"
#include "assert.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include "crapstate.h"

void display_framebuffer(){
    assert(SDL_UpdateTexture(crapstate.renderer.SDL_texture, NULL, &crapstate.display.pixels, PIXELS_PER_SCANLINE * sizeof(uint16_t))==0);
    
    assert( SDL_RenderClear(crapstate.renderer.SDL_renderer)==0);
    assert(SDL_RenderCopy(crapstate.renderer.SDL_renderer, crapstate.renderer.SDL_texture, NULL, NULL)==0);
    SDL_RenderPresent(crapstate.renderer.SDL_renderer);
}