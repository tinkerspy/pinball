#include "Atm_score.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_score& Atm_score::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                 ON_ENTER    ON_LOOP  ON_EXIT  EVT_RESET  EVT_FINISHED  ELSE */
    /*     IDLE */           -1, ATM_SLEEP,      -1,     RESET,           -1,   -1,
    /*    RESET */    ENT_RESET,        -1,      -1,        -1,     FINISHED,   -1,
    /* FINISHED */ ENT_FINISHED,        -1,      -1,        -1,           -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_score::event( int id ) {
  switch ( id ) {
    case EVT_FINISHED:
      return state();
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_RESET, 0, <v>, <up> );
 *   push( connectors, ON_SCORE, 0, <v>, <up> );
 */

void Atm_score::action( int id ) {
  switch ( id ) {
    case ENT_RESET:
      for ( int i = 0; i < machine_count; i++ ) {
        counter[i]->trigger( Atm_em_counter::EVT_RESET );
      }
      return;
    case ENT_FINISHED:
      return;
  }
}

Atm_score& Atm_score::select( uint8_t n ) {
  selected = n;
  return *this;
}

Atm_score& Atm_score::addCounter( Machine& machine ) {
  counter[machine_count] = &machine;
  machine_count++;
  return *this;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_score& Atm_score::trigger( int event ) {  
  switch ( event ) {
    case EVT_10:
      counter[selected]->trigger( Atm_em_counter::EVT_10 );
      return *this;
    case EVT_100:
      counter[selected]->trigger( Atm_em_counter::EVT_100 );
      return *this;
    case EVT_1000:
      counter[selected]->trigger( Atm_em_counter::EVT_1000 );
      return *this;
    case EVT_500:
      counter[selected]->trigger( Atm_em_counter::EVT_500 );
      return *this;
    case EVT_5000:
      counter[selected]->trigger( Atm_em_counter::EVT_5000 );
      return *this;
  }
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_score::state( void ) {
  int st = 0;
  for ( int i = 0; i < machine_count; i++ ) {
    st |= counter[i]->state();
  }
  return st;
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_score& Atm_score::reset() {
  trigger( EVT_RESET );
  return *this;
}

/*
 * onReset() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_score& Atm_score::onReset( Machine& machine, int event ) {
  onPush( connectors, ON_RESET, 0, 1, 1, machine, event );
  return *this;
}

Atm_score& Atm_score::onReset( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_RESET, 0, 1, 1, callback, idx );
  return *this;
}

/*
 * onScore() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_score& Atm_score::onScore( Machine& machine, int event ) {
  onPush( connectors, ON_SCORE, 0, 1, 1, machine, event );
  return *this;
}

Atm_score& Atm_score::onScore( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_SCORE, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_score& Atm_score::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "SCORE\0EVT_RESET\0EVT_FINISHED\0ELSE\0IDLE\0RESET\0FINISHED" );
  return *this;
}
