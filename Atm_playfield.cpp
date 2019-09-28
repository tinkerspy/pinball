#include "Atm_playfield.hpp"

// TODO: Add catchall on onPress()/onRelease()

Atm_playfield& Atm_playfield::begin( IO& io, t_scan_callback scan, Atm_led_scheduler& led ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER  ON_LOOP  ON_EXIT  EVT_DISABLE EVT_ENABLE  EVT_TIMER, EVT_READY,     ELSE */
    /*  IDLE     */           -1,      -1,      -1,    DISABLED,        -1,        -1,     READY,     WAIT,
    /*  WAIT     */           -1,      -1,      -1,    DISABLED,        -1,      SCAN,     READY,       -1,
    /*  SCAN     */     ENT_SCAN,      -1,      -1,    DISABLED,        -1,        -1,     READY,     SCAN,
    /*  DISABLED */ ENT_DISABLED,      -1,      -1,          -1,      SCAN,        -1,     READY, DISABLED, 
    /*  READY    */    ENT_READY,      -1,      -1,          -1,      SCAN,        -1,        -1,    READY, 
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  io_scan = scan;
  this->io = &io;
  this->pleds = &led;
  memset( connectors, 0, sizeof( connectors ) );
  memset( prof, 0, sizeof( prof ) );
  timer.set( STARTUP_DELAY_MS );  
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_playfield::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
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
      pf_enabled = true;
      scan_matrix();
      return;
    case ENT_DISABLED:
      pf_enabled = false;
      scan_matrix();
      return;
    case ENT_READY:
      pf_enabled = false;
      scan_matrix();
      return;
  }
}

Atm_playfield& Atm_playfield::disable() {
  trigger( EVT_DISABLE );
  return *this;   
}

Atm_playfield& Atm_playfield::enable() {
  trigger( EVT_ENABLE );
  return *this;   
}

bool Atm_playfield::enabled() {
  return pf_enabled;   
}

bool Atm_playfield::isPressed( int16_t n ) {
  return io->isPressed( n );
}

void Atm_playfield::scan_matrix( void ) {
  int16_t sw = io_scan();
  if ( sw != 0 ) {
    if ( sw > 0 ) {
      push( connectors, ON_PRESS, sw, sw, 1 ); 
      if ( prof[sw].initialized ) prof[sw].element->trigger( Atm_element::EVT_KICK ); 
    } else {
      sw = abs( sw );
      push( connectors, ON_RELEASE, sw, sw, 0 ); 
      if ( prof[sw].initialized ) prof[sw].element->trigger( Atm_element::EVT_RELEASE );
    }
  }
}

void Atm_playfield::switch_changed( int16_t n, uint8_t v ) {

}

Atm_element& Atm_playfield::element( int16_t n, int16_t coil_led /* -1 */, int16_t light_led /* -1 */, int8_t coil_profile /* -1 */, int8_t led_profile /* -1 */ ) {
  if ( !prof[n].initialized ) {
    prof[n].element = new Atm_element();  
    prof[n].element->begin( *this, n, coil_led, light_led, coil_profile, led_profile );
    prof[n].initialized = true;
  } else {
    if ( coil_led >= 0 || light_led >= 0 ) prof[n].element->initialize( coil_led, light_led, coil_profile, led_profile );
  }
  return *prof[n].element;
}

// The led/watch objects should perhaps be stored in a separate pointer table so that they can be accessed later

Atm_element& Atm_playfield::watch( int16_t light_led, int16_t cnt ) {
    Atm_element* element = new Atm_element();
    element->begin( *this, -1, -1, light_led, -1, -1, cnt );
    pleds->onWatch( light_led, element, Atm_element::EVT_WATCH );
    return *element;    
}

Atm_element& Atm_playfield::led( int16_t light_led, int8_t led_profile ) {
    Atm_element* element = new Atm_element();
    element->begin( *this, -1, -1, light_led, -1, led_profile );
    return *element;    
}

Atm_led_scheduler& Atm_playfield::leds() {
  return *pleds;
}

Atm_playfield& Atm_playfield::defineProfile( uint8_t prof, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 ) {
  pleds->defineProfile( prof, T0, L1, T1, L2 );
  return *this;
}

bool Atm_playfield::ready() {
  return state() == READY;
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
    "PLAYFIELD\0EVT_DISABLE\0EVT_ENABLE\0EVT_TIMER\0EVT_READY\0ELSE\0IDLE\0WAIT\0SCAN\0DISABLED\0READY" );
  return *this;
}
