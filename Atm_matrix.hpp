#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>

#define MAX_SWITCHES 70

struct matrix_switch_profile { 
    uint8_t switch_state : 1;
    uint8_t persistent : 1;
    uint8_t disabled : 1;
    uint8_t debounce_delay; 
    uint16_t retrigger_delay;
    uint16_t last_change; 
};

class Atm_matrix: public Machine { // Beter: Atm_switch_matrix

 public:
  enum { IDLE, SCAN, DISABLED }; 
  enum { EVT_DISABLE, EVT_ENABLE, ELSE }; // EVENTS
  Atm_matrix( void ) : Machine() {};
  Atm_matrix& begin( int8_t rows[], int8_t cols[], uint8_t no_of_rows, uint8_t no_of_cols);
  Atm_matrix& trace( Stream & stream );
  Atm_matrix& trigger( int event );
  int state( void );
  Atm_matrix& onPress( int sub, Machine& machine, int event = 0 );
  Atm_matrix& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_matrix& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_matrix& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_matrix& debounce( uint8_t v );
  Atm_matrix& debounce( uint8_t l, uint8_t v );
  Atm_matrix& retrigger( uint16_t v );
  Atm_matrix& retrigger( uint8_t l, uint16_t v );
  Atm_matrix& persistent( uint8_t l, bool v = true );
  Atm_matrix& disable();
  Atm_matrix& enable();

 private:
  enum { ENT_SCAN, ENT_DISABLED }; // ACTIONS
  enum { ON_PRESS, ON_RELEASE = MAX_SWITCHES, CONN_MAX = MAX_SWITCHES * 2 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void scan_matrix_column( bool active );
  void switch_changed( uint8_t l, uint8_t v );
  
  int no_of_rows, no_of_cols;
  int8_t* cols;
  int8_t* rows;
  bool active;
  matrix_switch_profile prof[ MAX_SWITCHES ];
  int8_t scan_col = 0;
};

/* 
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_matrix">
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

