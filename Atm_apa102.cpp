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
#ifdef CORE_TEENSY
  spi4teensy3::init();
#else
  SPI.begin();
#endif
  for ( int i = 0; i < number_of_leds; i++ ) {
    leds.led[i].gb = 255;
    meta[i].status = 1; // Force off
  }
  gbrgb( 31, 255, 255, 255 ).fade( 0 ).off(); // Default for the whole strip
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
        if ( meta[i].fade_in ) {
          int16_t diff = (uint16_t) millis() - meta[i].last_millis; 
          if ( diff >= meta[i].fade_speed ) {
            if ( meta[i].fade_step < 31 ) {
              meta[i].fade_step++;
              set( i, 31, slope[meta[i].fade_step], slope[meta[i].fade_step], slope[meta[i].fade_step] );          
              meta[i].last_millis = millis();
              running = 1; 
            }
          } else {
            running = 1;
          }
        }
        if ( meta[i].fade_out ) {
          int16_t diff = (uint16_t) millis() - meta[i].last_millis; 
          if ( diff >= meta[i].fade_speed ) {
            if ( meta[i].fade_step < 31 ) {
              meta[i].fade_step++;
              set( i, 31, slope[31 - meta[i].fade_step], slope[31 - meta[i].fade_step], slope[31 - meta[i].fade_step] );          
              meta[i].last_millis = millis();
              running = 1; 
            }
          } else {
            running = 1;
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


  
Atm_apa102& Atm_apa102::dump( Stream& stream ) {

  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    stream.print( "led gb=" );
    stream.print( leds.led[ledno].gb );
    stream.print( ", r=" );
    stream.print( leds.led[ledno].r );
    stream.print( ", g=" );
    stream.print( leds.led[ledno].g );
    stream.print( ", b=" );
    stream.println( leds.led[ledno].b );
  }
  return *this;
}


Atm_apa102& Atm_apa102::dump_meta( Stream& stream ) {

  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    stream.print( "meta gb=" );
    stream.print( meta[ledno].gb );
    stream.print( ", r=" );
    stream.print( meta[ledno].r );
    stream.print( ", g=" );
    stream.print( meta[ledno].g );
    stream.print( ", b=" );
    stream.println( meta[ledno].b );
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

Atm_apa102& Atm_apa102::fade( int ledno, int speed ) {

  if ( ledno > -1 ) {
    meta[ledno].fade_speed = speed;
  }
  return *this;
}

Atm_apa102& Atm_apa102::fade( int speed ) {

  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    meta[ledno].fade_speed = speed;
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
    meta[ledno].pulsing = 0;
    if ( meta[ledno].fade_speed ) {
      meta[ledno].fade_step = 0;
      meta[ledno].fade_in = 1;   
      meta[ledno].fade_out = 0;   
      meta[ledno].last_millis = millis();        
      meta[ledno].fade_step = 0;
      running = 1;
      sleep( 0 );
    } else {
      leds.led[ledno].gb = meta[ledno].gb + 0xE0;    
      leds.led[ledno].r = meta[ledno].r;
      leds.led[ledno].g = meta[ledno].g;
      leds.led[ledno].b = meta[ledno].b;
      refresh = 1; 
      trigger( EVT_UPDATE ); 
    }
    meta[ledno].status = 1;  
  } 
  return *this;
}

Atm_apa102& Atm_apa102::off( int ledno, bool no_update /* = false */ ) {
  
  if ( ledno > -1 && meta[ledno].status > 0 ) {
    if ( meta[ledno].fade_speed ) {
      meta[ledno].fade_step = 0;
      meta[ledno].fade_in = 0;   
      meta[ledno].fade_out = 1;   
      meta[ledno].last_millis = millis();        
      meta[ledno].fade_step = 0;
      running = 1;
      sleep( 0 );
    } else { 
      leds.led[ledno].gb = 0xFF;    
      leds.led[ledno].r = 0;
      leds.led[ledno].g = 0;
      leds.led[ledno].b = 0;
      if ( !no_update ) {
        trigger( EVT_UPDATE );
        refresh = 1; 
      }
    }
    meta[ledno].status = 0;  
  }  
  return *this;
}

Atm_apa102& Atm_apa102::off() {
  for ( int ledno = 0; ledno < number_of_leds; ledno++ ) {
    off( ledno, true );
  }  
  refresh = 1; 
  trigger( EVT_UPDATE );
  return *this;
}

Atm_apa102& Atm_apa102::toggle( int ledno ) {
  if ( meta[ledno].status ) {
    off( ledno );
  } else {
    on( ledno );
  }
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

#ifdef CORE_TEENSY
Atm_apa102& Atm_apa102::showLeds( void ) { // Teensy 3.x version
  // Enable pin 13 for SPI and open the MOSI gate
  CORE_PIN13_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2); //   Alt2=SPIO_SCK.  chip pin PTC5 (13) 
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
  // Disable pin 13 for SPI and close the MOSI gate
  if ( this->gate_pin > -1 ) digitalWriteFast( this->gate_pin, LOW );
  CORE_PIN13_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(1); //   Alt1 = PTC5.  chip pin PTC5 (13)   
  return *this;
}

#else

Atm_apa102& Atm_apa102::showLeds( void ) { // Arduino UNO version
  if ( this->gate_pin > -1 ) digitalWrite( this->gate_pin, HIGH );
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  SPI.transfer( (uint8_t) 0 ); // Startframe
  SPI.transfer( (uint8_t) 0 ); 
  SPI.transfer( (uint8_t) 0 ); 
  SPI.transfer( (uint8_t) 0 );
  for ( int ledno = 0; ledno < this->number_of_leds; ledno++ ) { 
    SPI.transfer( leds.led[ledno].gb ); 
    SPI.transfer( leds.led[ledno].r ); 
    SPI.transfer( leds.led[ledno].g ); 
    SPI.transfer( leds.led[ledno].b ); 
  }
  SPI.transfer( (uint8_t) 0xFF ); // 16 leds endframe
  SPI.transfer( (uint8_t) 0xFF ); // 32 leds
  SPI.transfer( (uint8_t) 0xFF ); // 48 leds
  SPI.transfer( (uint8_t) 0xFF ); // 54 leds
  SPI.endTransaction();
  if ( this->gate_pin > -1 ) digitalWrite( this->gate_pin, LOW );
  return *this;
}

#endif

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
