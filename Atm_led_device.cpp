#include "Atm_led_device.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_led_device& Atm_led_device::begin( Atm_playfield &playfield, int16_t led_group, int16_t* device_script ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_NOTIFY  ELSE */
    /*   IDLE */         -1, ATM_SLEEP,      -1,     NOTIFY,   -1,
    /* NOTIFY */ ENT_NOTIFY,        -1,      -1,         -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->playfield = &playfield;
  this->leds = &playfield.leds();
  global_counter = 0;
  trigger_flags = 0;
  input_persistence = 0; 
  output_persistence = 0;
  memset( connectors, 0, sizeof( connectors ) ); // This is really needed!
  if ( led_group != -1 ) set_led( led_group );
  if ( device_script ) set_script( device_script );
  return *this;          
}

Atm_led_device& Atm_led_device::set_script( int16_t* script ) {
  this->script = parse_code( script );
  run_code( 0 );
  return *this;
}

Atm_led_device& Atm_led_device::set_led( int16_t led_group ) {
  this->led_group = led_group;
  return *this;
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_led_device::event( int id ) {
  switch ( id ) {
    case EVT_NOTIFY:
      return trigger_flags > 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_CHANGE, <sub>, <v>, <up> );
 */

void Atm_led_device::action( int id ) {
  switch ( id ) {
    case ENT_NOTIFY:
      for ( uint8_t i = 0; i < 16; i++ ) {
        if ( trigger_flags & ( 1 << i ) ) {
          if ( playfield->enabled() ) 
            push( connectors, ON_CHANGE, i, i, 0 );
        }
      }
      trigger_flags = 0;
      return;
  }
}

int16_t* Atm_led_device::parse_code( int16_t* device_script ) {
  int16_t* p = device_script;
  while ( p[0] != -1 ) *p++ = 0;
  numberOfInputs = p - device_script;
  p++;
  while ( p[0] != -1 ) {
    int iid = p[0];
    if ( iid > -1 && iid < numberOfInputs ) {
      device_script[iid] = ( p - device_script ) + 1;
    } else {
      if ( callback_trace ) 
        stream_trace->printf( "Parse error: illegal input id %d (max input number: %d)\n", iid, numberOfInputs );        
    }
    p++;
    while ( p[0] != -1 ) {
      //if ( callback_trace ) 
      //  stream_trace->printf( "Atm_led_device parse %03d: %c %d ? %d : %d\n", iid, p[0], p[1], p[2], p[3] );
      p += 4;
    }
    p++;
  }
  return device_script;
}

void Atm_led_device::run_code( int16_t e ) {
  if ( e > -1 && e < numberOfInputs && script[e] > 0 ) {
    int16_t p = script[e];
    if ( callback_trace ) 
      stream_trace->printf( "run_code %03d called\n", e );
    while ( script[p] != -1 ) {
      int16_t opcode = script[p++];
      int16_t selector = script[p++];
      int16_t action_t = script[p++];
      int16_t action_f = script[p++];
      int16_t selected_action = 0;
      if ( callback_trace ) 
        stream_trace->printf( "run_code %03d: %c %d ? %d : %d\n", e, opcode, selector, action_t, action_f );
      switch ( opcode ) {
        // Add: JC jump on counter, RP repeat event with delay
        case 'J': // Jump on LED state
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          if ( selected_action  > -1 ) {
            p += selected_action * 4;          
          } else {
            if ( callback_trace ) 
              stream_trace->printf( "run_code %03d: jump exit\n", e );
            return;
          }            
          break;
        case 'H': // ON - HIGH: led on
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          leds->on( leds->index( led_group, selected_action ) );
          break;
        case 'L': // OF - LOW: led off
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          leds->off( leds->index( led_group, selected_action ) );
          break;
        case 'S': // SB - SUB: subroutine call
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          run_code( selected_action );
          break;
        case 'I': // IC - INC: increment counter
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          if ( selected_action > - 1 ) {
            global_counter += selected_action;
          } else {
            global_counter = 0;
          }
          break;
        case 'D': // DC - DEC: decrement counter
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          if ( selected_action > - 1 ) {
            global_counter -= selected_action;
          } else {
            global_counter = 0;
          }
          break;
        case 'T': // TC - TRIG: trigger external event on counter
          selected_action = ( global_counter == selector ) ? action_t : action_f;
          if ( selected_action > -1 ) {
              trigger_flags |= ( 1 << selected_action );
          } 
          sleep( 0 );
          break;
        case 'P': // IP/OP - Input persistence / Output persistence (default off)
          selected_action = leds->active( leds->index( led_group, selector ) ) ? action_t : action_f;
          input_persistence = selected_action;
          output_persistence = selected_action;
          break;
      }
    }
    if ( callback_trace ) 
      stream_trace->printf( "run_code %03d: regular exit\n", e );
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_led_device& Atm_led_device::trigger( int event ) {
  if ( playfield->enabled() )
    run_code( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_led_device::state( void ) {
  return global_counter;
}


Atm_led_device& Atm_led_device::init( void ) {
  run_code( 0 );
  return *this;  
}

Atm_led_device& Atm_led_device::press( void ) {
  run_code( 1 );
  return *this;  
}

Atm_led_device& Atm_led_device::release( void ) {
  run_code( 2 );
  return *this;  
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

/*
 * onChange() push connector variants ( slots 8, autostore 0, broadcast 0 )
 */

Atm_led_device& Atm_led_device::onChange( Machine& machine, int event ) {
  onPush( connectors, ON_CHANGE, 0, 8, 1, machine, event );
  return *this;
}

Atm_led_device& Atm_led_device::onChange( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CHANGE, 0, 8, 1, callback, idx );
  return *this;
}

Atm_led_device& Atm_led_device::onChange( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_CHANGE, sub, 8, 0, machine, event );
  return *this;
}

Atm_led_device& Atm_led_device::onChange( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CHANGE, sub, 8, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_led_device& Atm_led_device::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "LED_DEVICE\0EVT_NOTIFY\0ELSE\0IDLE\0NOTIFY" );
  return *this;
}
