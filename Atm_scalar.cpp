#include "Atm_scalar.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_scalar& Atm_scalar::begin( Atm_led_scheduler& leds, int16_t led_group, int16_t def, int16_t max, bool fill /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP  ON_EXIT  EVT_ZERO  EVT_COUNTER  EVT_TIMER  EVT_ADVANCE  EVT_RESET  EVT_COLLECT  ELSE */
    /*    IDLE */          -1, ATM_SLEEP,      -1,       -1,          -1,        -1,        NEXT,     RESET,     COLLECT,   -1,
    /*    NEXT */    ENT_NEXT,        -1,      -1,       -1,          -1,        -1,          -1,        -1,          -1, IDLE,
    /*   RESET */   ENT_RESET,        -1,      -1,       -1,          -1,        -1,          -1,        -1,          -1, IDLE,
    /* COLLECT */ ENT_COLLECT,        -1,      -1,    RESET,          -1,   COLLOOP,          -1,        -1,          -1,   -1,
    /* COLLOOP */ ENT_COLLOOP,        -1,      -1,   COLRPT,          -1,        -1,          -1,        -1,          -1, WAIT,
    /*    WAIT */          -1,        -1,      -1,       -1,          -1,   COLLOOP,          -1,        -1,          -1,   -1,
    /* COLREPT */  ENT_COLRPT,        -1,      -1,       -1,       RESET,   COLLECT,          -1,        -1,          -1,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->max = max;
  this->def = def;
  this->fill_mode = fill;
  this->leds = &leds;
  this->led_group = led_group;
  this->leds->scalar( this->led_group, this->def, this->fill_mode );
  value = this->def;
  multiplier( 1 );
  timer.set( 500 );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_scalar::event( int id ) {
  switch ( id ) {
    case EVT_ZERO:
      return countdown <= def;      
    case EVT_COUNTER:
      return countmulti == 0;
    case EVT_TIMER:
      return timer.expired( this );
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_scalar::action( int id ) {
  switch ( id ) {
    case ENT_NEXT:
      if ( !lock_advance ) { 
        if ( value < max ) { 
          leds->scalar( led_group, ++value, fill_mode ); 
        }
      }
      return;
    case ENT_RESET:
      value = def;
      leds->scalar( led_group, value, fill_mode );
      multiplier( 1 );
      lock( false );
      return;
    case ENT_COLLECT:
      countdown = value;
      return;
    case ENT_COLLOOP:
      push( connectors, ON_COLLECT, 0, --countdown, 0 );
      leds->scalar( led_group, countdown, fill_mode );
      return;
    case ENT_COLRPT:
      countmulti--;
      return;
  }
}

Atm_scalar& Atm_scalar::set( int16_t n ) {
  leds->scalar( led_group, value = n, fill_mode );
  return *this;
}

Atm_scalar& Atm_scalar::lock( bool v  /* = true */ ) {
  lock_advance = v;
  return *this;
}

Atm_scalar& Atm_scalar::multiplier( uint8_t n ) {
  countmulti = multiply = n;
  return *this;
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
  return value;
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

/*
 * onCollect() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_scalar& Atm_scalar::onCollect( Machine& machine, int event ) {
  onPush( connectors, ON_COLLECT, 0, 1, 1, machine, event );
  return *this;
}

Atm_scalar& Atm_scalar::onCollect( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_COLLECT, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_scalar& Atm_scalar::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "SCALAR\0EVT_ZERO\0EVT_COUNTER\0EVT_TIMER\0EVT_ADVANCE\0EVT_RESET\0EVT_COLLECT\0ELSE\0"
    "IDLE\0NEXT\0RESET\0COLLECT\0COLLOOP\0WAIT\0COLRPT" );
  return *this;
}
