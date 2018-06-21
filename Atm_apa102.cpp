#include "Atm_apa102.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

/* 
 *  TODO: add pulse delay (before pulse starts)
 *
 */

Atm_apa102& Atm_apa102::begin( int number_of_leds, int idx ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                 ON_ENTER    ON_LOOP  ON_EXIT  EVT_DONE  EVT_RUN  EVT_UPDATE  EVT_MILLI,    ELSE */
    /*     IDLE */           -1, ATM_SLEEP,      -1,       -1, RUNNING,   UPDATING,        -1,      -1,
    /*  WAITING */           -1,        -1,      -1,       -1,      -1,         -1,   RUNNING,      -1,    
    /*  RUNNING */  ENT_RUNNING,        -1,      -1,     IDLE,      -1,         -1,        -1, WAITING,
    /* UPDATING */ ENT_UPDATING,        -1,      -1,       -1, WAITING,         -1,        -1,    IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->number_of_leds = number_of_leds;
  switch ( idx ) {
    case SPI_11_13: 
      controller = &FastLED.addLeds<APA102, 11, 13>(leds, number_of_leds);
      break;
    case SPI_11_20: 
      controller = &FastLED.addLeds<APA102, 11, 20>(leds, number_of_leds);
      break;
    case SPI_21_13: 
      controller = &FastLED.addLeds<APA102, 21, 13>(leds, number_of_leds);
      break;
    case SPI_21_20: 
      controller = &FastLED.addLeds<APA102, 21, 20>(leds, number_of_leds);
      break;
  }
  rgb( 0x050505 ); // Safe default
  controller->showLeds();
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_apa102::event( int id ) {
  switch ( id ) {
    case EVT_DONE:
      return !running;
    case EVT_RUN:
      return running;
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

void Atm_apa102::action( int id ) {
  switch ( id ) {
    case ENT_RUNNING:
      running = 0;
      for ( int i = 0; i < number_of_leds; i++ ) {
        if ( meta[i].pulsing ) {
          // Leaving out the intermediate diff results in missed pulses
          int16_t diff = (uint16_t) millis() - meta[i].last_millis; 
          if ( diff >= meta[i].pulse_millis ) {
            meta[i].pulsing = 0;
            set( i, 0 ); // Set to black (no fade)
          } else {
            running = 1; // We're still running
          }
        }
      }
      if ( refresh ) {
        controller->showLeds();
        refresh = 0;
      }
      return;
    case ENT_UPDATING:
      controller->showLeds();
      refresh = 0;
      return;
    case ENT_IDLE:
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_apa102& Atm_apa102::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_apa102::state( void ) {
  return Machine::state();
}
  
Atm_apa102& Atm_apa102::rgb( uint32_t rgb ) {

  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    meta[ledno].rgb = rgb;     
  }
  return *this;  
}

Atm_apa102& Atm_apa102::level( uint8_t v ) {

  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    meta[ledno].rgb = v << 16 | v << 8 | v;     
  }
  return *this;  
}

Atm_apa102& Atm_apa102::level( uint8_t ledno, uint8_t v ) {

  if ( ledno > -1 ) meta[ledno].rgb = v << 16 | v << 8 | v;     
  return *this;  
}

Atm_apa102& Atm_apa102::rgb( int ledno, uint32_t rgb ) {

  if ( ledno > -1 ) meta[ledno].rgb = rgb;
  return *this;  
}

Atm_apa102& Atm_apa102::rgb( int ledno, uint8_t r, uint8_t g, uint8_t b ) {
  
  if ( ledno > -1 ) meta[ledno].rgb = r << 16 | g << 8 | b;     
  return *this;
}

Atm_apa102& Atm_apa102::set( int ledno, uint32_t rgb ) {

  if ( ledno > -1 ) {
    leds[ledno] = rgb;
    meta[ledno].status = rgb > 0 ? 1 : 0;  
    refresh = 1; 
    trigger( EVT_UPDATE ); 
  }
  return *this;  
}

Atm_apa102& Atm_apa102::set( int ledno, uint8_t r, uint8_t g, uint8_t b ) {
  
  if ( ledno > -1 ) {
    leds[ledno] = CRGB( r, g, b );
    meta[ledno].status = r > 0 || g > 0 || b > 0 ? 1 : 0;  
    refresh = 1; 
    trigger( EVT_UPDATE );
  }  
  return *this;
}

Atm_apa102& Atm_apa102::on( int ledno ) {

  if ( ledno > -1 ) {
    leds[ledno] = meta[ledno].rgb;
    meta[ledno].status = 1;  
    refresh = 1; 
    trigger( EVT_UPDATE ); 
  } 
  return *this;
}

Atm_apa102& Atm_apa102::off( int ledno ) {
  
  if ( ledno > -1 ) {
    leds[ledno] = 0;
    meta[ledno].status = 0;  
    refresh = 1; 
    trigger( EVT_UPDATE );
  }  
  return *this;
}

Atm_apa102& Atm_apa102::pulse( int ledno, uint16_t duration ) {

  if ( ledno > -1 ) {
    set( ledno, meta[ledno].rgb ); // Set to default brightness (no fade)
    if ( duration != 0xFFFF ) {
      // Set up pulse monitoring
      meta[ledno].pulsing = 1;
      meta[ledno].last_millis = millis();
      meta[ledno].pulse_millis = duration;
      running = 1;
      sleep( 0 );
    }
  }
  return *this;
}

int Atm_apa102::active( int ledno ) {

  return ledno > -1 ? meta[ledno].status : 0;  
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


Atm_apa102& Atm_apa102::update() {
  trigger( EVT_UPDATE );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_apa102& Atm_apa102::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "APA102\0EVT_DONE\0EVT_RUN\0EVT_UPDATE\0EVT_MILLI\0ELSE\0IDLE\0WAITING\0RUNNING\0UPDATING" );
  return *this;
}



