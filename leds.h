#pragma once

// Note that all symbols must be longer than 1 character (or they'll be confused with opcodes)

const char led_symbols[] = 
  "coil_save_gate, coil_flipper_r, coil_feeder, coil_sling_r, coil_sling_l, coil_flipper_l, coil_kicker_r,"
  "coil_kicker_l, coil_bumper_a, coil_bumper_b, coil_bumper_c, coil_gi, led_target_a, led_bumper_c," 
  "led_bumper_a, led_bumper_b, led_target_b, led_uplane_r, led_kicker_r, led_again0, led_triple_bonus," 
  "led_extra, led_kicker_l, led_uplane_l, led_oxo_3a, led_oxo_3b, led_oxo_3c, led_oxo_6a, led_oxo_6b," 
  "led_oxo_6c, led_oxo_2a, led_oxo_2b, led_oxo_2c, led_oxo_1a, led_oxo_1b, led_oxo_1c, led_oxo_5a," 
  "led_oxo_5b, led_oxo_5c, led_oxo_9a, led_oxo_9b, led_oxo_9c, led_oxo_8a, led_oxo_8b, led_oxo_8c," 
  "led_oxo_4a, led_oxo_4b, led_oxo_4c, led_oxo_7a, led_oxo_7b, led_oxo_7c, led_oxo_o, led_oxo_x," 
  "coil_chime0,  coil_chime1, coil_chime2, coil_knocker, vled_counter0, vled_counter1, vled_counter2," 
  "vled_counter3, vled_collecting, vled_5, vled_6, vled_7, coil_counter0_10, coil_counter0_100, coil_counter0_1k," 
  "coil_counter0_10k, coil_counter2_10, coil_counter2_100, coil_counter2_1k, coil_counter2_10k, coil_counter3_10k," 
  "coil_counter3_1k, coil_counter3_100, coil_counter3_10, coil_counter1_10k, coil_counter1_1k, coil_counter1_100," 
  "coil_counter1_10, led_up2, led_oxo_ani2, led_oxo_ani1, led_oxo_ani0, led_up1, led_up3, led_up4, led_tilt," 
  "led_play4, led_play3, led_play2, led_play1, led_again2, led_again1, led_ball3, led_ball5, led_ball2, led_ball4," 
  "led_ball1, led_game_over, led_kicker_grp, led_uplane_grp, led_oxo_grp, led_bumper_grp, led_bumper_a_grp," 
  "led_bumper_b_grp, led_bumper_c_grp, led_target_grp, led_counter0_grp, led_counter1_grp, led_counter2_grp," 
  "led_counter3_grp, coil_counter_grp, led_again_grp, led_oxo_ani_grp, led_headbox_grp, led_flipper_grp," 
  "led_slingshot_grp, led_chime_grp, led_playerup_grp, led_ballup_grp, led_players_grp, led_game_grp, led_none_grp\n";   

