#include "IO.hpp"

#undef DEBUG

// TODO zorgvuldig alle switch events checken op geldigheid
// Komen ze wel van een actieve matrix-node???
// Filteren direct bij input van matrix scanner.
// Input filter bitmap construeren in switchMap()???
//     soll[row][col] = filter[row][col] & (...
// default   switchMap( 0, 0, 0, 0, 0 );
// switch_map kan dan weg 

IO& IO::begin( int pin_clock, int pin_latch, uint8_t *address, uint8_t *inputs, uint8_t *gate ) {
  this->pin_clock = pin_clock;
  this->pin_latch = pin_latch;
  this->gate = gate;
  this->address = address;
  this->inputs = inputs;
  memset( profile, 0, sizeof( profile ) ); 
  for ( int i = 0; i < NUM_IOPORTS; i++ ) {
    pinMode( inputs[i], INPUT_MODE ); 
    pinMode( gate[i], OUTPUT ); 
    led_dirty[i] = -1;
  }
  for ( int i = 0; i < 3; i++ )
    pinMode( address[i], OUTPUT ); 
  pinMode( pin_latch, OUTPUT );
  pinMode( pin_clock, OUTPUT );
  IOWRITE( pin_clock, LOW );  
  switchMap( 8, 8, 8, 8, 8 );
  readMatrix( MATRIX_NODES, MATRIX_SWITCHES, true );
  debounce( 0, 0, 0,  0 );
  select( 0 );
  node_ptr = 0;
  switch_ptr = 0;
  last_press = millis();
  return *this;
}

IO& IO::addStrips( IO_Adafruit_NeoPixel strip[] ) { 
  for ( int n = 0; n < NUM_IOPORTS; n++ ) {
    strip[n].begin();
    strip[n].show();
    led_strip[n] = &strip[n];
    for ( uint8_t i = 0; i < strip[n].numPixels(); i++ ) {
      led[led_cnt].strip = n;
      led[led_cnt].led = i;
      led_cnt++;
    }
    led_dirty[n] = strip[n].numPixels() - 1; // Force full strip update on next IO::show()
  }
  return *this;
}

IO& IO::addStrip( IO_Adafruit_NeoPixel *strip ) { 
  strip->begin();
  strip->show();
  led_strip[last_strip] = strip;
  for ( uint8_t i = 0; i < strip->numPixels(); i++ ) {
    led[led_cnt].strip = last_strip;
    led[led_cnt].led = i;
    led_cnt++;
  }
  led_dirty[last_strip] = strip->numPixels() - 1; // Force full strip update on next IO::show()
  last_strip++;
  return *this;
}

IO& IO::setPixelColor( int16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w ) {
  if ( n < led_cnt ) {
#ifdef DEBUG
    Serial.printf( "%d setPixelColor %d: strip=%d, led=%d, value=%d, %d, %d, %d\n", millis(), n, led[n].strip, led[n].led, r, g, b, w );    
#endif
#ifdef TRACE_LED
    if ( n == TRACE_LED ) 
      Serial.printf( "%d setPixelColor %d: strip=%d, led=%d, value=%d, %d, %d, %d\n", millis(), n, led[n].strip, led[n].led, r, g, b, w );    
#endif
    uint8_t nled = led[n].led;
    uint8_t strip = led[n].strip;
    if ( led_strip[strip]->bytesPerPixel() == 3 ) { // Convert w value to rgb for 3 byte strips
      if ( w > 0 && r + g + b == 0 ) r = g = b = w;    
    }
    led_strip[strip]->setPixelColor( led[n].led, r, g, b, w );
    led_dirty[strip] = max( nled, led_dirty[strip] );   
    log_last_pixel = n;
  }
  return *this;
}

uint16_t IO::numberOfLeds( void ) {
  return led_cnt;
}

uint16_t IO::numberOfSwitches( void ) {
  return switch_cnt * 8;
}

uint32_t IO::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t w ) {
   return (uint32_t) r << 24 | (uint32_t) g << 16 | (uint32_t) b << 8 | (uint32_t) w;
}

int16_t IO::lastPixel( void ) {
  return log_last_pixel;
}

