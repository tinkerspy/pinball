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

//SPROFILE_DEFAULT    20,  20,  0,  -1, 
//SPROFILE_ROLLOVER   20,  20,  0,  -1, 

  -1,  
};


// Coils & leds (outputs) /////////////////////////////////////////////////////////////////////////////////

enum { 
/* coil node 1 */
  /* a */  COIL_SAVE_GATE, 
  /* b */  COIL_FLIPPER_R,
  /* c */  COIL_FEEDER,
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
  /* r */  LED_UPLANE_R,
  /* s */  LED_KICKER_R,
  /* t */  LED_AGAIN0,
  /* u */  LED_TRIPLE_BONUS,
  /* v */  LED_EXTRA, 
  /* w */  LED_KICKER_L,
  /* x */  LED_UPLANE_L,  
  
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

  COIL_CHIME0,  // Strip on P2 (Cabinet)
  COIL_CHIME1,
  COIL_CHIME2,
  COIL_KNOCKER,
  
  COIL_COUNTER0_10, COIL_COUNTER0_100, COIL_COUNTER0_1K, COIL_COUNTER0_10K, // Strip on P3 (headbox)
  COIL_COUNTER2_10, COIL_COUNTER2_100, COIL_COUNTER2_1K, COIL_COUNTER2_10K,
  COIL_COUNTER3_10K, COIL_COUNTER3_1K, COIL_COUNTER3_100, COIL_COUNTER3_10,
  COIL_COUNTER1_10K, COIL_COUNTER1_1K, COIL_COUNTER1_100, COIL_COUNTER1_10,
  LED_UP2,
  LED_OXO_ANI2, LED_OXO_ANI1, LED_OXO_ANI0,
  LED_UP1, LED_UP3, LED_UP4,
  LED_TILT,
  LED_PLAY4, LED_PLAY3, LED_PLAY2, LED_PLAY1,
  LED_AGAIN2, LED_AGAIN1,
  LED_BALL3, LED_BALL5, LED_BALL2, LED_BALL4, LED_BALL1,
  LED_GAME_OVER,
  
  /* Virtual LED groups are declared after physical leds */
  LED_KICKER_GRP,
  LED_UPLANE_GRP,  
  LED_OXO_GRP,
  LED_OXO_CELLS,
  LED_BUMPER_GRP,
  LED_BUMPER_A_GRP,
  LED_BUMPER_B_GRP,
  LED_BUMPER_C_GRP,
  LED_FLASHER_GRP,
  LED_TARGET_GRP,
  COIL_COUNTER0_GRP,
  COIL_COUNTER1_GRP,
  COIL_COUNTER2_GRP,
  COIL_COUNTER3_GRP,
  LED_AGAIN_GRP,
  LED_OXO_ANI_GRP,
  LED_HEADBOX_GRP,
  LED_FLIPPER_GRP,
  LED_SLINGSHOT_GRP,
  LED_CHIME_GRP,
  LED_PLAYERUP_GRP, 
  LED_BALLUP_GRP, 
  LED_PLAYERS_GRP,
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
  UPLANE_L,
  BUMPER_A,
  BUMPER_C,
  KICKER_R,
  BUMPER_B,
  UPLANE_R,
  TARGET_C,
  OUTLANE, // MX12
  INLANE_L,
  SLING_L,
  SLING_R,
  ROLLOVER,
  INLANE_R,
  BALL_ENTER,
  BALL_EXIT,
  TILT_PEND, // MX21
  TILT_RAMP,
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
  
/* Virtual switch groups are declared after physical switches */
  SAVE_GATE,
  CHIME0,
  CHIME1,
  CHIME2,
  MULTILANE,
  DUAL_TARGET,
  OXO,
  GI,
  KICKER,
  FLIPPER,
  UPLANE,
  SLINGSHOT,
  LOWER,
  FEEDER,
  AGAIN,
  CHIMES,
  PLAYERS,
  PLAYERUP,
  BALLUP,
  ANIMATION,
  SWITCHES,

/* Don't forget to add new entries to the switch_group_definition jumptable and array! */
};

/* group_map - Defines virtual LEDs (groups of leds) /////////////////////////////////////////////////////////////////////////
 *  
 * Record starts with the group ID followed by the elements, ended by a -1 entry.
 * The last record in the list is followed by a second -1 entry to signal end of list 
 * 
 * Leds and switches can be mixed as long as you make sure no Atm_led_scheduler actions
 * are performed on the group.
 */

