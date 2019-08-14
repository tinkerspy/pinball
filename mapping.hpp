#pragma once

/*
 * Aims:
 * - Minimal wiring
 * - Minimal I/O pins
 * - Minimal code
 * - Minimal latency
 */

uint8_t gate[5] = { 15, 22, 23, 16, 17 };
uint8_t shift_inputs[5] = { 2, 14, 7, 8, 6 };
uint8_t addr[3] = { 5, 21, 20 };

const int pin_data = 11;
const int pin_clock = 13;
const int pin_latch = 4;

// Led & coil profiles 

enum { 
  PROFILE_COIL, 
  PROFILE_LED, 
  PROFILE_FLIPPER, 
  PROFILE_KICKER, 
  PROFILE_FEEDER, 
  PROFILE_GATE, 
  PROFILE_BUMPER,
  PROFILE_GI,
  PROFILE_OXO,
  PROFILE_COUNTER,
  PROFILE_BRIGHT,
};

/* 
 *  - ID Profile ID
 *  - T0 Wait time before led on
 *  - L1 Initial led level (0-255)
 *  - T1 Initial period
 *  - L2 Hold level
 */ 

const int16_t profile_definition[] = {
/*                    T0   L1  T1   L2  */ 
  PROFILE_COIL,        0, 255, 30,   0, -1,
  PROFILE_LED,         0,   0,  0, 127, -1,
  PROFILE_FLIPPER,     0,   0,  0, 255, -1,
  PROFILE_KICKER,   1000,  95, 30,   0, -1,
  PROFILE_GATE,        0,   0,  0, 255, -1,
  PROFILE_BUMPER,      0, 255, 30,   0, -1,
  PROFILE_FEEDER,   1000, 127, 30,   0, -1,
  PROFILE_GI,          0,   0,  0, 255, -1,
  PROFILE_OXO,         0,   0,  0, 127, -1,
  PROFILE_COUNTER,     0, 255, 30,   0, -1,
  PROFILE_BRIGHT,      0,   0,  0, 255, -1,
  -1,  
};

// Coils & leds (outputs) /////////////////////////////////////////////////////////////////////////////////

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
  LED_OXO_O, LED_OXO_X, // FIXME these are the other way around!

  COIL_CHIME_10,  // Dummy entries for led strip on P2 (cabinet)
  COIL_CHIME_100,
  COIL_CHIME_1K,
  COIL_KNOCKER,
  
  COIL_COUNTER0_10, COIL_COUNTER0_100, COIL_COUNTER0_1K, COIL_COUNTER0_10K, // Strip on P3 (headbox)
  COIL_COUNTER2_10, COIL_COUNTER2_100, COIL_COUNTER2_1K, COIL_COUNTER2_10K,
  COIL_COUNTER3_10K, COIL_COUNTER3_1K, COIL_COUNTER3_100, COIL_COUNTER3_10,
  COIL_COUNTER1_10K, COIL_COUNTER1_1K, COIL_COUNTER1_100, COIL_COUNTER1_10,
  LED_UP1,
  LED_OXO_ANI3, LED_OXO_ANI2, LED_OXO_ANI1,
  LED_UP0, LED_UP3, LED_UP2,
  LED_TILT,
  LED_PLAY4, LED_PLAY3, LED_PLAY2, LED_PLAY1,
  LED_AGAIN2, LED_AGAIN1,
  LED_BALL3, LED_BALL5, LED_BALL2, LED_BALL4, LED_BALL1,
  LED_GAME_OVER,
  
  /* Virtual LED groups - up to 32 - are declared after physical leds */
  LED_KICKER_GRP,
  LED_UP_LANE_GRP,  
  LED_OXO_GRP,
  LED_OXO_CELLS,
  LED_BUMPER_GRP,
  LED_FLASHER_GRP,
  LED_TARGET_GRP,
  COIL_COUNTER0_GRP,
  COIL_COUNTER1_GRP,
  COIL_COUNTER2_GRP,
  COIL_COUNTER3_GRP,
  LED_UP_GRP,
  LED_PLAY_GRP,
  LED_BALL_GRP,
  LED_AGAIN_GRP,
  LED_OXO_ANI_GRP,
  LED_HEADBOX_GRP,
};

// Switches (inputs) ///////////////////////////////////////////////////////////////////////////////////

enum { 
/* Dummy, switches start at 1 and go up to 320 */  
  NULL_SW,
/* Physical switches */  
  TARGET_B, // MX10
  PORT_3X,
  PORT_3O,
  PORT_2X,
  PORT_2O,
  PORT_1X,
  PORT_1O,
  TARGET_A,
  KICKER_L, // MX11
  UP_LANE_L,
  BUMPER_A,
  BUMPER_C,
  KICKER_R,
  BUMPER_B,
  UP_LANE_R,
  TARGET_C,
  OUT_LANE, // MX12
  IN_LANE_L,
  SLING_L,
  SLING_R,
  ROLLOVER,
  IN_LANE_R,
  BALL_ENTER,
  BALL_EXIT,
  SWITCH211, // MX21
  SWITCH212,
  SWITCH213,
  FLIPPER_L,
  FLIPPER_R,
  SWITCH216,
  SWITCH217,
  FRONTBTN,
  COUNTER2,// MX31
  COUNTER0, 
  SWITCH313,
  SWITCH314,
  SWITCH315,
  SWITCH316,
  COUNTER1,
  COUNTER3,
  
/* Virtual switches (for elements without physical switches) */  
  SAVE_GATE,
};

