#include "Atm_element.hpp"

Atm_element& Atm_element::begin( Atm_led_scheduler& led, int16_t coil /* -1 */, int16_t light /* -1 */, uint8_t coil_profile /* 0 */, uint8_t led_profile /* 1 */  ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                   ON_ENTER    ON_LOOP  ON_EXIT    EVT_ON    EVT_OFF  EVT_TOGGLE, EVT_KICK  EVT_RELEASE  EVT_INPUT  EVT_INIT  EVT_DISABLE  EVT_ENABLE  EVT_TIMER    EVT_LIT      ELSE */
    /*      IDLE */            -1, ATM_SLEEP,      -1, LIGHT_ON, LIGHT_OFF,     TOGGLE,  KICKING,     RELEASE, INPUTTING,     INIT,    DISABLED,         -1,        -1,        -1,       -1,
/* DELAY IS NOT USED ANYMORE */
    /*     DELAY */            -1,        -1,      -1,       -1,        -1,         -1,       -1,        IDLE,        -1,       -1,          -1,         -1,   KICKING,        -1,       -1,
    /*   KICKING */   ENT_KICKING,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1,        -1,     IDLE,
/* Insert extra wait state to stop machine gunning (retrigger) */    
    /*  DISABLED */            -1, ATM_SLEEP,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,       IDLE,        -1,        -1,       -1,
    /*      INIT */      ENT_INIT,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1,        -1,     IDLE,
    /* INPUTTING */     ENT_INPUT,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1,        -1,     IDLE,
    /*   RELEASE */   ENT_RELEASE,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1,        -1,     IDLE,
    /*  LIGHT_ON */  ENT_LIGHT_ON,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1,        -1,     IDLE,
    /* LIGHT_OFF */ ENT_LIGHT_OFF,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1,        -1,     IDLE,
    /*    TOGGLE */            -1,        -1,      -1,       -1,        -1,         -1,       -1,          -1,        -1,       -1,          -1,         -1,        -1, LIGHT_OFF, LIGHT_ON,

  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  light_led = light;
  coil_led = coil;
  this->led = &led;
  led.profile( coil_led, coil_profile );
  led.profile( light_led, led_profile );
  switch_state = false;
  autolight = false;
  autokick = true;
  memset( connectors, 0, sizeof( connectors ) ); // Needed for dynamically allocated memory?
  changed = millis();
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_element::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_LIT:
      return state();
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_INIT, <sub>, <v>, <up> );
 *   push( connectors, ON_INPUT, <sub>, <v>, <up> );
 *   push( connectors, ON_KICK, <sub>, <v>, <up> );
 *   push( connectors, ON_LIGHT, <sub>, <v>, <up> );
 */

void Atm_element::action( int id ) {
  changed = millis();
  switch ( id ) {
    case ENT_KICKING:
      switch_state = true;
      if ( autokick ) 
        led->on( coil_led );
      if ( autolight ) { 
        connectors[ON_LIGHT+1].push( 1 ); 
        led->on( light_led );
      }
      //connectors[ON_KICK+2].push( 1 );
      if ( state() ) {  
        connectors[ON_KICK+1].push( 1 );
      } else {
        connectors[ON_KICK+0].push( 1 );
      }
      if ( state() ) { 
        if ( score_lit) counter->trigger( score_lit );
      } else {
        if ( score_unlit) counter->trigger( score_unlit );
      }
      return;
    case ENT_INIT:
      connectors[ON_INIT].push( 1 );
      led->off( light_led );
      return;
    case ENT_INPUT:
      //connectors[ON_INPUT+2].push( 1 );
      if ( state() ) {  
        connectors[ON_INPUT+1].push( 1 );
      } else {
        connectors[ON_INPUT+0].push( 1 );
      }
      return;
    case ENT_RELEASE:
      switch_state = false;
      led->off( coil_led );      
      return;
    case ENT_LIGHT_ON:
      led->on( light_led );
      //connectors[ON_LIGHT+2].push( 1 );
      connectors[ON_LIGHT+1].push( 1 ); 
      return;
    case ENT_LIGHT_OFF:
      led->off( light_led );
      //connectors[ON_LIGHT+2].push( 1 );
      connectors[ON_LIGHT+0].push( 1 );
      return;
  }
}

uint32_t Atm_element::idle() {
  return millis() - changed;
}