const int16_t led_groups[] = {
/*  
  LED_KICKER_GRP,
  LED_UPLANE_GRP,  
  LED_OXO_GRP,
  LED_OXO_CELLS,
  LED_BUMPER_GRP,
  LED_BUMPER_A_GRP,
  LED_BUMPER_B_GRP,
  LED_BUMPER_C_GRP,
  LED_FLASHER_GRP,
  LED_TARGET_GRP,
  COIL_COUNTER0_GRP,
  COIL_COUNTER1_GRP,
  COIL_COUNTER2_GRP,
  COIL_COUNTER3_GRP,
  LED_AGAIN_GRP,
  LED_OXO_ANI_GRP,
  LED_HEADBOX_GRP,
  LED_FLIPPER_GRP,
  LED_SLINGSHOT_GRP,
  LED_CHIME_GRP,
  LED_PLAYERUP_GRP, 
  LED_BALLUP_GRP, 
  LED_PLAYERS_GRP,
  -1,
*/
  
  LED_KICKER_GRP, 
    COIL_KICKER_L, COIL_KICKER_R, LED_KICKER_L, LED_KICKER_R, -1,
  LED_UPLANE_GRP, 
    LED_UPLANE_L, LED_UPLANE_R, -1,
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
    LED_OXO_O, LED_OXO_X, LED_TRIPLE_BONUS, -1,
  LED_BUMPER_GRP,
    LED_BUMPER_A, LED_BUMPER_B, LED_BUMPER_C, LED_TARGET_A, LED_TARGET_B, COIL_SAVE_GATE, -1,  
  LED_FLASHER_GRP,
    LED_BUMPER_GRP, LED_UPLANE_GRP, LED_KICKER_GRP, LED_AGAIN0, LED_AGAIN1, LED_AGAIN2, LED_GAME_OVER, LED_TRIPLE_BONUS, 
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
  LED_AGAIN_GRP,
    LED_AGAIN0, LED_AGAIN1, LED_AGAIN2, -1,
  LED_OXO_ANI_GRP,
    LED_OXO_ANI0, LED_OXO_ANI1, LED_OXO_ANI2, -1,
  LED_HEADBOX_GRP,
    LED_PLAYERUP_GRP, LED_PLAYERS_GRP, LED_BALLUP_GRP, LED_AGAIN_GRP, LED_OXO_ANI_GRP, LED_GAME_OVER, LED_TILT, -1,
  LED_FLIPPER_GRP,
    COIL_FLIPPER_L, COIL_FLIPPER_R, -1,
  LED_BUMPER_A_GRP,
    COIL_BUMPER_A, LED_BUMPER_A, -1,
  LED_BUMPER_B_GRP,
    COIL_BUMPER_B, LED_BUMPER_B, -1,
  LED_BUMPER_C_GRP,
    COIL_BUMPER_C, LED_BUMPER_C, -1,
  LED_SLINGSHOT_GRP,
    COIL_SLING_L, COIL_SLING_R, -1,
  LED_CHIME_GRP,
    COIL_CHIME0, COIL_CHIME1, COIL_CHIME2, -1,
  LED_PLAYERS_GRP,
    LED_PLAY1, LED_PLAY2, LED_PLAY3, LED_PLAY4, LED_PLAY4, LED_PLAY4, -1,
  LED_PLAYERUP_GRP, 
    LED_UP1, LED_UP2, LED_UP3, LED_UP4, LED_UP4, LED_UP4, -1,
  LED_BALLUP_GRP,
    LED_BALL1, LED_BALL2, LED_BALL3, LED_BALL4, LED_BALL5, LED_BALL5, -1,  
  -1, 
};

