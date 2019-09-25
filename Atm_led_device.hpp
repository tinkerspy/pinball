#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

// Number of outputs is also limited by the 32 bits in trigger_flags 

#define MAX_OUTPUTS 20
#define MAX_REGISTERS 8
#define STACK_SIZE 16
#define DYNAMIC_ELEMENTS

struct core_state {
  int16_t ptr;
  uint8_t reg_ptr;
  uint8_t stack_ptr;
  int16_t stack[STACK_SIZE];  
  bool yield_enabled;
};

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
  Atm_led_device& trigger( int event, uint32_t sel );
  int state( void );
  int state( uint32_t sel );
  Atm_led_device& onEvent( int sub, Machine& machine, int event = 0 );
  Atm_led_device& onEvent( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_led_device& init( void );
  Atm_led_device& press( void );
  Atm_led_device& release( void );
  Atm_led_device& reg( uint8_t r, int16_t v );
  int16_t reg( uint8_t r );
  Atm_led_device& chain( Atm_led_device& next );
  Atm_led_device& select( uint32_t mask );
  static const uint32_t DEV_SELECT_ALL = 0xFFFFFFFF;

 private:
  enum { ENT_NOTIFY, ENT_RESUME }; // ACTIONS
  enum { ON_EVENT, CONN_MAX = MAX_OUTPUTS }; // CONNECTORS
  atm_connector connectors[CONN_MAX+1];
  int event( int id ); 
  void action( int id ); 
  void start_code( int16_t e );
  void run_code( uint8_t active_core );
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
  uint8_t input_persistence, output_persistence;
  int16_t numberOfInputs;
  atm_timer_millis timer;
  Atm_led_device* next;
  bool enabled;
  core_state core[2];
  
};
