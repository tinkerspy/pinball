#include "Atm_led_matrix.hpp"
#include "io.hpp"

Atm_led_matrix& Atm_led_matrix::begin( IO &io, int16_t* group_definition ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                 ON_ENTER    ON_LOOP  ON_EXIT  EVT_DONE  EVT_RUN  EVT_UPDATE  EVT_MILLI,    ELSE */
    /*     IDLE */           -1, ATM_SLEEP,      -1,       -1, RUNNING,   UPDATING,        -1,      -1,
    /*  WAITING */           -1,        -1,      -1,       -1,      -1,         -1,   RUNNING,      -1,
    /*  RUNNING */  ENT_RUNNING,        -1,      -1,     IDLE,      -1,   UPDATING,        -1, WAITING,
    /* UPDATING */ ENT_UPDATING,        -1,      -1,       -1, WAITING,         -1,        -1,    IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->io = &io;
  numberOfLeds = io.numPixels();
  for ( int i = 0; i < io.numPixels(); i++ ) {
    profile( i, 0, 127, 50, 0 ); // Select a safe default profile
  }
  io.show();
  last_milli = millis();
  numberOfLeds = io.numPixels();  
  numberOfGroups = 0;  
  if ( group_definition ) {
    group_def = parseGroups( group_definition );
  }
  return *this;
}

/* Add C++ code for each internally handled event (input)
   The code must return 1 to trigger the event
*/