int16_t switch_groups[] = {
  SAVE_GATE,
  CHIME0,
  CHIME1,
  CHIME2,
  MULTILANE,
  DUAL_TARGET,
  OXO,
  GI,
  KICKER,
  FLIPPER,
  UPLANE,
  SLINGSHOT,
  LOWER,
  FEEDER,
  AGAIN,
  CHIMES,
  PLAYERS,
  PLAYERUP,
  BALLUP,
  ANIMATION,
  SWITCHES,
  -1,

  SWITCHES,
    TARGET_B, PORT_3X, PORT_3O, PORT_2X, PORT_2O, PORT_1X, PORT_1O, TARGET_A, KICKER_L, UPLANE_L, BUMPER_A, BUMPER_C, KICKER_R,  
    BUMPER_B, UPLANE_R, TARGET_C, OUTLANE, INLANE_L, SLING_L, SLING_R, ROLLOVER, INLANE_R, BALL_ENTER, BALL_EXIT, TILT_PEND, TILT_RAMP,  
    SWITCH213, FLIPPER_L, FLIPPER_R, SWITCH216, SWITCH217,FRONTBTN, COUNTER2, COUNTER0, SWITCH313, SWITCH314, SWITCH315, SWITCH316, COUNTER1, COUNTER3,
    -1,      
  SAVE_GATE, -1,
  CHIME0, -1,
  CHIME1, -1,
  CHIME2, -1,
  MULTILANE,
    PORT_1O, PORT_1X, PORT_2O, PORT_2X, PORT_3O, PORT_3X, -1,
  DUAL_TARGET,
    TARGET_A, TARGET_B, -1,
  OXO, -1,
  GI, -1,
  KICKER, 
    KICKER_L, KICKER_R, -1,
  FLIPPER, 
    FLIPPER_L, FLIPPER_R, -1,
  UPLANE,
    UPLANE_L, UPLANE_R, -1,
  SLINGSHOT,
    SLING_L, SLING_R, -1,
  LOWER,
    TARGET_C, INLANE_L, INLANE_R, ROLLOVER, OUTLANE, BALL_EXIT, BALL_ENTER, FRONTBTN, -1,
  FEEDER, -1,
  AGAIN, -1,
  CHIMES, -1, 
  PLAYERS, -1,
  PLAYERUP, -1,
  BALLUP, -1,
  ANIMATION, -1, 
  -1,
};


// New style profiles table for leds and switches (debounce):

const int16_t profiles[] = {

/*                     L/S     T0   L1  T1   L2      LED ITEMS/GROUPS */ 
/* DEFAULT LED    */   'L',     0,   0,  0, 127,     LED_KICKER_L, LED_KICKER_R, LED_TARGET_GRP, LED_UPLANE_GRP, LED_BUMPER_A, LED_BUMPER_B, 
                                                       LED_BUMPER_C, LED_AGAIN0, LED_TRIPLE_BONUS, LED_EXTRA, -1,
/* HEADBOX        */   'L',     0,   0,  0, 255,     LED_HEADBOX_GRP, -1,
/* OXO            */   'L',     0,   0,  0, 127,     LED_OXO_GRP, -1,
/* GI             */   'L',     0,   0,  0, 255,     COIL_GI, -1,
/* SLING          */   'L',     0, 255, 30,   0,     COIL_SLING_R, COIL_SLING_L, -1,
/* FLIPPER        */   'L',     0,   0,  0, 255,     COIL_FLIPPER_L, COIL_FLIPPER_R, -1,
/* KICKER         */   'L',  1000,  95, 30,   0,     COIL_KICKER_L, COIL_KICKER_R, -1,
/* GATE           */   'L',     0,   0,  0, 255,     COIL_SAVE_GATE, -1,
/* BUMPER         */   'L',     0, 255, 30,   0,     COIL_BUMPER_A, COIL_BUMPER_B, COIL_BUMPER_C, -1,
/* FEEDER         */   'L',  1000, 127, 30,   0,     COIL_FEEDER, -1,
/* COUNTER        */   'L',     0, 255, 30,   0,     COIL_COUNTER0_GRP, COIL_COUNTER1_GRP, COIL_COUNTER2_GRP, COIL_COUNTER3_GRP, -1,

/*                     L/S     MK,  BR, DT           SWITCHES */ 
/* DEFAULT SWITCH */   'S',    20,  20,  0,          SWITCHES, -1,
/* MULTILANE      */   'S',    20,  20,  0,          MULTILANE, -1, 
/* SLINGSHOTS     */   'S',    20, 200,  0,          SLINGSHOT, -1, 
/* FLIPPERS       */   'S',     5,   0,  0,          FLIPPER, -1,

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
  "UPLANE_L  \0"
  "BUMPER_A  \0"
  "BUMPER_C  \0"
  "KICKER_R  \0"
  "BUMPER_B  \0"
  "UPLANE_R  \0"
  "TARGET_C  \0"
  "OUT_LANE  \0"
  "IN_LANE_L \0"
  "SLING_L   \0"
  "SLING_R   \0"
  "ROLLOVER  \0"
  "IN_LANE_R \0"
  "BALL_ENTER\0"
  "BALL_EXIT \0" 
  "TILT_PEND \0"
  "TILT_RAMP \0"
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
  "COUNTER3  \0";
