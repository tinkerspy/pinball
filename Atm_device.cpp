#include "Atm_device.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_device& Atm_device::begin( Atm_switch_matrix* playfield, int16_t switch_group, int16_t led_group, const int16_t* device_script,
    int16_t r0, int16_t r1, int16_t r2, int16_t r3, int16_t r4, int16_t r5, int16_t r6, int16_t r7 ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_NOTIFY  EVT_TIMER  EVT_YIELD  ELSE */
    /*   IDLE */         -1, ATM_SLEEP,      -1,     NOTIFY,        -1,     YIELD,   -1,
    /* NOTIFY */ ENT_NOTIFY,        -1,      -1,         -1,        -1,        -1, IDLE,
    /*  YIELD */         -1,        -1,      -1,     NOTIFY,    RESUME,        -1,   -1,
    /* RESUME */ ENT_RESUME,        -1,      -1,         -1,        -1,        -1, IDLE,
  };
  // clang-format on
  Symbolic_Machine::begin( state_table, ELSE );
  this->playfield = playfield;
  this->leds = playfield->leds();
  this->switch_group = switch_group;
  trigger_flags = 0;
  input_persistence = 0; 
  output_persistence = 0;
  next = NULL;
  enabled = true;
  memset( connectors, 0, sizeof( connectors ) ); // This is really needed!
  memset( registers, 0, sizeof( registers ) ); 
  registers[0] = r0;
  registers[1] = r1;
  registers[2] = r2;
  registers[3] = r3;
  registers[4] = r4;
  registers[5] = r5;
  registers[6] = r6;
  registers[7] = r7;
  timer.set( ATM_TIMER_OFF );
  core[0].ptr = 0;
  core[1].ptr = 0;
  xctr = 0;
  event_map = 0;
  if ( device_script != NULL ) {
    set_led( led_group );
    set_script( device_script );
  }
  return *this;          
}

int16_t Atm_device::switchGroup( void) {
  return this->switch_group;
}

int16_t Atm_device::ledGroup( void) {
  return this->led_group;
}

int16_t Atm_device::handler( int16_t e ) {
  if ( e > -1 && e < numberOfInputs && script[e] > 0 ) { 
    return script[e];
  }
  return 0;
}

Machine* Atm_device::outputPtr( int16_t n ) {
  return connectors[n].machine;
}

int16_t Atm_device::outputEvent( int16_t n ) {
  return connectors[n].event;
}

Atm_device& Atm_device::set_script( const int16_t* script ) {
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
  return next;
}

Atm_device& Atm_device::chain( int16_t sw ) {
  //Serial.printf( "dev %x: set next to %x\n", (long)(this), (long)&next ); 
  Atm_device& dev = playfield->device( sw );
  this->next = &dev;  
  return dev;
}

Atm_device& Atm_device::chain( const char s[] ) {
  return chain( playfield->findSymbol( s ) );
}

Atm_device& Atm_device::select( uint32_t mask ) {
  this->enabled = mask & 1;
  if ( next ) {
    next->select( mask >> 1 );
  }
  return *this;
}

