#include "debounce.hpp"

bool Debounce::debounce( int16_t n, bool press, uint16_t make_delay, uint16_t break_delay, uint16_t throttle_delay ) {

  if ( press ) { // make
    uint16_t millis_passed = (uint16_t) millis() - sw[n].last_change;
    if ( millis_passed >= make_delay ) {
      millis_passed = (uint16_t) millis() - sw[n].last_press;
      if ( millis_passed >= throttle_delay ) {
        sw[n].last_press = millis(); // Set stamp when 'make' state becomes stable
        return true; // And process 'make'
      }
    } else {
      sw[n].last_change = millis(); // Set stamp when 'make' state is still unstable    
    }
  } else { // break
    uint16_t millis_passed = (uint16_t) millis() - sw[n].last_press;
    if ( millis_passed >= ( make_delay + break_delay ) ) {      
      return true;
    }
  }
  return false; // Ignore this event
}

