#include <arduino.h>

#define PULSE_WIDTH_USEC 0

class IO {
  private:
    uint8_t pin_data; 
    uint8_t pin_clock; 
    uint8_t pin_pl; 
    uint8_t *gate;
    uint8_t *inp;
    uint8_t *addr;
    uint8_t row_ptr = 0;
    uint8_t col_ptr = 0;
    uint8_t row_max = 8;
    uint8_t col_max = 8;
    uint32_t last_read_time = 0;
        
    uint8_t soll[8][8];
    uint8_t ist[8][8];
    IO& readRow( uint8_t row, uint8_t mx_width = 8 );
    IO& readMatrix( uint8_t mx_depth, uint8_t mx_width, bool init = false );
    uint16_t decimal_encode( uint8_t row, uint8_t col, uint8_t bus );    
   
  public:
    IO& begin( int pin_data, int pin_clock, int pin_pl, uint8_t *addr, uint8_t *inp, uint8_t *gate );
    IO& range( uint8_t row_max, uint8_t col_max );
    IO& select( int row, bool latch = false );
    uint16_t isPressed( int16_t code );
    int16_t scan(); 
    uint32_t timer(); // Last time in usec needed for matrix read
    IO& retrigger(); // Makes all buttons in a pressed state trigger a keypress event
    
};
