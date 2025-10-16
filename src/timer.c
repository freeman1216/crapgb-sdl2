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
    if (crapstate.io.TAC & TIMA_ENABLE_MASK) { // Timer enabled
        uint8_t tac = crapstate.io.TAC & TIMA_CLOCK_SELECT_MASK;
        uint16_t threshold = (tac == 0) ? 1024 : // 4096 Hz
                            (tac == 1) ? 16 :   // 262144 Hz
                            (tac == 2) ? 64 :   // 65536 Hz
                            256;                // 16384 Hz
        
        crapstate.timing.tima_cycles += cycles;
        while (crapstate.timing.tima_cycles >= threshold) {
            crapstate.timing.tima_cycles -= threshold;
            if (crapstate.io.TIMA == 0xFF) {
                crapstate.io.TIMA = crapstate.io.TMA; 
                REQUEST_INTERRUPT(INTERRUPT_TIMER); 
            } else {
                crapstate.io.TIMA++;
            }
        }
    }
}