enum { 
/* 000 */ COIL_SAVE_GATE, // Strip on P1 (playfield) - Powernode 1
/* 001 */ COIL_FLIPPER_R,
/* 002 */ COIL_FEEDER,
/* 003 */ COIL_SLING_R,
/* 004 */ COIL_SLING_L, // Powernode 2
/* 005 */ COIL_FLIPPER_L,
/* 006 */ COIL_KICKER_R,
/* 007 */ COIL_KICKER_L,
/* 008 */ COIL_BUMPER_A, // Powernode 3
/* 009 */ COIL_BUMPER_B,
/* 010 */ COIL_BUMPER_C,
/* 011 */ COIL_GI,
/* 012 */ LED_TARGET_A,
/* 013 */ LED_BUMPER_C,
/* 014 */ LED_BUMPER_A,
/* 015 */ LED_BUMPER_B,
/* 016 */ LED_TARGET_B,
/* 017 */ LED_UPLANE_R,
/* 018 */ LED_KICKER_R,
/* 019 */ LED_AGAIN0,
/* 020 */ LED_TRIPLE_BONUS,
/* 021 */ LED_EXTRA,
/* 022 */ LED_KICKER_L,
/* 023 */ LED_UPLANE_L,
/* 024 */ LED_OXO_3A,
/* 025 */ LED_OXO_3B,
/* 026 */ LED_OXO_3C,
/* 027 */ LED_OXO_6A,
/* 028 */ LED_OXO_6B,
/* 029 */ LED_OXO_6C,
/* 030 */ LED_OXO_2A,
/* 031 */ LED_OXO_2B,
/* 032 */ LED_OXO_2C,
/* 033 */ LED_OXO_1A,
/* 034 */ LED_OXO_1B,
/* 035 */ LED_OXO_1C,
/* 036 */ LED_OXO_5A,
/* 037 */ LED_OXO_5B,
/* 038 */ LED_OXO_5C,
/* 039 */ LED_OXO_9A,
/* 040 */ LED_OXO_9B,
/* 041 */ LED_OXO_9C,
/* 042 */ LED_OXO_8A,
/* 043 */ LED_OXO_8B,
/* 044 */ LED_OXO_8C,
/* 045 */ LED_OXO_4A,
/* 046 */ LED_OXO_4B,
/* 047 */ LED_OXO_4C,
/* 048 */ LED_OXO_7A,
/* 049 */ LED_OXO_7B,
/* 050 */ LED_OXO_7C,
/* 051 */ LED_OXO_O,
/* 052 */ LED_OXO_X,
/* 053 */ COIL_CHIME0,  // Strip on P2 (Cabinet) - Powernode 4
/* 054 */ COIL_CHIME1,
/* 055 */ COIL_CHIME2,
/* 056 */ COIL_KNOCKER,
/* 057 */ VLED_COUNTER0,       // 8 Virtual leds that don't physically exist
/* 058 */ VLED_COUNTER1,
/* 059 */ VLED_COUNTER2,
/* 060 */ VLED_COUNTER3,
/* 061 */ VLED_COLLECTING,
/* 062 */ VLED_5,
/* 063 */ VLED_6,
/* 064 */ VLED_7,
/* 065 */ COIL_COUNTER0_10, // Strip on P3 (Headbox) - Powernode 5
/* 066 */ COIL_COUNTER0_100,
/* 067 */ COIL_COUNTER0_1K,
/* 068 */ COIL_COUNTER0_10K,
/* 069 */ COIL_COUNTER2_10, // Powernode 6
/* 070 */ COIL_COUNTER2_100,
/* 071 */ COIL_COUNTER2_1K,
/* 072 */ COIL_COUNTER2_10K,
/* 073 */ COIL_COUNTER3_10K, // Powernode 7
/* 074 */ COIL_COUNTER3_1K,
/* 075 */ COIL_COUNTER3_100,
/* 076 */ COIL_COUNTER3_10,
/* 077 */ COIL_COUNTER1_10K, // Powernode 8
/* 078 */ COIL_COUNTER1_1K,
/* 079 */ COIL_COUNTER1_100,
/* 080 */ COIL_COUNTER1_10,
/* 081 */ LED_UP2,
/* 082 */ LED_OXO_ANI2,
/* 083 */ LED_OXO_ANI1,
/* 084 */ LED_OXO_ANI0,
/* 085 */ LED_UP1,
/* 086 */ LED_UP3,
/* 087 */ LED_UP4,
/* 088 */ LED_TILT,
/* 089 */ LED_PLAY4,
/* 090 */ LED_PLAY3,
/* 091 */ LED_PLAY2,
/* 092 */ LED_PLAY1,
/* 093 */ LED_AGAIN2,
/* 094 */ LED_AGAIN1,
/* 095 */ LED_BALL3,
/* 096 */ LED_BALL5,
/* 097 */ LED_BALL2,
/* 098 */ LED_BALL4,
/* 099 */ LED_BALL1,
/* 100 */ LED_GAME_OVER, // Last physical led
/* 101 */ LED_KICKER_GRP, // Logical leds start here
/* 102 */ LED_UPLANE_GRP,
/* 103 */ LED_OXO_GRP,
/* 104 */ LED_BUMPER_GRP,
/* 105 */ LED_BUMPER_A_GRP,
/* 106 */ LED_BUMPER_B_GRP,
/* 107 */ LED_BUMPER_C_GRP,
/* 108 */ LED_TARGET_GRP,
/* 109 */ LED_COUNTER0_GRP,
/* 110 */ LED_COUNTER1_GRP,
/* 111 */ LED_COUNTER2_GRP,
/* 112 */ LED_COUNTER3_GRP,
/* 113 */ COIL_COUNTER_GRP,
/* 114 */ LED_AGAIN_GRP,
/* 115 */ LED_OXO_ANI_GRP,
/* 116 */ LED_HEADBOX_GRP,
/* 117 */ LED_FLIPPER_GRP,
/* 118 */ LED_SLINGSHOT_GRP,
/* 119 */ LED_CHIME_GRP,
/* 120 */ LED_PLAYERUP_GRP,
/* 121 */ LED_BALLUP_GRP,
/* 122 */ LED_PLAYERS_GRP,
/* 123 */ LED_GAME_GRP,
/* 124 */ LED_NONE_GRP,
};

