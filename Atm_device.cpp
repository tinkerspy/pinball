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
  code_ptr = 0;
  xctr = 0;
  event_map = 0;
  switch_map = 0;
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
  //Serial.printf( "dev %x: set next to %x\r\n", (long)(this), (long)&next ); 
  this->next = &next;  
  return next;
}

Atm_device& Atm_device::chain( int16_t sw ) {
  //Serial.printf( "dev %x: set next to %x\r\n", (long)(this), (long)&next ); 
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

uint8_t Atm_device::sleep( int8_t v /* = -1 */ ) { // 0 running, 1 sleeping, 2 waiting
  if ( v > -1 ) {
    return Machine::sleep( v );
  }
  if ( Machine::sleep() ) {
    return code_ptr == 0 ? 1 : 2;    
  } else {
    return 0;
  }
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
          push( connectors, ON_EVENT, i, i, 0 );
        }
      }
      trigger_flags = 0;
      return;
    case ENT_RESUME:
      timer.set( ATM_TIMER_OFF );
      run_code();
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
      stream_trace->printf( "Atm_device led arg #%d maps to physical led %d\r\n", selector, n );
    } else { 
      stream_trace->printf( "Atm_device led arg #%d not used\r\n", selector );    
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
  //Serial.printf( "%d %X Start code %d\r\n", millis(), (long)(this), e );
  //Serial.printf( "%d > -1 && %d < %d && %d > 0\r\n", e, e, numberOfInputs, script[e] );
  if ( e > -1 && e < numberOfInputs && script[e] > 0 ) { 
    reg_ptr = 0;
    stack_ptr = 0;
    code_ptr = script[e];
    if ( trace_code ) 
      tc_stream->printf( "run_code event %03d called for %d -> %03d\r\n", e, switch_group, code_ptr );
    xctr++;
    run_code();      
  }
}

void Atm_device::decompile( uint16_t ip, char* s, bool hide_ptr /* = 0 */ ) {
  int16_t opcode = script[ip];
  int16_t selector = script[ip+1];
  int16_t action_t = script[ip+2];
  int16_t action_f = script[ip+3];
  int16_t abs_ip = ip;
  const char* me = playfield->findSymbol( switchGroup(), 1 );
  int16_t entry = 0;
  int16_t offset = 0;
  for ( int16_t i = 0; i < countSymbols( 0 ); i++ ) {
    // Klopt niet
    // tc game; press game; press game
    // Hij detecteert 'init' i.p.v. 'sub_press_start' (omdat subs niet op volgorde staan!)
    if ( script[i] <= ip && script[i] > script[entry] ) {
      entry = i;
      offset = script[i];
    }
  }
  ip -= offset;  
  switch ( opcode ) {
    case 'J':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %d : %d", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 2, "-1" ), action_t, action_f );
      break;
    case 'H':
    case 'L':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %s : %s", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 2, "-1" ), findSymbol( action_t, 2, "-1" ), findSymbol( action_f, 2, "-1" ) );
      break;
    case 'T':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %s : %s", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 2, "-1" ), findSymbol( action_t, 1, "-1" ), findSymbol( action_f, 1, "-1" ) );
      break;
    case 'E':
    case 'K':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %d : %d", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 0, "-1" ), action_t, action_f );
      break;
    case 'R':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %s : %s", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 2, "-1" ), findSymbol( action_t, 3, "-1" ), findSymbol( action_f, 3, "-1" ) );
      break;
    case 'Q':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %s : %s", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 3, "-1" ), findSymbol( action_t, 4, "-1" ), findSymbol( action_f, 4, "-1" ) );
      break;
    case 'D':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %s : %s", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 2, "-1" ), findSymbol( action_t, 3, "-1" ), findSymbol( action_f, 3, "-1" ) );
      break;
    case 'S':
    case 'A':
      sprintf( s, "%lu %s::%s[%03d]: %c %s ? %s : %s", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        findSymbol( selector, 2, "-1" ), findSymbol( action_t, 0, "-1" ), findSymbol( action_f, 0, "-1" ) );
      break;
    default:
      sprintf( s, "%lu %s::%s[%03d]: %c %d ? %d : %d", 
        millis(), me, findSymbol( entry, 0 ), ip >> 2, opcode, 
        selector, action_t, action_f );
      break;      
  }
  if ( script[abs_ip+4] == -1 ) strcat( s, ";" );
  if ( code_ptr == abs_ip + 4 && !hide_ptr) strcat( s, " // <<<" );    
}

