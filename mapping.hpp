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
};

/* 
 *  - ID Profile ID
 *  - T0 Wait time before led on
 *  - L1 Initial led level (0-255)
 *  - T1 Initial period
 *  - T2 Hold level
 */ 

const int16_t profile_definition[] = {
  PROFILE_COIL,        0, 255, 30,   0, -1,
  PROFILE_LED,         0,   0,  0, 127, -1,
  PROFILE_FLIPPER,     0, 255, 50, 255, -1,
  PROFILE_KICKER,   1000,  95, 30,   0, -1,
  PROFILE_GATE,        0, 255, 30, 255, -1,
  PROFILE_BUMPER,      0, 255, 30,   0, -1,
  PROFILE_FEEDER,   1000, 127, 30,   0, -1,
  PROFILE_GI,          0,   1,  1,   3, -1,
  PROFILE_OXO,         0,   1,  1, 255, -1,
  PROFILE_COUNTER,     0, 255, 30,   0, -1,
  -1,  
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

  COIL_CHIME_10,  // Dummy entries for led strip on P2 (cabinet)
  COIL_CHIME_100,
  COIL_CHIME_1K,
  COIL_KNOCKER,
  
  COIL_COUNTER0_10, // Strip on P3 (headbox)
  COIL_COUNTER0_100, 
  COIL_COUNTER0_1K,
  COIL_COUNTER0_10K, // Let op: strip staat op length=4!

  /* Virtual LED groups - up to 32 - are declared after physical leds */
  LED_KICKER_GRP,
  LED_UP_LANE_GRP,  
  LED_OXO_GRP,
  LED_BUMPER_GRP,
  LED_FLASHER_GRP,
  COIL_COUNTER0_GRP,
  COIL_COUNTER1_GRP,
  COIL_COUNTER2_GRP,
  COIL_COUNTER3_GRP,
};

// Switches (inputs)

enum { 
/* Dummy, switches start at 1 and go up to 320 */  
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
  SWITCH211,
  SWITCH212,
  SWITCH213,
  FLIPPER_L,
  FLIPPER_R,
  SWITCH216,
  SWITCH217,
  FRONTBTN,
  SWITCH311,
  COUNTER0,
  COUNTER1,
  COUNTER2,
  COUNTER3,
  
/* Virtual switches (for elements without physical switches) */  
  SAVE_GATE, 
  LED_FLASHERS,
};

/* group_map - Defines virtual LEDs (groups of leds)
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
    LED_BUMPER_GRP, LED_UP_LANE_GRP, LED_OXO_GRP, LED_KICKER_GRP, LED_SHOOTS_AGAIN, LED_TRIPLE_BONUS, -1,  
  // Add groups per score counter
  COIL_COUNTER0_GRP,
    COIL_COUNTER0_10K, COIL_COUNTER0_1K, COIL_COUNTER0_100, COIL_COUNTER0_10, -1,
//COIL_COUNTER1_GRP,
//  COIL_COUNTER1_10K, COIL_COUNTER1_1K, COIL_COUNTER1_100, COIL_COUNTER1_10, -1,
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
  "SWITCH311 \0"
  "COUNTER0  \0"
  "COUNTER1  \0"
  "COUNTER2  \0"
  "COUNTER3  \0"
  "SAVE_GATE \0";
  