/* group_map - Defines virtual LEDs (groups of leds) 
 */

int16_t led_groups[] = {
/* 101 */ LED_KICKER_GRP, // Logical leds start here
/* 102 */ LED_UPLANE_GRP,
/* 103 */ LED_OXO_GRP,
/* 104 */ LED_BUMPER_GRP,
/* 105 */ LED_BUMPER_A_GRP,
/* 106 */ LED_BUMPER_B_GRP,
/* 107 */ LED_BUMPER_C_GRP,
/* 108 */ LED_TARGET_GRP,
/* 109 */ LED_COUNTER0_GRP,
/* 110 */ LED_COUNTER1_GRP,
/* 111 */ LED_COUNTER2_GRP,
/* 112 */ LED_COUNTER3_GRP,
/* 113 */ COIL_COUNTER_GRP,
/* 114 */ LED_AGAIN_GRP,
/* 115 */ LED_OXO_ANI_GRP,
/* 116 */ LED_HEADBOX_GRP,
/* 117 */ LED_FLIPPER_GRP,
/* 118 */ LED_SLINGSHOT_GRP,
/* 119 */ LED_CHIME_GRP,
/* 120 */ LED_PLAYERUP_GRP,
/* 121 */ LED_BALLUP_GRP,
/* 122 */ LED_PLAYERS_GRP,
/* 123 */ LED_GAME_GRP,
/* 124 */ LED_NONE_GRP,
  -1,
  
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
    LED_OXO_O, LED_OXO_X, LED_TRIPLE_BONUS, VLED_COLLECTING, -1,
  LED_BUMPER_GRP,
    LED_BUMPER_A, LED_BUMPER_B, LED_BUMPER_C, LED_TARGET_A, LED_TARGET_B, COIL_SAVE_GATE, -1,  
  LED_COUNTER0_GRP,
    COIL_COUNTER0_10K, COIL_COUNTER0_1K, COIL_COUNTER0_100, COIL_COUNTER0_10, LED_UP1, VLED_COUNTER0, -1,
  LED_COUNTER1_GRP,
    COIL_COUNTER1_10K, COIL_COUNTER1_1K, COIL_COUNTER1_100, COIL_COUNTER1_10, LED_UP2, VLED_COUNTER1, -1,
  LED_COUNTER2_GRP,
    COIL_COUNTER2_10K, COIL_COUNTER2_1K, COIL_COUNTER2_100, COIL_COUNTER2_10, LED_UP3, VLED_COUNTER2, -1,
  LED_COUNTER3_GRP,
    COIL_COUNTER3_10K, COIL_COUNTER3_1K, COIL_COUNTER3_100, COIL_COUNTER3_10, LED_UP4, VLED_COUNTER3, -1,
  COIL_COUNTER_GRP,
    COIL_COUNTER0_10K, COIL_COUNTER0_1K, COIL_COUNTER0_100, COIL_COUNTER0_10,
    COIL_COUNTER1_10K, COIL_COUNTER1_1K, COIL_COUNTER1_100, COIL_COUNTER1_10,
    COIL_COUNTER2_10K, COIL_COUNTER2_1K, COIL_COUNTER2_100, COIL_COUNTER2_10,
    COIL_COUNTER3_10K, COIL_COUNTER3_1K, COIL_COUNTER3_100, COIL_COUNTER3_10,
    -1,
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
  LED_GAME_GRP,
    LED_EXTRA, VLED_COLLECTING, LED_AGAIN0, VLED_COUNTER0, VLED_COUNTER1, VLED_COUNTER2, VLED_COUNTER3, -1,
  LED_NONE_GRP, -1,
  
  -1, 
};

