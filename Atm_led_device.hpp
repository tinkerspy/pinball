#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

#define MAX_EVENTS 64 

class Atm_led_device: public Machine {

 public:
  enum { IDLE, NOTIFY }; // STATES
  enum { EVT_NOTIFY, ELSE }; // EVENTS
  Atm_led_device( void ) : Machine() {};
  Atm_led_device& begin( Atm_playfield &playfield, int16_t led_group, int16_t* device_script );
  Atm_led_device& trace( Stream & stream );
  Atm_led_device& trigger( int event );
  int state( void );
  Atm_led_device& onChange( Machine& machine, int event = 0 );
  Atm_led_device& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_led_device& onChange( int sub, Machine& machine, int event = 0 );
  Atm_led_device& onChange( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_led_device& init( void );
  Atm_led_device& press( void );
  Atm_led_device& release( void );

 private:
  enum { ENT_NOTIFY }; // ACTIONS
  enum { ON_CHANGE, CONN_MAX = 8 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void run_code( int16_t e );
  Atm_led_device& parse_code( const int16_t* device_script );

  const int16_t* event_ptr[MAX_EVENTS];
  uint8_t trigger_flags; 
  Atm_led_scheduler *leds;
  Atm_playfield *playfield;
  int16_t global_counter;
  int16_t led_group;
};