/* group_map - Defines virtual LEDs (groups of leds) /////////////////////////////////////////////////////////////////////////
 *  
 * Record starts with the group ID followed by the elements, ended by a -1 entry.
 * The last record in the list is followed by a second -1 entry to signal end of list 
 * 
 * Leds and switches can be mixed as long as you make sure no Atm_led_scheduler actions
 * are performed on the group.
 */

const int16_t group_definition[] = {
  LED_KICKER_GRP, 
    LED_KICKER_L, LED_KICKER_R, -1,
  LED_UP_LANE_GRP, 
    LED_UP_LANE_L, LED_UP_LANE_R, -1,
  LED_TARGET_GRP,
    LED_TARGET_A, LED_TARGET_B, -1,
  LED_OXO_GRP,
    LED_OXO_1A, LED_OXO_1B, LED_OXO_1C, 
    LED_OXO_2A, LED_OXO_2B, LED_OXO_2C,
    LED_OXO_3A, LED_OXO_3B, LED_OXO_3C,
    LED_OXO_4A, LED_OXO_4B, LED_OXO_4C,
    LED_OXO_5A, LED_OXO_5B, LED_OXO_5C,
    LED_OXO_6A, LED_OXO_6B, LED_OXO_6C,
    LED_OXO_7A, LED_OXO_7B, LED_OXO_7C,
    LED_OXO_8A, LED_OXO_8B, LED_OXO_8C,
    LED_OXO_9A, LED_OXO_9B, LED_OXO_9C,
    LED_OXO_O, LED_OXO_X, -1,
  LED_BUMPER_GRP,
    LED_BUMPER_A, LED_BUMPER_B, LED_BUMPER_C, LED_TARGET_A, LED_TARGET_B, COIL_SAVE_GATE, -1,  
  LED_FLASHER_GRP,
    LED_BUMPER_GRP, LED_UP_LANE_GRP, LED_KICKER_GRP, LED_SHOOTS_AGAIN, LED_TRIPLE_BONUS, 
    LED_OXO_1A, LED_OXO_1B, LED_OXO_1C, 
    LED_OXO_2A, LED_OXO_2B, LED_OXO_2C,
    LED_OXO_3A, LED_OXO_3B, LED_OXO_3C,
    LED_OXO_4A, LED_OXO_4B, LED_OXO_4C,
    LED_OXO_5A, LED_OXO_5B, LED_OXO_5C,
    LED_OXO_6A, LED_OXO_6B, LED_OXO_6C,
    LED_OXO_7A, LED_OXO_7B, LED_OXO_7C,
    LED_OXO_8A, LED_OXO_8B, LED_OXO_8C,
    LED_OXO_9A, LED_OXO_9B, LED_OXO_9C,
    -1,  
  LED_OXO_CELLS,
    LED_OXO_1A, LED_OXO_1B, 
    LED_OXO_2A, LED_OXO_2B,
    LED_OXO_3A, LED_OXO_3B,
    LED_OXO_4A, LED_OXO_4B,
    LED_OXO_5A, LED_OXO_5B,
    LED_OXO_6A, LED_OXO_6B,
    LED_OXO_7A, LED_OXO_7B,
    LED_OXO_8A, LED_OXO_8B,
    LED_OXO_9A, LED_OXO_9B,
    -1,  
  COIL_COUNTER0_GRP,
    COIL_COUNTER0_10K, COIL_COUNTER0_1K, COIL_COUNTER0_100, COIL_COUNTER0_10, -1,
  COIL_COUNTER1_GRP,
    COIL_COUNTER1_10K, COIL_COUNTER1_1K, COIL_COUNTER1_100, COIL_COUNTER1_10, -1,
  COIL_COUNTER2_GRP,
    COIL_COUNTER2_10K, COIL_COUNTER2_1K, COIL_COUNTER2_100, COIL_COUNTER2_10, -1,
  COIL_COUNTER3_GRP,
    COIL_COUNTER3_10K, COIL_COUNTER3_1K, COIL_COUNTER3_100, COIL_COUNTER3_10, -1,
  LED_UP_GRP,
    LED_UP0, LED_UP1, LED_UP2, LED_UP3, -1,
  LED_PLAY_GRP,
    LED_PLAY1, LED_PLAY2, LED_PLAY3, LED_PLAY4, -1,
  LED_BALL_GRP,
    LED_BALL1, LED_BALL2, LED_BALL3, LED_BALL4, LED_BALL5, -1,  
  LED_AGAIN_GRP,
    LED_AGAIN1, LED_AGAIN2, -1,
  LED_OXO_ANI_GRP,
    LED_OXO_ANI1, LED_OXO_ANI2, LED_OXO_ANI3, -1,
  LED_HEADBOX_GRP,
    LED_UP_GRP, LED_PLAY_GRP, LED_BALL_GRP, LED_AGAIN_GRP, LED_OXO_ANI_GRP, LED_GAME_OVER, LED_TILT, -1,
  -1, 
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
  "SWITCH211 \0"
  "SWITCH212 \0"
  "SWITCH213 \0"
  "FLIPPER_L \0"
  "FLIPPER_R \0"
  "SWITCH216 \0"
  "SWITCH217 \0"
  "FRONTBTN  \0"
  "COUNTER0  \0"
  "COUNTER2  \0"
  "SWITCH313 \0"
  "SWITCH314 \0"
  "SWITCH315 \0"
  "SWITCH316 \0"
  "COUNTER1  \0"
  "COUNTER3  \0"
  "FLASHERS  \0";
