#include "Atm_scalar.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_scalar& Atm_scalar::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP  ON_EXIT  EVT_ZERO  EVT_COUNTER  EVT_TIMER  EVT_ADVANCE  EVT_RESET  EVT_COLLECT  ELSE */
    /*    IDLE */          -1, ATM_SLEEP,      -1,       -1,          -1,        -1,        NEXT,     RESET,     COLLECT,   -1,
    /*    NEXT */    ENT_NEXT,        -1,      -1,       -1,          -1,        -1,          -1,        -1,          -1, IDLE,
    /*   RESET */   ENT_RESET,        -1,      -1,       -1,          -1,        -1,          -1,        -1,          -1, IDLE,
    /* COLLECT */ ENT_COLLECT,        -1,      -1,     IDLE,        WAIT,        -1,          -1,        -1,          -1,   -1,
    /*    WAIT */          -1,        -1,      -1,       -1,          -1,   COLLECT,          -1,        -1,          -1,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_scalar::event( int id ) {
  switch ( id ) {
    case EVT_ZERO:
      return 0;
    case EVT_COUNTER:
      return 0;
    case EVT_TIMER:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_scalar::action( int id ) {
  switch ( id ) {
    case ENT_NEXT:
      return;
    case ENT_RESET:
      return;
    case ENT_COLLECT:
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_scalar& Atm_scalar::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_scalar::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_scalar& Atm_scalar::advance() {
  trigger( EVT_ADVANCE );
  return *this;
}

Atm_scalar& Atm_scalar::reset() {
  trigger( EVT_RESET );
  return *this;
}

Atm_scalar& Atm_scalar::collect() {
  trigger( EVT_COLLECT );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_scalar& Atm_scalar::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "SCALAR\0EVT_ZERO\0EVT_COUNTER\0EVT_TIMER\0EVT_ADVANCE\0EVT_RESET\0EVT_COLLECT\0ELSE\0IDLE\0NEXT\0RESET\0COLLECT\0WAIT" );
  return *this;
}



