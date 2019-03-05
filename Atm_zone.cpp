#include "Atm_zone.hpp"

// TODO: Move retrigger to element() level
// TODO: Allow for active HIGH switches
// TODO: Add catchall on onPress()/onRelease()

/* 
 *  Some way to pulse score-digits (while/until switch is enabled)
 */

Atm_zone& Atm_zone::begin( IO& io, Atm_led_scheduler& led ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER  ON_LOOP  ON_EXIT  EVT_DISABLE EVT_ENABLE      ELSE */
    /*  IDLE     */           -1,      -1,      -1,    DISABLED,        -1,     SCAN,
    /*  SCAN     */     ENT_SCAN,      -1,      -1,    DISABLED,        -1,     SCAN,
    /*  DISABLED */ ENT_DISABLED,      -1,      -1,          -1,      SCAN, DISABLED, // Add INIT state!!! (send init events to all elements)
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->io = &io;
  this->led = &led;
  for ( int i = 0; i < MAX_SWITCHES; i++ ) 
    debounce( i, 5 );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_zone::event( int id ) {
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

void Atm_zone::action( int id ) {
  switch ( id ) {
    case ENT_SCAN:
      scan_matrix( true );
      return;
    case ENT_DISABLED:
      scan_matrix( false );
      return;
  }
}

Atm_zone& Atm_zone::debounce( uint8_t v ) {
  for ( uint16_t i = 0; i < MAX_SWITCHES; i++ ) {
    prof[i].debounce_delay = v; 
  }
  return *this;   
}

Atm_zone& Atm_zone::debounce( uint8_t n, uint8_t v ) {
  prof[n].debounce_delay = v; 
  return *this;   
}

Atm_zone& Atm_zone::retrigger( uint16_t v ) {
  for ( uint8_t i = 0; i < MAX_SWITCHES; i++ ) {
    prof[i].retrigger_delay = v; 
  }
  return *this;   
}

Atm_zone& Atm_zone::retrigger( uint8_t n, uint16_t v ) {
  prof[n].retrigger_delay = v; 
  return *this;   
}

Atm_zone& Atm_zone::persistent( uint8_t n, bool v /* = true */ ) {
  prof[n].persistent = v ? 1 : 0; 
  return *this;   
}

Atm_zone& Atm_zone::disable() {
  trigger( EVT_DISABLE );
  return *this;   
}

Atm_zone& Atm_zone::enable() {
  trigger( EVT_ENABLE );
  return *this;   
}

void Atm_zone::scan_matrix( bool active ) {
  int16_t sw = io->scan();
  if ( sw != 0 ) {
    switch_changed( abs( sw ), sw > 0 );
  }
}

void Atm_zone::switch_changed( uint8_t n, uint8_t v ) {
  uint16_t millis_passed = (uint16_t) millis() - prof[n].last_change;
  if ( millis_passed > prof[n].debounce_delay ) {
    if ( v ) {
      if ( millis_passed > prof[n].retrigger_delay ) {
        prof[n].switch_state = 1;
        prof[n].last_change = millis();
        push( connectors, ON_PRESS, n, n, 1 ); 
        if ( prof[n].element.initialized ) prof[n].element.trigger( Atm_element::EVT_KICK ); 
      }
    } else {
      prof[n].switch_state = 0;      
      prof[n].last_change = millis();
      push( connectors, ON_RELEASE, n, n, 0 ); 
      if ( prof[n].element.initialized ) prof[n].element.trigger( Atm_element::EVT_RELEASE );
    }
  }
}

Atm_element& Atm_zone::element( int n, int light_led /* = -1 */, int coil_led /* = -1 */, int coil_profile /* = 0 */ ) {
  if ( !prof[n].element.initialized ) {
    prof[n].element.begin( *led, light_led, coil_led, coil_profile );
  }
  return prof[n].element;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_zone& Atm_zone::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_zone::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


Atm_zone& Atm_zone::onPress( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, machine, event );
  return *this;
}

Atm_zone& Atm_zone::onPress( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, callback, idx );
  return *this;
}

Atm_zone& Atm_zone::onRelease( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, machine, event );
  return *this;
}

Atm_zone& Atm_zone::onRelease( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_zone& Atm_zone::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "zone\0EVT_DISABLE\0EVT_ENABLE\0ELSE\0IDLE\0SCAN\0DISABLED" );
  return *this;
}
