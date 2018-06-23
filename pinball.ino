#include <Automaton.h>
#include "Atm_zone.hpp"
#include "Atm_apa102.hpp"
#include "Atm_element.hpp"
#include "Atm_widget_oxo.hpp"

Atm_zone playfield;
Atm_apa102 led_strip_pf, led_strip_bb, led_strip_cb, led_strip_oxo;
Atm_widget_oxo oxo;
Atm_led led;
Atm_timer timer;

int8_t rows[] = { 20, 21, 22, 23 };
int8_t cols[] = { 16, 17, 18, 19 };

enum { LEFT_FLIPPER  = 0 * 4, LEFT_BUMPER, LITE_BUMPER };
enum { RIGHT_FLIPPER = 1 * 4, BUMPER_100, BUMPER_1000, MY_TEST2 };
enum { LEVEL_05 = 3 * 4, LEVEL_50, LEVEL_100, LEVEL_255 };

#define LEFT_BUMPER_LED 8
#define LEFT_BUMPER_COIL 9
#define LEFT_FLIPPER_COIL 5
#define RIGHT_FLIPPER_COIL 6

  enum { // Leds in led strip order
    OXO_LED_9_OA, OXO_LED_9_X, OXO_LED_9_OB,
    OXO_LED_8_OA, OXO_LED_8_X, OXO_LED_8_OB, 
    OXO_LED_7_OA, OXO_LED_7_X, OXO_LED_7_OB, 
    
    OXO_LED_4_OA, OXO_LED_4_X, OXO_LED_4_OB, 
    OXO_LED_5_OA, OXO_LED_5_X, OXO_LED_5_OB, 
    OXO_LED_6_OA, OXO_LED_6_X, OXO_LED_6_OB,
    
    OXO_LED_3_OA, OXO_LED_3_X, OXO_LED_3_OB,
    OXO_LED_2_OA, OXO_LED_2_X, OXO_LED_2_OB, 
    OXO_LED_1_OA, OXO_LED_1_X, OXO_LED_1_OB, 
    OXO_LED_X, OXO_LED_O }; 

  int8_t oxo_leds[] =  { // Leds in OXO game order
    OXO_LED_1_OA, OXO_LED_1_X, OXO_LED_1_OB,   OXO_LED_2_OA, OXO_LED_2_X, OXO_LED_2_OB,   OXO_LED_3_OA, OXO_LED_3_X, OXO_LED_3_OB,
    OXO_LED_4_OA, OXO_LED_4_X, OXO_LED_4_OB,   OXO_LED_5_OA, OXO_LED_5_X, OXO_LED_5_OB,   OXO_LED_6_OA, OXO_LED_6_X, OXO_LED_6_OB,
    OXO_LED_7_OA, OXO_LED_7_X, OXO_LED_7_OB,   OXO_LED_8_OA, OXO_LED_8_X, OXO_LED_8_OB,   OXO_LED_9_OA, OXO_LED_9_X, OXO_LED_9_OB,
    OXO_LED_X, OXO_LED_O };  


/*
sub check {
 my $piece = shift; # X or O
 my $move  = shift; # 1 through 9
 return 0 if $piece !~ /^[XO]$/ or $move !~ /^[1-9]$/;
 return grep { /$piece/ && /$move/ && /\d${piece}?\d/ } @wins;
}
*/

// OFF, ON, RANDOM, BOT, AI

int finished( oxo_wins_t wins, uint16_t available ) {
  if ( !available ) return 0;
  for ( int w = 0; w < 8; w++ ) 
    if ( wins[w][0] > 9 && wins[w][0] == wins[w][1] && wins[w][1] == wins[w][2] ) {
      Serial.println( "Finished!" );
      return 1; 
    }
  return 0;
}

int random_bot( oxo_wins_t &wins, uint16_t available ) {
  
  int cnt = 0;
  int mem[9];
  for ( int c = 1; c < 10; c++ )
    if ( available & ( 1 << c ) )
      mem[cnt++] = c;
  if ( cnt == 0 ) return 0;
  int v = random( 1, cnt ); // Checkme!
  return mem[v-1];
}

// random_bot, stupid_bot, smart_bot

int stupid_bot( oxo_wins_t &wins, uint16_t available ) {

  if ( finished( wins, available ) ) return 0; // Give up when all is lost

  if ( available & ( 1 << 5 ) ) return 5; // If cell 5 is free, take it

  // Try to find a winning square for the bot
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      for ( int w = 0; w < 8; w++ ) {
        bool success = true;
        for ( int p = 0; p < 3; p++ ) {
          if ( wins[w][p] != 'X' && wins[w][p] != c ) success = false; 
        }
        if ( success ) {
          Serial.println( "Winning move" );
          return c;
        }
      }
    }
  }
  // Try to find a winning square for the opponent, then block it
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      for ( int w = 0; w < 8; w++ ) {
        bool success = true;
        for ( int p = 0; p < 3; p++ ) {
          if ( wins[w][p] != 'O' && wins[w][p] != c ) success = false; 
        }
        if ( success ) {
          Serial.println( "Blocking move" );
          return c;
        }
      }
    }
  }
  // Fall back to random bot 
  return random_bot( wins, available );
}

int bitCount( uint16_t v ) {
  int r = 0;
  while ( v )  {
    if ( v & 1 ) r++;
    v >>= 1;
  }
  return r;
}