int Atm_led_matrix::event( int id ) {
  switch ( id ) {
    case EVT_DONE:
      return !running && !refresh;
    case EVT_RUN:
      return running || refresh;
    case EVT_UPDATE:
      return refresh;
    case EVT_MILLI: // Becomes true when milli value changes
      uint8_t milli_byte = millis();
      if ( last_milli != milli_byte ) {
        last_milli = milli_byte;
        return 1;
      }
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
   This generates the 'output' for the state machine
*/

void Atm_led_matrix::action( int id ) {
  switch ( id ) {
    case ENT_RUNNING:
      running = 0;
      for ( int i = 0; i < numberOfLeds; i++ ) {
        // Set running = 1; while there is work to do
        switch ( meta[i].state ) {
          case LED_STATE_DELAY:
            if ( millis() - meta[i].last_millis >= meta[i].T0 ) {
              set( i, meta[i].L1 );
              meta[i].state = LED_STATE_RGBW1;
              meta[i].last_millis = millis();
              refresh = 1;
            }
            running = 1;
            break;
          case LED_STATE_RGBW1:
            if ( millis() - meta[i].last_millis >= meta[i].T1 ) {
              if ( meta[i].L2 ) {
                set( i, meta[i].L2 );
                meta[i].state = LED_STATE_RGBW2;
                refresh = 1;
              } else {
                set( i, 0 );
                led_notify( i );
                meta[i].state = LED_STATE_IDLE;
                refresh = 1;
              }
            } else {
              running = 1;
            }
            break;
        }
      }
      if ( refresh ) {
        if ( io->show() ) refresh = 0;
      }
      return;
    case ENT_UPDATING:
      if ( io->show() ) refresh = 0;
      return;
    case ENT_IDLE:
      return;
  }
}

int16_t* Atm_led_matrix::parseGroups( int16_t* group_def ) {
  int16_t* p = group_def;
  while ( p[0] != -1 ) *p++ = 0;
  numberOfGroups = p - group_def;
  p++;
  while ( p[0] != -1 ) {
    int gid = p[0] - numberOfLeds; // Leds start at 0 while switches start at 1, does that create a problem here???
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
    if ( p[0] == 0 ) *p = numberOfGroups;
      p++;
  }
  return group_def;
}

Atm_led_matrix& Atm_led_matrix::readProfiles(  char label, const int16_t* profile_def ) {
  const int16_t* p = profile_def;
  while ( p[0] != -1 ) {
    int16_t ptype = *p++;
    if ( ptype == label ) {
      int16_t T0 = *p++;
      int16_t L1 = *p++;
      int16_t T1 = *p++;
      int16_t L2 = *p++;
      while ( p[0] != -1 ) {
        profile( *p++, T0, L1, T1, L2 ); 
      }
    } else {
      while ( *p != -1 ) p++;
    }
    p++;
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::set( int16_t ledno, uint32_t c ) {
  io->setPixelColor( ledno, c >> 24, c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_set( int16_t ledno, uint32_t c ) {
  int16_t p = group_def[ledno - numberOfLeds];
  while ( p != -1 && group_def[p] != -1 ) {
    set( group_def[p++], c );
  }
  return *this;
}

/* Optionally override the default trigger() method
   Control how your machine processes triggers
*/

Atm_led_matrix& Atm_led_matrix::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
   Control what the machine returns when another process requests its state
*/

int Atm_led_matrix::state( void ) {
  return Machine::state();
}

Atm_led_matrix& Atm_led_matrix::profile( int16_t ledno, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 /* = 0 */  ) {
  if ( ledno > -1 ) {
    if ( ledno < numberOfLeds ) { 
      meta[ledno].T0 = T0;
      meta[ledno].L1 = L1;
      meta[ledno].T1 = T1;
      meta[ledno].L2 = L2;
    } else {
      int16_t p = group_def[ledno - numberOfLeds];
      while ( p != -1 && group_def[p] != -1 )
        profile( group_def[p++], T0, L1, T1, L2 );
    }
  }
  return *this;
}



Atm_led_matrix& Atm_led_matrix::on( int ledno, bool no_update /* = false */  ) {
  if ( ledno > -1 ) {
    if ( ledno >= numberOfLeds ) return group_on( ledno );
    if ( meta[ledno].state == LED_STATE_IDLE || meta[ledno].state == LED_STATE_RGBW2 ) {
      meta[ledno].last_millis = millis();
      if ( meta[ledno].T0 ) {
        meta[ledno].state = LED_STATE_DELAY;
        set( ledno, 0 );
      } else {
        if ( meta[ledno].T1 == 0 ) {
          set( ledno, meta[ledno].L2 );
          meta[ledno].state = LED_STATE_RGBW2;
        } else {
          set( ledno, meta[ledno].L1 );
          meta[ledno].state = LED_STATE_RGBW1;
        }
      }
      refresh = 1;
      led_notify( ledno );
      if ( !no_update ) trigger( EVT_UPDATE );
    }
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_on( int ledno ) {
  int16_t p = group_def[ledno - numberOfLeds];
  while ( p != -1 && group_def[p] != -1 )
    on( group_def[p++], true );
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::off( int ledno, bool no_update /* = false */ ) {
  if ( ledno >= numberOfLeds ) return group_off( ledno );
  if ( ledno > -1 && meta[ledno].L2 ) { // Ignore off() for leds in pulse mode
    meta[ledno].state = LED_STATE_IDLE;
    set( ledno, 0 );
    led_notify( ledno );
    refresh = 1;
    if ( !no_update ) trigger( EVT_UPDATE );
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_off( int ledno ) {
  int16_t p = group_def[ledno - numberOfLeds];
    while ( p != -1 && group_def[p] != -1 )
      off( group_def[p++], true );
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::off() {
  for ( int ledno = 0; ledno < numberOfLeds; ledno++ )
    off( ledno, true );
  refresh = 1;
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::toggle( int ledno, int v /* = -1 */ ) {
  if ( ledno >= numberOfLeds ) return group_toggle( ledno, v );
  if ( v > -1 ) {
    if ( v ) on( ledno ); else off( ledno );
    return *this;
  }
  if ( meta[ledno].state ) {
    off( ledno );
  } else {
    on( ledno );
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_toggle( int ledno, int v /* = -1 */ ) {
  int16_t p = group_def[ledno - numberOfLeds];
  while ( p != -1 && group_def[p] != -1 )
    toggle( group_def[p++], v );
  trigger( EVT_UPDATE );
  return *this;
}

int Atm_led_matrix::active( int ledno ) {
  if ( ledno >= numberOfLeds ) return group_active( ledno );
  return ledno > -1 ? meta[ledno].state : 0;
}

// group_active simply returns the state of the first led in the group

int Atm_led_matrix::group_active( int ledno ) {
  int16_t p = group_def[ledno - numberOfLeds];
  return active( group_def[p] );
}

Atm_led_matrix& Atm_led_matrix::led_register( int16_t ledno, uint8_t idx ) {
  if ( ledno > -1 ) { // legal led
    if ( ledno < numberOfLeds ) { // Physical led
      meta[ledno].watch |= ( 1 << idx ); // Set watch bit for idx
    } else {   // Virtual led -> expand & recurse
      int16_t p = group_def[ledno - numberOfLeds];
      while ( p != -1 && group_def[p] != -1 )
        led_register( group_def[p++], idx );
    }
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::led_notify( int16_t ledno ) {

  if ( meta[ledno].watch ) {
    for ( uint8_t i = 0; i < watcher_cnt; i++ ) {
      if ( meta[ledno].watch & ( 1 << i ) ) {
        watchers[i].machine->trigger( watchers[i].event );
      }
    }
  }
  return *this;
}

int16_t Atm_led_matrix::count( int16_t ledno, int8_t led_active /* = -1 */ ) {
  int16_t cnt = 0;
  if ( ledno > -1 ) {
    if ( ledno < numberOfLeds ) { // Physical led
      if ( led_active == -1 ) {
        cnt++;
      } else {
        if ( ( meta[ledno].state > 0 ) == led_active ) cnt++;
      }
    } else {   // Virtual led -> expand & recurse
      int16_t p = group_def[ledno - numberOfLeds];
      while ( p != -1 && group_def[p] != -1 ) 
        cnt += this->count( group_def[p++], led_active );
    }
  }
  return cnt;
}

// Returns the n'th led in a led group or -1
// Single led is group of 1, -1 led always returns -1

int16_t Atm_led_matrix::index( int16_t ledno, int16_t n ) {
  int16_t cnt = 0;
  /*
  Serial.print( "Index " );
  Serial.print( n );
  Serial.print( " for " );
  Serial.print( ledno );
  Serial.print( ": " );
  */
  if ( ledno > -1 ) {
    if ( ledno < numberOfLeds ) { // Physical led, return ledno 
        return n == 0 ? ledno : -1;  
    } else {   // Virtual led -> expand & recurse
      int16_t p = group_def[ledno - numberOfLeds];
      while ( p != -1 && group_def[p] != -1 ) {
        if ( cnt++ == n ) {
          //Serial.println( *p );
          return group_def[p]; 
        }
        p++;
      }
    }    
  }
  //Serial.println( -1 );
  return -1;  
}



Atm_led_matrix& Atm_led_matrix::onWatch( int16_t ledno, Machine& machine, int16_t event ) {
  if ( watcher_cnt < MAX_WATCHERS ) {
    watchers[watcher_cnt].machine = &machine;
    watchers[watcher_cnt].event = event;
    led_register( ledno, watcher_cnt );
    watcher_cnt++;
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::onWatch( int16_t ledno, Machine* machine, int16_t event ) {
  if ( watcher_cnt < MAX_WATCHERS ) {
    watchers[watcher_cnt].machine = machine;
    watchers[watcher_cnt].event = event;
    led_register( ledno, watcher_cnt );
    watcher_cnt++;
  }
  return *this;
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods

*/


Atm_led_matrix& Atm_led_matrix::update() {
  trigger( EVT_UPDATE );
  return *this;
}

/* State trace method
   Sets the symbol table and the default logging method for serial monitoring
*/

Atm_led_matrix& Atm_led_matrix::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
                     "APA102\0EVT_DONE\0EVT_RUN\0EVT_UPDATE\0EVT_MILLI\0ELSE\0IDLE\0WAITING\0RUNNING\0UPDATING" );
  return *this;
}
