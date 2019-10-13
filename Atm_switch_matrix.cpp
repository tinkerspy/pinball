#include "Atm_switch_matrix.hpp"

Atm_switch_matrix& Atm_switch_matrix::begin( IO& io, Atm_led_matrix& leds, int16_t* group_definition, int16_t status_led ) {
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
  Machine::begin( state_table, ELSE );
  this->io = &io;
  this->pleds = &leds;
  this->status_led = status_led;
  memset( prof, 0, sizeof( prof ) ); 
  timer.set( STARTUP_DELAY_MS );
  numOfSwitches = io.numberOfSwitches();  
  numOfGroups = 0;  
  if ( group_definition ) {
    group_def = parseGroups( group_definition );
  }
  pleds->off( status_led );
  return *this;          
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
      pleds->on( status_led );
      pf_enabled = true;
      scan_matrix();
      return;
    case ENT_DISABLED:
      pleds->off( status_led );
      pf_enabled = false;
      scan_matrix();
      return;
    case ENT_READY:
      pleds->off( status_led );
      pf_enabled = false;
      scan_matrix();
      return;
    case ENT_INIT:
      pleds->off( status_led );
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

Atm_switch_matrix& Atm_switch_matrix::profile( int16_t n, int16_t press_100us, int16_t release_100us, int16_t throttle_100us ) {
  if ( n > numOfSwitches ) {
    int16_t p = group_def[n - numOfSwitches - 1];
    while ( group_def[p] != -1 ) {
      if ( callback_trace ) 
        stream_trace->printf( "Atm_switch_matrix::profile( %d, %d, %d, %d );\n", group_def[p], press_100us, release_100us, throttle_100us );
      io->debounce( group_def[p], press_100us, release_100us, throttle_100us );  
      p++;
    }
  } else {
    if ( callback_trace ) 
      stream_trace->printf( "Atm_switch_matrix::profile( %d, %d, %d, %d );\n", n, press_100us, release_100us, throttle_100us );
    io->debounce( n, press_100us, release_100us, throttle_100us );        
  }
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::readProfiles(  char label, const int16_t* profile_def ) {
  const int16_t* p = profile_def;
  while ( p[0] != -1 ) {
    int16_t ptype = *p++;
    if ( ptype == label ) {
      int16_t press_100us = *p++;
      int16_t release_100us = *p++;
      int16_t throttle_100us = *p++;
      while ( p[0] != -1 ) {
        profile( *p++, press_100us, release_100us, throttle_100us );  
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
    } else {
      sw = abs( sw );
      if ( prof[sw].device_index ) {
        uint16_t e = 2 + ( (prof[sw].device_index - 1) * 2 );
        //Serial.printf( "device trigger (release) %d, idx=%d, e=%d\n", n, prof[sw].device_index, e ); delay( 100 );
        prof[sw].device->trigger( e, 1 );         
      }    
    }
  }
}

// TODO: Voor een switch group het device object koppelen aan alle fysieke switches!

Atm_device& Atm_switch_matrix::device( int16_t n, int16_t led_group /* = -1 */, int16_t* device_script /* = NULL */,
    int16_t r0, int16_t r1, int16_t r2, int16_t r3, int16_t r4, int16_t r5, int16_t r6, int16_t r7 ) {
  //Serial.printf( "Device rquest %d\n", n, led_group ); 
  if ( n == -1 ) { // Create a floating device (untested)
    Atm_device* device = new Atm_device();
    device->begin( this, led_group, device_script, r0, r1, r2, r3, r4, r5, r6, r7 );
    return *device;
  }
  if ( prof[n].device_index == 0 ) { 
    Atm_device* device = new Atm_device(); // Create device
    device->begin( this, led_group, device_script, r0, r1, r2, r3, r4, r5, r6, r7 );
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

bool Atm_switch_matrix::ready() {
  return state() == READY;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_switch_matrix& Atm_switch_matrix::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_switch_matrix::state( void ) {
  return Machine::state();
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_switch_matrix& Atm_switch_matrix::trace( Stream & stream ) {
  
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "ATM_SWITCH_MATRIX\0EVT_DISABLE\0EVT_ENABLE\0EVT_TIMER\0EVT_READY\0EVT_INIT\0ELSE\0IDLE\0WAIT\0SCAN\0DISABLED\0READY\0INIT" );
  Serial.printf( "%d Tracing enabled %s@%X\n", millis(), symbols, (long)(this) );
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::trace( void ) {
  Serial.printf( "%d Tracing disabled %s@%X\n", millis(), symbols, (long)(this) );
  Machine::setTrace( NULL, NULL, "" );
  return *this;
}
