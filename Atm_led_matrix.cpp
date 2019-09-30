#include "Atm_led_matrix.hpp"
#include "io.hpp"

Atm_led_matrix& Atm_led_matrix::begin( IO &io, const int16_t* group_definition, const int16_t* profile_definition ) {
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
  number_of_leds = io.numPixels();
  defineProfile( PROFILE_COIL, 0, 127, 30,   0 );  // Default for coils (30 ms pulse at 50%)
  defineProfile(  PROFILE_LED, 0, 127,  0, 127 );  // Default for leds (hold level at 50%)
  io.show();
  last_milli = millis();
  parseGroups( group_definition );
  parseProfiles( profile_definition );
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
      for ( int i = 0; i < number_of_leds; i++ ) {
        // Set running = 1; while there is work to do
        switch ( meta[i].state ) {
          case LED_STATE_DELAY:
            if ( millis() - meta[i].last_millis >= led_profile[meta[i].profile].T0 ) {
              set( i, led_profile[meta[i].profile].L1 );
              meta[i].state = LED_STATE_RGBW1;
              meta[i].last_millis = millis();
              refresh = 1;
            }
            running = 1;
            break;
          case LED_STATE_RGBW1:
            if ( millis() - meta[i].last_millis >= led_profile[meta[i].profile].T1 ) {
              if ( led_profile[meta[i].profile].L2 ) {
                set( i, led_profile[meta[i].profile].L2 );
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

Atm_led_matrix& Atm_led_matrix::defineProfile( uint8_t prof, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 ) {
  led_profile[prof].T0 = T0;
  led_profile[prof].L1 = L1;
  led_profile[prof].T1 = T1;
  led_profile[prof].L2 = L2;
  return *this;
}

Atm_led_matrix& Atm_led_matrix::parseGroups( const int16_t* group_def ) {
  while ( group_def[0] != -1 ) {
    int gid = group_def[0] - number_of_leds;
    led_group[gid] = &group_def[1];
    group_def++;
    while ( group_def[0] != -1 ) {
      group_def++;
    }
    group_def++;
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::parseProfiles( const int16_t* profile_def ) {
  while ( profile_def[0] != -1 ) {
    int prof = profile_def[0];
    led_profile[prof].T0 = profile_def[1];
    led_profile[prof].L1 = profile_def[2];
    led_profile[prof].T1 = profile_def[3];
    led_profile[prof].L2 = profile_def[4];
    profile_def++;
    while ( profile_def[0] != -1 ) {
      profile_def++;
    }
    profile_def++;
  }
  return *this;
}

const int16_t* Atm_led_matrix::group( int16_t gid ) {
  return led_group[gid - number_of_leds];
}

Atm_led_matrix& Atm_led_matrix::set( int16_t ledno, uint32_t c ) {
  io->setPixelColor( ledno, c >> 24, c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_set( int16_t ledno, uint32_t c ) {
  const int16_t* p = group( ledno );
  while ( *p != -1 ) {
    set( *p++, c );
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

Atm_led_matrix& Atm_led_matrix::profile( int16_t ledno, uint8_t prof ) {
  if ( ledno > -1 ) {
    if ( ledno >= number_of_leds ) return group_profile( ledno, prof );
    meta[ledno].profile = prof;
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_profile( int16_t ledno, uint8_t prof  ) {
  const int16_t* p = group( ledno );
  while ( *p != -1 )
    profile( *p++, prof );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::scalar( int16_t ledno, int8_t index, bool fill /* = false */ ) {
  if ( ledno >= number_of_leds ) {
    uint8_t cnt = 0;
    const int16_t* p = group( ledno );
    while ( *p != -1 ) {
      if ( fill ) {
        if ( cnt <= index ) {
          on( *p++, true );
        } else {
          off( *p++, true );
        }        
      } else {
        if ( cnt == index ) {
          on( *p++, true );
        } else {
          off( *p++, true );
        }
      }
      cnt++;
    }
    trigger( EVT_UPDATE );
  }
  return *this;
}

int16_t Atm_led_matrix::scalar( int16_t ledno ) {
  int16_t r = -1;
  if ( ledno >= number_of_leds ) {
    uint8_t cnt = 0;
    const int16_t* p = group( ledno );
    while ( *p != -1 ) {
      if ( active( *p++ ) ) {
        r = cnt;
      }
      cnt++;
    }
  }
  return r;
}


Atm_led_matrix& Atm_led_matrix::on( int ledno, bool no_update /* = false */  ) {
  if ( ledno > -1 ) {
    if ( ledno >= number_of_leds ) return group_on( ledno );
    if ( meta[ledno].state == LED_STATE_IDLE || meta[ledno].state == LED_STATE_RGBW2 ) {
      meta[ledno].last_millis = millis();
      if ( led_profile[meta[ledno].profile].T0 ) {
        meta[ledno].state = LED_STATE_DELAY;
        set( ledno, 0 );
      } else {
        if ( led_profile[meta[ledno].profile].T1 == 0 ) {
          set( ledno, led_profile[meta[ledno].profile].L2 );
          meta[ledno].state = LED_STATE_RGBW2;
        } else {
          set( ledno, led_profile[meta[ledno].profile].L1 );
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
  const int16_t* p = group( ledno );
  while ( *p != -1 )
    on( *p++, true );
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::off( int ledno, bool no_update /* = false */ ) {
  if ( ledno >= number_of_leds ) return group_off( ledno );
  if ( ledno > -1 && led_profile[meta[ledno].profile].L2 ) { // Ignore off() for leds in pulse mode
    meta[ledno].state = LED_STATE_IDLE;
    set( ledno, 0 );
    led_notify( ledno );
    refresh = 1;
    if ( !no_update ) trigger( EVT_UPDATE );
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::group_off( int ledno ) {
  const int16_t* p = group( ledno );
  while ( *p != -1 )
    off( *p++, true );
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::off() {
  for ( int ledno = 0; ledno < number_of_leds; ledno++ )
    off( ledno, true );
  refresh = 1;
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::toggle( int ledno, int v /* = -1 */ ) {
  if ( ledno >= number_of_leds ) return group_toggle( ledno, v );
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
  const int16_t* p = group( ledno );
  while ( *p != -1 )
    toggle( *p++, v );
  trigger( EVT_UPDATE );
  return *this;
}

int Atm_led_matrix::active( int ledno ) {
  if ( ledno >= number_of_leds ) return group_active( ledno );
  return ledno > -1 ? meta[ledno].state : 0;
}

// group_active simply returns the state of the first led in the group

int Atm_led_matrix::group_active( int ledno ) {
  const int16_t* p = group( ledno );
  return active( *p );
}

Atm_led_matrix& Atm_led_matrix::led_register( int16_t ledno, uint8_t idx ) {
  if ( ledno > -1 ) { // legal led
    if ( ledno < number_of_leds ) { // Physical led
      meta[ledno].watch |= ( 1 << idx ); // Set watch bit for idx
    } else {   // Virtual led -> expand & recurse
      const int16_t* p = group( ledno );
      while ( *p != -1 )
        led_register( *p++, idx );
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
    if ( ledno < number_of_leds ) { // Physical led
      if ( led_active == -1 ) {
        cnt++;
      } else {
        if ( ( meta[ledno].state > 0 ) == led_active ) cnt++;
      }
    } else {   // Virtual led -> expand & recurse
      const int16_t* p = group( ledno );
      while ( *p != -1 )
        cnt += this->count( *p++, led_active );
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
    if ( ledno < number_of_leds ) { // Physical led, return ledno 
        return n == 0 ? ledno : -1;  
    } else {   // Virtual led -> expand & recurse
      const int16_t* p = group( ledno );
      while ( *p != -1 ) {
        if ( cnt++ == n ) {
          //Serial.println( *p );
          return *p; 
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
