#include <Automaton.h>
#include "Atm_zone.hpp"
#include "Atm_apa102.hpp"
#include "Atm_element.hpp"

Atm_zone playfield;
Atm_apa102 led;
Atm_timer timer;

int8_t rows[] = { 23, 22, 21, 20 };
int8_t cols[] = { 19, 18, 17, 16 };

enum { LEFT_FLIPPER  = 0 * 4, LEFT_BUMPER, LITE_BUMPER };
enum { RIGHT_FLIPPER = 1 * 4, BUMPER_100, BUMPER_1000, MY_TEST2 };
enum { LEVEL_05 = 3 * 4, LEVEL_50, LEVEL_100, LEVEL_255 };

#define LEFT_BUMPER_LIGHT 8
#define LEFT_BUMPER_COIL 9
#define LEFT_FLIPPER_COIL 5
#define RIGHT_FLIPPER_COIL 6

void setup() {
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
  
  led.begin( 10 );
  playfield.begin( led, rows, cols, 4, 4 );

  playfield.element( LEFT_BUMPER, LEFT_BUMPER_LIGHT, LEFT_BUMPER_COIL, 50 ) // optional pulse, lead, force arguments
    .onKick( [] ( int idx, int v, int up ) { // Creates duplicate callbacks
      Serial.print( millis() );
      Serial.println( " Left bumper kicked" );
    }).autoLite();
   playfield.element( LEFT_FLIPPER, -1, LEFT_FLIPPER_COIL ) // optional pulse, lead, force arguments
    .onKick( [] ( int idx, int v, int up ) { // Creates duplicate callbacks
      Serial.print( millis() );
      Serial.println( " Left flipper kicked" );
    });
   playfield.element( RIGHT_FLIPPER, -1, RIGHT_FLIPPER_COIL ) // optional pulse, lead, force arguments
    .onKick( [] ( int idx, int v, int up ) { // Creates duplicate callbacks
      Serial.print( millis() );
      Serial.println( " Right flipper kicked" );
    });
}

void loop() {
  automaton.run();
}

