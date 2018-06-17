#include "Atm_matrix.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_matrix& Atm_matrix::begin( int8_t* rows, int8_t* cols, uint8_t no_of_rows, uint8_t no_of_cols) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER  ON_LOOP  ON_EXIT  EVT_DISABLE EVT_ENABLE      ELSE */
    /*  IDLE     */           -1,      -1,      -1,    DISABLED,        -1,     SCAN,
    /*  SCAN     */     ENT_SCAN,      -1,      -1,    DISABLED,        -1,     SCAN,
    /*  DISABLED */ ENT_DISABLED,      -1,      -1,          -1,      SCAN, DISABLED,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  for ( int r = 0; r < no_of_rows; r++ ) { 
    pinMode( rows[r], INPUT_PULLUP ); 
  }
  for ( int c = 0; c < no_of_cols; c++ ) { 
    pinMode( cols[c], INPUT ); 
  }  
  this->no_of_cols = no_of_cols;
  this->no_of_rows = no_of_rows;
  this->cols = cols;
  this->rows = rows;
  debounce( 5 );
  retrigger( 0 );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_matrix::event( int id ) {
  switch ( id ) {
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_CHANGE, <sub>, <v>, <up> );
 */

void Atm_matrix::action( int id ) {
  switch ( id ) {
    case ENT_SCAN:
      scan_matrix_column( true );
      return;
    case ENT_DISABLED:
      scan_matrix_column( false );
      return;
  }
}

Atm_matrix& Atm_matrix::debounce( uint8_t v ) {
  for ( uint8_t i = 0; i < MAX_SWITCHES; i++ ) {
    prof[i].debounce_delay = v; 
  }
  return *this;   
}

Atm_matrix& Atm_matrix::debounce( uint8_t l, uint8_t v ) {
  prof[l].debounce_delay = v; 
  return *this;   
}

Atm_matrix& Atm_matrix::retrigger( uint16_t v ) {
  for ( uint8_t i = 0; i < MAX_SWITCHES; i++ ) {
    prof[i].retrigger_delay = v; 
  }
  return *this;   
}

Atm_matrix& Atm_matrix::retrigger( uint8_t l, uint16_t v ) {
  prof[l].retrigger_delay = v; 
  return *this;   
}

Atm_matrix& Atm_matrix::persistent( uint8_t l, bool v /* = true */ ) {
  prof[l].persistent = v ? 1 : 0; 
  return *this;   
}

Atm_matrix& Atm_matrix::disable() {
  trigger( EVT_DISABLE );
  return *this;   
}

Atm_matrix& Atm_matrix::enable() {
  trigger( EVT_ENABLE );
  return *this;   
}

void Atm_matrix::scan_matrix_column( bool active ) {
    pinMode( cols[scan_col], OUTPUT );
    digitalWrite( cols[scan_col], LOW );
    uint8_t logical = scan_col * no_of_rows;
    for ( int r = 0; r < no_of_rows; r++ ) {
      int v = !digitalRead( rows[r] );      
      if ( v xor prof[logical].switch_state ) {
        if ( active || prof[logical].persistent ) {
          switch_changed( logical, v );
        }
      }
      logical++;
    }
    pinMode( cols[scan_col], INPUT );    
    scan_col = ++scan_col % no_of_cols; 
}

void Atm_matrix::switch_changed( uint8_t logical, uint8_t v ) {

  uint16_t millis_passed = (uint16_t) millis() - prof[logical].last_change;
  if ( millis_passed > prof[logical].debounce_delay ) {
    if ( v ) {
      if ( millis_passed > prof[logical].retrigger_delay ) {
        prof[logical].last_change = millis();
        prof[logical].switch_state = 1;
        push( connectors, ON_PRESS, logical, logical, 1 );
      }
    } else {
      prof[logical].last_change = millis();
      prof[logical].switch_state = 0;      
      push( connectors, ON_RELEASE, logical, logical, 0 );
    }
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_matrix& Atm_matrix::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_matrix::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


Atm_matrix& Atm_matrix::onPress( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, machine, event );
  return *this;
}

Atm_matrix& Atm_matrix::onPress( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, callback, idx );
  return *this;
}

Atm_matrix& Atm_matrix::onRelease( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, machine, event );
  return *this;
}

Atm_matrix& Atm_matrix::onRelease( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_matrix& Atm_matrix::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "matrix\0EVT_DISABLE\0EVT_ENABLE\0ELSE\0IDLE\0SCAN\0DISABLED" );
  return *this;
}

