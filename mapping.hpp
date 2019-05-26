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
  PROFILE_BUMPER,
  PROFILE_GI,
  PROFILE_OXO
};

// Coils & leds (outputs)

enum { 
/* coil node 1 */
  /* a */  COIL_SAVE_GATE, 
  /* b */  COIL_FLIPPER_R,
  /* c */  COIL_BALL_FEEDER,
  /* d */  COIL_SLING_R,
/* coil node 2 */
  /* e */  COIL_SLING_L,  
  /* f */  COIL_FLIPPER_L,
  /* g */  COIL_KICKER_R,
  /* h */  COIL_KICKER_L,
/* coil node 3 */
  /* i */  COIL_BUMPER_A, 
  /* j */  COIL_BUMPER_B,
  /* k */  COIL_BUMPER_C,
  /* l */  COIL_GI,
/* playfield leds */  
  /* m */  LED_TARGET_A,
  /* n */  LED_BUMPER_C,
  /* o */  LED_BUMPER_A,
  /* p */  LED_BUMPER_B,
  /* q */  LED_TARGET_B,
  /* r */  LED_UP_LANE_R,
  /* s */  LED_KICKER_R,
  /* t */  LED_SHOOTS_AGAIN,
  /* u */  LED_TRIPLE_BONUS,
  /* v */  LED_EXTRA, 
  /* w */  LED_KICKER_L,
  /* x */  LED_UP_LANE_L,  
  
/* OXO leds: 27 + 2 */
  LED_OXO_3A, LED_OXO_3B, LED_OXO_3C, // OXO leds in physical order
  LED_OXO_6A, LED_OXO_6B, LED_OXO_6C,
  LED_OXO_2A, LED_OXO_2B, LED_OXO_2C,
  LED_OXO_1A, LED_OXO_1B, LED_OXO_1C,
  LED_OXO_5A, LED_OXO_5B, LED_OXO_5C,
  LED_OXO_9A, LED_OXO_9B, LED_OXO_9C,  
  LED_OXO_8A, LED_OXO_8B, LED_OXO_8C,
  LED_OXO_4A, LED_OXO_4B, LED_OXO_4C, 
  LED_OXO_7A, LED_OXO_7B, LED_OXO_7C,
  LED_OXO_O, LED_OXO_X,  
};

// OXO led mapping 
uint8_t oxo_map[] = {
  LED_OXO_1A, LED_OXO_1B, LED_OXO_1C, 
  LED_OXO_2A, LED_OXO_2B, LED_OXO_2C,
  LED_OXO_3A, LED_OXO_3B, LED_OXO_3C,
  LED_OXO_4A, LED_OXO_4B, LED_OXO_4C,
  LED_OXO_5A, LED_OXO_5B, LED_OXO_5C,
  LED_OXO_6A, LED_OXO_6B, LED_OXO_6C,
  LED_OXO_7A, LED_OXO_7B, LED_OXO_7C,
  LED_OXO_8A, LED_OXO_8B, LED_OXO_8C,
  LED_OXO_9A, LED_OXO_9B, LED_OXO_9C,
  LED_OXO_O, LED_OXO_X,
};

// Switches (inputs)
enum { 
  NULL_SW,
/* Physical switches */  
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
/* Dummy switches (for elements without physical switches) */  
  SAVE_GATE, 
};

#define SWITCH_NAME( sw ) switch_names + ( sw * 11 )

char switch_names[] = 
  "NULL_SW   \0"
  "TARGET_B  \0"
  "PORT_3X   \0"
  "PORT_3O   \0"
  "PORT_2X   \0"
  "PORT_2O   \0"
  "PORT_1X   \0"
  "PORT_1O   \0"
  "TARGET_A  \0"
  "KICKER_L  \0"
  "UP_LANE_L \0"
  "BUMPER_A  \0"
  "BUMPER_C  \0"
  "KICKER_R  \0"
  "BUMPER_B  \0"
  "UP_LANE_R \0"
  "TARGET_C  \0"
  "OUT_LANE  \0"
  "IN_LANE_L \0"
  "SLING_L   \0"
  "SLING_R   \0"
  "ROLLOVER  \0"
  "IN_LANE_R \0"
  "BALL_ENTER\0"
  "BALL_EXIT \0"
  "DUMMY1    \0"
  "DUMMY2    \0"
  "DUMMY3    \0"
  "FLIPPER_L \0"
  "FLIPPER_R \0"
  "SAVE_GATE \0";