bool IO::show() {
  bool success = true;
  for ( uint8_t i = 0; i < NUM_IOPORTS; i++ ) {
    if ( led_dirty[i] > -1 ) {
      if ( led_strip[i]->canShow() ) {
#ifdef DEBUG
        Serial.print( "Show strip " );
        Serial.println( i );
#endif      
        select( i );
        led_strip[i]->show( led_dirty[i] + 1 ); // Partial refresh
        //led_strip[i]->show( 0 ); // Full refresh
        led_dirty[i] = -1; 
      } else {
        success = false;
      }
    }
  }
  return success;
}

IO& IO::switchMap( uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t r5 ) {
  row_map[0] = 0;
  row_map[1] = r1;
  row_map[2] = row_map[1] + r2;
  row_map[3] = row_map[2] + r3;
  row_map[4] = row_map[3] + r4;
  switch_map = 0;
  if ( r1 ) switch_map |= 0B00000001;
  if ( r2 ) switch_map |= 0B00000010;
  if ( r3 ) switch_map |= 0B00000100;
  if ( r4 ) switch_map |= 0B00001000;
  if ( r5 ) switch_map |= 0B00010000;
  node_max = max( max( max( max( r1, r2 ), r3 ), r4 ), r5 );  
  switch_cnt = r1 + r2 + r3 + r4 + r5;
  return *this;
}

uint32_t IO::timer() {
  return last_read_time;
}

IO& IO::retrigger() {
    memset( ist, 0, sizeof( ist ) );
    return *this;
}

IO& IO::range( uint8_t node_max, uint8_t col_max ) {
  this->node_max = node_max;
  this->switch_max = switch_max;
  return *this;
}

IO& IO::invert( uint8_t code ) {
  if ( code != 0 ) {
    code = abs( code ) - 1;
    int bus = NUM_IOPORTS - 1;
    while ( ( row_map[bus] * MATRIX_SWITCHES ) > code ) bus--;
    nc[code / MATRIX_SWITCHES - row_map[bus]][code % MATRIX_NODES] |= ( 1 << bus );; 
  }
  return *this;
}
    
IO& IO::invert( uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, uint8_t c6, uint8_t c7, uint8_t c8 ) {
  if ( c1 > 0 ) invert( c1 );
  if ( c2 > 0 ) invert( c2 );
  if ( c3 > 0 ) invert( c3 );
  if ( c4 > 0 ) invert( c4 );
  if ( c5 > 0 ) invert( c5 );
  if ( c6 > 0 ) invert( c6 );
  if ( c7 > 0 ) invert( c7 );
  if ( c8 > 0 ) invert( c8 );
  return *this;
}
    
IO& IO::select( int row, bool latch /* = false */ ) {
  IOWRITE( address[0], row & 0b00000001 );
  IOWRITE( address[1], row & 0b00000010 );
  IOWRITE( address[2], row & 0b00000100 );
  delayMicroseconds( PULSE_WIDTH_USEC ); 
  IOWRITE( pin_latch, LOW );
  delayMicroseconds( PULSE_WIDTH_USEC );
  if ( latch ) {
    IOWRITE( pin_latch, HIGH );
    delayMicroseconds( PULSE_WIDTH_USEC );        
  }
  IOWRITE( gate[selected], LOW );
  selected = row;
  IOWRITE( gate[selected], HIGH );
  return *this;
}

IO& IO::readRow( uint8_t row, uint8_t mx_width ) {
  select( row, true );
  for ( uint8_t col = 0; col < mx_width; col++ ) {
    soll[row][col] = 
      ( IOREAD( inputs[0] ) ) |
      ( IOREAD( inputs[1] ) << 1 ) |
      ( IOREAD( inputs[2] ) << 2 ) |
      ( IOREAD( inputs[3] ) << 3 ) |
      ( IOREAD( inputs[4] ) << 4 );
    IOWRITE( pin_clock, LOW );
    delayMicroseconds( PULSE_WIDTH_USEC );
    IOWRITE( pin_clock, HIGH );
    delayMicroseconds( PULSE_WIDTH_USEC );
  }
  return *this;
}

