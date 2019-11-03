#pragma once

#include <arduino.h>
#include "IO_Adafruit_NeoPixel.hpp"

#define PULSE_WIDTH_USEC 0
#define NUM_IOPORTS 5
#define MATRIX_NODES 8
#define MATRIX_SWITCHES 8
#define MAX_LEDS_PER_STRIP 256
#define IO_TICK_MS 100UL

#ifdef CORE_TEENSY
  #define IOREAD( _pin ) digitalReadFast( _pin )
  #define IOWRITE( _pin, _value ) digitalWriteFast( _pin, _value )
  #define INPUT_MODE INPUT_PULLDOWN
#else
  #define IOREAD( _pin ) digitalRead( _pin )
  #define IOWRITE( _pin, _value ) digitalWrite( _pin, _value )
  #define INPUT_MODE INPUT
#endif

// Converts multiple physical strips into one logical strip
// Keeps track of the highest changed pixel in every strip
// Show is limited up to the highest touched pixel

class IO {

  struct logical_led {
    uint8_t strip;
    uint8_t led;
  };

  struct switch_record { 
    uint8_t state; 
    uint32_t timer;
    uint8_t throttle_state;
    uint32_t throttle_timer;
    uint32_t press_micros; // press must be stable for this period to register (if not: unscan event)
    uint32_t release_micros; // minimum wait period after registered press for a break (if not: unscan event)
    uint32_t throttle_millis; // after a registered press/release wait this long before allowing a new press/release
    uint32_t separate_millis; // don't allow this event too soon after another
  };
 
  protected:
#ifdef TRACE_SWITCH
    uint32_t trace_micros;
#endif
    uint8_t pin_clock; 
    uint8_t pin_latch; 
    uint8_t *gate;
    uint8_t *inputs;
    uint8_t *address;
    uint8_t io_ptr = 0;     // 5 IO strings per controller
    uint8_t node_ptr = 0;   // Max 8 matrix nodes within an IO string
    uint8_t switch_ptr = 0; // Max 8 wwitches connected to a matrix node 
    uint8_t switch_max = MATRIX_SWITCHES;
    uint8_t switch_map = 0;
    uint32_t last_read_time = 0;
    uint8_t last_strip = 0;
    uint8_t selected = 0; 
    IO_Adafruit_NeoPixel *led_strip[NUM_IOPORTS];
    logical_led led[NUM_IOPORTS * MAX_LEDS_PER_STRIP];
    int16_t led_dirty[NUM_IOPORTS];
    uint16_t led_cnt = 0;
    uint16_t switch_cnt = 0;
    int16_t log_last_pixel = -1;        
    uint8_t soll[MATRIX_NODES][MATRIX_SWITCHES];
    uint8_t sub[MATRIX_NODES][MATRIX_SWITCHES];
    uint8_t input_filter[MATRIX_NODES][MATRIX_SWITCHES];
    uint8_t ist[MATRIX_NODES][MATRIX_SWITCHES];
    uint8_t nc[MATRIX_NODES][MATRIX_SWITCHES];
    uint8_t row_map[NUM_IOPORTS];
    uint8_t node_max; 
    IO& readRow( uint8_t row, uint8_t mx_width = MATRIX_SWITCHES );
    IO& readMatrix( uint8_t mx_depth, uint8_t mx_width, bool init = false );
    uint16_t decimal_encode( uint8_t row, uint8_t col, uint8_t bus );    
    IO& select( int row, bool latch = false );
    switch_record profile[NUM_IOPORTS * MATRIX_NODES * MATRIX_SWITCHES + 1];
    uint32_t last_press;
    
  public:
    IO& begin( int pin_clock, int pin_latch, uint8_t *address, uint8_t *inputs, uint8_t *gate );
    IO& range( uint8_t row_max, uint8_t col_max ); // obsolete?
    IO& addStrips( IO_Adafruit_NeoPixel strip[] );
    IO& addStrip( IO_Adafruit_NeoPixel *strip );
    IO& setPixelColor( int16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w );
    IO& switchMap( uint8_t r1, uint8_t r2 = 0, uint8_t r3 = 0, uint8_t r4 = 0, uint8_t r5 = 0 );
    static uint32_t Color( uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0 ); 
    uint16_t numberOfLeds( void );
    uint16_t numberOfSwitches( void );
    int16_t lastPixel( void ); // Last pixel set
    bool show();
    uint16_t isPressed( int16_t code );
    int16_t scan_raw(); 
    int16_t debounce( int16_t code );
    int16_t throttle( int16_t code );
    int16_t separate( int16_t code );
    int16_t scan( void );
    IO& subscribe( void );
    IO& unsubscribe( void );
    uint32_t timer(); // Last time in usec needed for matrix read
    IO& retrigger(); // Makes all buttons in a pressed state trigger a keypress event
    IO& invert( uint8_t code );
    IO& invert( uint8_t c1, uint8_t c2, uint8_t c3 = 0, uint8_t c4 = 0, uint8_t c5 = 0, uint8_t c6 = 0, uint8_t c7 = 0, uint8_t c8 = 0 );
    IO& debounce( int16_t n, uint16_t press_ticks, uint16_t release_ticks, uint16_t throttle_ticks, uint16_t separate_ticks );
    IO& debounce( uint16_t press_ticks, uint16_t release_ticks, uint16_t throttle_ticks, uint16_t separate_ticks );

};
