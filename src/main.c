#include "cpu.h"
#include "defines.h"
#include "mem.h"
#include <stdint.h>
#include <stdio.h>
#include "crapstate.h"
#include "timer.h"
#include "cpu.h"
#include "ppu.h"



int main(int argc,char** argv ){
    if(argc>1){
        FILE* cartridge = fopen(argv[1],"rb");
        mem_init(cartridge);
        crapstate_init();
        
        while(1){
            update_cpu();
            update_timer(crapstate.cpu.cycles);
            update_ppu(crapstate.cpu.cycles);
            crapstate.cpu.cycles=0;
        }
    }
    
    
    return 0;
}