uint16_t checkWins( oxo_wins_t wins, char piece, int move ) {

  uint16_t r = 0;
  for ( int w = 0; w < 8; w++ ) {
    bool mine = false;
    bool his = false;
    for ( int p = 0; p < 3; p++ ) {
      if ( wins[w][p] == piece ) mine = true;
      if ( wins[w][p] > 9 && wins[w][p] != piece ) his = true;
    }
    if ( mine && !his ) r |= 1 << w;
  }
  Serial.print( "checkWins( " );
  Serial.print( piece );
  Serial.print( ", " );
  Serial.print( move );
  Serial.print( " ): " );
  Serial.println( r, BIN );
  return r;
}

int smart_bot( oxo_wins_t &wins, uint16_t available ) {

  if ( finished( wins, available ) ) return 0; // Give up when all is lost

  if ( available & ( 1 << 5 ) ) return 5; // If cell 5 is free, take it

  // Try to find a winning square for the bot
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      for ( int w = 0; w < 8; w++ ) {
        bool success = true;
        for ( int p = 0; p < 3; p++ ) {
          if ( wins[w][p] != 'X' && wins[w][p] != c ) success = false; 
        }
        if ( success ) {
          Serial.println( "Winning move" );
          return c;
        }
      }
    }
  }
  Serial.println( "No winning moves" );
  // Try to find a winning square for the opponent, then block it
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      for ( int w = 0; w < 8; w++ ) {
        bool success = true;
        for ( int p = 0; p < 3; p++ ) {
          if ( wins[w][p] != 'O' && wins[w][p] != c ) success = false; 
        }
        if ( success ) {
          Serial.println( "Blocking move" );
          return c;
        }
      }
    }
  }
  Serial.println( "No blocking moves" );
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      if ( bitCount( checkWins( wins, 'X', c ) ) > 1 ) {
        Serial.println( "Forking move for X" );
        return c;
      }
    }
  }
  Serial.println( "No forks for me, fallback to random" );
  // Fall back to random bot 
  return random_bot( wins, available );
}

void setup() {
  randomSeed( analogRead( 9 ) );

  timer.begin( 1000 )
    .onTimer( [] ( int idx, int v, int up ) {
        oxo_wins_t oxo_wins;
        uint16_t available = oxo.loadWins( oxo_wins );
        int m = smart_bot( oxo_wins, available );
        Serial.print( "Bot move " );
        Serial.println( m );
        if ( m ) oxo.set( m, 'X' );
        oxo.dump( Serial );
        available= oxo.loadWins( oxo_wins );
        oxo.dumpWins( Serial, oxo_wins );
        Serial.println( available, BIN );
    });
  
  Serial.begin( 9600 );
  //playfield.trace( Serial );
  delay( 1000 );

  Serial.print( "Size of playfield obj: " );
  Serial.println( sizeof( playfield ) );
  Serial.println( "=======================================================================\n");

  for ( int i = 0; i < 10; i++ ) {
    Serial.print( "Test " );
    Serial.println( i );
    delay( 100 );
  }

  led.begin( 2 );

  playfield.begin( led_strip_pf, cols, rows, 4, 4 )
//    .onPress(  0, oxo, Atm_widget_oxo::EVT_1X )
//    .onPress(  1, oxo, Atm_widget_oxo::EVT_2X )
//    .onPress(  2, oxo, Atm_widget_oxo::EVT_3X )
    .onPress(  4, oxo, Atm_widget_oxo::EVT_1O )
    .onPress(  5, oxo, Atm_widget_oxo::EVT_2O )
    .onPress(  6, oxo, Atm_widget_oxo::EVT_3O )
    .onPress(  8, oxo, Atm_widget_oxo::EVT_4 )
    .onPress(  9, oxo, Atm_widget_oxo::EVT_5 )
    .onPress( 10, oxo, Atm_widget_oxo::EVT_6 )
    .onPress( 12, oxo, Atm_widget_oxo::EVT_7 )
    .onPress( 13, oxo, Atm_widget_oxo::EVT_8 )
    .onPress( 14, oxo, Atm_widget_oxo::EVT_9 )
    .onPress( 15, oxo, Atm_widget_oxo::EVT_INIT )
    .onPress( 11, oxo, Atm_widget_oxo::EVT_TOGGLE );
    
  led_strip_oxo.begin( 29, SPI_11_13 ).rgb( 0xffffff );

  oxo.begin( led_strip_oxo, oxo_leds ).select( 'O' )
    .onInit(  [] ( int idx, int v, int up ) {
      led.off();
      oxo.dump( Serial );
      oxo_wins_t oxo_wins;
      uint16_t available = oxo.loadWins( oxo_wins );
      oxo.dumpWins( Serial, oxo_wins );
      Serial.println( available, BIN );
    })
    .onMatch( led, led.EVT_ON )
    .onSet( [] ( int idx, int v, int up ) {
      oxo.dump( Serial );
      oxo_wins_t oxo_wins;
      uint16_t available = oxo.loadWins( oxo_wins );
      oxo.dumpWins( Serial, oxo_wins );
      Serial.println( available, BIN );
      if ( up == 0 ) {
        timer.start();
      }
    });

  //oxo.trace( Serial );
}

uint32_t cnt = 0;

void loop() {
  automaton.run();
//  if ( ++cnt % 1000 == 0 ) Serial.println( millis() ); // 10 cycles/millisecond, 100 usec latency per matrix_row
}

