#pragma once

// Note that all symbols must be longer than 1 character (or they'll be confused with opcodes)

const char switch_symbols[] = 
  "null_sw, target_b, port_3x, port_3o, port_2x, port_2o, port_1x, port_1o, target_a, kicker_l, uplane_l," 
  "bumper_a, bumper_c, kicker_r, bumper_b, uplane_r, target_c, outlane, inlane_l, sling_l, sling_r," 
  "rollover, inlane_r, ball_enter, ball_exit, tilt_pend, tilt_ramp, switch213, flipper_l, flipper_r," 
  "switch216, switch217, frontbtn, counter2, counter0, switch313, switch314, switch315, switch316," 
  "counter1, counter3, save_gate, multilane, dual_target, oxo, gi, kicker, flipper, uplane, slingshot," 
  "lower, feeder, again, chimes, players, playerup, ballup, animation, game_over, switches, game"; 

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
    switch213, flipper_l, flipper_r, switch216, switch217, frontbtn, counter2, counter0, switch313, switch314, switch315, switch316, counter1, counter3;
  game, frontbtn;
)"""";
