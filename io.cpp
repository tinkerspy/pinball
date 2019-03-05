#include "IO.hpp"

#undef DEBUG

IO& IO::begin( int pin_clock, int pin_pl, uint8_t *addr, uint8_t *inp, uint8_t *gate ) {
  this->pin_clock = pin_clock;
  this->pin_pl = pin_pl;
  this->gate = gate;
  this->addr = addr;
  this->inp = inp;
  for ( int i = 0; i < 5; i++ )
    pinMode( inp[i], INPUT_MODE ); 
  for ( int i = 0; i < 3; i++ )
    pinMode( addr[i], OUTPUT ); 
  for ( int i = 0; i < 5; i++ )
    pinMode( gate[i], OUTPUT ); 
  pinMode( pin_pl, OUTPUT );
  pinMode( pin_clock, OUTPUT );
  IOWRITE( pin_clock, LOW );  
  readMatrix( 8, 8, true );
  select( 0 );
  row_ptr = 0;
  col_ptr = 0;
  for ( int i = 0; i < NUM_LED_STRIPS; i++ )
    led_dirty[i] = -1;
  return *this;
}

IO& IO::strip( uint8_t n, IO_Adafruit_NeoPixel &s ) { // TODO: automatically count 'n'
  s.begin();
  led_strip[n] = &s;
  led_dirty[n] = 255; // Force update on next IO::show()
  for ( uint8_t i = 0; i < s.numPixels(); i++ ) {
    log_led[log_led_cnt].strip = n;
    log_led[log_led_cnt].led = i;
    log_led_cnt++;
  }
  return *this;
}

IO& IO::setPixelColor( uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w ) {
  if ( n < log_led_cnt ) {
#ifdef DEBUG    
    Serial.print( millis() );
    Serial.print( " setPixelColor " );
    Serial.print( n );
    Serial.print( ": strip=" );
    Serial.print( log_led[n].strip );
    Serial.print( ", led=" );
    Serial.print( log_led[n].led );
    Serial.print( ", value=" );
    Serial.print( r );
    Serial.print( "," );
    Serial.print( g );
    Serial.print( "," );
    Serial.print( b );
    Serial.print( "," );
    Serial.println( w );
#endif
    if ( led_strip[log_led[n].strip]->bytesPerPixel() == 3 ) { // Convert w value to rgb for 3 byte strips
      if ( w > 0 && r + g + b == 0 ) r = g = b = w;    
    }
    led_strip[log_led[n].strip]->setPixelColor( log_led[n].led, r, g, b, w );
    if ( log_led[n].led > led_dirty[log_led[n].strip] ) 
      led_dirty[log_led[n].strip] = log_led[n].led;   
    log_last_pixel = n;
  }
  return *this;
}

IO& IO::setPixelMono( uint16_t n, uint8_t w ) {
  if ( n < log_led_cnt ) {
    if ( led_strip[log_led[n].strip]->bytesPerPixel() == 3 ) {
      setPixelColor( n, w, w, w, 0 );
    } else {
      setPixelColor( n, 0, 0, 0, w );
    }
  }
  return *this;
}

uint16_t IO::numPixels( void ) {
  return log_led_cnt;
}

uint32_t IO::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t w ) {
   return r << 24 | g << 16 | b << 8 | w;
}

uint32_t IO::Mono( uint8_t w ) {
   return w;
}

int16_t IO::lastPixel( void ) {
  return log_last_pixel;
}

bool IO::show() {
  bool success = true;
  for ( uint8_t i = 0; i < NUM_LED_STRIPS; i++ ) {
    if ( led_dirty[i] > -1 ) {
      if ( led_strip[i]->canShow() ) {
#ifdef DEBUG
        Serial.print( "Show strip " );
        Serial.println( i );
#endif      
        select( i );
        led_strip[i]->show();
        led_dirty[i] = -1; 
      } else {
        success = false;
      }
    }
  }
  return success;
}

IO& IO::map( uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t r5 ) {
  row_map[0] = 0;
  row_map[1] = r1;
  row_map[2] = row_map[1] + r2;
  row_map[3] = row_map[2] + r3;
  row_map[4] = row_map[3] + r4;
  row_max = max( max( max( max( r1, r2 ), r3 ), r4 ), r5 );  
  return *this;
}

uint32_t IO::timer() {
  return last_read_time;
}

IO& IO::retrigger() {
    memset( ist, 0, sizeof( ist ) );
    return *this;
}

IO& IO::range( uint8_t row_max, uint8_t col_max ) {
  this->row_max = row_max;
  this->col_max = col_max;
  return *this;
}
    
IO& IO::select( int row, bool latch /* = false */ ) {
  IOWRITE( addr[0], row & 0b00000001 );
  IOWRITE( addr[1], row & 0b00000010 );
  IOWRITE( addr[2], row & 0b00000100 );
  delayMicroseconds( PULSE_WIDTH_USEC ); 
  IOWRITE( pin_pl, LOW );
  delayMicroseconds( PULSE_WIDTH_USEC );
  if ( latch ) {
    IOWRITE( pin_pl, HIGH );
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
      ( IOREAD( inp[0] ) ) |
      ( IOREAD( inp[1] ) << 1 ) |
      ( IOREAD( inp[2] ) << 2 ) |
      ( IOREAD( inp[3] ) << 3 ) |
      ( IOREAD( inp[4] ) << 4 );
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

uint16_t IO::decimal_encode( uint8_t row, uint8_t col, uint8_t bus ) { 
  return col + row * 8 + row_map[bus] * 8 + 1;
}

uint16_t IO::isPressed( int16_t code ) {
  if ( code != 0 ) {
    code = abs( code ) - 1;
    int bus = 4;
    while ( ( row_map[bus] * 8 ) > code ) bus--;
    return ( ist[code / 8 - row_map[bus]][code % 8] & ( 1 << bus ) ) > 0; 
  } else {
    return 0;
  }
}

int16_t IO::scan() {
  for (;;) {      
    // Check one byte for changes
    if ( uint8_t xdiff = ist[row_ptr][col_ptr] ^ soll[row_ptr][col_ptr] ) {
      uint8_t bitpos = 0;
      while ( ( xdiff & 1 ) == 0 ) {
        xdiff >>= 1;
        bitpos++;
      }
      if ( ( 1 << bitpos ) & soll[row_ptr][col_ptr] ) { // press
        ist[row_ptr][col_ptr] |= ( 1 << bitpos );          
        return decimal_encode( row_ptr, col_ptr, bitpos );
      } else { // release
        ist[row_ptr][col_ptr] &= ~( 1 << bitpos );                    
        return decimal_encode( row_ptr, col_ptr, bitpos ) * -1;
      }
    }
    // Increment col & row pointers modulo 8 (should be col_max, row_max?)
    if ( ! ( col_ptr = ( col_ptr + 1 ) % 8 ) ) 
      if ( ! ( row_ptr = ( row_ptr + 1 ) % 8 ) ) {
        // On row wrap around 7 -> 0 read the matrix
        last_read_time = micros();
        readMatrix( row_max, col_max );          
        last_read_time = micros() - last_read_time;
        return 0;
      }
  }
}  
