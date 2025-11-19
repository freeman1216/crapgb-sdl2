#include "badstate.h"
#include "timer.h"
#include "defines.h"

void update_timer(uint16_t cycles) {
    
    
    // DIV register (always running)
    badstate.timing.div_cycles+= cycles;
    while (badstate.timing.div_cycles >= 256) { // 16384 Hz
        badstate.timing.div_cycles -= 256;
        badstate.io.DIV++; // DIV
    }
    
    // TIMA register (controlled by TAC)
    if (badstate.io.TAC & TIMA_ENABLE_MASK) { // Timer enabled
        uint8_t tac = badstate.io.TAC & TIMA_CLOCK_SELECT_MASK;
        uint16_t threshold = (tac == 0) ? 1024 : // 4096 Hz
                            (tac == 1) ? 16 :   // 262144 Hz
                            (tac == 2) ? 64 :   // 65536 Hz
                            256;                // 16384 Hz
        
        badstate.timing.tima_cycles += cycles;
        while (badstate.timing.tima_cycles >= threshold) {
            badstate.timing.tima_cycles -= threshold;
            if (badstate.io.TIMA == 0xFF) {
                badstate.io.TIMA = badstate.io.TMA; 
                REQUEST_INTERRUPT(INTERRUPT_TIMER); 
            } else {
                badstate.io.TIMA++;
            }
        }
    }
}