#pragma once

#include <arduino.h>
#include <Adafruit_NeoPixel.h>

#define PULSE_WIDTH_USEC 0
#define NUM_LED_STRIPS 5
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
// Show is limited to one strip (lowest id) and up to the highest touched pixel (Adafruit_Neopixel patch necessary, move numBytes, woffset & roffset to public)

class IO {
//  private:
  public:
    uint8_t pin_data; 
    uint8_t pin_clock; 
    uint8_t pin_pl; 
    uint8_t *gate;
    uint8_t *inp;
    uint8_t *addr;
    uint8_t row_ptr = 0;
    uint8_t col_ptr = 0;
    uint8_t col_max = 8;
    uint32_t last_read_time = 0;
    uint8_t selected = 0; 
    Adafruit_NeoPixel *led_strip[NUM_LED_STRIPS];
    logical_led log_led[NUM_LED_STRIPS * MAX_LEDS_PER_STRIP];
    int16_t led_dirty[NUM_LED_STRIPS];
    uint8_t led_bytes[NUM_LED_STRIPS];
    uint16_t log_led_cnt = 0;
    int16_t log_last_pixel = -1;        
    uint8_t soll[8][8];
    uint8_t ist[8][8];
    uint8_t row_map[5];
    uint8_t row_max;
    IO& readRow( uint8_t row, uint8_t mx_width = 8 );
    IO& readMatrix( uint8_t mx_depth, uint8_t mx_width, bool init = false );
    uint16_t decimal_encode( uint8_t row, uint8_t col, uint8_t bus );    
   
  public:
    IO& begin( int pin_data, int pin_clock, int pin_pl, uint8_t *addr, uint8_t *inp, uint8_t *gate );
    IO& range( uint8_t row_max, uint8_t col_max );
    IO& select( int row, bool latch = false );
    IO& strip( uint8_t n, Adafruit_NeoPixel &s, uint8_t bytes_per_pixel );
    IO& setPixelColor( uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w );
    IO& setPixelMono( uint16_t n, uint8_t w );
    IO& map( uint8_t r1, uint8_t r2 = 0, uint8_t r3 = 0, uint8_t r4 = 0, uint8_t r5 = 0 );
    static uint32_t Color( uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0 ); 
    static uint32_t Mono( uint8_t w ); 
    uint16_t numPixels( void );
    int16_t lastPixel( void ); // Last pixel set
    bool show();

    uint16_t isPressed( int16_t code );
    int16_t scan(); 
    uint32_t timer(); // Last time in usec needed for matrix read
    IO& retrigger(); // Makes all buttons in a pressed state trigger a keypress event
    
};
