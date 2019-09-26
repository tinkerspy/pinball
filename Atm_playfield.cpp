#include "Atm_playfield.hpp"

// TODO: Add catchall on onPress()/onRelease()

Atm_playfield& Atm_playfield::begin( IO& io, Atm_led_scheduler& led, int16_t* group_definition ) {
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
  numberOfSwitches = io.numSwitches();  
  numberOfGroups = 0;  
  if ( group_definition ) {
    group_def = parseGroups( group_definition );
  }
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


int16_t* Atm_playfield::parseGroups( int16_t* group_def ) {
  int16_t* p = group_def;
  while ( p[0] != -1 ) *p++ = 0;
  numberOfGroups = p - group_def;
  p++;
  while ( p[0] != -1 ) {
    int gid = p[0] - numberOfSwitches - 1;
    group_def[gid] = p - group_def + 1;
    p++;
    while ( p[0] != -1 ) {
      p++;
    }
    p++;
  }
  // 0 entries vervangen door pointer naar -1 aan einde lijst? (numberOfGroups)
  // Dan kunnen 'lege group' placeholders gewoon worden weggelaten in de lijst!
  
  // Make unused entries point to -1 at end of index 
  //p = group_def;
  //while ( p[0] != -1 ) {
  //  if ( p[0] == 0 ) *p++ = numberOfGroups;
  //}
  return group_def;
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
      if ( prof[n].device_index ) {
        uint16_t e = 1 + ( (prof[n].device_index - 1) * 2 );
        //Serial.printf( "device trigger (press) %d, idx=%d, e=%d\n", n, prof[n].device_index, e );
        prof[n].device->trigger( e, 1 );         
      } else {
      }
      prof[n].last_change = millis();
      prof[n].make_wait = 0;
      return;
    }
  } else {
    if ( ( millis_passed >= prof[n].break_delay ) ) {
      prof[n].switch_state = 0;      
      push( connectors, ON_RELEASE, n, n, 0 ); 
      if ( prof[n].device_index ) {
        uint16_t e = 2 + ( (prof[n].device_index - 1) * 2 );
        //Serial.printf( "device trigger (release) %d, idx=%d, e=%d\n", n, prof[n].device_index, e ); delay( 100 );
        prof[n].device->trigger( e, 1 );         
      }
      prof[n].last_change = millis();
      prof[n].make_wait = 0;
      return;
    }
  }
  //Serial.println( "unscan" );
  io->unscan(); // Cancels the last scan() event (makes event sticky in case of debounce)
}

// TODO: Voor een switch group het device object koppelen aan alle fysieke switches!

Atm_device& Atm_playfield::device( int16_t n, int16_t led_group /* = -1 */, int16_t* device_script /* = NULL */,
    int16_t r0, int16_t r1, int16_t r2, int16_t r3, int16_t r4, int16_t r5, int16_t r6, int16_t r7 ) {
  //Serial.printf( "Device switch=%d, led=%d\n", n, led_group ); 
  if ( n == -1 ) { // Create a floating device (untested)
    Atm_device* device = new Atm_device();
    device->begin( *this, led_group, device_script );
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
    device->begin( *this, led_group, device_script );
    prof[n].device = device; // Attach device to one switch
    prof[n].device_index = 1;
    //Serial.printf( "Attach root %d, index %d (NOS: %d)\n", n, 0, numberOfSwitches ); 
    if ( n >= numberOfSwitches ) {
      if ( group_def && n <= numberOfSwitches + numberOfGroups ) { 
        int p = group_def[n - numberOfSwitches - 1];
        int cnt = 0;
        while ( group_def[p] != -1 ) {
          if ( group_def[p] < numberOfSwitches ) {
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
  // This seems to be unnecessary
  //if ( n < numberOfSwitches && io->isPressed( n ) ) {      // We're linked to a single physical switch
  //  prof[n].device->trigger( 1, 1 );                       // Make sure the device knows if it's pressed
  //}
  return *prof[n].device;
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
