
// Profiles table for leds and switches (debounce):

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
/* BUMPER         */   'L',     0, 255, 40,   0,     COIL_BUMPER_A, COIL_BUMPER_B, COIL_BUMPER_C, -1,
/* FEEDER         */   'L',     0, 127, 30,   0,     COIL_FEEDER, -1,
/* COUNTER        */   'L',     0, 127, 20,   0,     COIL_COUNTER_GRP, -1,  
/* VIRTUAL LEDS   */   'L',     0,   0,  0, 127,     VLED_COUNTER0, VLED_COUNTER1, VLED_COUNTER2, VLED_COUNTER3, VLED_COLLECTING, VLED_5, VLED_6, VLED_7, -1,

/*                     L/S     PR,    RL,    TH   SP   SWITCHES (debounce delays are specified in 1/10's of a millisecond!) */ 
/* DEFAULT SWITCH */   'S',    200,    0,     0,   0,  SWITCHES, -1,
/* MULTILANE      */   'S',    200,  200,     0,   0,  MULTILANE, -1, 
/* SLINGSHOTS     */   'S',      5,    0,  2000,   0,  SLING_L, SLING_R, -1, 
/* BUMPERS        */   'S',     10,    0,  2000,   0,  BUMPER_A, BUMPER_B, BUMPER_C, -1, 
/* FLIPPERS       */   'S',      0,   10,     0,   0,  FLIPPER, -1,
/* COUNTERS       */   'S',      0,  200,     0,   0,  COUNTER, COUNTER1, COUNTER2, COUNTER3, -1,
/* ROLLOVERS      */   'S',      5,    0,  5000,   0,  PORT_3X, PORT_3O, PORT_2X, PORT_2O, PORT_1X, PORT_1O, UPLANE_L,  
                                                       UPLANE_R, OUTLANE, INLANE_L, INLANE_R, BALL_ENTER, -1, 
/* ROLLOVER CENTER*/   'S',      1,    0,  5000,   0,  ROLLOVER, -1,
/* TARGETS        */   'S',      2,    0,     0,   0,  TARGET_A, TARGET_B, TARGET_C, -1,
/* BALL_EXIT      */   'S',      0,   50,  5000,   0,  BALL_EXIT, -1,
/* BALL_ENTER     */   'S',    200,    0,     0,   0,  BALL_ENTER, -1,
/* FRONTBTN       */   'S',      0,   50,  5000,   0,  FRONTBTN, -1,
-1,  
};
