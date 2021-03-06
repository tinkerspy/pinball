#include "Atm_switch_matrix.hpp"

Atm_switch_matrix& Atm_switch_matrix::begin( IO& io, Atm_led_matrix& leds ) {
  if ( initialized ) return *this; // idempotent
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER  ON_LOOP  ON_EXIT  EVT_DISABLE EVT_ENABLE  EVT_TIMER, EVT_READY, EVT_INIT,    ELSE */
    /*  IDLE     */           -1,      -1,      -1,    DISABLED,        -1,        -1,     READY,     INIT,     WAIT,
    /*  WAIT     */           -1,      -1,      -1,    DISABLED,        -1,      SCAN,     READY,     INIT,       -1,
    /*  SCAN     */     ENT_SCAN,      -1,      -1,    DISABLED,        -1,        -1,     READY,     INIT,     SCAN,
    /*  DISABLED */ ENT_DISABLED,      -1,      -1,          -1,      SCAN,        -1,     READY,     INIT, DISABLED, 
    /*  READY    */    ENT_READY,      -1,      -1,          -1,      SCAN,        -1,        -1,     INIT,    READY, 
    /*  INIT     */     ENT_INIT,      -1,      -1,          -1,      SCAN,        -1,        -1,       -1, DISABLED, 
  };

  // clang-format on
  Symbolic_Machine::begin( state_table, ELSE );
  this->io = &io;
  this->pleds = &leds;
  memset( prof, 0, sizeof( prof ) ); 
  timer.set( STARTUP_DELAY_MS );
  numOfSwitches = io.numberOfSwitches();  
  Symbolic_Machine::loadSymbols( event_symbols );
  initialized = 1;
  return *this;          
}

Atm_switch_matrix& Atm_switch_matrix::loadSymbols( const char switches[] ) {
  Symbolic_Machine::loadSymbols( switches );
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::loadGroups( const char groups[] ) {
  numOfGroups = countSymbols( 1 ) - numOfSwitches;
  int16_t data_size = loadIntList( symbols, groups, NULL, numOfGroups, numOfSwitches + 1 );
  int16_t* pdata = (int16_t *) malloc( data_size * 2 );
  memset( pdata, 0, data_size * 2 );
  loadIntList( symbols, groups, pdata, numOfGroups, numOfSwitches + 1 );
  group_def = pdata;
  return *this;
}

int16_t Atm_switch_matrix::exists( int16_t n ) {
  if ( n > 0 && n <= numOfSwitches + numOfGroups ) {
    return prof[n].device_index;
  }
  return 0;
}

int Atm_switch_matrix::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
  }
  return 0;
}

void Atm_switch_matrix::action( int id ) {
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
    case ENT_INIT:
      for ( int16_t n = 0; n < numOfSwitches + numOfGroups; n++ ) {
        if ( prof[n+1].device != NULL ) {
          prof[n+1].device->trigger( 0 ); 
        }
      }
      return;
  }
}

int16_t* Atm_switch_matrix::parseGroups( int16_t* group_def ) {
  int16_t* p = group_def;
  while ( p[0] != -1 ) *p++ = 0;
  numOfGroups = p - group_def;
  p++;
  while ( p[0] != -1 ) {
    int gid = p[0] - numOfSwitches - 1;
    group_def[gid] = p - group_def + 1;
    p++;
    while ( p[0] != -1 ) {
      p++;
    }
    p++;
  }
  // Make unused entries point to -1 at end of index 
  p = group_def;
  while ( p[0] != -1 ) {
    if ( p[0] == 0 ) *p = numOfGroups;
      p++;
  }
  return group_def;
}

int16_t Atm_switch_matrix::index( int16_t swno, int16_t n ) {
  int16_t cnt = 0;
  if ( swno > -1 ) {
    if ( swno <= numOfSwitches ) {
      return n == 0 ? swno : -1;
    } else {
      int16_t p = group_def[swno - numOfSwitches - 1];
      while ( p != -1 && group_def[p] != -1 ) {
        if ( cnt++ == n ) {
          return group_def[p];
        }
        p++;
      }
    }
  }
  return -1;  
}

int16_t Atm_switch_matrix::count( int16_t swno ) {
  int16_t cnt = 0;
  if ( swno > -1 ) {
    if ( swno <= numOfSwitches ) {
      return 1;
    } else {
      int16_t p = group_def[swno - numOfSwitches - 1];
      while ( p != -1 && group_def[p] != -1 ) {
        cnt++;
        p++;
      }
    }
  }
  return cnt;  
}

