#include "io.hpp"

IO& IO::begin( int pin_data, int pin_clock, int pin_pl, uint8_t *addr, uint8_t *inp, uint8_t *gate ) {
  this->pin_data = pin_data;
  this->pin_clock = pin_clock;
  this->pin_pl = pin_pl;
  this->gate = gate;
  this->addr = addr;
  this->inp = inp;
  for ( int i = 0; i < 5; i++ )
    pinMode( inp[i], INPUT_PULLDOWN ); 
  for ( int i = 0; i < 3; i++ )
    pinMode( addr[i], OUTPUT ); 
  pinMode( pin_pl, OUTPUT );
  pinMode( pin_data, OUTPUT );
  pinMode( pin_clock, OUTPUT );
  digitalWrite( pin_clock, LOW );  
  readMatrix( 8, 8, true );
  row_ptr = 0;
  col_ptr = 0;
  return *this;
}

uint32_t IO::timer() {
  return this->last_read_time;
}

IO& IO::retrigger() {
    for ( uint8_t row = 0; row < 8; row++ ) 
      for ( uint8_t col = 0; col < 8; col++ ) 
        this->ist[row][col] = 0;  
}

IO& IO::range( uint8_t row_max, uint8_t col_max ) {
  this->row_max = row_max;
  this->col_max = col_max;
  return *this;
}
    
IO& IO::select( int row, bool latch /* = false */ ) {
  digitalWriteFast( this->addr[0], row & 0b00000001 );
  digitalWriteFast( this->addr[1], row & 0b00000010 );
  digitalWriteFast( this->addr[2], row & 0b00000100 );
  delayMicroseconds( PULSE_WIDTH_USEC ); 
  digitalWrite( this->pin_pl, LOW );
  delayMicroseconds( PULSE_WIDTH_USEC );
  if ( latch ) {
    digitalWrite( this->pin_pl, HIGH );
    delayMicroseconds( PULSE_WIDTH_USEC );        
  }
  return *this;
}

IO& IO::readRow( uint8_t row, uint8_t mx_width ) {
  select( row, true );
  for ( uint8_t col = 0; col < mx_width; col++ ) {
    this->soll[row][col] = 
      ( digitalReadFast( this->inp[0] ) ) |
      ( digitalReadFast( this->inp[1] ) << 1 ) |
      ( digitalReadFast( this->inp[2] ) << 2 ) |
      ( digitalReadFast( this->inp[3] ) << 3 ) |
      ( digitalReadFast( this->inp[4] ) << 4 );
    digitalWrite( this->pin_clock, LOW );
    delayMicroseconds( PULSE_WIDTH_USEC );
    digitalWrite( this->pin_clock, HIGH );
    delayMicroseconds( PULSE_WIDTH_USEC );
  }
  return *this;
}

IO& IO::readMatrix( uint8_t mx_depth, uint8_t mx_width, bool init /* = false */ ) {
  for ( uint8_t i = 0; i < mx_depth; i++ ) readRow( i, mx_width );
  if ( init ) 
    for ( uint8_t row = 0; row < 8; row++ ) 
      for ( uint8_t col = 0; col < 8; col++ ) 
        this->ist[row][col] = this->soll[row][col];
  return *this;
}

uint16_t IO::decimal_encode( uint8_t row, uint8_t col, uint8_t bus ) {
  return ( bus + 1 ) * 100 + ( row + 1 ) * 10 + col;
}

uint16_t IO::isPressed( int16_t code ) {
  code = abs( code );
  return ( this->ist[( ( code % 100 ) / 10 ) -1][code % 10] & ( 1 << ( ( code / 100 ) - 1 ) ) ) > 0; 
}

int16_t IO::scan() {
  for (;;) {      
    if ( uint8_t xdiff = this->ist[this->row_ptr][this->col_ptr] ^ this->soll[this->row_ptr][this->col_ptr] ) {
      uint8_t bitpos = 0;
      while ( ( xdiff & 1 ) == 0 ) {
        xdiff >>= 1;
        bitpos++;
      }
      if ( ( 1 << bitpos ) & this->soll[this->row_ptr][this->col_ptr] ) { // press
        this->ist[this->row_ptr][this->col_ptr] |= ( 1 << bitpos );          
        return decimal_encode( this->row_ptr, this->col_ptr, bitpos );
      } else { // release
        this->ist[this->row_ptr][this->col_ptr] &= ~( 1 << bitpos );                    
        return decimal_encode( this->row_ptr, this->col_ptr, bitpos ) * -1;
      }
    }
    if ( ! ( this->col_ptr = ++this->col_ptr % 8 ) ) 
      if ( ! ( this->row_ptr = ++this->row_ptr % 8 ) ) {
        this->last_read_time = micros();
        readMatrix( row_max, col_max );          
        this->last_read_time = micros() - this->last_read_time;
        return 0;
      }
  }
}  
