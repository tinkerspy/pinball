#pragma once

#include <arduino.h>
#include "io.hpp"

#define NUM_IOPORTS 5
#define MATRIX_NODES 8
#define MATRIX_SWITCHES 8

// Let op: profiles & groups behoren tot de realm van het playfield!
// Playfield roept debounce() aan

class Debounce {

struct switch_record { 
    uint8_t make_wait : 1;
    uint8_t switch_state : 1;
    uint8_t init : 1;
    uint16_t last_change;
    uint16_t last_press;
    uint16_t make_delay; 
    uint16_t break_delay; 
    uint16_t throttle_delay
;};

  public:
    Debounce& begin( IO& io );
    Debounce& debounce( int16_t n, uint16_t make_delay, uint16_t break_delay, uint16_t throttle_delay );
    Debounce& debounce( uint16_t make_delay, uint16_t break_delay, uint16_t throttle_delay );
    int16_t scan( void );

  protected:
    switch_record sw[NUM_IOPORTS * MATRIX_NODES * MATRIX_SWITCHES + 1];
    IO* io;
    
};
