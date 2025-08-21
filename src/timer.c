#include "crapstate.h"
#include "timer.h"
#include "defines.h"

void update_timer(uint16_t cycles) {
    
    
    // DIV register (always running)
    crapstate.timing.div_cycles+= cycles;
    while (crapstate.timing.div_cycles >= 256) { // 16384 Hz
        crapstate.timing.div_cycles -= 256;
        crapstate.io.DIV++; // DIV
    }
    
    // TIMA register (controlled by TAC)
    if (crapstate.io.TAC & 0x04) { // Timer enabled
        uint8_t tac = crapstate.io.TAC & 0x03;
        uint16_t threshold = (tac == 0) ? 1024 : // 4096 Hz
                            (tac == 1) ? 16 :   // 262144 Hz
                            (tac == 2) ? 64 :   // 65536 Hz
                            256;                // 16384 Hz
        
        crapstate.timing.tima_cycles += cycles;
        while (crapstate.timing.tima_cycles >= threshold) {
            crapstate.timing.tima_cycles -= threshold;
            if (crapstate.io.TIMA == 0xFF) {
                crapstate.io.TIMA = crapstate.io.TMA; // TMA
                crapstate.io.if_reg|= (1<<TIMA_IF_BIT); // Timer interrupt
            } else {
                crapstate.io.TIMA++;
            }
        }
    }
}