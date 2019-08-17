#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"

class Atm_scalar: public Machine {

 public:
  enum { IDLE, NEXT, RESET, COLLECT, WAIT }; // STATES
  enum { EVT_ZERO, EVT_COUNTER, EVT_TIMER, EVT_ADVANCE, EVT_RESET, EVT_COLLECT, ELSE }; // EVENTS
  Atm_scalar( void ) : Machine() {};
  Atm_scalar& begin(  Atm_led_scheduler& leds, int16_t led_group, int16_t def, int16_t max, bool fill = false );
  Atm_scalar& trace( Stream & stream );
  Atm_scalar& trigger( int event );
  int state( void );
  Atm_scalar& advance( void );
  Atm_scalar& reset( void );
  Atm_scalar& collect( void );
  Atm_scalar& lock( bool v = true ); // Locks EVT_ADVANCE event
 
 private:
  enum { ENT_NEXT, ENT_RESET, ENT_COLLECT }; // ACTIONS
  int event( int id ); 
  void action( int id ); 
  bool lock_advance = 0;
  Atm_led_scheduler* leds;
  int16_t max, def, led_group;
  bool fill_mode = false;
  atm_timer_millis timer;
};
