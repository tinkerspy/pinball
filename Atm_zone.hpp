#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_element.hpp"
#include "Atm_led_scheduler.hpp"

#define MAX_SWITCHES 320

struct switch_record { 
    uint8_t switch_state : 1;
    uint8_t persistent : 1;
    uint8_t disabled : 1;
    uint8_t debounce_delay; 
    uint16_t retrigger_delay;
    uint16_t last_change;
    bool initialized = false; 
    Atm_element *element;
};

class Atm_zone: public Machine { // Beter: Atm_switch_zone

 public:
  enum { IDLE, SCAN, DISABLED }; 
  enum { EVT_DISABLE, EVT_ENABLE, ELSE }; // EVENTS
  Atm_zone( void ) : Machine() {};
  Atm_zone& begin( IO& io, Atm_led_scheduler& led );
  Atm_zone& trace( Stream & stream );
  Atm_zone& trigger( int event );
  int state( void );
  Atm_zone& onPress( int sub, Machine& machine, int event = 0 );
  Atm_zone& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_zone& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_zone& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_zone& debounce( uint8_t v );
  Atm_zone& debounce( uint8_t n, uint8_t v );
  Atm_zone& retrigger( uint16_t v );
  Atm_zone& retrigger( uint8_t n, uint16_t v );
  Atm_zone& persistent( uint8_t n, bool v = true );
  Atm_zone& disable();
  Atm_zone& enable();
  Atm_element& element( int n, int light_led = -1, int coil_led = -1, int coil_profile = 0 );

 protected:
  enum { ENT_SCAN, ENT_DISABLED }; // ACTIONS
  enum { ON_PRESS, ON_RELEASE = MAX_SWITCHES, CONN_MAX = MAX_SWITCHES * 2 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void scan_matrix( bool active );
  void switch_changed( uint8_t n, uint8_t v );
  
  int no_of_rows, no_of_cols;
  int8_t* cols;
  int8_t* rows;
  bool active;
  switch_record prof[MAX_SWITCHES ];
  int8_t scan_col = 0;
  Atm_led_scheduler *led;
  IO *io;
};
