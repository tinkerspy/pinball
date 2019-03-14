#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_element.hpp"
#include "Atm_led_scheduler.hpp"

#define MAX_SWITCHES 320
#define DYNAMIC_ELEMENTS

struct switch_record { 
    uint8_t switch_state : 1;
    uint8_t persistent : 1;
    uint8_t disabled : 1;
    uint8_t debounce_delay; 
    uint16_t retrigger_delay;
    uint16_t last_change;
    bool initialized; 
#ifdef DYNAMIC_ELEMENTS    
    Atm_element *element;
#else
    Atm_element element;
#endif    
};

class Atm_playfield: public Machine { // Beter: Atm_switch_zone

 public:
  enum { IDLE, SCAN, DISABLED }; 
  enum { EVT_DISABLE, EVT_ENABLE, ELSE }; // EVENTS
  Atm_playfield( void ) : Machine() {};
  Atm_playfield& begin( IO& io, Atm_led_scheduler& led );
  Atm_playfield& trace( Stream & stream );
  Atm_playfield& trigger( int event );
  int state( void );
  Atm_playfield& onPress( int sub, Machine& machine, int event = 0 );
  Atm_playfield& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_playfield& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_playfield& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_playfield& debounce( uint8_t v );
  Atm_playfield& debounce( int16_t n, uint8_t v );
  Atm_playfield& retrigger( int16_t v );
  Atm_playfield& retrigger( int16_t n, uint16_t v );
  Atm_playfield& persistent( int16_t n, bool v = true );
  Atm_playfield& disable();
  Atm_playfield& enable();
  Atm_element& element( int16_t n, int16_t coil_led = -1, int16_t light_led = -1, uint8_t coil_profile = 0 , uint8_t led_profile = 1 );

 protected:
  enum { ENT_SCAN, ENT_DISABLED }; // ACTIONS
  enum { ON_PRESS, ON_RELEASE = MAX_SWITCHES, CONN_MAX = MAX_SWITCHES * 2 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void scan_matrix( bool active );
  void switch_changed( int16_t n, uint8_t v );
  
  int no_of_rows, no_of_cols;
  int8_t* cols;
  int8_t* rows;
  bool active;
  switch_record prof[MAX_SWITCHES + 1 ];
  int8_t scan_col = 0;
  Atm_led_scheduler *led;
  uint16_t global_last_kick;
  IO *io;
};
