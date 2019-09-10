#include "Atm_led_device.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_led_device& Atm_led_device::begin( Atm_led_scheduler &leds, const int16_t* device_script ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_NOTIFY  ELSE */
    /*   IDLE */         -1, ATM_SLEEP,      -1,     NOTIFY,   -1,
    /* NOTIFY */ ENT_NOTIFY,        -1,      -1,         -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  parse_code( device_script );
  this->leds = &leds;
  trigger_flags = 0;
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
      for ( uint8_t i = 0; i < 8; i++ ) {
        if ( trigger_flags & ( 1 << i ) ) {
          trigger_flags &= ~( 1 << i );
          push( connectors, ON_CHANGE, i, i, 0 );
        }
      }
      return;
  }
}

Atm_led_device& Atm_led_device::parse_code( const int16_t* device_script ) {
  while ( device_script[0] != -1 ) {
    int did = device_script[0];
    event_ptr[did] = &device_script[1];
    device_script++;
    while ( device_script[0] != -1 ) {
      device_script++;
    }
    device_script++;
  }
  return *this;
}

void Atm_led_device::run_code( int16_t e ) {
  if ( e > -1 ) {
    int counter = 0;
    const int16_t* p = event_ptr[e];
    while ( *p != -1 ) {
      int16_t opcode = *p++;
      int16_t selector = *p++;
      int16_t action_t = *p++;
      int16_t action_f = *p++;
      switch ( opcode ) {
        case 'J':
          if ( leds->active( selector ) ) {
            if ( action_t > - 1 ) {
              p += action_t * 4;          
            } else {
              return;
            }
          } else {
            if ( action_f > - 1 ) {
              p += action_f * 4;          
            } else {
              return;
            }
          }
          break;
        case 'H':
          leds->on( leds->active( selector ) ? action_t : action_f );
          break;
        case 'L':
          leds->off( leds->active( selector ) ? action_t : action_f );
          break;
        case 'S':
          run_code( leds->active( selector ) ? action_t : action_f );
          break;
        case 'C':
          counter += leds->active( selector ) ? action_t : action_f;
          break;
        case 'T':
          if ( counter == selector ) {
            if ( action_t > -1 ) 
              trigger_flags |= ( 1 << action_t );
          } else {
            if ( action_f > -1 ) 
              trigger_flags |= ( 1 << action_f );
          }
          break;
      };
    }
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_led_device& Atm_led_device::trigger( int event ) {
  run_code( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_led_device::state( void ) {
  return Machine::state();
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
