#include <Automaton.h>
#include "IO.hpp"
#include "Atm_playfield.hpp"
#include "Atm_led_scheduler.hpp"
#include "IO_Adafruit_NeoPixel.hpp"
#include "freeram.hpp"
#include "mapping.hpp"

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;

void setup() {
  delay( 1000 );
  Serial.println( "start" );
  delay( 100 );
  
  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 2 )
    .addStrip( new IO_Adafruit_NeoPixel( 25, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // SK6812
    .invert( ROLLOVER, BALL_ENTER )
    .retrigger()
    .show();

  leds.begin( io )
    .defineProfile( PROFILE_COIL, 0, 255, 30 ) // T0, L1, T1, L2
    .defineProfile( PROFILE_LED, 0, 0, 0, 127 )
    .defineProfile( PROFILE_FLIPPER, 0, 255, 50, 20 )
    .defineProfile( PROFILE_KICKER, 1000, 127, 30 )
    .defineProfile( PROFILE_GATE, 0, 255, 30, 255 )
    .defineProfile( PROFILE_BUMPER, 0, 255, 30 )
    .defineProfile( PROFILE_FEEDER, 1000, 127, 30 );

  // IDEA: embed led scheduler in playfield...
  // NOPE: because the oxo machine will need access to the leds object as well

  playfield.begin( io, leds ).debounce( 20, 20 );

  // Playfield element instantiation

  playfield.element( TARGET_A, -1, LED_TARGET_A );
  playfield.element( TARGET_B, -1, LED_TARGET_B );
  playfield.element( BUMPER_A, COIL_BUMPER_A, LED_BUMPER_A, PROFILE_BUMPER ); 
  playfield.element( BUMPER_B, COIL_BUMPER_B, LED_BUMPER_B, PROFILE_BUMPER );
  playfield.element( BUMPER_C, COIL_BUMPER_C, LED_BUMPER_C, PROFILE_BUMPER );
  playfield.element( KICKER_L, COIL_KICKER_L, LED_KICKER_L, PROFILE_KICKER );
  playfield.element( KICKER_R, COIL_KICKER_R, LED_KICKER_R, PROFILE_KICKER );
  playfield.element( SLING_L, COIL_SLING_L, -1 );
  playfield.element( SLING_R, COIL_SLING_R, -1 );
  playfield.element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER );
  playfield.element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER );
  playfield.element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );  
  playfield.element( BALL_EXIT, COIL_BALL_FEEDER, -1, PROFILE_FEEDER );

  // Special debounce rules 
  
  playfield.debounce( FLIPPER_L, 5, 0 );  
  playfield.debounce( FLIPPER_R, 5, 0 );  
  playfield.debounce( SLING_L, 20, 200 );
  playfield.debounce( SLING_R, 20, 200 );

  // TARGET -> BUMPER -> GATE logic

  playfield.element( TARGET_A )
    .autoLight()
    .onLight( true, playfield.element( BUMPER_A ), Atm_element::EVT_ON ); 
    
  playfield.element( TARGET_B )
    .autoLight()
    .onLight( true, playfield.element( BUMPER_B ), Atm_element::EVT_ON ); 

  playfield.element( BUMPER_A )
    .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_INPUT );

  playfield.element( BUMPER_B )
    .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_INPUT );

  playfield.element( BUMPER_C )
    .onInput( [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_A ).state() && playfield.element( BUMPER_B ).state() ) {
        playfield.leds().on( LED_BUMPER_C );
        playfield.element( SAVE_GATE ).kick();
      }
    });
    
  playfield
    .onPress( BALL_ENTER, [] ( int idx, int v, int up ) { 
      if ( playfield.element( BALL_EXIT ).idle( 2000 ) || playfield.element( OUT_LANE ).idle( 2000 ) ) {
        Serial.println( "Clear bumper A, B & C and close gate" );
        playfield.element( TARGET_A ).off();
        playfield.element( TARGET_B ).off();
        playfield.element( BUMPER_A ).off();
        playfield.element( BUMPER_B ).off();
        playfield.element( BUMPER_C ).off();
        playfield.element( SAVE_GATE ).release();
      }      
    });
    
  // end of logic

  Serial.println( FreeRam() );
}

void loop() {  
  automaton.run();
}
