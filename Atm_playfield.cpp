#include "Atm_playfield.hpp"

// TODO: Add catchall on onPress()/onRelease()

Atm_playfield& Atm_playfield::begin( IO& io, Atm_led_scheduler& led ) {
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
  this->io = &io;
  this->pleds = &led;
  memset( connectors, 0, sizeof( connectors ) );
  memset( prof, 0, sizeof( prof ) );
  debounce( 5, 0, 0 );
  timer.set( STARTUP_DELAY_MS );
  numberofSwitches = io.numSwitches();  
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

Atm_playfield& Atm_playfield::debounce( uint8_t b, uint16_t r, uint16_t m ) {
  for ( int16_t i = 0; i < MAX_SWITCHES; i++ ) {
    prof[i].break_delay = b; 
    prof[i].make_delay = m; 
    prof[i].retrigger_delay = r; 
  }
  return *this;   
}

Atm_playfield& Atm_playfield::debounce( int16_t n, uint8_t b, uint16_t r, uint16_t m ) {
  prof[n].break_delay = b; 
  prof[n].make_delay = m; 
  prof[n].retrigger_delay = r; 
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

bool Atm_playfield::enabled() {
  return pf_enabled;   
}

bool Atm_playfield::isPressed( int16_t n ) {
  return io->isPressed( n );
}

void Atm_playfield::scan_matrix( void ) {
  int16_t sw = io->scan();
  if ( sw != 0 ) {
    switch_changed( abs( sw ), sw > 0 );
  }
}

void Atm_playfield::switch_changed( int16_t n, uint8_t v ) {
  uint16_t millis_passed;
  millis_passed = (uint16_t) millis() - prof[n].last_change;
/*
  Serial.print( n );
  Serial.print( " make_delay: " );
  Serial.print( prof[n].make_delay );
  Serial.print( " break_delay: " );
  Serial.println( prof[n].break_delay );
  */
  if ( v ) {
    if ( prof[n].make_delay > 0 ) {
  //    Serial.print( "make_delay: " );
      if ( !prof[n].make_wait ) prof[n].last_change = millis();
      millis_passed = (uint16_t) millis() - prof[n].last_change;
  //    Serial.print( millis_passed );
  //    Serial.print( " < " );
  //    Serial.println( prof[n].make_delay );
      if ( millis_passed < prof[n].make_delay ) {
        prof[n].make_wait = 1;
        io->unscan(); // Cancels the last scan() event (makes event sticky in case of debounce)
        return;
      }
      prof[n].make_wait = 0;
    } 
    if ( millis_passed > prof[n].retrigger_delay ) {
      prof[n].switch_state = 1;
      push( connectors, ON_PRESS, n, n, 1 ); 
      if ( prof[n].initialized ) prof[n].element->trigger( Atm_element::EVT_KICK ); 
      prof[n].last_change = millis();
      prof[n].make_wait = 0;
      return;
    }
  } else {
    if ( ( millis_passed >= prof[n].break_delay ) ) {
      prof[n].switch_state = 0;      
      push( connectors, ON_RELEASE, n, n, 0 ); 
      if ( prof[n].initialized ) prof[n].element->trigger( Atm_element::EVT_RELEASE );
      prof[n].last_change = millis();
      prof[n].make_wait = 0;
      return;
    }
  }
  io->unscan(); // Cancels the last scan() event (makes event sticky in case of debounce)
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

// TODO: Voor een switch group het device object koppelen aan alle fysieke switches!

Atm_led_device& Atm_playfield::device( int16_t n, int16_t led_group /* = -1 */, int16_t* device_script /* = NULL */ ) {
  if ( n == -1 ) { // Create a floating device
    Atm_led_device* device = new Atm_led_device();
    device->begin( *this, led_group, device_script );
    return *device;
  }
  if ( !prof[n].device_initialized ) { 
    prof[n].device = new Atm_led_device(); // Create attached device 
    prof[n].device->begin( *this, led_group, device_script );
    prof[n].device_initialized = true;
  } else { 
    if ( device_script ) prof[n].device->set_script( device_script );
    if ( led_group ) prof[n].device->set_led( led_group );
  }
  return *prof[n].device;
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
