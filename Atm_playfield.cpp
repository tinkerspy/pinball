#include "Atm_playfield.hpp"

// TODO: Move retrigger to element() level
// TODO: Allow for active HIGH switches
// TODO: Add catchall on onPress()/onRelease()

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
  this->pleds = &led;
  memset( connectors, 0, sizeof( connectors ) );
  memset( prof, 0, sizeof( prof ) );
  for ( int i = 0; i < MAX_SWITCHES; i++ ) 
    debounce( i, 5 );       
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

Atm_playfield& Atm_playfield::debounce( uint8_t d, uint16_t r  ) {
  for ( int16_t i = 0; i < MAX_SWITCHES; i++ ) {
    prof[i].debounce_delay = d; 
    prof[i].retrigger_delay = r; 
  }
  return *this;   
}

Atm_playfield& Atm_playfield::debounce( int16_t n, uint8_t d, uint16_t r ) {
  prof[n].debounce_delay = d; 
  prof[n].retrigger_delay = r; 
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

bool Atm_playfield::isPressed( int16_t n ) {
  return io->isPressed( n );
}

void Atm_playfield::scan_matrix( bool active ) {
  int16_t sw = io->scan();
  if ( sw != 0 ) {
    switch_changed( abs( sw ), sw > 0 );
  }
}

void Atm_playfield::switch_changed( int16_t n, uint8_t v ) {
  uint16_t millis_passed = (uint16_t) millis() - prof[n].last_change;
  if ( millis_passed >= prof[n].debounce_delay ) {
    if ( v ) {
      if ( millis_passed > prof[n].retrigger_delay ) {
        prof[n].switch_state = 1;
        prof[n].last_change = millis();
        push( connectors, ON_PRESS, n, n, 1 ); 
        if ( prof[n].initialized ) prof[n].element->trigger( Atm_element::EVT_KICK ); 
      }
    } else {
      prof[n].switch_state = 0;      
      prof[n].last_change = millis();
      push( connectors, ON_RELEASE, n, n, 0 ); 
      if ( prof[n].initialized ) prof[n].element->trigger( Atm_element::EVT_RELEASE );
    }
  } else {
    io->unscan(); // Cancels the last scan() event (makes event sticky in case of debounce)
  }
}

Atm_element& Atm_playfield::element( int16_t n, int16_t coil_led /* -1 */, int16_t light_led /* -1 */, int8_t coil_profile /* -1 */, int8_t led_profile /* -1 */ ) {
  if ( !prof[n].initialized ) {
    prof[n].element = new Atm_element();  
    prof[n].element->begin( *this, coil_led, light_led, coil_profile, led_profile );
    prof[n].initialized = true;
  } else {
    if ( coil_led >= 0 || light_led >= 0 ) prof[n].element->initialize( coil_led, light_led, coil_profile, led_profile );
  }
  return *prof[n].element;
}

// The led objects should be stored in a separate pointer table so that they can be accessed later

Atm_element& Atm_playfield::led( int16_t light_led, int8_t led_profile, int16_t cnt ) {
    Atm_element* element = new Atm_element();
    element->begin( *this, -1, light_led, -1, led_profile, cnt );
    pleds->onWatch( light_led, element, Atm_element::EVT_WATCH );
    return *element;    
}

Atm_led_scheduler& Atm_playfield::leds() {
  return *pleds;
}

Atm_playfield& Atm_playfield::defineProfile( uint8_t prof, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 ) {
  pleds->defineProfile( prof, T0, L1, T1, L2 );
  return *this;
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
