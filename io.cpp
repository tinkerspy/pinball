#include "io.hpp"

IO& IO::begin( int pin_data, int pin_clock, int pin_pl, uint8_t *addr, uint8_t *inp, uint8_t *gate ) {
  this->pin_data = pin_data;
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
  pinMode( pin_data, OUTPUT );
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

IO& IO::strip( uint8_t n, Adafruit_NeoPixel &s, uint8_t bytes_per_pixel ) {
  s.begin();
  led_strip[n] = &s;
  led_dirty[n] = 255;
  led_bytes[n] = bytes_per_pixel;
  for ( uint8_t i = 0; i < s.numPixels(); i++ ) {
    log_led[log_led_cnt].strip = n;
    log_led[log_led_cnt].led = i;
    log_led_cnt++;
  }
  return *this;
}

IO& IO::setPixelColor( uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w ) {
  if ( n < log_led_cnt ) {
    Serial.print( "setPixelColor " );
    Serial.print( n );
    Serial.print( ": strip=" );
    Serial.print( log_led[n].strip );
    Serial.print( ", led=" );
    Serial.println( log_led[n].led );
    led_strip[log_led[n].strip]->setPixelColor( log_led[n].led, r, g, b, w );
    if ( log_led[n].led > led_dirty[log_led[n].strip] ) 
      led_dirty[log_led[n].strip] = log_led[n].led;   
    log_last_pixel = n;
  }
  return *this;
}

IO& IO::setPixelMono( uint16_t n, uint8_t w ) {
  if ( n < log_led_cnt ) {
    if ( led_bytes[log_led[n].strip] == 3 ) {
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

int16_t IO::lastPixel( void ) {
  return log_last_pixel;
}

IO& IO::show() {
  for ( uint8_t i = 0; i < NUM_LED_STRIPS; i++ ) {
    if ( led_dirty[i] > -1 ) {
      Serial.print( "Show strip " );
      Serial.println( i );
      select( i );
      led_strip[i]->show();
      led_dirty[i] = -1; 
    }
  }
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

uint16_t IO::decimal_encode( uint8_t row, uint8_t col, uint8_t bus ) {
  return ( bus + 1 ) * 100 + ( row + 1 ) * 10 + col;
}

uint16_t IO::isPressed( int16_t code ) {
  code = abs( code );
  return ( ist[( ( code % 100 ) / 10 ) -1][code % 10] & ( 1 << ( ( code / 100 ) - 1 ) ) ) > 0; 
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
        // On row wrap around 8 -> 0 read the matrix
        last_read_time = micros();
        readMatrix( row_max, col_max );          
        last_read_time = micros() - last_read_time;
        return 0;
      }
  }
}  