const char led_group_list[] = R""""(
 led_kicker_grp, coil_kicker_l, coil_kicker_r, led_kicker_l, led_kicker_r;
  led_uplane_grp, led_uplane_l, led_uplane_r;
  led_target_grp, led_target_a, led_target_b;
  led_oxo_grp, 
    led_oxo_1a, led_oxo_1b, led_oxo_1c, 
    led_oxo_2a, led_oxo_2b, led_oxo_2c, 
    led_oxo_3a, led_oxo_3b, led_oxo_3c, 
    led_oxo_4a, led_oxo_4b, led_oxo_4c, 
    led_oxo_5a, led_oxo_5b, led_oxo_5c, 
    led_oxo_6a, led_oxo_6b, led_oxo_6c, 
    led_oxo_7a, led_oxo_7b, led_oxo_7c, 
    led_oxo_8a, led_oxo_8b, led_oxo_8c, 
    led_oxo_9a, led_oxo_9b, led_oxo_9c, 
    led_oxo_o, led_oxo_x, led_triple_bonus, vled_collecting;
  led_bumper_grp, led_bumper_a, led_bumper_b, led_bumper_c, led_target_a, led_target_b, coil_save_gate;
  led_counter0_grp, coil_counter0_10k, coil_counter0_1k, coil_counter0_100, coil_counter0_10, led_up1, vled_counter0;
  led_counter1_grp, coil_counter1_10k, coil_counter1_1k, coil_counter1_100, coil_counter1_10, led_up2, vled_counter1;
  led_counter2_grp, coil_counter2_10k, coil_counter2_1k, coil_counter2_100, coil_counter2_10, led_up3, vled_counter2;
  led_counter3_grp, coil_counter3_10k, coil_counter3_1k, coil_counter3_100, coil_counter3_10, led_up4, vled_counter3;
  coil_counter_grp, 
    coil_counter0_10k, coil_counter0_1k, coil_counter0_100, coil_counter0_10, 
    coil_counter1_10k, coil_counter1_1k, coil_counter1_100, coil_counter1_10, 
    coil_counter2_10k, coil_counter2_1k, coil_counter2_100, coil_counter2_10, 
    coil_counter3_10k, coil_counter3_1k, coil_counter3_100, coil_counter3_10;
  led_again_grp, led_again0, led_again1, led_again2;
  led_oxo_ani_grp, led_oxo_ani0, led_oxo_ani1, led_oxo_ani2;
  led_headbox_grp, led_playerup_grp, led_players_grp, led_ballup_grp, led_again_grp, led_oxo_ani_grp, led_game_over, led_tilt;
  led_flipper_grp, coil_flipper_l, coil_flipper_r;
  led_bumper_a_grp, coil_bumper_a, led_bumper_a;
  led_bumper_b_grp, coil_bumper_b, led_bumper_b;
  led_bumper_c_grp, coil_bumper_c, led_bumper_c;
  led_slingshot_grp, coil_sling_l, coil_sling_r;
  led_chime_grp, coil_chime0, coil_chime1, coil_chime2;
  led_players_grp, led_play1, led_play2, led_play3, led_play4, led_play4, led_play4;
  led_playerup_grp, led_up1, led_up2, led_up3, led_up4, led_up4, led_up4;
  led_ballup_grp, led_ball1, led_ball2, led_ball3, led_ball4, led_ball5, led_ball5;
  led_game_grp, led_extra, vled_collecting, led_again0, vled_counter0, vled_counter1, vled_counter2, vled_counter3;
  led_none_grp;
)"""";
  
