#include <Automaton.h>
#include "Atm_zone.hpp"
#include "Atm_apa102.hpp"
#include "Atm_element.hpp"
#include "Atm_widget_oxo.hpp"

Atm_zone playfield;
Atm_apa102 led_strip_pf, led_strip_bb, led_strip_cb, led_strip_oxo;
Atm_widget_oxo oxo;
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
    OXO_LED_1_OA, OXO_LED_1_X, OXO_LED_1_OB, OXO_LED_2_OA, OXO_LED_2_X, OXO_LED_2_OB, OXO_LED_3_OA, OXO_LED_3_X, OXO_LED_3_OB,
    OXO_LED_4_OA, OXO_LED_4_X, OXO_LED_4_OB, OXO_LED_5_OA, OXO_LED_5_X, OXO_LED_5_OB, OXO_LED_6_OA, OXO_LED_6_X, OXO_LED_6_OB,
    OXO_LED_7_OA, OXO_LED_7_X, OXO_LED_7_OB, OXO_LED_8_OA, OXO_LED_8_X, OXO_LED_8_OB, OXO_LED_9_OA, OXO_LED_9_X, OXO_LED_9_OB,
    OXO_LED_X, OXO_LED_O }; 

  int8_t oxo_leds[] =  { // Leds in OXO game order
    OXO_LED_1_OA, OXO_LED_1_X, OXO_LED_1_OB,   OXO_LED_2_OA, OXO_LED_2_X, OXO_LED_2_OB,   OXO_LED_3_OA, OXO_LED_3_X, OXO_LED_3_OB,
    OXO_LED_4_OA, OXO_LED_4_X, OXO_LED_4_OB,   OXO_LED_5_OA, OXO_LED_5_X, OXO_LED_5_OB,   OXO_LED_6_OA, OXO_LED_6_X, OXO_LED_6_OB,
    OXO_LED_7_OA, OXO_LED_7_X, OXO_LED_7_OB,   OXO_LED_8_OA, OXO_LED_8_X, OXO_LED_8_OB,   OXO_LED_9_OA, OXO_LED_9_X, OXO_LED_9_OB,
    OXO_LED_X, OXO_LED_O };  


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

  playfield.begin( led_strip_pf, cols, rows, 4, 4 )
    .onPress( 0, oxo, Atm_widget_oxo::EVT_1X )
    .onPress( 1, oxo, Atm_widget_oxo::EVT_1O )
    .onPress( 2, oxo, Atm_widget_oxo::EVT_2X )
    .onPress( 3, oxo, Atm_widget_oxo::EVT_2O )
    .onPress( 4, oxo, Atm_widget_oxo::EVT_3X )
    .onPress( 5, oxo, Atm_widget_oxo::EVT_3O )
    .onPress( 15, oxo, Atm_widget_oxo::EVT_INIT );

  oxo.begin( led_strip_oxo.begin( 10, SPI_11_13 ), oxo_leds )
    .onMatch( [] ( int idx, int v, int up ) {
      led_strip_oxo.on( 9 );
      Serial.println( "You win!" );
      oxo.dump( Serial );
    } )
    .onSet( [] ( int idx, int v, int up ) {
      Serial.print( "Set cell " );
      Serial.print( v );
      Serial.print( " to " );
      Serial.println( up );
      oxo.dump( Serial );
    });

  oxo.trace( Serial );
}

uint32_t cnt = 0;

void loop() {
  automaton.run();
//  if ( ++cnt % 1000 == 0 ) Serial.println( millis() ); // 10 cycles/millisecond, 100 usec latency per matrix_row
}

