#include <Automaton.h>
#include "IO.hpp"
#include "Atm_playfield.hpp"
#include "Atm_led_scheduler.hpp"
#include "IO_Adafruit_NeoPixel.hpp"
#include "freeram.hpp"
#include "mapping.hpp"

// TODO: machine gun protection, (done: retrigger 500);
// switch inversion (done: TEST)
// TARGET_A -> BUMPER_A -> BUMPER_C doesn't work!
// Latency (caused by serial/debugging output?, measure with logic analyzer)
// Flippers latch (measure ref voltage while stuck, possibly add capacitor)

// Er is een commit met meer recente wijzigingen, maar die heeft problemen met de led HOLD state.

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
    .defineProfile( PROFILE_GATE, 0, 255, 30, 50 )
    .defineProfile( PROFILE_BUMPER, 0, 255, 30 )
    .defineProfile( PROFILE_FEEDER, 1000, 127, 30 );

  playfield.begin( io, leds ) // IDEA: embed led scheduler in playfield...
    .debounce( 20, 100 )
    .debounce( FLIPPER_R, 10, 0 ) 
    .debounce( FLIPPER_L, 10, 0 );

  // Playfield element instantiation

  playfield.element( TARGET_A, -1, LED_TARGET_A ).autoLight();
  playfield.element( TARGET_B, -1, LED_TARGET_B ).autoLight();
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

  // playfield.solo( 5, BUMPER_A, BUMPER_B, BUMPER_C ); // action on one element blocks the others in the chain for 5 ms

  // TARGET -> BUMPER -> GATE logic
  
  playfield.element( BUMPER_C  )
    .onLight( true, playfield.element( SAVE_GATE ), Atm_element::EVT_KICK );

  playfield.element( BUMPER_A )
    .onLight( true, [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_B ).state() ) {
        Serial.println( "Trigger bumper C" );
        playfield.element( BUMPER_C ).on();
      }
    });

  playfield.element( BUMPER_B )
    .onLight( true, [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_A ).state() ) {
        Serial.println( "Trigger bumper C" );
        playfield.element( BUMPER_C ).on();
      }
    });

  playfield.element( TARGET_A )
    .onLight( true, [] ( int idx, int v, int up ) { 
      Serial.println( "Trigger bumper A" );
      playfield.element( BUMPER_A ).on();
    });
    
  playfield.element( TARGET_B )
    .onLight( true, [] ( int idx, int v, int up ) { 
      Serial.println( "Trigger bumper B" );
      playfield.element( BUMPER_B ).on();
    });
    
  playfield
    .onPress( BALL_ENTER, [] ( int idx, int v, int up ) { 
      if ( playfield.element( BALL_EXIT ).idle( 2000 ) || playfield.element( OUT_LANE ).idle( 2000 ) ) {
        Serial.println( "Clear bumper A & B and close gate" );
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
