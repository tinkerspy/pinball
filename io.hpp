#pragma once

#include <arduino.h>
#include "IO_Adafruit_NeoPixel.hpp"

#define PULSE_WIDTH_USEC 0
#define NUM_IOPORTS 5
#define MATRIX_ROWS 8
#define MATRIX_COLS 8
#define MAX_LEDS_PER_STRIP 256

#ifdef CORE_TEENSY
  #define IOREAD( _pin ) digitalReadFast( _pin )
  #define IOWRITE( _pin, _value ) digitalWriteFast( _pin, _value )
  #define INPUT_MODE INPUT_PULLDOWN
#else
  #define IOREAD( _pin ) digitalRead( _pin )
  #define IOWRITE( _pin, _value ) digitalWrite( _pin, _value )
  #define INPUT_MODE INPUT
#endif

struct logical_led {
    uint8_t strip;
    uint8_t led;
};


// Converts multiple physical strips into one logical strip
// Keeps track of the highest changed pixel in every strip
// Show is limited up to the highest touched pixel

class IO {
  protected:
    uint8_t pin_clock; 
    uint8_t pin_latch; 
    uint8_t *gate;
    uint8_t *inputs;
    uint8_t *address;
    uint8_t row_ptr = 0;
    uint8_t col_ptr = 0;
    uint8_t col_max = MATRIX_COLS;
    uint32_t last_read_time = 0;
    uint8_t last_strip = 0;
    uint8_t selected = 0; 
    IO_Adafruit_NeoPixel *led_strip[NUM_IOPORTS];
    logical_led led[NUM_IOPORTS * MAX_LEDS_PER_STRIP];
    int16_t led_dirty[NUM_IOPORTS];
    uint16_t led_cnt = 0;
    int16_t log_last_pixel = -1;        
    uint8_t soll[MATRIX_ROWS][MATRIX_COLS];
    uint8_t ist[MATRIX_ROWS][MATRIX_COLS];
    uint8_t row_map[NUM_IOPORTS];
    uint8_t row_max;
    IO& readRow( uint8_t row, uint8_t mx_width = MATRIX_COLS );
    IO& readMatrix( uint8_t mx_depth, uint8_t mx_width, bool init = false );
    uint16_t decimal_encode( uint8_t row, uint8_t col, uint8_t bus );    
    IO& select( int row, bool latch = false );
   
  public:
    IO& begin( int pin_clock, int pin_latch, uint8_t *address, uint8_t *inputs, uint8_t *gate );
    IO& range( uint8_t row_max, uint8_t col_max ); // obsolete?
    IO& addStrips( IO_Adafruit_NeoPixel strip[] );
    IO& addStrip( IO_Adafruit_NeoPixel *strip );
    IO& setPixelColor( int16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w );
    IO& switchMap( uint8_t r1, uint8_t r2 = 0, uint8_t r3 = 0, uint8_t r4 = 0, uint8_t r5 = 0 );
    static uint32_t Color( uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0 ); 
    uint16_t numPixels( void );
    int16_t lastPixel( void ); // Last pixel set
    bool show();
    uint16_t isPressed( int16_t code );
    int16_t scan(); 
    uint32_t timer(); // Last time in usec needed for matrix read
    IO& retrigger(); // Makes all buttons in a pressed state trigger a keypress event
    
};
