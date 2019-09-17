#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

#define MAX_OUTPUTS 20
#define DYNAMIC_ELEMENTS

class Atm_playfield;

class Atm_led_device: public Machine {

 public:
  enum { IDLE, NOTIFY }; // STATES
  enum { EVT_NOTIFY, ELSE }; // EVENTS
  Atm_led_device( void ) : Machine() {};
  Atm_led_device& begin( Atm_playfield &playfield, int16_t led_group, int16_t* device_script );
  Atm_led_device& set_script( int16_t* script );
  Atm_led_device& set_led( int16_t led_group );
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
  enum { ON_CHANGE, CONN_MAX = MAX_OUTPUTS }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void run_code( int16_t e );
  int16_t* parse_code( int16_t* device_script );
  uint16_t trigger_flags; 
  Atm_led_scheduler *leds;
  Atm_playfield *playfield;
  int16_t global_counter;
  int16_t led_group = -1;
  int16_t* script;
  uint8_t input_persistence, output_persistence;
  int16_t numberOfInputs;
};
