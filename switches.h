#pragma once

// Note that all symbols must be longer than 1 character (or they'll be confused with opcodes)

const char switch_symbols[] = 
  "null_sw, target_b, port_3x, port_3o, port_2x, port_2o, port_1x, port_1o, target_a, kicker_l, uplane_l," 
  "bumper_a, bumper_c, kicker_r, bumper_b, uplane_r, target_c, outlane, inlane_l, sling_l, sling_r," 
  "rollover, inlane_r, ball_enter, ball_exit, tilt_pend, tilt_ramp, switch213, flipper_l, flipper_r," 
  "switch216, switch217, frontbtn, counter2, counter, switch313, switch314, switch315, switch316," 
  "counter1, counter3, save_gate, multilane, dual_target, oxo, gi, kicker, flipper, uplane, slingshot," 
  "lower, feeder, again, chimes, players, playerup, ballup, animation, game_over, switches"; 

enum { 
/* 000 */   NULL_SW,  // Dummy, physical switches start at 1
/* 001 */   TARGET_B, // MX10 first playfield matrix node
/* 002 */   PORT_3X,
/* 003 */   PORT_3O,
/* 004 */   PORT_2X,
/* 005 */   PORT_2O,
/* 006 */   PORT_1X,
/* 007 */   PORT_1O,
/* 008 */   TARGET_A,
/* 009 */   KICKER_L, // MX11
/* 010 */   UPLANE_L,
/* 011 */   BUMPER_A,
/* 012 */   BUMPER_C,
/* 013 */   KICKER_R,
/* 014 */   BUMPER_B,
/* 015 */   UPLANE_R,
/* 016 */   TARGET_C,
/* 017 */   OUTLANE, // MX12
/* 018 */   INLANE_L,
/* 019 */   SLING_L,
/* 020 */   SLING_R,
/* 021 */   ROLLOVER,
/* 022 */   INLANE_R,
/* 023 */   BALL_ENTER,
/* 024 */   BALL_EXIT,
/* 025 */   TILT_PEND, // MX21 cabinet matrix node
/* 026 */   TILT_RAMP,
/* 027 */   SWITCH213,
/* 028 */   FLIPPER_L,
/* 029 */   FLIPPER_R,
/* 030 */   SWITCH216,
/* 031 */   SWITCH217,
/* 032 */   FRONTBTN,
/* 033 */   COUNTER2, // MX31 headbox matrix node
/* 034 */   COUNTER,
/* 035 */   SWITCH313,
/* 036 */   SWITCH314,
/* 037 */   SWITCH315,
/* 038 */   SWITCH316,
/* 039 */   COUNTER1,
/* 040 */   COUNTER3,  // Last physical switch

/* 041 */   SAVE_GATE, // Logical switches start here
/* 042 */   MULTILANE,
/* 043 */   DUAL_TARGET,
/* 044 */   OXO,
/* 045 */   GI,
/* 046 */   KICKER,
/* 047 */   FLIPPER,
/* 048 */   UPLANE,
/* 049 */   SLINGSHOT,
/* 050 */   LOWER,
/* 051 */   FEEDER,
/* 052 */   AGAIN,
/* 053 */   CHIMES,
/* 054 */   PLAYERS,
/* 055 */   PLAYERUP,
/* 056 */   BALLUP,
/* 057 */   ANIMATION,
/* 058 */   GAME_OVER,
/* 059 */   SWITCHES,
};

int16_t switch_groups[] = {

  // Index table
  /* 041 */   SAVE_GATE,
  /* 042 */   MULTILANE,
  /* 043 */   DUAL_TARGET,
  /* 044 */   OXO,
  /* 045 */   GI,
  /* 046 */   KICKER,
  /* 047 */   FLIPPER,
  /* 048 */   UPLANE,
  /* 049 */   SLINGSHOT,
  /* 050 */   LOWER,
  /* 051 */   FEEDER,
  /* 052 */   AGAIN,
  /* 053 */   CHIMES,
  /* 054 */   PLAYERS,
  /* 055 */   PLAYERUP,
  /* 056 */   BALLUP,
  /* 057 */   ANIMATION,
  /* 058 */   GAME_OVER,
  /* 059 */   SWITCHES,
  -1,

  // Group definitions:
  MULTILANE,
    PORT_1O, PORT_1X, PORT_2O, PORT_2X, PORT_3O, PORT_3X, -1,
  DUAL_TARGET,
    TARGET_A, TARGET_B, -1,
  KICKER, 
    KICKER_L, KICKER_R, -1,
  FLIPPER, 
    FLIPPER_L, FLIPPER_R, -1,
  UPLANE,
    UPLANE_L, UPLANE_R, -1,
  SLINGSHOT,
    SLING_L, SLING_R, -1,
  LOWER,
    TARGET_C, INLANE_L, INLANE_R, ROLLOVER, OUTLANE, BALL_EXIT, BALL_ENTER, -1,
  SWITCHES,
    TARGET_B, PORT_3X, PORT_3O, PORT_2X, PORT_2O, PORT_1X, PORT_1O, TARGET_A, KICKER_L, UPLANE_L, BUMPER_A, BUMPER_C, KICKER_R,  
    BUMPER_B, UPLANE_R, TARGET_C, OUTLANE, INLANE_L, SLING_L, SLING_R, ROLLOVER, INLANE_R, BALL_ENTER, BALL_EXIT, TILT_PEND, TILT_RAMP,  
    SWITCH213, FLIPPER_L, FLIPPER_R, SWITCH216, SWITCH217, FRONTBTN, COUNTER2, COUNTER, SWITCH313, SWITCH314, SWITCH315, SWITCH316, COUNTER1, COUNTER3,
    -1,      
  // End of list of group definitions:
  -1,
};

const char switch_group_list[] = R""""(
  multilane, port_1o, port_1x, port_2o, port_2x, port_3o, port_3x;
  dual_target, target_a, target_b;
  kicker, kicker_l, kicker_r;
  flipper, flipper_l, flipper_r;
  uplane, uplane_l, uplane_r;
  slingshot, sling_l, sling_r;
  lower, target_c, inlane_l, inlane_r, rollover, outlane, ball_exit, ball_enter;
  switches, 
    target_b, port_3x, port_3o, port_2x, port_2o, port_1x, port_1o, target_a, kicker_l, uplane_l, bumper_a, bumper_c, kicker_r, 
    bumper_b, uplane_r, target_c, outlane, inlane_l, sling_l, sling_r, rollover, inlane_r, ball_enter, ball_exit, tilt_pend, tilt_ramp, 
    switch213, flipper_l, flipper_r, switch216, switch217, frontbtn, counter2, counter, switch313, switch314, switch315, switch316, counter1, counter3;
)"""";


// Table of switch names for debugging purposes: 
// Serial.println( SWITCH_NAME( 4 ) ) -> "PORT_3O"

#define SWITCH_NAME( sw ) switch_names + ( sw * 11 )

const char switch_names[] = 
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
