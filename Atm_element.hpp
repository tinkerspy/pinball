#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

// Element should eventually accommodate score counter digits
// - respond to kick by pulsing coil if enabled ( or if led_state is on )
// - reset function (pulse until zero switch goes low/high) 

class Atm_playfield;

class Atm_element: public Machine {

 public:
  enum { IDLE, DELAY, KICKING, DISABLED, INIT, INPUTTING, RELEASE, LIGHT_ON, LIGHT_OFF, WATCH, TOGGLE }; // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_KICK, EVT_RELEASE, EVT_INPUT, EVT_INIT, EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_LIT, EVT_WATCH, ELSE }; // EVENTS
  Atm_element( void ) : Machine() {};
  Atm_element& begin( Atm_playfield &playfield, int16_t coil = -1, int16_t light = -1, int8_t coil_profile = Atm_led_scheduler::PROFILE_COIL, int8_t led_profile = Atm_led_scheduler::PROFILE_LED, int16_t cnt = -1 ); 
  Atm_element& initialize( int16_t coil = -1, int16_t light = -1, int8_t coil_profile = Atm_led_scheduler::PROFILE_COIL, int8_t led_profile = Atm_led_scheduler::PROFILE_LED, int16_t cnt = -1 ); 
  Atm_element& trace( Stream & stream );
  Atm_element& trigger( int event );
  int state( void );
  Atm_element& onInit( Machine& machine, int event = 0 );
  Atm_element& onInit( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onInput( Machine& machine, int event = 0 );
  Atm_element& onInput( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onInput( int sub, Machine& machine, int event = 0 );
  Atm_element& onInput( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& onScore( Machine& machine, int event_unlit, int event_lit = -1 );
  Atm_element& onScore( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onScore( int sub, Machine& machine, int event = 0 );
  Atm_element& onScore( int sub, atm_cb_push_t callback, int idx = 0 );
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
  Atm_element& debounce( uint8_t d, uint16_t r );
  
  uint32_t idle( void ); // Number of millis element has been idle
  bool idle( uint32_t maximum ); // Element has been idle for at most 'maximum' millis  

 protected:
  enum { ENT_KICKING, ENT_INIT, ENT_INPUT, ENT_RELEASE, ENT_LIGHT_ON, ENT_LIGHT_OFF, ENT_WATCH }; // ACTIONS
  enum { ON_INIT, ON_INPUT, ON_KICK = 4, ON_LIGHT = 7, CONN_MAX = 10 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  atm_timer_millis timer;
  int16_t light_led, coil_led, autolight, autokick;
  int score_lit = 0, score_unlit = 0;
  Machine *counter;
  Atm_playfield *playfield;
  bool switch_state;
  uint32_t changed;
  uint8_t watch_state = 0;
  int16_t led_cnt = 0, watch_cnt = 0;
};
