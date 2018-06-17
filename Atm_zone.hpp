#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_element.hpp"
#include "Atm_apa102.hpp"

#define MAX_SWITCHES 70

struct zone_switch_profile { 
    uint8_t switch_state : 1;
    uint8_t persistent : 1;
    uint8_t disabled : 1;
    uint8_t debounce_delay; 
    uint16_t retrigger_delay;
    uint16_t last_change; 
};

struct zone_element { 
    uint8_t initialized;
    Atm_element element;
};

class Atm_zone: public Machine { // Beter: Atm_switch_zone

 public:
  enum { IDLE, SCAN, DISABLED }; 
  enum { EVT_DISABLE, EVT_ENABLE, ELSE }; // EVENTS
  Atm_zone( void ) : Machine() {};
  Atm_zone& begin( Atm_apa102& led, int8_t rows[], int8_t cols[], uint8_t no_of_rows, uint8_t no_of_cols);
  Atm_zone& trace( Stream & stream );
  Atm_zone& trigger( int event );
  int state( void );
  Atm_zone& onPress( int sub, Machine& machine, int event = 0 );
  Atm_zone& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_zone& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_zone& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_zone& debounce( uint8_t v );
  Atm_zone& debounce( uint8_t l, uint8_t v );
  Atm_zone& retrigger( uint16_t v );
  Atm_zone& retrigger( uint8_t l, uint16_t v );
  Atm_zone& persistent( uint8_t l, bool v = true );
  Atm_zone& disable();
  Atm_zone& enable();
  Atm_element& element( int switch_no, int light_led, int coil_led, int pulse_time = -1 );
  Atm_element& element( int switch_no );


 private:
  enum { ENT_SCAN, ENT_DISABLED }; // ACTIONS
  enum { ON_PRESS, ON_RELEASE = MAX_SWITCHES, CONN_MAX = MAX_SWITCHES * 2 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void scan_zone_column( bool active );
  void switch_changed( uint8_t l, uint8_t v );
  
  int no_of_rows, no_of_cols;
  int8_t* cols;
  int8_t* rows;
  bool active;
  zone_switch_profile prof[ MAX_SWITCHES ];
  int8_t scan_col = 0;
  zone_element elem[MAX_SWITCHES];
  Atm_apa102 *led;

};

/* 
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_zone">
    <states>
      <IDLE index="0">
        <ELSE>SCAN</ELSE>
      </IDLE>
      <SCAN index="1" on_enter="ENT_SCAN">
      </SCAN>
    </states>
    <events>
    </events>
    <connectors>
      <CHANGE autostore="0" broadcast="0" dir="PUSH" slots="32"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end 
*/

