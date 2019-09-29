#pragma once

#include <arduino.h>
#include "io.hpp"

#define NUM_IOPORTS 5
#define MATRIX_NODES 8
#define MATRIX_SWITCHES 8

// Let op: profiles & groups behoren tot de realm van het playfield!
// Playfield roept debounce() aan
// The led T1/L1 interval is also relevant here! (flippers!)
// Allocate profile memory dynamically according to io->numSwitches()

class Debounce {

// The use of 16 bit unsigned integers & millis gives a maximum switch delay of 65 seconds
// The use of 16 bit unsigned integers & micros gives a maximum switch delay of 65 milliseconds (may be acceptable for press/release delays???)
// We may need the precision of micros() for effective edge-debouncing (1ms is perhaps too coarse, but 32000 us - 32 ms is too limited for throttling))
// Move to 32 bit fields for timestamps, specify delays in 100s of us (range 0.0001 - 3.2 seconds) for switches as well as throttle delays


struct switch_record { 
    uint8_t throttling : 1;
    uint32_t last_change;
    uint32_t last_press;
    uint32_t press_micros; // press must be stable for this period to register (if not: unscan event)
    uint32_t release_micros; // minimum wait period after registered press for a break (if not: unscan event)
    uint32_t throttle_micros; // after a registered press/release wait this long before allowing a new press/release
};
 
  public:
    Debounce& begin( IO& io );
    Debounce& debounce( int16_t n, uint16_t press_100us, uint16_t release_100us, uint16_t throttle_100us );
    Debounce& debounce( uint16_t press_100us, uint16_t release_100us, uint16_t throttle_100us );
    int16_t scan_event( void );
    int16_t scan( void );

  protected:
    switch_record profile[NUM_IOPORTS * MATRIX_NODES * MATRIX_SWITCHES + 1];
    IO* io;
    
};
