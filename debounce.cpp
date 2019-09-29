#include "debounce.hpp"

Debounce& Debounce::debounce( int16_t n, uint16_t press_100us, uint16_t release_100us, uint16_t throttle_100us ) {
  profile[n].press_micros = press_100us * 100UL;
  profile[n].release_micros = release_100us * 100UL;
  profile[n].throttle_micros = throttle_100us * 100UL;
  return *this;  
}

Debounce& Debounce::debounce( uint16_t press_100us, uint16_t release_100us, uint16_t throttle_100us ) {
  for ( int16_t i = 0; i < io->numSwitches(); i++ ) {
    debounce( i + 1, press_100us, release_100us, throttle_100us );
  }
  return *this;  
}

// First layer of debouncing (leading and trailing edges)

int16_t Debounce::scan_event( void ) { 
  int16_t code = io->scan_matrix();
  int16_t addr = abs( code );
  if ( code != 0 ) {
    uint32_t millis_passed = micros() - profile[addr].last_change;
    if ( code > 0 ) {
      if ( millis_passed >= profile[addr].press_micros ) {
        return code; // And process 'press'
      } else {
        profile[addr].last_change = micros(); // Update stamp when 'press' state is still unstable    
        return io->reject();
      }    
    } else {
      if ( millis_passed >= profile[addr].release_micros ) {      
        return code;
      } else {
        return io->reject();
      }
    }
  }
  return 0;
}

// Second layer of debouncing (throttling)

int16_t Debounce::scan( void ) { // Handles switch throttling
  int16_t code = scan_event();
  int16_t addr = abs( code );
  if ( code != 0 ) {
    if ( code > 0 ) {
      uint32_t millis_passed = micros() - profile[addr].last_press;
      if ( millis_passed < profile[addr].throttle_micros ) {
        profile[addr].throttling = 1;
        return 0;
      } else {
        profile[addr].last_press = micros();
        return code;
      }
    } else {
      if ( profile[addr].throttling ) {
        profile[addr].throttling = 0;
        return 0;
      } else {
        return code;
      }
    }
  }
  return 0;
}
