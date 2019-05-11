#include "Atm_led_scheduler.hpp"
#include "io.hpp"

// Atm_Led_scheduler????

//  enum { IDLE, WAITING, RUNNING, UPDATING }; // STATES
//  enum { EVT_DONE, EVT_RUN, EVT_UPDATE, EVT_MILLI, ELSE }; // EVENTS


Atm_led_scheduler& Atm_led_scheduler::begin( IO &io ) {
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
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_led_scheduler::event( int id ) {
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
 * This generates the 'output' for the state machine
 */

void Atm_led_scheduler::action( int id ) {
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

Atm_led_scheduler& Atm_led_scheduler::defineProfile( uint8_t prof, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 ) {
  led_profile[prof].T0 = T0;
  led_profile[prof].L1 = L1;
  led_profile[prof].T1 = T1;
  led_profile[prof].L2 = L2;
  return *this;
}

Atm_led_scheduler& Atm_led_scheduler::set( int16_t n, uint32_t c ) {
  io->setPixelColor( n, c >> 24, c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff );
  return *this;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_led_scheduler& Atm_led_scheduler::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_led_scheduler::state( void ) {
  return Machine::state();
}

Atm_led_scheduler& Atm_led_scheduler::profile( int16_t n, uint8_t prof ) {
  if ( n > -1 ) {
    meta[n].profile = prof;
  }
  return *this;
}


Atm_led_scheduler& Atm_led_scheduler::on( int ledno ) {

  if ( ledno > -1 ) {
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
      trigger( EVT_UPDATE );
    }
  } 
  return *this;
}

Atm_led_scheduler& Atm_led_scheduler::off( int ledno, bool no_update /* = false */ ) {
  if ( ledno > -1 && led_profile[meta[ledno].profile].L2 ) { // Ignore off() for leds in pulse mode 
    meta[ledno].state = LED_STATE_IDLE;
    set( ledno, 0 );
    refresh = 1; 
    if ( !no_update ) trigger( EVT_UPDATE );
  }
  return *this;
}

Atm_led_scheduler& Atm_led_scheduler::off() {
  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    off( ledno, true );
  }  
  refresh = 1; 
  trigger( EVT_UPDATE );
  return *this;
}

Atm_led_scheduler& Atm_led_scheduler::toggle( int ledno, int v /* = -1 */ ) {
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

int Atm_led_scheduler::active( int ledno ) {
  return ledno > -1 ? meta[ledno].state : 0;  
}


/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


Atm_led_scheduler& Atm_led_scheduler::update() {
  trigger( EVT_UPDATE );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_led_scheduler& Atm_led_scheduler::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "APA102\0EVT_DONE\0EVT_RUN\0EVT_UPDATE\0EVT_MILLI\0ELSE\0IDLE\0WAITING\0RUNNING\0UPDATING" );
  return *this;
}
