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
Atm_controller targets;
Atm_timer timer;

// Output profiles (leds, coils)

enum { PROFILE_COIL, PROFILE_LED, PROFILE_FLIPPER, PROFILE_KICKER, PROFILE_FEEDER };

// Coils & leds

enum { 
  COIL_BALL_FEEDER, COIL_FLIPPER_RIGHT, COIL_SLING_RIGHT, COIL_SAVE_GATE, 
  COIL_KICKER_RIGHT, COIL_FLIPPER_LEFT, COIL_SLING_LEFT, COIL_KICKER_LEFT,
  
  COIL_BUMPER_LEFT, COIL_BUMPER_CENTER, COIL_BUMPER_RIGHT, GI_PLAYFIELD,
  LED_LANE_UP_LEFT, LED_KICKER_LEFT, LED_KICKER_RIGHT, LED_LANE_UP_RIGHT,
  LED_BUMPER_CENTER, LED_BUMPER_LEFT, LED_BUMPER_RIGHT, LED_TARGET_A, 
  LED_TARGET_B, LED_SHOOTS_AGAIN, LED_TRIPLE_BONUS,
};

// Switches

enum { 
  SWITCH_NULL,
  SWITCH_TARGET_B, SWITCH_PORT_3_X, SWITCH_PORT_3_O, SWITCH_PORT_2_X, SWITCH_PORT_2_O, SWITCH_PORT_1_X, SWITCH_PORT_1_O, SWITCH_TARGET_A,
  SWITCH_KICKER_LEFT, SWITCH_LANE_UP_LEFT, SWITCH_BUMPER_LEFT, SWITCH_BUMPER_CENTER, SWITCH_KICKER_RIGHT, SWITCH_BUMPER_RIGHT, SWITCH_LANE_UP_RIGHT, SWITCH_TARGET_CENTER,  
  SWITCH_LANE_OUT, SWITCH_LANE_IN_LEFT, SWITCH_SLING_LEFT, SWITCH_SLING_RIGHT, SWITCH_ROLLOVER, SWITCH_LANE_IN_RIGHT, SWITCH_BALL_EXIT, SWITCH_BALL_ENTER,

  SWITCH_FLIPPER_LEFT, SWITCH_FLIPPER_RIGHT, SWITCH_FLIPPER_EOS_LEFT, SWITCH_FLIPPER_EOS_RIGHT, SWITCH_TILT_PENDULUM, SWITCH_TILT_RAMP,
  SWITCH_SCORE0_00010, SWITCH_SCORE0_00100, SWITCH_SCORE0_01000, SWITCH_SCORE0_10000,
  SWITCH_SCORE1_00010, SWITCH_SCORE1_00100, SWITCH_SCORE1_01000, SWITCH_SCORE1_10000,
  SWITCH_SCORE2_00010, SWITCH_SCORE2_00100, SWITCH_SCORE2_01000, SWITCH_SCORE2_10000,
  SWITCH_SCORE3_00010, SWITCH_SCORE3_00100, SWITCH_SCORE3_01000, SWITCH_SCORE3_10000,
  
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
    .defineProfile( PROFILE_COIL, 0, 127, 30 ) // wait_time, kick_level, kick_time, (hold_level)  
    .defineProfile( PROFILE_LED, 0, 0, 0, 5 )
    .defineProfile( PROFILE_FLIPPER, 0, 127, 30, 20 )
    .defineProfile( PROFILE_KICKER, 1000, 127, 30 )
    .defineProfile( PROFILE_FEEDER, 1000, 127, 30 );

  playfield.begin( io, leds ); // IDEA: embed led scheduler in playfield...

  playfield.element( SWITCH_BUMPER_LEFT, COIL_BUMPER_LEFT, -1 );
  playfield.element( SWITCH_BUMPER_CENTER, COIL_BUMPER_CENTER, -1 );
  playfield.element( SWITCH_BUMPER_RIGHT, COIL_BUMPER_RIGHT, -1 );
  playfield.element( SWITCH_KICKER_LEFT, COIL_KICKER_LEFT, -1, PROFILE_KICKER );
  playfield.element( SWITCH_KICKER_RIGHT, COIL_KICKER_RIGHT, -1, PROFILE_KICKER );
  playfield.element( SWITCH_SLING_LEFT, COIL_SLING_LEFT, -1 );
  playfield.element( SWITCH_SLING_RIGHT, COIL_SLING_RIGHT, -1 ).kick();
  playfield.element( SWITCH_FLIPPER_LEFT, COIL_FLIPPER_LEFT, -1, PROFILE_FLIPPER );
  playfield.element( SWITCH_FLIPPER_RIGHT, COIL_FLIPPER_RIGHT, -1, PROFILE_FLIPPER );
  playfield.element( SWITCH_BALL_EXIT, COIL_BALL_FEEDER, -1, PROFILE_FEEDER );

  Serial.println( FreeRam() );
}

void loop() {  
  automaton.run();
}