bool Atm_element::idle( uint32_t maximum ) { // maximum time idle
  return ( millis() - changed ) < maximum;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_element& Atm_element::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_element::state( void ) {
  // If there's a led return its state else return the switch state
  if ( light_led > -1 ) {
    return led->active( light_led ); 
  } else {
    return switch_state;    
  }
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_element& Atm_element::on() {
  trigger( EVT_ON );
  return *this;
}

Atm_element& Atm_element::off() {
  trigger( EVT_OFF );
  return *this;
}

Atm_element& Atm_element::kick() {
  trigger( EVT_KICK );
  return *this;
}

Atm_element& Atm_element::release() {
  trigger( EVT_RELEASE );
  return *this;
}

Atm_element& Atm_element::input() {
  trigger( EVT_INPUT );
  return *this;
}

Atm_element& Atm_element::init() {
  trigger( EVT_INIT );
  return *this;
}

Atm_element& Atm_element::disable() {
  trigger( EVT_DISABLE );
  return *this;
}

Atm_element& Atm_element::enable() {
  trigger( EVT_ENABLE );
  return *this;
}

Atm_element& Atm_element::toggle() {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_element& Atm_element::autoLight( int v ) {
  autolight = v ? 1 : 0;
  return *this;
}

Atm_element& Atm_element::autoKick( int v ) {
  autokick = v ? 1 : 0;
  return *this;
}

Atm_element& Atm_element::onScore( Machine& machine, int event_unlit, int event_lit /* = -1 */ ) {
  score_unlit = event_unlit;
  score_lit = event_lit == -1 ? event_unlit : event_lit;
  counter = &machine;
  return *this;
}

/*
 * onInit() push connector variants ( slots 3, autostore 0, broadcast 0 )
 */

Atm_element& Atm_element::onInit( Machine& machine, int event ) {
  onPush( connectors, ON_INIT, 0, 3, 1, machine, event );
  return *this;
}

Atm_element& Atm_element::onInit( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_INIT, 0, 3, 1, callback, idx );
  return *this;
}

/*
 * onInput() push connector variants ( slots 3, autostore 0, broadcast 0 )
 */

Atm_element& Atm_element::onInput( Machine& machine, int event ) {
  onPush( connectors, ON_INPUT, 0, 3, 1, machine, event );
  return *this;
}

Atm_element& Atm_element::onInput( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_INPUT, 0, 3, 1, callback, idx );
  return *this;
}

Atm_element& Atm_element::onInput( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_INPUT, sub, 3, 0, machine, event );
  return *this;
}

Atm_element& Atm_element::onInput( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_INPUT, sub, 3, 0, callback, idx );
  return *this;
}

/*
 * onPress() push connector variants ( slots 3, autostore 0, broadcast 0 )
 */

Atm_element& Atm_element::onPress( Machine& machine, int event ) {
  onPush( connectors, ON_KICK, 0, 3, 1, machine, event );
  return *this;
}

Atm_element& Atm_element::onPress( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_KICK, 0, 3, 1, callback, idx );
  return *this;
}

Atm_element& Atm_element::onPress( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_KICK, sub, 3, 0, machine, event );
  return *this;
}

Atm_element& Atm_element::onPress( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_KICK, sub, 3, 0, callback, idx );
  return *this;
}

/*
 * onLight() push connector variants ( slots 3, autostore 0, broadcast 0 )
 */

Atm_element& Atm_element::onLight( Machine& machine, int event ) {
  onPush( connectors, ON_LIGHT, 0, 3, 1, machine, event );
  return *this;
}

Atm_element& Atm_element::onLight( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_LIGHT, 0, 3, 1, callback, idx );
  return *this;
}

Atm_element& Atm_element::onLight( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_LIGHT, sub, 3, 0, machine, event );
  return *this;
}

Atm_element& Atm_element::onLight( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_LIGHT, sub, 3, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_element& Atm_element::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "ELEMENT\0EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_KICK\0EVT_RELEASE\0EVT_INPUT\0EVT_INIT\0EVT_DISABLE\0EVT_ENABLE\0EVT_TIMER\0EVT_LIT\0ELSE\0IDLE\0DELAY\0KICKING\0DISABLED\0INIT\0INPUTTING\0RELEASE\0LIGHT_ON\0LIGHT_OFF\0TOGGLE" );
  return *this;
}
