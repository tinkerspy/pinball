#pragma once

#include <Automaton.h>
#include "Atm_em_counter.hpp"

#define MAX_COUNTER 6

class Atm_score: public Machine {

 public:
  enum { IDLE, RESET, FINISHED }; // STATES
  enum { EVT_RESET, EVT_FINISHED, ELSE, EVT_10, EVT_100, EVT_1000, EVT_500, EVT_5000 }; // EVENTS
  Atm_score( void ) : Machine() {};
  Atm_score& begin( void );
  Atm_score& trace( Stream & stream );
  Atm_score& trigger( int event );
  int state( void );
  Atm_score& onReset( Machine& machine, int event = 0 );
  Atm_score& onReset( atm_cb_push_t callback, int idx = 0 );
  Atm_score& onDigit( int sub, Machine& machine, int event = 0 ); // WARNING: call after addCounter() 
  Atm_score& onDigit( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_score& addCounter( Atm_em_counter& machine );
  Atm_score& select( uint8_t n );
  Atm_score& reset( void );
  bool touched( void );

 private:
  enum { ENT_RESET, ENT_FINISHED }; // ACTIONS
  enum { ON_RESET, ON_SCORE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 

  Atm_em_counter* counter[MAX_COUNTER];
  uint8_t machine_count = 0;
  uint8_t selected = 0;
  bool score_touched = false;

};