IO& IO::readMatrix( uint8_t mx_depth, uint8_t mx_width, bool init /* = false */ ) {
  for ( uint8_t i = 0; i < mx_depth; i++ ) readRow( i, mx_width );
  if ( init ) 
    memcpy( ist, soll, sizeof( ist ) );
  return *this;
}

// Return 1-based mapped index

uint16_t IO::decimal_encode( uint8_t bus, uint8_t row, uint8_t col ) {
  uint16_t code = col + row * MATRIX_SWITCHES + row_map[bus] * MATRIX_NODES + 1;
  return ( code <= switch_cnt * 8 ) ? code : 0; // Zou niet nodig moeten zijn...
}

uint16_t IO::isPressed( int16_t code ) {
  if ( code != 0 ) {
    code = abs( code ) - 1;
    int bus = NUM_IOPORTS - 1;
    while ( ( row_map[bus] * MATRIX_SWITCHES ) > code ) bus--;
    return ( ist[code / MATRIX_SWITCHES - row_map[bus]][code % MATRIX_NODES] & ( 1 << bus ) ) > 0; 
  } else {
    return 0;
  }
}

int16_t IO::scan_raw() {
  for (;;) {      
    // Check one normalized (=corrected for normally closed switches) byte (5 bits) for changes
    uint8_t soll_normalized = soll[node_ptr][switch_ptr] ^ nc[node_ptr][switch_ptr];
    uint8_t changes = ist[node_ptr][switch_ptr] ^ soll_normalized;
    uint8_t subscriptions = sub[node_ptr][switch_ptr];
    if ( changes || subscriptions ) {
      while ( io_ptr < NUM_IOPORTS ) {
        if ( changes & ( 1 << io_ptr ) & switch_map ) { // Send changes
          if ( ( 1 << io_ptr ) & soll_normalized ) { // it's a press
            ist[node_ptr][switch_ptr] |= ( 1 << io_ptr );
            io_ptr++; // Skip next scan() call         
            return decimal_encode( io_ptr - 1, node_ptr, switch_ptr );
          } else { // it's a release
            ist[node_ptr][switch_ptr] &= ~( 1 << io_ptr );                    
            io_ptr++; // Skip next scan() call         
            return decimal_encode( io_ptr - 1, node_ptr, switch_ptr ) * -1;
          }
        }
        if ( subscriptions & ( 1 << io_ptr ) ) { // Send updates
          if ( ist[node_ptr][switch_ptr] & ( 1 << io_ptr ) ) {
            io_ptr++;
            return decimal_encode( io_ptr - 1, node_ptr, switch_ptr );            
            //return decimal_encode( io_ptr++, node_ptr, switch_ptr );  // etc (if everything works)          
          } else {
            io_ptr++;
            return decimal_encode( io_ptr - 1, node_ptr, switch_ptr ) * -1;                        
          }
        }
        io_ptr++;
      }
    }
    io_ptr = 0;
    // Increment col & row pointers modulo 8 (should be col_max, row_max?)
    if ( ! ( switch_ptr = ( switch_ptr + 1 ) % MATRIX_SWITCHES ) ) {
      // On row wrap around 7 -> 0 read the next row
      if ( ! ( node_ptr = ( node_ptr + 1 ) % MATRIX_NODES ) ) {
        // On row wrap around 7 -> 0 re-read the matrix
        last_read_time = micros();
        readMatrix( node_max, switch_max );          
        last_read_time = micros() - last_read_time;
        return 0; // return neutral code
      }
    }
  }
}  

// Subscribe/unsubscribe to the most recently triggered switch

IO& IO::subscribe( void ) {
  sub[node_ptr][switch_ptr] |= ( 1 << ( io_ptr - 1 ) );
  return *this;
}

IO& IO::unsubscribe( void ) {
  sub[node_ptr][switch_ptr] &= ~( 1 << ( io_ptr - 1 ) );
  return *this;
}

// First layer of debouncing (leading and trailing edges)

