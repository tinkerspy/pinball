#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

// Number of outputs is also limited by the 32 bits in trigger_flags 

#define MAX_OUTPUTS 20
#define MAX_REGISTERS 16
#define DYNAMIC_ELEMENTS

class Atm_playfield;

class Atm_led_device: public Machine {

 public:
  enum { IDLE, NOTIFY, YIELD, RESUME }; // STATES
  enum { EVT_NOTIFY, EVT_TIMER, EVT_YIELD, ELSE }; // EVENTS
  Atm_led_device( void ) : Machine() {};
  Atm_led_device& begin( Atm_playfield &playfield, int16_t led_group, int16_t* device_script );
  Atm_led_device& set_script( int16_t* script );
  Atm_led_device& set_led( int16_t led_group );
  Atm_led_device& trace( Stream & stream );
  Atm_led_device& trigger( int event );
  int state( void );
  Atm_led_device& onEvent( Machine& machine, int event = 0 );
  Atm_led_device& onEvent( atm_cb_push_t callback, int idx = 0 );
  Atm_led_device& onEvent( int sub, Machine& machine, int event = 0 );
  Atm_led_device& onEvent( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_led_device& init( void );
  Atm_led_device& press( void );
  Atm_led_device& release( void );

 private:
  enum { ENT_NOTIFY, ENT_RESUME }; // ACTIONS
  enum { ON_EVENT, CONN_MAX = MAX_OUTPUTS }; // CONNECTORS
  atm_connector connectors[CONN_MAX+1];
  int event( int id ); 
  void action( int id ); 
  void start_code( int16_t e );
  void run_code( void );
  int16_t* parse_code( int16_t* device_script );
  int16_t led_index( int16_t led_group, int16_t selector );
  bool led_active( int16_t led_group, int16_t selector );
  void led_on( int16_t led_group, int16_t selector );
  void led_off( int16_t led_group, int16_t selector );

  uint32_t trigger_flags; 
  Atm_led_scheduler *leds;
  Atm_playfield *playfield;
  int16_t registers[MAX_REGISTERS];
  int16_t led_group = -1;
  int16_t* script;
  int16_t ptr, reg_ptr;
  uint8_t input_persistence, output_persistence;
  int16_t numberOfInputs;
  int16_t callstack[16];
  int16_t stackptr;
  atm_timer_millis timer;
};
