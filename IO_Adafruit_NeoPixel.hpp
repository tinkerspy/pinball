#pragma once

#include <Adafruit_NeoPixel.h>

class IO_Adafruit_NeoPixel : public Adafruit_NeoPixel {

public:
  IO_Adafruit_NeoPixel( uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800 ) : Adafruit_NeoPixel( n, p, t ){};

  uint8_t bytesPerPixel() {
    return ( wOffset == rOffset ) ? 3 : 4;
  }

  void show( int n = 0 ) { 
    if ( n == 0 ) {
      Adafruit_NeoPixel::show(); 
    } else {
      uint32_t saveBytes = numBytes; 
      numBytes = n * bytesPerPixel();
      Adafruit_NeoPixel::show();
      numBytes = saveBytes;
    }
  }


};