int16_t IO::debounce( int16_t code ) {
  enum { IDLE, WAIT_ACTIVE, ACTIVE, WAIT_IDLE };
  int16_t addr = abs( code );
  if ( code != 0 ) {
    switch ( profile[addr].state ) {
      case IDLE:
        if ( code > 0 ) {
          if ( profile[addr].press_micros == 0 ) {
            profile[addr].state = ACTIVE; // Shortcut to ACTIVE
            profile[addr].timer = micros();
            subscribe();
            return code;            
          } else {
            profile[addr].state = WAIT_ACTIVE; // press_micros set, wait for timer
            profile[addr].timer = micros();
            subscribe();
            return 0;
          }
        }
        break;
      case WAIT_ACTIVE:
        if ( micros() - profile[addr].timer >= profile[addr].press_micros ) {
          if ( profile[addr].release_micros == 0 ) {
            profile[addr].state = WAIT_IDLE; // Shortcut to WAIT_IDLE    
            unsubscribe();
            return code; // PRESS
          } else {
            profile[addr].state = ACTIVE;  
            profile[addr].timer = micros();
            subscribe();
            return code; // PRESS
          }
        }  
        if ( code < 0 ) {
          profile[addr].state = IDLE; 
          unsubscribe();
        } 
        break;
      case ACTIVE:
        if ( micros() - profile[addr].timer >= profile[addr].release_micros ) {
          unsubscribe();
          if ( code > 0 ) {
            profile[addr].state = WAIT_IDLE; 
          } else {
            profile[addr].state = IDLE;
            return -addr; // RELEASE      
          } 
        }
        break;
      case WAIT_IDLE:
        if ( code < 0 ) {
          profile[addr].state = IDLE;
          return code; // RELEASE
        }
        break;    
    }
  }
  return 0;
}  

// Second layer of debouncing (throttling)
// Uses millis to minimize chance of overflow
// Does not use subscribe()

int16_t IO::throttle( int16_t code ) { 
  enum { IDLE, BLOCKING };
  int16_t addr = abs( code );
  if ( code != 0 ) {
    switch ( profile[addr].throttle_state ) {
      case IDLE:
        if ( code > 0 ) {
          if ( profile[addr].throttle_millis != 0 ) {
            profile[addr].throttle_timer = millis(); // Move to blocking
            profile[addr].throttle_state = BLOCKING;
          }
          return code; // PRESS
        } else {
          return code; // RELEASE
        }
        break;
      case BLOCKING:
        if ( code > 0 ) {
          if ( millis() - profile[addr].throttle_timer >= profile[addr].throttle_millis ) {
            profile[addr].throttle_timer = millis(); // Stay in blocking
            profile[addr].throttle_state = BLOCKING; // PRESS
            return code;                        
          }
          return 0; // IGNORE 
        } else {
          if ( millis() - profile[addr].throttle_timer >= profile[addr].throttle_millis ) {
            profile[addr].throttle_state = IDLE;     // Leave blocking
          }
          return code; // RELEASE
        }  
        break;      
    }
  }
  return 0;
}

// Separation filter
// Only fire the switch after a minimum 'quiet' time (no other presses)

int16_t IO::separate( int16_t code ) {
  if ( code > 0 ) {
    int16_t addr = abs( code );
    if ( profile[addr].separate_millis != 0 ) {
      if ( millis() - last_press < profile[addr].separate_millis ) {
        return 0;
      }      
    }    
    last_press = millis();
  }
  return code;
}

int16_t IO::scan( void ) { 
  return separate( throttle( debounce( scan_raw() ) ) );
}

// Test edge case: laatste switch!
IO& IO::debounce( int16_t n, uint16_t press_ticks, uint16_t release_ticks, uint16_t throttle_ticks, uint16_t separate_ticks ) {
  if ( n > 0 && n <= numberOfSwitches() ) {
    profile[n].press_micros = press_ticks * IO_TICK_MS;
    profile[n].release_micros = release_ticks * IO_TICK_MS;
    profile[n].throttle_millis = throttle_ticks / 10;
    profile[n].separate_millis = separate_ticks / 10;
    profile[n].throttle_state = 0;
    profile[n].state = 0;
  }
  return *this;  
}

IO& IO::debounce( uint16_t press_ticks, uint16_t release_ticks, uint16_t throttle_ticks, uint16_t separate_ticks ) {
  for ( int16_t i = 0; i < this->numberOfSwitches(); i++ ) {
    debounce( i + 1, press_ticks, release_ticks, throttle_ticks, separate_ticks );
  }
  return *this;  
}
