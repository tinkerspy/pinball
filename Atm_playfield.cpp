#include "Atm_playfield.hpp"

// TODO: Move retrigger to element() level?
// TODO: Add catchall on onPress()/onRelease()
// BUG: RELEASE occurring inside debounce interval is lost

/* 
 *  Some way to pulse score-digits (while/until switch is enabled)
 */

Atm_playfield& Atm_playfield::begin( IO& io, Atm_led_scheduler& led ) {
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
  memset( connectors, 0, sizeof( connectors ) );
  memset( prof, 0, sizeof( prof ) );
  debounce( 5 );      
  retrigger( 500 ); 
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_playfield::event( int id ) {
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

void Atm_playfield::action( int id ) {
  switch ( id ) {
    case ENT_SCAN:
      scan_matrix( true );
      return;
    case ENT_DISABLED:
      scan_matrix( false );
      return;
  }
}

Atm_playfield& Atm_playfield::debounce( uint8_t v ) {
  for ( int16_t i = 0; i <= MAX_SWITCHES; i++ ) {
    prof[i].debounce_delay = v; 
  }
  return *this;   
}

Atm_playfield& Atm_playfield::debounce( int16_t n, uint8_t v ) {
  prof[n].debounce_delay = v; 
  return *this;   
}

Atm_playfield& Atm_playfield::retrigger( int16_t v ) {
  for ( int16_t i = 0; i <= MAX_SWITCHES; i++ ) {
    prof[i].retrigger_delay = v; 
  }
  return *this;   
}

Atm_playfield& Atm_playfield::retrigger( int16_t n, uint16_t v ) {
  prof[n].retrigger_delay = v; 
  return *this;   
}

Atm_playfield& Atm_playfield::persistent( int16_t n, bool v /* = true */ ) {
  prof[n].persistent = v ? 1 : 0; 
  return *this;   
}

Atm_playfield& Atm_playfield::disable() {
  trigger( EVT_DISABLE );
  return *this;   
}

Atm_playfield& Atm_playfield::enable() {
  trigger( EVT_ENABLE );
  return *this;   
}

void Atm_playfield::scan_matrix( bool active ) {
  int16_t sw = io->scan();
  if ( sw != 0 ) {
    switch_changed( abs( sw ), sw > 0 );
  }
  // FIXME optimize: Use a global switch_state variable that indicates any switch is in switch_state 1, clear if none are
  for ( int n = 1; n <= MAX_SWITCHES; n++ ) {
    if ( prof[n].switch_state ) {
      uint16_t millis_passed = (uint16_t) millis() - prof[n].last_change;
      if ( millis_passed > prof[n].debounce_delay ) {
        if ( !io->isPressed( n ) ) switch_changed( n, 0 );
      }
    }
  }
}

void Atm_playfield::switch_changed( int16_t n, uint8_t v ) {
  uint16_t millis_passed = (uint16_t) millis() - prof[n].last_change;
  uint16_t global_millis_passed = (uint16_t) millis() - global_last_kick;
  if ( millis_passed > prof[n].debounce_delay && global_millis_passed > prof[n].debounce_delay ) {
    if ( v ) { // KICK
      if ( millis_passed > prof[n].retrigger_delay ) {
        prof[n].switch_state = 1;
        prof[n].last_change = global_last_kick = millis();
        push( connectors, ON_PRESS, n, n, 1 ); 
#ifdef DYNAMIC_ELEMENTS        
        if ( prof[n].initialized ) prof[n].element->trigger( Atm_element::EVT_KICK ); 
#else 
        if ( prof[n].initialized ) prof[n].element.trigger( Atm_element::EVT_KICK ); 
#endif        
      }
    } else { // RELEASE
      if ( prof[n].switch_state == 1 ) { // Only if switch is actually pressed
        prof[n].switch_state = 0;      
        prof[n].last_change = global_last_kick = millis();
        push( connectors, ON_RELEASE, n, n, 0 ); 
#ifdef DYNAMIC_ELEMENTS        
        if ( prof[n].initialized ) prof[n].element->trigger( Atm_element::EVT_RELEASE );
#else
        if ( prof[n].initialized ) prof[n].element.trigger( Atm_element::EVT_RELEASE );
#endif
      }
    }
  }
}

Atm_element& Atm_playfield::element( int16_t n, int16_t coil_led /* -1 */, int16_t light_led /* -1 */, uint8_t coil_profile /* 0 */, uint8_t led_profile /* 0 */ ) {
  if ( !prof[n].initialized ) {
    prof[n].initialized = true;
#ifdef DYNAMIC_ELEMENTS        
    prof[n].element = new Atm_element();  
    prof[n].element->begin( *led, coil_led, light_led, coil_profile, led_profile );
#else 
    prof[n].element.begin( *led, coil_led, light_led, coil_profile, led_profile );
#endif    
  }
#ifdef DYNAMIC_ELEMENTS        
  return *prof[n].element;
#else   
  return prof[n].element;
#endif  
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_playfield& Atm_playfield::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_playfield::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


Atm_playfield& Atm_playfield::onPress( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, machine, event );
  return *this;
}

Atm_playfield& Atm_playfield::onPress( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, callback, idx );
  return *this;
}

Atm_playfield& Atm_playfield::onRelease( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, machine, event );
  return *this;
}

Atm_playfield& Atm_playfield::onRelease( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_playfield& Atm_playfield::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "zone\0EVT_DISABLE\0EVT_ENABLE\0ELSE\0IDLE\0SCAN\0DISABLED" );
  return *this;
}
