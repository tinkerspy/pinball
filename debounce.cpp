#include "debounce.hpp"

Debounce& Debounce::begin( IO& io ) {
  this->io = &io;
  return *this;  
}

Debounce& Debounce::debounce( int16_t n, uint16_t make_delay, uint16_t break_delay, uint16_t throttle_delay ) {
  sw[n].make_delay = make_delay;
  sw[n].break_delay = break_delay;
  sw[n].throttle_delay = throttle_delay;
  return *this;  
}

Debounce& Debounce::debounce( uint16_t make_delay, uint16_t break_delay, uint16_t throttle_delay ) {
  for ( int16_t i = 0; i < io->numSwitches(); i++ ) {
    debounce( i + 1, make_delay, break_delay, throttle_delay );
  }
  return *this;  
}

int16_t Debounce::scan( void ) {
  int16_t c = io->scan();
  int16_t a = abs( c );

  return c; // For now!
  
  if ( c == 0 || ( sw[a].make_delay == 0 && sw[a].break_delay == 0 && sw[a].throttle_delay == 0 ) ) {
    return c;
  }  
  if ( c > 0 ) {
    uint16_t millis_passed = (uint16_t) millis() - sw[a].last_change;
    if ( millis_passed >= sw[a].make_delay ) {
      millis_passed = (uint16_t) millis() - sw[a].last_press;
      if ( millis_passed >= sw[a].throttle_delay ) {
        sw[a].last_press = millis(); // Set stamp when 'make' state becomes stable
        return c; // And process 'make'
      }
    } else {
      sw[a].last_change = millis(); // Set stamp when 'make' state is still unstable    
    }    
  } else {
    uint16_t millis_passed = (uint16_t) millis() - sw[a].last_press;
    if ( millis_passed >= ( sw[a].make_delay + sw[a].break_delay ) ) {      
      return c;
    }    
  }
  // Ignore this event 
  io->unscan();
  return 0;
}
