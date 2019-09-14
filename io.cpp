#include "IO.hpp"

#undef DEBUG

IO& IO::begin( int pin_clock, int pin_latch, uint8_t *address, uint8_t *inputs, uint8_t *gate ) {
  this->pin_clock = pin_clock;
  this->pin_latch = pin_latch;
  this->gate = gate;
  this->address = address;
  this->inputs = inputs;
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
  select( 0 );
  node_ptr = 0;
  switch_ptr = 0;
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
    Serial.print( millis() );
    Serial.print( " setPixelColor " );
    Serial.print( n );
    Serial.print( ": strip=" );
    Serial.print( led[n].strip );
    Serial.print( ", led=" );
    Serial.print( led[n].led );
    Serial.print( ", value=" );
    Serial.print( r );
    Serial.print( "," );
    Serial.print( g );
    Serial.print( "," );
    Serial.print( b );
    Serial.print( "," );
    Serial.println( w );
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

uint16_t IO::numPixels( void ) {
  return led_cnt;
}

uint16_t IO::numSwitches( void ) {
  return switch_cnt;
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
        led_strip[i]->show( 0 );
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
  return col + row * MATRIX_SWITCHES + row_map[bus] * MATRIX_NODES + 1;
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

// TODO: remember last scan bitpos and continue from there...

int16_t IO::scan() {
  for (;;) {      
    // Check one normalized (=corrected for normally closed switches) byte (5 bits) for changes
    uint8_t soll_normalized = soll[node_ptr][switch_ptr] ^ nc[node_ptr][switch_ptr];
    if ( uint8_t changes = ist[node_ptr][switch_ptr] ^ soll_normalized ) {
      while ( io_ptr < NUM_IOPORTS ) {
        if ( changes & ( 1 << io_ptr ) ) { // something changed
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

IO& IO::unscan() { // Mark the last keypress as unprocessed so that will generate another scan() event
  ist[node_ptr][switch_ptr] ^= ( 1 << io_ptr );
  return *this;
}
