#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"

// Element should eventually accommodate score counter digits
// - respond to kick by pulsing coil if enabled ( or if led_state is on )
// - reset function (pulse until zero switch goes low/high) 

class Atm_element: public Machine {

 public:
  enum { IDLE, DELAY, KICKING, DISABLED, INIT, INPUTTING, RELEASE, LIGHT_ON, LIGHT_OFF, TOGGLE }; // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_KICK, EVT_RELEASE, EVT_INPUT, EVT_INIT, EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_LIT, ELSE }; // EVENTS
  Atm_element( void ) : Machine() {};
  Atm_element& begin( Atm_led_scheduler& led, int16_t coil = -1, int16_t light = -1, uint8_t coil_profile = Atm_led_scheduler::PROFILE_COIL, uint8_t led_profile = Atm_led_scheduler::PROFILE_LED ); 
  Atm_element& trace( Stream & stream );
  Atm_element& trigger( int event );
  int state( void );
  Atm_element& onInit( Machine& machine, int event = 0 );
  Atm_element& onInit( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onInput( Machine& machine, int event = 0 );
  Atm_element& onInput( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onInput( int sub, Machine& machine, int event = 0 );
  Atm_element& onInput( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& onPress( Machine& machine, int event = 0 );
  Atm_element& onPress( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onPress( int sub, Machine& machine, int event = 0 );
  Atm_element& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& onLight( Machine& machine, int event = 0 );
  Atm_element& onLight( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onLight( int sub, Machine& machine, int event = 0 );
  Atm_element& onLight( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& on( void );
  Atm_element& off( void );
  Atm_element& kick( void );
  Atm_element& release( void );
  Atm_element& input( void );
  Atm_element& init( void );
  Atm_element& disable( void );
  Atm_element& enable( void );
  Atm_element& toggle( void );
  Atm_element& autoLight( int v = 1 ); // Default false, switch triggers light
  Atm_element& autoKick( int v = 1 ); // Default true, switch triggers coil 
  
  uint32_t idle( void ); // Number of millis element has been idle
  bool idle( uint32_t maximum ); // Element has been idle for at most 'maximum' millis  

 protected:
  enum { ENT_KICKING, ENT_INIT, ENT_INPUT, ENT_RELEASE, ENT_LIGHT_ON, ENT_LIGHT_OFF }; // ACTIONS
  enum { ON_INIT, ON_INPUT, ON_KICK = 4, ON_LIGHT = 7, CONN_MAX = 10 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  atm_timer_millis timer;
  int16_t light_led, coil_led, autolight, autokick;
  Atm_led_scheduler *led;
  bool switch_state, led_state;
  uint32_t changed;
};
