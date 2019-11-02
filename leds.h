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
  