Atm_switch_matrix& Atm_switch_matrix::profile( int16_t n, int16_t press_ticks, int16_t release_ticks, int16_t throttle_ticks, int16_t separate_ticks  ) {
  if ( n > numOfSwitches ) {
    int16_t p = group_def[n - numOfSwitches - 1];
    while ( group_def[p] != -1 ) {
      if ( callback_trace ) 
        stream_trace->printf( "Atm_switch_matrix::profile( %d, %d, %d, %d, %d );\n", group_def[p], press_ticks, release_ticks, throttle_ticks, separate_ticks );
      io->debounce( group_def[p], press_ticks, release_ticks, throttle_ticks, separate_ticks );  
      p++;
    }
  } else {
    if ( callback_trace ) 
      stream_trace->printf( "Atm_switch_matrix::profile( %d, %d, %d, %d, %d );\n", n, press_ticks, release_ticks, throttle_ticks, separate_ticks );
    io->debounce( n, press_ticks, release_ticks, throttle_ticks, separate_ticks );        
  }
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::profile( const char* switch_group_str, int16_t press_ticks, int16_t release_ticks, int16_t throttle_ticks, int16_t separate_ticks  ) {
  return profile( findSymbol( switch_group_str ), press_ticks, release_ticks, throttle_ticks, separate_ticks );
}

Atm_switch_matrix& Atm_switch_matrix::readProfiles(  char label, const int16_t* profile_def ) {
  const int16_t* p = profile_def;
  while ( p[0] != -1 ) {
    int16_t ptype = *p++;
    if ( ptype == label ) {
      int16_t press_ticks = *p++;
      int16_t release_ticks = *p++;
      int16_t throttle_ticks = *p++;
      int16_t separate_ticks = *p++;
      while ( p[0] != -1 ) {
        profile( *p++, press_ticks, release_ticks, throttle_ticks, separate_ticks );  
      }
    } else {
      while ( *p != -1 ) p++;
    }
    p++;
  }
  return *this;
}

int16_t Atm_switch_matrix::numberOfGroups( void ) {
  return numOfGroups;
}

Atm_switch_matrix& Atm_switch_matrix::disable() {
  trigger( EVT_DISABLE );
  return *this;   
}

Atm_switch_matrix& Atm_switch_matrix::enable() {
  trigger( EVT_ENABLE );
  return *this;   
}

bool Atm_switch_matrix::enabled() {
  return pf_enabled;   
}

Atm_led_matrix* Atm_switch_matrix::leds( void ) {
  return this->pleds;
}

bool Atm_switch_matrix::isPressed( int16_t n ) {
  return io->isPressed( n );
}

void Atm_switch_matrix::scan_matrix( void ) {
  int16_t sw = io->scan();
  if ( sw != 0 ) {
    if ( sw > 0 ) {
      if ( prof[sw].device_index ) {
        uint16_t e = 1 + ( (prof[sw].device_index - 1) * 2 );
        //Serial.printf( "device trigger (press) %d, idx=%d, e=%d\n", sw, prof[sw].device_index, e );
        prof[sw].device->trigger( e, 1 );         
      }
      if ( trace_switches & ATMSM_TRACE_PRESS ) {
        ts_stream->printf( "%d PRESS %d %s\n", millis(), sw, findSymbol( sw, 1 ) );
      }
    } else {
      sw = abs( sw );
      if ( prof[sw].device_index ) {
        uint16_t e = 2 + ( (prof[sw].device_index - 1) * 2 );
        //Serial.printf( "device trigger (release) %d, idx=%d, e=%d\n", n, prof[sw].device_index, e ); delay( 100 );
        prof[sw].device->trigger( e, 1 );         
      }    
      if ( trace_switches & ATMSM_TRACE_RELEASE ) {
        ts_stream->printf( "%d RELEASE %d %s\n", millis(), sw, findSymbol( sw, 1 ) );
      }
    }
  }
}


Atm_device& Atm_switch_matrix::device( int16_t n, int16_t led_group /* = -1 */, int16_t* device_script /* = NULL */,
    int16_t r0, int16_t r1, int16_t r2, int16_t r3, int16_t r4, int16_t r5, int16_t r6, int16_t r7 ) {
  //Serial.printf( "Device rquest %d\n", n, led_group ); 
  if ( n == -1 ) { // Create a floating device (untested)
    Atm_device* device = new Atm_device();
    device->begin( this, n, led_group, device_script, r0, r1, r2, r3, r4, r5, r6, r7 );
    return *device;
  }
  if ( prof[n].device_index == 0 ) { 
    Atm_device* device = new Atm_device(); // Create device
    device->begin( this, n, led_group, device_script, r0, r1, r2, r3, r4, r5, r6, r7 );
    // Check and warn (Serial stream) if switch was already in use!
    if ( callback_trace && prof[n].device ) 
      stream_trace->printf( "Atm_switch_matrix::device( %d ): switch already in use!\n", n );
    prof[n].device = device; // Attach device to one switch
    prof[n].device_index = 1;
    //Serial.printf( "Attach root %d, index %d (NOS: %d)\n", n, 0, numOfSwitches ); 
    if ( n > numOfSwitches ) {
      if ( group_def && n <= numOfSwitches + numOfGroups ) { 
        int p = group_def[n - numOfSwitches - 1];
        int cnt = 0;
        while ( p != -1 && group_def[p] != -1 ) {
          if ( group_def[p] < numOfSwitches ) {
            //Serial.printf( "Group attach %d, index %d\n", group_def[p], cnt ); 
            if ( callback_trace && prof[group_def[p]].device ) 
              stream_trace->printf( "Atm_switch_matrix::device( %d ): switch %d already in use!\n", n, group_def[p] );
            prof[group_def[p]].device = device;
            prof[group_def[p]].device_index = cnt + 1;
          }
          cnt++;
          p++; 
        }
      }
    } 
  } else { 
    if ( device_script != NULL ) {
      prof[n].device->reg( 0, r0 );
      prof[n].device->reg( 1, r1 );
      prof[n].device->reg( 2, r2 );
      prof[n].device->reg( 3, r3 );
      prof[n].device->reg( 4, r4 );
      prof[n].device->reg( 5, r5 );
      prof[n].device->reg( 6, r6 );
      prof[n].device->reg( 7, r7 );
      prof[n].device->set_led( led_group );
      prof[n].device->set_script( device_script );
    }
  }
  //Serial.printf( "Device %d returned %x\n", n, (long)prof[n].device ); 
  return *prof[n].device;
}

Atm_device& Atm_switch_matrix::device( const char dev_str[], int16_t led_group /* = -1 */, int16_t* device_script /* = NULL */,
  int16_t r0, int16_t r1, int16_t r2, int16_t r3, int16_t r4, int16_t r5, int16_t r6, int16_t r7 ) {
  return device( findSymbol( dev_str ), led_group, device_script, r0, r1, r2, r3, r4, r5, r6, r7 );
}
 
 Atm_device& Atm_switch_matrix::device( const char dev_str[], const char led_group_str[], int64_t code_pack,
  int16_t r0, int16_t r1, int16_t r2, int16_t r3, int16_t r4, int16_t r5, int16_t r6, int16_t r7 ) {
  symbol_code_pack cp;
  cp.pack = code_pack;  
  return device( findSymbol( dev_str ), pleds->findSymbol( led_group_str ), cp.code, r0, r1, r2, r3, r4, r5, r6, r7 ).linkSymbols( cp.symbols );  
}
 
Atm_device& Atm_switch_matrix::link( const char src_str[], const char out_str[], const char dest_str[], const char in_str[] ) {
  return device( findSymbol( src_str ) ).onEvent( out_str, dest_str, in_str );  
}

Atm_device& Atm_switch_matrix::link( const char src_str[], const char out_str[], Symbolic_Machine& dest, const char in_str[] ) {
  //Serial.printf( "playfield link: output %d to input %d (%s)\n", 
  //  device( src_str ).findSymbol( out_str ), dest.findSymbol( in_str ), in_str );
  return device( src_str ).onEvent( device( src_str ).findSymbol( out_str ), dest, dest.findSymbol( in_str ) );
}

bool Atm_switch_matrix::ready() {
  return state() == READY;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_switch_matrix& Atm_switch_matrix::trigger( int event ) {
  //Serial.printf( "Playfield trigger %d\n", event );
  Symbolic_Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_switch_matrix::state( void ) {
  return Symbolic_Machine::state();
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_switch_matrix& Atm_switch_matrix::traceSwitches( Stream & stream, uint8_t bitmap /* = 1 */ ) {
  ts_stream = &stream;
  trace_switches = bitmap;
  return *this;
}
  
Atm_switch_matrix& Atm_switch_matrix::traceSwitches( Stream* stream, uint8_t bitmap /* = 1 */ ) {
  ts_stream = stream;
  trace_switches = bitmap;
  return *this;
}
  
Atm_switch_matrix& Atm_switch_matrix::trace( Stream & stream ) {
  
  Symbolic_Machine::setTrace( &stream, atm_serial_debug::trace,
    "ATM_SWITCH_MATRIX\0EVT_DISABLE\0EVT_ENABLE\0EVT_TIMER\0EVT_READY\0EVT_INIT\0ELSE\0IDLE\0WAIT\0SCAN\0DISABLED\0READY\0INIT" );
  Serial.printf( "%d Tracing enabled %s@%X\n", millis(), symbols, (long)(this) );
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::trace( void ) {
  Serial.printf( "%d Tracing disabled %s@%X\n", millis(), symbols, (long)(this) );
  Symbolic_Machine::setTrace( NULL, NULL, "" );
  return *this;
}
