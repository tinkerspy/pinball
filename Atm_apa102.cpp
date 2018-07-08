#include "Atm_apa102.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

/* 
 *  TODO: pulse method: add delay_time (before pulse starts) and intensity arguments
 *
 */

Atm_apa102& Atm_apa102::begin( int number_of_leds, int gate_pin ) {
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
  this->gate_pin = gate_pin;
  if ( this->gate_pin > -1 ) {
    pinMode( this->gate_pin, OUTPUT );
    digitalWrite( this->gate_pin, LOW );
  }
  spi4teensy3::init();
  gbrgb( 31, 5, 5, 5 ).off(); // Safe default
  this->showLeds();
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
            set( i, 0, 0, 0, 0 ); // Set to black (no fade)
          } else {
            running = 1; // We're still running
          }
        }
      }
      if ( refresh ) {
        this->showLeds();
        refresh = 0;
      }
      return;
    case ENT_UPDATING:
      this->showLeds();
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


  
Atm_apa102& Atm_apa102::dump( void ) {

  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    Serial.printf( "gb=%d, r=%d, g=%d, b=%d\n", leds.led[ledno].gb, leds.led[ledno].r, leds.led[ledno].g, leds.led[ledno].b );
  }
  return *this;
}


Atm_apa102& Atm_apa102::gbrgb( int ledno, int gb, int r, int g, int b ) {
  
  if ( ledno > -1 ) {
    if ( gb > -1 ) meta[ledno].gb = gb;
    if ( r > -1 ) meta[ledno].r = r;
    if ( g > -1 ) meta[ledno].g = g;
    if ( b > -1 ) meta[ledno].b = b;     
  }
  return *this;
}

Atm_apa102& Atm_apa102::gbrgb( int gb, int r, int g, int b ) {
  
  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    if ( gb > -1 ) meta[ledno].gb = gb;
    if ( r > -1 ) meta[ledno].r = r;
    if ( g > -1 ) meta[ledno].g = g;
    if ( b > -1 ) meta[ledno].b = b;     
  }
  return *this;
}

Atm_apa102& Atm_apa102::set( int ledno, int gb, int r, int g, int b ) {

  if ( ledno > -1 ) {
    leds.led[ledno].gb = gb + 0xE0;    
    leds.led[ledno].r = r;
    leds.led[ledno].g = g;
    leds.led[ledno].b = b;
    meta[ledno].status = ( r > 0 || g > 0 || b > 0 ) && gb > 0 ? 1 : 0;  
    refresh = 1; 
    trigger( EVT_UPDATE ); 
  }
  return *this;  
}

Atm_apa102& Atm_apa102::on( int ledno ) {

  if ( ledno > -1 ) {
    leds.led[ledno].gb = meta[ledno].gb + 0xE0;    
    leds.led[ledno].r = meta[ledno].r;
    leds.led[ledno].g = meta[ledno].g;
    leds.led[ledno].b = meta[ledno].b;
    meta[ledno].status = 1;  
    refresh = 1; 
    trigger( EVT_UPDATE ); 
  } 
  return *this;
}

Atm_apa102& Atm_apa102::off( int ledno ) {
  
  if ( ledno > -1 ) {
    leds.led[ledno].gb = 0xFF;    
    leds.led[ledno].r = 0;
    leds.led[ledno].g = 0;
    leds.led[ledno].b = 0;
    meta[ledno].status = 0;  
    refresh = 1; 
    trigger( EVT_UPDATE );
  }  
  return *this;
}

Atm_apa102& Atm_apa102::off() {
  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    leds.led[ledno].gb = 0xFF;    
    leds.led[ledno].r = 0;
    leds.led[ledno].g = 0;
    leds.led[ledno].b = 0;
    meta[ledno].status = 0;  
  }  
  refresh = 1; 
  trigger( EVT_UPDATE );
  return *this;
}

Atm_apa102& Atm_apa102::pulse( int ledno, uint16_t duration ) {

  if ( ledno > -1 ) {
    set( ledno, meta[ledno].gb, meta[ledno].r, meta[ledno].g, meta[ledno].b ); // Set to default brightness (no fade)
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


Atm_apa102& Atm_apa102::showLeds( void ) {
  
  if ( this->gate_pin > -1 ) digitalWriteFast( this->gate_pin, HIGH );
  spi4teensy3::send( 0 ); // Startframe
  spi4teensy3::send( 0 ); 
  spi4teensy3::send( 0 ); 
  spi4teensy3::send( 0 );
  for ( int ledno = 0; ledno < this->number_of_leds; ledno++ ) { // Sending as one block is slightly faster
    spi4teensy3::send( &leds.led[ledno], 4 ); 
  }
  spi4teensy3::send( 0xFF ); // 16 leds endframe
  spi4teensy3::send( 0xFF ); // 32 leds
  spi4teensy3::send( 0xFF ); // 48 leds
  spi4teensy3::send( 0xFF ); // 54 leds
  if ( this->gate_pin > -1 ) digitalWriteFast( this->gate_pin, LOW );
  return *this;
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



