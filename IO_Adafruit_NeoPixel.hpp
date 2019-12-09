#pragma once

#include <Adafruit_NeoPixel.h>

class IO_Adafruit_NeoPixel : public Adafruit_NeoPixel {

public:

struct t_neo_symbol { /* TODO two-way symbol lookup table */
  char symbol[20];
  uint16_t v;
} symbol[4] = { /* Commands must be entered lowercase */
  { "NEO_RBG", NEO_RBG },
  { "NEO_GRB", NEO_GRB },
  { "NEO_GBR", NEO_GBR },
  { "", 0 },
};

  IO_Adafruit_NeoPixel( uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800 ) : Adafruit_NeoPixel( n, p, t ){};

  static uint16_t str2int( const char s[] ) {
    if ( strcasecmp( s, "NEO_RBG" ) == 0 ) return NEO_RBG; 
    if ( strcasecmp( s, "NEO_GRB" ) == 0 ) return NEO_GRB; 
    if ( strcasecmp( s, "NEO_GBR" ) == 0 ) return NEO_GBR; 
    if ( strcasecmp( s, "NEO_BRG" ) == 0 ) return NEO_BRG; 
    if ( strcasecmp( s, "NEO_BGR" ) == 0 ) return NEO_BGR; 
    if ( strcasecmp( s, "NEO_WRGB" ) == 0 ) return NEO_WRGB; 
    if ( strcasecmp( s, "NEO_WRBG" ) == 0 ) return NEO_WRBG; 
    if ( strcasecmp( s, "NEO_WGRB" ) == 0 ) return NEO_WGRB; 
    if ( strcasecmp( s, "NEO_WGBR" ) == 0 ) return NEO_WGBR; 
    if ( strcasecmp( s, "NEO_WBRG" ) == 0 ) return NEO_WBRG; 
    if ( strcasecmp( s, "NEO_WBGR" ) == 0 ) return NEO_WBGR; 
    if ( strcasecmp( s, "NEO_RWGB" ) == 0 ) return NEO_RWGB; 
    if ( strcasecmp( s, "NEO_RWBG" ) == 0 ) return NEO_RWBG; 
    if ( strcasecmp( s, "NEO_RGWB" ) == 0 ) return NEO_RGWB; 
    if ( strcasecmp( s, "NEO_RGBW" ) == 0 ) return NEO_RGBW; 
    if ( strcasecmp( s, "NEO_RBWG" ) == 0 ) return NEO_RBWG; 
    if ( strcasecmp( s, "NEO_RBGW" ) == 0 ) return NEO_RBGW; 
    if ( strcasecmp( s, "NEO_GWRB" ) == 0 ) return NEO_GWRB; 
    if ( strcasecmp( s, "NEO_GWBR" ) == 0 ) return NEO_GWBR; 
    if ( strcasecmp( s, "NEO_GRWB" ) == 0 ) return NEO_GRWB; 
    if ( strcasecmp( s, "NEO_GRBW" ) == 0 ) return NEO_GRBW; 
    if ( strcasecmp( s, "NEO_GBWR" ) == 0 ) return NEO_GBWR; 
    if ( strcasecmp( s, "NEO_GBRW" ) == 0 ) return NEO_GBRW; 
    if ( strcasecmp( s, "NEO_BWRG" ) == 0 ) return NEO_BWRG; 
    if ( strcasecmp( s, "NEO_BWGR" ) == 0 ) return NEO_BWGR; 
    if ( strcasecmp( s, "NEO_BRWG" ) == 0 ) return NEO_BRWG; 
    if ( strcasecmp( s, "NEO_BRGW" ) == 0 ) return NEO_BRGW; 
    if ( strcasecmp( s, "NEO_BGWR" ) == 0 ) return NEO_BGWR; 
    if ( strcasecmp( s, "NEO_BGRW" ) == 0 ) return NEO_BGRW; 
    if ( strcasecmp( s, "NEO_KHZ800" ) == 0 ) return NEO_KHZ800;
    if ( strcasecmp( s, "NEO_KHZ400" ) == 0 ) return NEO_KHZ400;
    return 0;  
  }

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
