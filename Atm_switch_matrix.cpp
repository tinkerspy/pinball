#include "Atm_switch_matrix.hpp"

// TODO: Add catchall on onPress()/onRelease()

Atm_switch_matrix& Atm_switch_matrix::begin( IO& io, Atm_led_matrix& leds, int16_t* group_definition ) {
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
  this->pleds = &leds;
  memset( connectors, 0, sizeof( connectors ) ); 
  memset( prof, 0, sizeof( prof ) ); 
  timer.set( STARTUP_DELAY_MS );
  numOfSwitches = io.numberOfSwitches();  
  numOfGroups = 0;  
  if ( group_definition ) {
    group_def = parseGroups( group_definition );
  }
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
      push( connectors, ON_PRESS, sw, sw, 1 ); 
      if ( prof[sw].device_index ) {
        uint16_t e = 1 + ( (prof[sw].device_index - 1) * 2 );
        //Serial.printf( "device trigger (press) %d, idx=%d, e=%d\n", sw, prof[sw].device_index, e );
        prof[sw].device->trigger( e, 1 );         
      }
    } else {
      sw = abs( sw );
      push( connectors, ON_RELEASE, sw, sw, 0 ); 
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
  //Serial.printf( "Device switch=%d, led=%d\n", n, led_group ); 
  if ( n == -1 ) { // Create a floating device (untested)
    Atm_device* device = new Atm_device();
    device->begin( this, led_group, device_script );
    return *device;
  }
  if ( prof[n].device_index == 0 ) { 
    Atm_device* device = new Atm_device(); // Create device
    if ( device_script != NULL ) {
      device->reg( 0, r0 );
      device->reg( 1, r1 );
      device->reg( 2, r2 );
      device->reg( 3, r3 );
      device->reg( 4, r4 );
      device->reg( 5, r5 );
      device->reg( 6, r6 );
      device->reg( 7, r7 );
    }
    device->begin( this, led_group, device_script );
    prof[n].device = device; // Attach device to one switch
    prof[n].device_index = 1;
    //Serial.printf( "Attach root %d, index %d (NOS: %d)\n", n, 0, numOfSwitches ); 
    if ( n >= numOfSwitches ) {
      if ( group_def && n <= numOfSwitches + numOfGroups ) { 
        int p = group_def[n - numOfSwitches - 1];
        int cnt = 0;
        while ( group_def[p] != -1 ) {
          if ( group_def[p] < numOfSwitches ) {
            //Serial.printf( "Attach %d, index %d\n", group_def[p], cnt ); 
            prof[group_def[p]].device = device;
            prof[group_def[p]].device_index = cnt + 1;
          }
          cnt++;
          p++; 
        }
      }
    } 
    //Serial.printf( "Switches attached\n" ); 
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

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


Atm_switch_matrix& Atm_switch_matrix::onPress( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, machine, event );
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::onPress( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_PRESS, sub, 32, 0, callback, idx );
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::onRelease( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, machine, event );
  return *this;
}

Atm_switch_matrix& Atm_switch_matrix::onRelease( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_RELEASE, sub, 32, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_switch_matrix& Atm_switch_matrix::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "PLAYFIELD\0EVT_DISABLE\0EVT_ENABLE\0EVT_TIMER\0EVT_READY\0ELSE\0IDLE\0WAIT\0SCAN\0DISABLED\0READY" );
  return *this;
}
