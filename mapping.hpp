#pragma once

uint8_t gate[5] = { 15, 22, 23, 16, 17 };
uint8_t shift_inputs[5] = { 2, 14, 7, 8, 6 };
uint8_t addr[3] = { 5, 21, 20 };

const int pin_data = 11;
const int pin_clock = 13;
const int pin_latch = 4;

// Output profiles (leds, coils)

enum { 
  PROFILE_COIL, 
  PROFILE_LED, 
  PROFILE_FLIPPER, 
  PROFILE_KICKER, 
  PROFILE_FEEDER, 
  PROFILE_GATE, 
  PROFILE_BUMPER 
};

// Coils & leds (outputs)

enum { 
  COIL_SAVE_GATE, // #1
  COIL_FLIPPER_R,
  COIL_BALL_FEEDER,
  COIL_SLING_R,
  COIL_SLING_L,   // #2
  COIL_FLIPPER_L,
  COIL_KICKER_R,
  COIL_KICKER_L,
  COIL_BUMPER_A,  // #3
  COIL_BUMPER_B,
  COIL_BUMPER_C,
  COIL_DUMMY1,
  LED_TARGET_A,
  LED_BUMPER_C,
  LED_BUMPER_A,
  LED_BUMPER_B,
  LED_TARGET_B,
  LED_UP_LANE_R,
  LED_KICKER_R,
  LED_SHOOTS_AGAIN,
  LED_TRIPLE_BONUS,
  LED_EXTRA, 
  LED_KICKER_L,
  LED_UP_LANE_L,
};

// Switches (inputs)

enum { 
  SWITCH_NULL,
  TARGET_B,
  PORT_3X,
  PORT_3O,
  PORT_2X,
  PORT_2O,
  PORT_1X,
  PORT_1O,
  TARGET_A,
  KICKER_L,
  UP_LANE_L,
  BUMPER_A,
  BUMPER_C,
  KICKER_R,
  BUMPER_B,
  UP_LANE_R,
  TARGET_C,
  OUT_LANE,
  IN_LANE_L,
  SLING_L,
  SLING_R,
  ROLLOVER,
  IN_LANE_R,
  BALL_ENTER,
  BALL_EXIT,
  DUMMY1,
  DUMMY2,
  DUMMY3,
  FLIPPER_L,
  FLIPPER_R,
  SAVE_GATE,
};
