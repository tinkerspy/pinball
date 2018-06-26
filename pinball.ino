#include <Automaton.h>
#include "Atm_zone.hpp"
#include "Atm_apa102.hpp"
#include "Atm_element.hpp"
#include "Atm_widget_oxo.hpp"

Atm_zone playfield;
Atm_apa102 led_strip_pf, led_strip_bb, led_strip_cb, led_strip_oxo;
Atm_widget_oxo oxo;
//Atm_bot_oxo bot;
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


// OFF, ON, RANDOM, BOT, AI

char winner( oxo_wins_t wins ) { // Returns O, X or 0
  for ( int w = 0; w < 8; w++ ) 
    if ( wins[w][0] > 9 && wins[w][0] == wins[w][1] && wins[w][1] == wins[w][2] ) {
      return wins[w][0]; 
    }
  return 0;
}

int randomBit( uint16_t v ) { // Selects a random 1 bit from the arg and returns its number (or -1 if arg = 0 )
  int cnt = 0;
  for ( int c = 0; c < 16; c++ )
    if ( v & ( 1 << c ) ) cnt++;
  int r = random( 0, cnt );
  for ( int c = 0; c < 16; c++ )
    if ( v & ( 1 << c ) )
      if ( !r-- ) return c;
  return -1;
}

uint16_t availableMoves( oxo_wins_t &wins ) { // Returns bitmap with free squares, 1 = bit1, 2 = bit2, etc. (1..9)
  uint16_t r = 0;
  for ( int w = 0; w < 3; w++ ) 
    for ( int p = 0; p < 3; p++ )   
      if ( wins[w][p] <= 9 ) r |= 1 << ( w * 3 + p + 1);
  return r;
}

/*
 * Remise: availableWins( 'O', wins ) == 0 && availableWins( 'X', wins ) == 0
 * Bot can't win: availableWins( 'O', wins ) == 0
 */

uint16_t availableWins( char piece, oxo_wins_t &wins ) { // Returns bitmap of possible wins (0..7) for piece
  uint16_t r = 0;
  for ( int w = 0; w < 3; w++ ) {
    bool winnable = true;
    for ( int p = 0; p < 3; p++ ) {  
      if ( wins[w][p] > 9 && wins[w][p] != piece ) winnable = false;
    }
    if ( winnable ) r |= 1 << w;
  }
  return r;
}

int bitCount( uint16_t v ) { // Returns the number of high bits in the argument
  int r = 0;
  while ( v )  {
    if ( v & 1 ) r++;
    v >>= 1;
  }
  return r;
}

int findWin( char piece, oxo_wins_t &wins ) {
  uint16_t available = availableMoves( wins );
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      for ( int w = 0; w < 8; w++ ) {
        bool success = true;
        for ( int p = 0; p < 3; p++ ) {
          if ( wins[w][p] != piece && wins[w][p] != c ) success = false; 
        }
        if ( success ) return c;
      }
    }
  }
  return 0;
}

uint16_t checkWins( oxo_wins_t wins, char piece, int move ) { // Returns bitmap of possible wins (0..8)

  uint16_t r = 0;
  for ( int w = 0; w < 8; w++ ) {
    bool his = false;
    bool mine = false;
    bool mover = false;
    for ( int p = 0; p < 3; p++ ) {
      if ( wins[w][p] == move ) mover = true;
      if ( wins[w][p] == piece ) mine = true;
      if ( wins[w][p] > 9 && wins[w][p] != piece ) his = true;
    }
    if ( mover && mine && !his ) r |= 1 << w;
  }
  return r;
}

int randomBot( char piece, oxo_wins_t &wins ) { // Makes a random valid move
  if ( winner( wins ) ) return 0; // Give up when all is lost
  int r = randomBit( availableMoves( wins ) );
  return r == -1 ? 0 : r;
}

int stupidBot( char piece, oxo_wins_t &wins ) { // Takes wins, blocks opponent wins 
  if ( winner( wins ) ) return 0; // Give up when all is lost
  uint16_t available = availableMoves( wins );
  if ( available & ( 1 << 5 ) ) return 5; // If cell 5 is free, take it
  char opp_piece = piece == 'X' ? 'O' : 'X'; 
  // Try to find a winning square for the bot
  int m = 0;
  if ( ( m = findWin( piece, wins ) ) ) return m;
  if ( ( m = findWin( opp_piece, wins ) ) ) return m;
  // Fall back to random  
  m = randomBit( available );
  return m > 0 ? m : 0;
}

// TODO: bot concedes defeat: no more wins possible

int smartBot( char piece, oxo_wins_t &wins ) { // Takes wins, blocks opponent wins, blocks opponent forks 
  if ( winner( wins ) ) return 0; // Give up when all is lost
  uint16_t available = availableMoves( wins );
  if ( available & ( 1 << 5 ) ) return 5; // If cell 5 is free, take it
  char opp_piece = piece == 'X' ? 'O' : 'X'; 
  int m = 0;
  // Try to find a winning square for the bot
  if ( ( m = findWin( piece, wins ) ) ) return m;
  if ( ( m = findWin( opp_piece, wins ) ) ) return m;
  Serial.println( "No blocking moves" );
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      Serial.printf( "Check wins for %d/%c: %d\n", 
        c, piece, bitCount( checkWins( wins, piece, c ) ) );
      if ( bitCount( checkWins( wins, piece, c ) ) > 1 ) {
        Serial.println( "Forking move for bot: " );
        m = c;
      }
    }
  }
  Serial.println( "No forks for bot" );
  if ( m ) return m;
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      uint16_t wmap = checkWins( wins, piece, c );
      for ( int w = 0; w < 8; w++ ) {
        if ( wmap & 1 << w ) {
          for ( int p = 0; p < 3; p++ ) {
            if ( wins[w][p] != piece && wins[w][p] != c ) {
              if ( bitCount( checkWins( wins, opp_piece, wins[w][p] ) ) < 2 ) {
                Serial.print( "Blocking fork for opponent: " );
                Serial.println( c );
                m = c;
              }
            }
          }
        }        
      }      
    }
  }  
  if ( m ) return m;
  Serial.println( "No opponent forks to block, fallback to random" );
  // Fall back to random
  if ( bitCount( available ) == 8 ) {
    m = randomBit( 0b1010001010 ); // Second move: only corners!
  } else {
    m = randomBit( available );
  }
  return m > 0 ? m : 0;
}

void setup() {
  randomSeed( analogRead( 9 ) );

  timer.begin( 500 )
    .onTimer( [] ( int idx, int v, int up ) {
        oxo_wins_t oxo_wins;
        Serial.println( "-------------------------" );
        oxo.loadWins( oxo_wins );
        Serial.print( "Possible wins: " );
        oxo.dumpWins( Serial, oxo_wins );
        Serial.print( "Available moves: " );        
        Serial.println( availableMoves( oxo_wins ), BIN );
        if ( availableMoves( oxo_wins ) ) {
          int m = smartBot( 'X', oxo_wins );
          Serial.print( "Bot move " );
          Serial.println( m );
          if ( m ) oxo.set( m, 'X' ); // Should use an event!
          oxo.dump( Serial );
        }
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
    })
    .onMatch( led, led.EVT_ON )
    .onSet( [] ( int idx, int v, int up ) {
      if ( up == 0 ) {
        Serial.print( "Player move: " );
        Serial.println( v );
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

