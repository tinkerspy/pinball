#include <Automaton.h>
#include "IO.hpp"
#include "Atm_playfield.hpp"
#include "Atm_led_scheduler.hpp"
#include "IO_Adafruit_NeoPixel.hpp"
#include "freeram.hpp"

uint8_t gate[5] = { 15, 22, 23, 16, 17 };
uint8_t shift_inputs[5] = { 2, 14, 7, 8, 6 };
uint8_t addr[3] = { 5, 21, 20 };

const int pin_data = 11;
const int pin_clock = 13;
const int pin_latch = 4;

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;

// Output profiles (leds, coils)

enum { PROFILE_COIL, PROFILE_LED, PROFILE_FLIPPER, PROFILE_KICKER, PROFILE_FEEDER, PROFILE_GATE };

// Coils & leds (outputs)

enum { 
  COIL_BALL_FEEDER, COIL_FLIPPER_R, COIL_SLING_R, COIL_SAVE_GATE, 
  COIL_KICKER_R, COIL_FLIPPER_L, COIL_SLING_L, COIL_KICKER_L,
  
  COIL_BUMPER_A, COIL_BUMPER_C, COIL_BUMPER_B, GI_PLAYFIELD,
  LED_LANE_UP_L, LED_KICKER_L, LED_KICKER_R, LED_LANE_UP_R,
  LED_BUMPER_C, LED_BUMPER_A, LED_BUMPER_B, LED_TARGET_A, 
  LED_TARGET_B, LED_SHOOTS_AGAIN, LED_TRIPLE_BONUS,
};

// Switches (inputs)

enum { 
  SWITCH_NULL,
  TARGET_B, PORT_3_X, PORT_3_O, PORT_2_X, PORT_2_O, PORT_1_X, PORT_1_O, TARGET_A,
  KICKER_L, LANE_UP_L, BUMPER_A, BUMPER_C, KICKER_R, BUMPER_B, LANE_UP_R, TARGET_C,  
  LANE_OUT, LANE_IN_L, SLING_L, SLING_R, ROLLOVER, LANE_IN_R, BALL_EXIT, BALL_ENTER,

  FLIPPER_L, FLIPPER_R, FLIPPER_EOS_L, FLIPPER_EOS_R, TILT_PENDULUM, TILT_RAMP,
  SCORE0_00010, SCORE0_00100, SCORE0_01000, SCORE0_10000,
  SCORE1_00010, SCORE1_00100, SCORE1_01000, SCORE1_10000,
  SCORE2_00010, SCORE2_00100, SCORE2_01000, SCORE2_10000,
  SCORE3_00010, SCORE3_00100, SCORE3_01000, SCORE3_10000,
  SAVE_GATE, // dummy element
  
};

void setup() {
  delay( 1000 );
  Serial.println( "start" );

  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 2 )
    .addStrip( new IO_Adafruit_NeoPixel( 10, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // SK6812
    .addStrip( new IO_Adafruit_NeoPixel(  8, pin_data, NEO_GRB  + NEO_KHZ800 ) ) // WS2812B 
    .retrigger()
    .show();

  leds.begin( io )
    .defineProfile( PROFILE_COIL, 0, 127, 30 ) // T0, L1, T1, L2
    .defineProfile( PROFILE_LED, 0, 0, 0, 5 )
    .defineProfile( PROFILE_FLIPPER, 0, 127, 30, 20 )
    .defineProfile( PROFILE_KICKER, 1000, 127, 30 )
    .defineProfile( PROFILE_GATE, 0, 127, 30, 50 )
    .defineProfile( PROFILE_FEEDER, 1000, 127, 30 );

  playfield.begin( io, leds ); // IDEA: embed led scheduler in playfield...

  // Element instantiation
    
  playfield.element( TARGET_A, -1, LED_TARGET_A ).autoLight();
  playfield.element( TARGET_A, -1, LED_TARGET_B ).autoLight();
  playfield.element( BUMPER_A, COIL_BUMPER_A, LED_BUMPER_A );
  playfield.element( BUMPER_B, COIL_BUMPER_B, LED_BUMPER_B );
  playfield.element( BUMPER_C, COIL_BUMPER_C, LED_BUMPER_C );
  playfield.element( KICKER_L, COIL_KICKER_L, LED_KICKER_L, PROFILE_KICKER );
  playfield.element( KICKER_R, COIL_KICKER_R, LED_KICKER_R, PROFILE_KICKER );
  playfield.element( SLING_L, COIL_SLING_L, -1 );
  playfield.element( SLING_R, COIL_SLING_R, -1 ).kick();
  playfield.element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER );
  playfield.element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER );
  playfield.element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );  
  playfield.element( BALL_EXIT, COIL_BALL_FEEDER, -1, PROFILE_FEEDER );  

  // TARGET -> BUMPER -> GATE logic

  playfield.element( TARGET_A )
    .onLight( true, playfield.element( BUMPER_A ), Atm_element::EVT_ON );
    
  playfield.element( TARGET_B )
    .onLight( true, playfield.element( BUMPER_B ), Atm_element::EVT_ON );

  playfield.element( BUMPER_A )
    .onLight( true, [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_B ).state() ) {
        playfield.element( BUMPER_C ).on();
      }
    });

  playfield.element( BUMPER_B )
    .onLight( true, [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_A ).state() ) {
        playfield.element( BUMPER_C ).on();
      }
    });

  playfield.element( BUMPER_C  )
    .onLight( true, playfield.element( SAVE_GATE ), Atm_element::EVT_KICK );

  // end of login

  Serial.println( FreeRam() );
}

void loop() {  
  automaton.run();
}
