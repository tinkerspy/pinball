#include "Atm_device.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_device& Atm_device::begin( Atm_switch_matrix* playfield, int16_t led_group, int16_t* device_script ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_NOTIFY  EVT_TIMER  EVT_YIELD  ELSE */
    /*   IDLE */         -1, ATM_SLEEP,      -1,     NOTIFY,        -1,     YIELD,   -1,
    /* NOTIFY */ ENT_NOTIFY,        -1,      -1,         -1,        -1,        -1, IDLE,
    /*  YIELD */         -1,        -1,      -1,     NOTIFY,    RESUME,        -1,   -1,
    /* RESUME */ ENT_RESUME,        -1,      -1,         -1,        -1,        -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->playfield = playfield;
  this->leds = playfield->leds();
  trigger_flags = 0;
  input_persistence = 0; 
  output_persistence = 0;
  next = NULL;
  enabled = true;
  memset( connectors, 0, sizeof( connectors ) ); // This is really needed!
  memset( registers, 0, sizeof( registers ) ); 
  timer.set( ATM_TIMER_OFF );
  core[0].ptr = 0;
  core[1].ptr = 0;
  switches = 0;
  if ( device_script != NULL ) {
    set_led( led_group );
    set_script( device_script );
  }
  return *this;          
}

Atm_device& Atm_device::set_script( int16_t* script ) {
  this->script = script;
  parse_code( this->script );
  start_code( 0 );
  return *this;
}

Atm_device& Atm_device::set_led( int16_t led_group ) {
  this->led_group = led_group;
  return *this;
}

Atm_device& Atm_device::reg( uint8_t r, int16_t v ) {
  registers[r] = v;
  return *this;
}

int16_t Atm_device::reg( uint8_t r ) {
  return registers[r];
}

Atm_device& Atm_device::chain( Atm_device& next ) {
  //Serial.printf( "dev %x: set next to %x\n", (long)(this), (long)&next ); 
  this->next = &next;  
  return *this;
}

Atm_device& Atm_device::chain( int16_t sw ) {
  //Serial.printf( "dev %x: set next to %x\n", (long)(this), (long)&next ); 
  this->next = &(playfield->device( sw ));  
  return *this;
}

Atm_device& Atm_device::select( uint32_t mask ) {
  this->enabled = mask & 1;
  if ( next ) {
    next->select( mask >> 1 );
  }
  return *this;
}

Atm_device& Atm_device::dump( Stream & stream ) {
  int16_t l = parse_code( this->script );
  for ( int16_t i = 0; i < l; i++ ) {
    if ( i && i % 16 == 0 ) {
      stream.println();
    }
    stream.printf( "%4d, ", script[i] );
  }
  stream.println();
  return *this;
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_device::event( int id ) {
  switch ( id ) {
    case EVT_NOTIFY:
      return trigger_flags > 0;
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_YIELD:
      return timer.value != ATM_TIMER_OFF;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_CHANGE, <sub>, <v>, <up> );
 */

void Atm_device::action( int id ) {
  switch ( id ) {
    case ENT_NOTIFY:
      for ( uint8_t i = 0; i < 16; i++ ) {
        if ( trigger_flags & ( 1 << i ) ) {
          if ( playfield->enabled() || output_persistence ) {
            push( connectors, ON_EVENT, i, i, 0 );
          }
        }
      }
      trigger_flags = 0;
      return;
    case ENT_RESUME:
      timer.set( ATM_TIMER_OFF );
      run_code( 0 );
      return;
  }
}

int16_t Atm_device::parse_code( int16_t* device_script ) {
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
      //  stream_trace->printf( "Atm_device parse %03d -> %d: %c %d ? %d : %d\n", iid, p - device_script, p[0], p[1], p[2], p[3] );
      p += 4;
    }
    p++;
  }
  return p - device_script;
}

int16_t Atm_device::led_index( int16_t led_group, int16_t selector ) {
  int16_t n = leds->index( led_group, selector );
  if ( callback_trace ) { 
    if ( n != -1 ) { 
      stream_trace->printf( "Atm_device led arg #%d maps to physical led %d\n", selector, n );
    } else { 
      stream_trace->printf( "Atm_device led arg #%d not used\n", selector );    
    }
  }
  return n;  
}

bool Atm_device::led_active( int16_t led_group, int16_t selector ) {
  if ( led_group != -1 &&  selector != -1 ) {
    return leds->active( led_index( led_group, selector ) );
  } 
  return false;
}

