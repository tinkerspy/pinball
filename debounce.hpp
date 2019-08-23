#pragma once

#include <arduino.h>
#include "io.hpp"

#define NUM_IOPORTS 5
#define MATRIX_NODES 8
#define MATRIX_SWITCHES 8

class Debounce {

struct switch_record { 
    uint8_t make_wait : 1;
    uint8_t switch_state : 1;
    uint8_t init : 1;
    uint16_t last_change;
    uint16_t last_press;
};

  public:
    bool debounce( int16_t n, bool press, uint16_t make_delay, uint16_t break_delay, uint16_t throttle_delay );     

  protected:
    switch_record sw[NUM_IOPORTS * MATRIX_NODES * MATRIX_SWITCHES + 1];
    

};

