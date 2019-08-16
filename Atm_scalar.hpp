#pragma once

#include <Automaton.h>

class Atm_scalar: public Machine {

 public:
  enum { IDLE, NEXT, RESET, COLLECT, WAIT }; // STATES
  enum { EVT_ZERO, EVT_COUNTER, EVT_TIMER, EVT_ADVANCE, EVT_RESET, EVT_COLLECT, ELSE }; // EVENTS
  Atm_scalar( void ) : Machine() {};
  Atm_scalar& begin( void );
  Atm_scalar& trace( Stream & stream );
  Atm_scalar& trigger( int event );
  int state( void );
  Atm_scalar& advance( void );
  Atm_scalar& reset( void );
  Atm_scalar& collect( void );
  Atm_scalar& lock( uint8_t v ); // Locks EVT_ADVANCE event
 
 private:
  enum { ENT_NEXT, ENT_RESET, ENT_COLLECT }; // ACTIONS
  int event( int id ); 
  void action( int id ); 

};