void Atm_device::led_on( int16_t led_group, int16_t selector ) {
  if ( led_group != -1 && selector != -1 ) {
    leds->on( led_index( led_group, selector ) );
  } 
}

void Atm_device::led_off( int16_t led_group, int16_t selector ) {
  if ( led_group != -1 &&  selector != -1 ) {
    leds->off( led_index( led_group, selector ) );
  } 
}

void Atm_device::start_code( int16_t e ) {
  if ( e > -1 && e < numberOfInputs && script[e] > 0 ) { 
    uint8_t active_core = core[0].ptr == 0 ? 0 : 1; // When the primary core is active we take the secundary
    core[active_core].reg_ptr = 0;
    core[active_core].stack_ptr = 0;
    core[active_core].ptr = script[e];
    core[active_core].yield_enabled = ( active_core == 0 );
    if ( callback_trace ) 
      stream_trace->printf( "run_code event %03d called -> %d%03d\n", e, active_core, core[active_core].ptr );
    run_code( active_core );      
  }
}

void Atm_device::run_code( uint8_t active_core ) {
  if ( core[active_core].ptr > 0 ) {
    while ( true ) {
      int16_t opcode = script[core[active_core].ptr++];
      int16_t selector = script[core[active_core].ptr++];
      int16_t action_t = script[core[active_core].ptr++];
      int16_t action_f = script[core[active_core].ptr++];
      int16_t selected_action = 0;
      if ( opcode > -1 ) {
        if ( callback_trace ) 
          stream_trace->printf( "run_code %d:%03d: %c %d ? %d : %d\n", active_core, core[active_core].ptr - 4, ( opcode > -1 ? opcode : '#' ), selector, action_t, action_f );
        switch ( opcode ) {
          case 'J': // JmpL
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            if ( selected_action  > -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( callback_trace ) 
                stream_trace->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case 'A': // JmpLA
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            if ( selected_action  > -1 ) {
              core[active_core].ptr = script[selected_action];          
            }
            break;
          case 'C': // JmpRE
            selected_action = ( registers[core[active_core].reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action  > -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( callback_trace ) 
                stream_trace->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case '0':  // Prim
            selected_action = ( active_core == 0 ? action_t : action_f );
            if ( selected_action  > -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( callback_trace ) 
                stream_trace->printf( "run_code %d:%03d: core exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;            
          case 'H': // LedOn
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            led_on( led_group, selected_action );
            break;
          case 'L': // LedOff
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            led_off( led_group, selected_action );
            break;
          case 'S': // GoSub
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            core[active_core].stack[core[active_core].stack_ptr++] = core[active_core].ptr;          
            core[active_core].ptr = script[selected_action];
            break;
          case 'I': // Inc
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            if ( selected_action > - 1 ) {
              registers[core[active_core].reg_ptr] += selected_action;
            } else {
              registers[core[active_core].reg_ptr] = 0;
            }
            break;
          case 'D': // Dec
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            if ( selected_action > - 1 ) {
              registers[core[active_core].reg_ptr] -= selected_action;
            } else {
              registers[core[active_core].reg_ptr] = 0;
            }
            break;
          case 'T': // Trig
            selected_action = ( registers[core[active_core].reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action > -1 ) {
                trigger_flags |= ( 1 << selected_action );
            } 
            sleep( 0 );
            break;
          case 'P': // PersI/PersO - Input persistence / Output persistence (default off)
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            input_persistence = selected_action;
            output_persistence = selected_action;
            break;
          case 'R': // Reg
            core[active_core].reg_ptr = led_active( led_group, selector ) ? action_t : action_f;
            break;           
          case 'Y': // Yield
            if ( core[active_core].yield_enabled ) {
              selected_action = led_active( led_group, selector ) ? action_t : action_f;
              selected_action = selected_action > 0 ? selected_action : registers[abs(selected_action)];
              timer.set( selected_action );
              sleep( 0 );
              if ( callback_trace ) 
                stream_trace->printf( "run_code %d:%03d: yield %d ms\n", active_core, core[active_core].ptr - 4, selected_action );
            } else {
              if ( callback_trace ) 
                stream_trace->printf( "run_code %d:%03d: FATAL error: secondary core cannot yield at %d\n", active_core, core[active_core].ptr - 4 );
              return;
            }
            return;           
          default:
            if ( callback_trace ) 
                stream_trace->printf( "run_code %d:%03d: abort, illegal opcode '%c', script out of sync? (missing comma?)\n", active_core, core[active_core].ptr - 4, opcode );
            return;
        }
      } else {
        core[active_core].ptr = 0;
      }        
      if ( core[active_core].ptr == 0 ) {
        if ( core[active_core].stack_ptr > 0 ) {
          if ( callback_trace ) 
            stream_trace->printf( "run_code %d:%03d: Return to %d\n", active_core, core[active_core].ptr, core[active_core].stack[core[active_core].stack_ptr-1] );
          core[active_core].ptr = core[active_core].stack[--core[active_core].stack_ptr];
        } else {
          if ( callback_trace ) 
            stream_trace->printf( "run_code %d:%03d: regular exit\n", active_core, core[active_core].ptr );
          return;
        }
      }
    }
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_device& Atm_device::trigger( int event ) {
  //Serial.printf( "%x trigger %d\n", (long)(this), event );
  if ( next ) {
    //Serial.printf( "%x next %x\n", (long)(this), (long)next );
    next->trigger( event );
  }
  if ( playfield->enabled() || input_persistence ) {
    if ( this->enabled ) {
      if ( event > 0 and event < 65 ) {
        int16_t sw = event - 1; // sw = 0..63
        if ( ( sw & 1 ) == 0 ) { // Press if bit 0 is not set
          switches |= ( 1 << ( sw >> 1 ) );  // Switch press -> set switch bit
        } else {
          switches &= ~( 1 << ( sw >> 1 ) ); // Switch release -> clear switch bit
        }
      }
      start_code( event );
    }
  }
  return *this;
}

Atm_device& Atm_device::trigger( int event, uint32_t sel ) {
  if ( next && sel > 1 ) {
    next->trigger( event, sel >> 1 );
  }
  if ( playfield->enabled() || input_persistence ) {
    if ( sel & 1 ) {
      if ( event > 0 and event < 65 ) {
        int16_t sw = event - 1; // sw = 0..63
        if ( ( sw & 1 ) == 0 ) { // Press if bit 0 is not set
          switches |= ( 1 << ( sw >> 1 ) );  // Switch press -> set switch bit
        } else {
          switches &= ~( 1 << ( sw >> 1 ) ); // Switch release -> clear switch bit
        }
      }
      start_code( event );
    }
  }
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_device::state( void ) {
  if ( next ) {
    return next->state() + ( enabled ? registers[0] : 0 );
  } else {
    return registers[0];
  }
}

int Atm_device::state( uint32_t sel ) {
  if ( next && sel > 1 ) {
    return next->state( sel >> 1 ) + ( sel & 1 ? registers[0] : 0 );
  } else {
    return registers[0];
  }
}

Atm_device& Atm_device::init( void ) {
  trigger( 0 );
  return *this;  
}

Atm_device& Atm_device::init( uint32_t sel ) {
  trigger( 0, sel );
  return *this;  
}

Atm_device& Atm_device::press( void ) {
  trigger( 1 );
  return *this;  
}

Atm_device& Atm_device::release( void ) {
  trigger( 2 );
  return *this;  
}

Atm_device& Atm_device::onEvent( int sub, Machine& machine, int event ) {
  if ( next ) next->onEvent( sub, machine, event );    
  if ( enabled ) onPush( connectors, ON_EVENT, sub, 8, 0, machine, event );
  return *this;
}

Atm_device& Atm_device::onEvent( int sub, atm_cb_push_t callback, int idx ) {
  if ( next ) next->onEvent( sub, callback, idx );    
  if ( enabled ) onPush( connectors, ON_EVENT, sub, 8, 0, callback, idx );
  return *this;
}

Atm_device& Atm_device::onEvent( int sub, int sw, int event ) {
  if ( next ) next->onEvent( sub, playfield->device( sw ), event );    
  if ( enabled ) onPush( connectors, ON_EVENT, sub, 8, 0, playfield->device( sw ), event );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_device& Atm_device::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "LED_DEVICE\0EVT_NOTIFY\0EVT_TIMER\0EVT_YIELD\0ELSE\0IDLE\0NOTIFY\0YIELD\0RESUME" );
  Serial.printf( "%d Tracing enabled %s@%X\n", millis(), symbols, (long)(this) );
  return *this;
}

Atm_device& Atm_device::trace( void ) {
  Serial.printf( "%d Tracing disabled %s@%X\n", millis(), symbols, (long)(this) );
  Machine::setTrace( NULL, NULL, "" );
  return *this;
}