Atm_device& Atm_device::dumpCode( Stream* stream, uint8_t event, bool clean /* = 0 */ ) {
  char buf[128];
  int16_t p = script[event];
  if ( p > 0 ) {
    stream->println();
    if ( clean ) {
      stream->println( findSymbol( event, 0 ) );
    }
    while ( script[p] != -1 ) {
      decompile( p, buf, clean );
      if ( clean ) {
        stream->println( strstr( buf, "]: " ) + 3 );
      } else {
        stream->println( buf );      
      }
      p += 4;
    }
  }
  return *this;
}

void Atm_device::run_code() {
  char buf[80];
  if ( code_ptr > 0 ) {
    while ( true ) {
      int16_t opcode = script[code_ptr++];
      int16_t selector = script[code_ptr++];
      int16_t action_t = script[code_ptr++];
      int16_t action_f = script[code_ptr++];
      int16_t selected_action = 0;
      if ( opcode > -1 ) {
        if ( trace_code ) { 
          decompile( code_ptr - 4, buf, true );
          tc_stream->println( buf );
        }
        switch ( opcode ) {
          case 'J': // JmpL
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }            
            break;
          case 'A': // JmpLA jump absolute on register equal
            selected_action = ( selector >= 0 && registers[reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              code_ptr = script[selected_action];          
            } else {
              code_ptr = 0;
            }            
            break;
          case 'Q': // Quote (FIXME: only in response to tm command)
            if ( selector == -1 ) {
              Serial.printf( "%d MSG %s %s\r\n", millis(), 
                playfield->findSymbol( switchGroup(), 1 ), findSymbol( action_f, 4 ) );
            } else {
              Serial.printf( "%d MSG %s %s: %d\r\n", millis(), 
                playfield->findSymbol( switchGroup(), 1 ), findSymbol( action_f, 4 ), registers[selector] );              
            }
            break;
          case '=': // JmpRE
            selected_action = ( selector >= 0 && registers[reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }            
            break;
          case '<': // JmpRL
            selected_action = ( selector >= 0 && registers[reg_ptr] < selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }            
            break;
          case '>': // JmpRG
            selected_action = ( selector >= 0 && registers[reg_ptr] > selector ) ? action_t : action_f;
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }            
            break;
          case 'O': 
            pinMode( selector, OUTPUT );            digitalWrite( selector, action_f > 0 );
            break; 
          case 'X':  // Xctr
            selected_action = ( xctr == (uint16_t)selector ? action_t : action_f );
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }            
            break;            
          case '!': // Stop script!
            stack_ptr = code_ptr = 0;
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
            stack[stack_ptr++] = code_ptr;          
            code_ptr = script[selected_action];
            break;
          case 'I': // Inc
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            registers[reg_ptr] += selected_action;
            break;
          case 'Z': // Zap (really: set to value)
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            registers[reg_ptr] = selected_action;
            break;
          case 'D': // Dup - duplicate into named register
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            registers[selected_action] = registers[reg_ptr];
            break;
          case 'T': // Trig
            selected_action = ( selector >= 0 && registers[reg_ptr] == selector ) ? action_t : action_f;
            if ( selected_action > -1 ) {
                trigger_flags |= ( 1 << selected_action );
            } 
            sleep( 0 );
            break;
          case 'P': // Jump on playfield enabled 
            selected_action = playfield->enabled() ? action_t : action_f;
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }            
            break;
          case 'R': // Reg
            reg_ptr = led_active( led_group, selector ) ? action_t : action_f;
            break;           
          case 'E': // Jump on event 
            selected_action = event_map & ( 1UL << selector ) ? action_t : action_f; // Check event
            //Serial.printf( "Event %d: state %d -> %d\r\n", selector, event_map & ( 1 << selector ), selected_action );   
            event_map &= ~( 1UL << selector ); // Clear event
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }                      
            break;           
          case 'K': // Jump on key (switch) state 
            selector--; // Switches start at zero
            selector >>= 1; // And have 1 bit per press/release pair
            selected_action = switch_map & ( 1UL << selector ) ? action_t : action_f; // Check event
            if ( selected_action  != -1 ) {
              code_ptr += selected_action * 4;          
            } else {
              code_ptr = 0;
            }                      
            break;           
          case 'W':
          case 'Y': // Yield (negative selector value uses register!)
            selected_action = led_active( led_group, selector ) ? action_t : action_f;
            selected_action = selected_action > 0 ? selected_action : registers[abs(selected_action)];
            if ( selector == -2 ) break; // deprecated
            if ( selected_action >= 0 ) { // negative time values have no yield effect but do trip the core check
              timer.set( selected_action == 0 ? ATM_TIMER_OFF : selected_action ); // Zero timer means wait forever
              sleep( 0 );
            }
            return;           
          default:
            if ( trace_code ) 
              tc_stream->printf( "run_code %03d: abort, illegal opcode '%c', script out of sync? (missing comma?)\r\n", code_ptr - 4, opcode );
            return;
        }
      } else {
        code_ptr = 0;
      }        
      if ( code_ptr == 0 ) {
        if ( stack_ptr > 0 ) {
          code_ptr = stack[--stack_ptr];
        } else {
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

Atm_device& Atm_device::update_switch( int event ) {
  if ( event > 0 and event < 32 ) {
    event_map |= ( 1UL << event );  // set event bit
    int16_t sw = event - 1; // sw = 0..31
    if ( ( sw & 1UL ) == 0 ) { // Press if bit 0 is not set
      switch_map |= ( 1UL << ( sw >> 1 ) );  // Switch press -> set switch bit
      //Serial.printf( "%d Set switch %d on, map=%08X\r\n", millis(), sw >> 1, switch_map );
    } else {
      switch_map &= ~( 1UL << ( sw >> 1 ) ); // Switch release -> clear switch bit
      //Serial.printf( "%d Set switch %d off, map=%08X\r\n", millis(), sw >> 1, switch_map );
    }
  }
  return *this;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_device& Atm_device::trigger( int event ) {
  //Serial.printf( "%x trigger %d\r\n", (long)(this), event );
  if ( next ) {
    //Serial.printf( "%x next %x\r\n", (long)(this), (long)next );
    next->trigger( event );
  }
  if ( this->enabled ) {
    update_switch( event );
    if ( code_ptr == 0 ) start_code( event ); // FIXME: Only if no code is currently running!
    if ( code_ptr > 0 && timer.value == ATM_TIMER_OFF ) { timer.set( 0 );  sleep( 0 ); }
  }
  return *this;
}

Atm_device& Atm_device::trigger( int event, uint32_t sel ) {
  //Serial.printf( "%x trigger2 %d\r\n", (long)(this), event );
  if ( next && sel > 1 ) {
    next->trigger( event, sel >> 1 );
  }
  if ( sel & 1 ) {
    update_switch( event );
    if ( code_ptr == 0 ) start_code( event ); 
    if ( code_ptr > 0 && timer.value == ATM_TIMER_OFF ) { timer.set( 0 );  sleep( 0 ); }
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
  //Serial.printf( "%s -> %s:%s --- %d -> %d:%d\r\n", sub_str, sw_str, event_str, sub, sw, event );
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
  Serial.printf( "%d Tracing disabled %s@%X\r\n", millis(), symbols, (long)(this) );
  Machine::setTrace( NULL, NULL, "" );
  return *this;
}
