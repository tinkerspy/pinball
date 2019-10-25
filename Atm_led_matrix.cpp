#include "Atm_led_matrix.hpp"

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
  Symbolic_Machine::begin( state_table, ELSE );
  this->io = &io;
  numOfLeds = io.numberOfLeds();
  for ( int i = 0; i < numOfLeds; i++ )
    profile( i, 0, 127, 50, 0 ); // Select a safe default profile
  io.show();
  last_milli = millis();
  numOfGroups = 0;  
  if ( group_definition ) 
    group_def = parseGroups( group_definition );
  return *this;
}

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

void Atm_led_matrix::action( int id ) {
  switch ( id ) {
    case ENT_RUNNING:
      running = 0;
      for ( int i = 0; i < numOfLeds; i++ ) {
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
  numOfGroups = p - group_def;
  p++;
  while ( p[0] != -1 ) {
    int gid = p[0] - numOfLeds; // Leds start at 0 while switches start at 1, does that create a problem here???
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

int16_t Atm_led_matrix::numberOfGroups( void ) {
  return numOfGroups;
}


Atm_led_matrix& Atm_led_matrix::set( int16_t ledno, uint32_t c ) {
  if ( ledno > -1 ) {
    if ( ledno >= numOfLeds ) {
      int16_t p = group_def[ledno - numOfLeds];
      while ( p != -1 && group_def[p] != -1 ) 
        set( group_def[p++], c );       
    } else {
      io->setPixelColor( ledno, c >> 24, c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff );
    }
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::trigger( int event ) {
  Symbolic_Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
   Control what the machine returns when another process requests its state
*/

int Atm_led_matrix::state( void ) {
  return Symbolic_Machine::state();
}

Atm_led_matrix& Atm_led_matrix::profile( int16_t ledno, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 /* = 0 */  ) {
  if ( ledno > -1 ) {
    if ( ledno < numOfLeds ) { 
      meta[ledno].T0 = T0;
      meta[ledno].L1 = L1;
      meta[ledno].T1 = T1;
      meta[ledno].L2 = L2;
    } else {
      int16_t p = group_def[ledno - numOfLeds];
      while ( p != -1 && group_def[p] != -1 )
        profile( group_def[p++], T0, L1, T1, L2 );
    }
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::on( int ledno, bool no_update /* = false */  ) {
  if ( ledno > -1 ) {
    if ( ledno >= numOfLeds ) {
      int16_t p = group_def[ledno - numOfLeds];
      while ( p != -1 && group_def[p] != -1 )
        on( group_def[p++], true );
      if ( !no_update ) trigger( EVT_UPDATE );
    } else {
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
        if ( !no_update ) trigger( EVT_UPDATE );
      }
    }
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::off( int ledno, bool no_update /* = false */ ) {
  if ( ledno > -1 ) {
    if ( ledno >= numOfLeds ) {
      int16_t p = group_def[ledno - numOfLeds];
        while ( p != -1 && group_def[p] != -1 )
          off( group_def[p++], true );
      if ( !no_update ) trigger( EVT_UPDATE );
    } else {
      if ( ledno > -1 && meta[ledno].L2 ) { // Ignore off() for leds in pulse mode
        meta[ledno].state = LED_STATE_IDLE;
        set( ledno, 0 );
        refresh = 1;
        if ( !no_update ) trigger( EVT_UPDATE );
      }
    }
  }
  return *this;
}

Atm_led_matrix& Atm_led_matrix::off() {
  for ( int ledno = 0; ledno < numOfLeds; ledno++ )
    off( ledno, true );
  refresh = 1;
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::toggle( int ledno, int v /* = -1 */ ) {
  if ( ledno > -1 ) {
    if ( ledno >= numOfLeds ) {
      int16_t p = group_def[ledno - numOfLeds];
      while ( p != -1 && group_def[p] != -1 )
        toggle( group_def[p++], v );
      trigger( EVT_UPDATE );
    } else {
      if ( v > -1 ) {
        if ( v ) on( ledno ); else off( ledno );
        return *this;
      }
      if ( meta[ledno].state ) {
        off( ledno );
      } else {
        on( ledno );
      }
    }
  }
  return *this;
}

int Atm_led_matrix::active( int ledno ) {
  if ( ledno > -1 ) {
    if ( ledno >= numOfLeds ) {
      int16_t p = group_def[ledno - numOfLeds];
      return active( group_def[p] );
    } else {
      return ledno > -1 ? meta[ledno].state : 0;
    }
  }
  return 0;
}

int16_t Atm_led_matrix::count( int16_t ledno, int8_t led_active /* = -1 */ ) {
  int16_t cnt = 0;
  if ( ledno > -1 ) {
    if ( ledno < numOfLeds ) { // Physical led
      if ( led_active == -1 ) {
        cnt++;
      } else {
        if ( ( meta[ledno].state > 0 ) == led_active ) cnt++;
      }
    } else {   // Virtual led -> expand & recurse
      int16_t p = group_def[ledno - numOfLeds];
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
  if ( ledno > -1 ) {
    if ( ledno < numOfLeds ) { // Physical led, return ledno 
        return n == 0 ? ledno : -1;  
    } else {   // Virtual led -> expand & recurse
      int16_t p = group_def[ledno - numOfLeds];
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

Atm_led_matrix& Atm_led_matrix::update() {
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::trace( Stream & stream ) {
  Symbolic_Machine::setTrace( &stream, atm_serial_debug::trace,
                     "APA102\0EVT_DONE\0EVT_RUN\0EVT_UPDATE\0EVT_MILLI\0ELSE\0IDLE\0WAITING\0RUNNING\0UPDATING" );
  Serial.printf( "%d Tracing enabled %s@%X\n", millis(), symbols, (long)(this) );
  return *this;
}

Atm_led_matrix& Atm_led_matrix::trace( void ) {
  Serial.printf( "%d Tracing disabled %s@%X\n", millis(), symbols, (long)(this) );
  Machine::setTrace( NULL, NULL, "" );
  return *this;
}
