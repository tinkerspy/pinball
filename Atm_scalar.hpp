#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"

class Atm_scalar: public Machine {

 public:
  enum { IDLE, NEXT, RESET, COLLECT, COLLOOP, WAIT, COLRPT }; // STATES
  enum { EVT_ZERO, EVT_COUNTER, EVT_TIMER, EVT_ADVANCE, EVT_RESET, EVT_COLLECT, ELSE }; // EVENTS
  Atm_scalar( void ) : Machine() {};
  Atm_scalar& begin(  Atm_led_scheduler& leds, int16_t led_group, int16_t def, int16_t max, bool fill = false );
  Atm_scalar& trace( Stream & stream );
  Atm_scalar& trigger( int event );
  int state( void );
  Atm_scalar& advance( void );
  Atm_scalar& reset( void );
  Atm_scalar& multiplier( uint8_t n );
  Atm_scalar& collect();
  Atm_scalar& lock( bool v = true ); // Locks EVT_ADVANCE event
  Atm_scalar& set( int16_t n );
  Atm_scalar& onCollect( Machine& machine, int event = 0 );
  Atm_scalar& onCollect( atm_cb_push_t callback, int idx = 0 );

 private:
  enum { ENT_NEXT, ENT_RESET, ENT_COLLECT, ENT_COLLOOP, ENT_COLRPT }; // ACTIONS
  enum { ON_COLLECT, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  bool lock_advance = 0;
  Atm_led_scheduler* leds;
  int16_t max, def, led_group;
  bool fill_mode = false;
  atm_timer_millis timer;
  int8_t multiply = 1;
  int16_t value, countdown, countmulti;
};