Atm_device& Atm_device::dump( Stream & stream, int16_t offset /* = 0 */ ) {
  int16_t l = parse_code( this->script );
  for ( int16_t i = offset; i < l; i++ ) {
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
            //Serial.printf( "%X Outgoing trigger: %d\n", (long)(this), i );
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

int16_t Atm_device::parse_code( const int16_t* device_script ) {
  const int16_t* p = device_script;
  while ( p[0] != -1 ) p++; // <<<<
  numberOfInputs = p - device_script;
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
  //Serial.printf( "%d %X Start code %d\n", millis(), (long)(this), e );
  //Serial.printf( "%d > -1 && %d < %d && %d > 0\n", e, e, numberOfInputs, script[e] );
  if ( e > -1 && e < numberOfInputs && script[e] > 0 ) { 
    uint8_t active_core = core[0].ptr == 0 ? 0 : 1; // When the primary core is active we take the secundary
    core[active_core].reg_ptr = 0;
    core[active_core].stack_ptr = 0;
    core[active_core].ptr = script[e];
    core[active_core].yield_enabled = ( active_core == 0 );
    if ( trace_code ) 
      tc_stream->printf( "run_code event %03d called for %d -> %d%03d\n", e, switch_group, active_core, core[active_core].ptr );
    xctr++;
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
        if ( trace_code ) 
          tc_stream->printf( "run_code %d:%03d: %c %d ? %d : %d\n", active_core, core[active_core].ptr - 4, ( opcode > -1 ? opcode : '#' ), selector, action_t, action_f );
        switch ( opcode ) {
          case 'J': // JmpL
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case 'A': // JmpLA jump absolute on register equal
            if ( trace_code ) 
              tc_stream->printf( "run_code %d:%03d: reg %d, %d, %d, %d, %d, %d\n", active_core, core[active_core].ptr - 4, registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]  );
            selected_action = ( selector >= 0 && registers[core[active_core].reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              core[active_core].ptr = script[selected_action];          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case '=': // JmpRE
            selected_action = ( selector >= 0 && registers[core[active_core].reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case '<': // JmpRL
            selected_action = ( selector >= 0 && registers[core[active_core].reg_ptr] < selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case '>': // JmpRG
            selected_action = ( selector >= 0 && registers[core[active_core].reg_ptr] > selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;
          case '0':  // Prim DEPRECATED: use 'Y', -2, -1, -1 for this (drop as soon as binaries are refreshed)
            selected_action = ( active_core == 0 ? action_t : action_f );
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: core exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break; 
          case 'U':  // Resume (only from secondary core)
            if ( active_core == 1 ) {
              sleep( 0 );
              timer.set( 0 ); // Cut timer short
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: core resume\n", active_core, core[active_core].ptr - 4 );
            }
            break;                       
          case 'X':  // Xctr
            selected_action = ( xctr == (uint16_t)selector ? action_t : action_f );
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: xctr exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }            
            break;            
          case '!': // Stop script!
            core[active_core].stack_ptr = 0;
            core[active_core].ptr = 0;
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
            registers[core[active_core].reg_ptr] += selected_action;
            break;
          case 'Z': // Zap (really: set to value)
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            registers[core[active_core].reg_ptr] = selected_action;
            break;
          case 'D': // Dup - duplicate into named register
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            registers[selected_action] = registers[core[active_core].reg_ptr];
            break;
          case 'T': // Trig
            selected_action = ( selector >= 0 && registers[core[active_core].reg_ptr] == selector ) ? action_t : action_f;
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
            if ( trace_code ) 
              tc_stream->printf( "run_code %d:%03d: reg %d, %d, %d, %d, %d, %d\n", active_core, core[active_core].ptr - 4, registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]  );
            break;           
          case 'E': // Jump on event 
            selected_action = event_map & ( 1 << selector ) ? action_t : action_f; // Check event
            //Serial.printf( "Event %d: state %d -> %d\n", selector, event_map & ( 1 << selector ), selected_action );   
            event_map &= ~( 1 << selector ); // Clear event
            if ( selected_action  != -1 ) {
              core[active_core].ptr += selected_action * 4;          
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: jump exit\n", active_core, core[active_core].ptr - 4 );
              core[active_core].ptr = 0;
            }                      
            break;           
          case 'Y': // Yield (negative selector value uses register!)
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            selected_action = selected_action > 0 ? selected_action : registers[abs(selected_action)];
            if ( core[active_core].yield_enabled ) {
              if ( selector == -2 ) break;
              if ( selected_action >= 0 ) { // negative time values have no yield effect but do trip the core check
                timer.set( selected_action == 0 ? ATM_TIMER_OFF : selected_action ); // Zero timer means wait forever
                sleep( 0 );
                if ( trace_code ) 
                  tc_stream->printf( "run_code %d:%03d: yield %d ms\n", active_core, core[active_core].ptr - 4, selected_action );
              }
            } else {
              if ( trace_code ) 
                tc_stream->printf( "run_code %d:%03d: FATAL: secondary core cannot yield at %d, terminating thread\n", active_core, core[active_core].ptr - 4 );
              return;
            }
            return;           
          default:
            if ( trace_code ) 
              tc_stream->printf( "run_code %d:%03d: abort, illegal opcode '%c', script out of sync? (missing comma?)\n", active_core, core[active_core].ptr - 4, opcode );
            return;
        }
      } else {
        core[active_core].ptr = 0;
      }        
      if ( core[active_core].ptr == 0 ) {
        if ( core[active_core].stack_ptr > 0 ) {
          if ( trace_code ) 
            tc_stream->printf( "run_code %d:%03d: Return to %d\n", active_core, core[active_core].ptr, core[active_core].stack[core[active_core].stack_ptr-1] );
          core[active_core].ptr = core[active_core].stack[--core[active_core].stack_ptr];
        } else {
          if ( trace_code ) 
            tc_stream->printf( "run_code %d:%03d: regular exit\n", active_core, core[active_core].ptr );
          return;
        }
      }
      //if ( active_core == 0 ) { 
      //  timer.set( 0 ); // This should multitask things... (but it doesn't work as planned): check 'tc counter' 'tr counter reset' -> core exit!
      //  return;
      //}
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
  if ( event == 0 || playfield->enabled() || input_persistence ) {
    if ( this->enabled ) {
      event_map |= ( 1 << event );  // set event bit
      start_code( event ); // FIXME: Only if no code is currently running!
    }
  }
  return *this;
}

Atm_device& Atm_device::trigger( int event, uint32_t sel ) {
  //Serial.printf( "%x trigger2 %d\n", (long)(this), event );
  if ( next && sel > 1 ) {
    next->trigger( event, sel >> 1 );
  }
  if ( event == 0 || playfield->enabled() || input_persistence ) {
    if ( sel & 1 ) {
      event_map |= ( 1 << event );  // set event bit
      start_code( event ); // FIXME: Only if no code is currently running!
    }
  }
  return *this;
}

Atm_device& Atm_device::loadSymbols( const char s[] ) {
  Symbolic_Machine::loadSymbols( s );
  return *this;
}

Atm_device& Atm_device::linkSymbols( symbolic_machine_table* sym ) {
  Symbolic_Machine::linkSymbols( sym );
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

Atm_device& Atm_device::onEvent( const char sub_str[], const char sw_str[], const char event_str[] ) {
  int sub = this->findSymbol( sub_str );
  int sw = playfield->findSymbol( sw_str );
  int event = playfield->device( sw ).findSymbol( event_str );
  if ( next ) next->onEvent( sub, playfield->device( sw ), event );    
  //Serial.printf( "%s -> %s:%s --- %d -> %d:%d\n", sub_str, sw_str, event_str, sub, sw, event );
  if ( enabled ) onPush( connectors, ON_EVENT, sub, 8, 0, playfield->device( sw ), event );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */
Atm_device& Atm_device::traceCode( Stream & stream, uint8_t bitmap /* = 1 */ ) {
  tc_stream = &stream;
  trace_code = bitmap;
  return *this;
}
  
Atm_device& Atm_device::trace( Stream & stream ) {
  Symbolic_Machine::setTrace( &stream, atm_serial_debug::trace,
    "DEVICE\0EVT_NOTIFY\0EVT_TIMER\0EVT_YIELD\0ELSE\0IDLE\0NOTIFY\0YIELD\0RESUME" );
  Serial.printf( "%d Tracing enabled %s@%X\n", millis(), symbols, (long)(this) );
  return *this;
}

Atm_device& Atm_device::trace( void ) {
  Serial.printf( "%d Tracing disabled %s@%X\n", millis(), symbols, (long)(this) );
  Machine::setTrace( NULL, NULL, "" );
  return *this;
}
