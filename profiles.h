
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
/* COUNTER        */   'L',     0, 127, 20,   0,     COIL_COUNTER0_GRP, COIL_COUNTER1_GRP, COIL_COUNTER2_GRP, COIL_COUNTER3_GRP, -1,  // pwm 127?

/*                     L/S     PR,    RL,    TH      SWITCHES (debounce delays are specified in 1/10's of a millisecond!) */ 
/* DEFAULT SWITCH */   'S',    200,  200,     0,     SWITCHES, -1,
/* MULTILANE      */   'S',    200,  200,     0,     MULTILANE, -1, 
/* SLINGSHOTS     */   'S',    200,    0,  5000,     SLING_L, SLING_R, -1, 
/* FLIPPERS       */   'S',     50,    0,     0,     FLIPPER, -1,
/* COUNTERS       */   'S',      0,  200,     0,     COUNTER0, COUNTER1, COUNTER2, COUNTER3, -1,
/* ROLLOVERS      */   'S',    100,    0,     0,     PORT_3X, PORT_3O, PORT_2X, PORT_2O, PORT_1X, PORT_1O, UPLANE_L,  
                                                     UPLANE_R, OUTLANE, INLANE_L, ROLLOVER, INLANE_R, BALL_ENTER, -1, 
/* BALL_EXIT      */   'S',      0,   50,     0,     BALL_EXIT, -1,
/* FRONTBTN       */   'S',    200,    0,  5000,     FRONTBTN, -1,

-1,  
};
