
  io.attach( 0, 3, new IO_Adafruit_NeoPixel( 53, pin_data, IO_Adafruit_NeoPixel::str2int( "neo_grbw" ) + IO_Adafruit_NeoPixel::str2int( "neo_khz800" ) ) );
  io.attach( 1, 1, new IO_Adafruit_NeoPixel( 12, pin_data, IO_Adafruit_NeoPixel::str2int( "neo_grbw" ) + IO_Adafruit_NeoPixel::str2int( "neo_khz800" ) ) );
  io.attach( 2, 1, new IO_Adafruit_NeoPixel( 36, pin_data, IO_Adafruit_NeoPixel::str2int( "neo_grbw" ) + IO_Adafruit_NeoPixel::str2int( "neo_khz800" ) ) );
  io.retrigger();
  io.show();

  leds.begin( io );
  playfield.begin( io, leds );

  leds.loadSymbols( led_symbols );
  leds.loadGroups( led_group_list  );

  playfield.loadSymbols( switch_symbols );
  playfield.loadGroups( switch_group_list );

  io.invert( playfield.findSymbol( "ball_enter" ) );

  Serial.println( "led profiles" ); //delay( 100 );

  //                                   T0,  L1,  T1,  L2  
  leds.profile( "led_kicker_l"     ,    0,   0,   0, 127 );
  leds.profile( "led_kicker_r"     ,    0,   0,   0, 127 );
  leds.profile( "led_target_grp"   ,    0,   0,   0, 127 );
  leds.profile( "led_uplane_grp"   ,    0,   0,   0, 127 );
  leds.profile( "led_bumper_a"     ,    0,   0,   0, 127 );
  leds.profile( "led_bumper_b"     ,    0,   0,   0, 127 );
  leds.profile( "led_bumper_c"     ,    0,   0,   0, 127 );
  leds.profile( "led_again0"       ,    0,   0,   0, 127 );
  leds.profile( "led_triple_bonus" ,    0,   0,   0, 127 );
  leds.profile( "led_extra"        ,    0,   0,   0, 127 );
  leds.profile( "led_headbox_grp"  ,    0,   0,   0, 255 );
  leds.profile( "led_oxo_grp"      ,    0,   0,   0, 127 );
  leds.profile( "coil_gi"          ,    0,   0,   0, 255 );
  leds.profile( "coil_sling_r"     ,    0, 255,  30,   0 );
  leds.profile( "coil_sling_l"     ,    0, 255,  30,   0 );
  leds.profile( "coil_flipper_l"   ,    0,   0,   0, 255 );
  leds.profile( "coil_flipper_r"   ,    0,   0,   0, 255 );
  leds.profile( "coil_kicker_r"    , 1000,  95,  30,   0 );
  leds.profile( "coil_kicker_l"    , 1000,  95,  30,   0 );
  leds.profile( "coil_save_gate"   ,    0,   0,   0, 255 );
  leds.profile( "coil_bumper_a"    ,    0, 255,  40,   0 );
  leds.profile( "coil_bumper_b"    ,    0, 255,  40,   0 );
  leds.profile( "coil_bumper_c"    ,    0, 255,  40,   0 );
  leds.profile( "coil_feeder"      ,    0, 127,  30,   0 );
  leds.profile( "coil_counter_grp" ,    0, 180,  20,   0 );
  leds.profile( "vled_counter0"    ,    0,   0,   0, 127 );
  leds.profile( "vled_counter1"    ,    0,   0,   0, 127 );
  leds.profile( "vled_counter2"    ,    0,   0,   0, 127 );
  leds.profile( "vled_counter3"    ,    0,   0,   0, 127 );
  leds.profile( "vled_collecting"  ,    0,   0,   0, 127 );
  leds.profile( "vled_5"           ,    0,   0,   0, 127 );
  leds.profile( "vled_6"           ,    0,   0,   0, 127 );
  leds.profile( "vled_7"           ,    0,   0,   0, 127 );
  
  Serial.println( "switch profiles" ); //delay( 100 );
  //                                    press,  rel,  thr,  sep
  playfield.profile( "switches"       ,   200,    0,    0,    0 );  // Default for switches
  playfield.profile( "multilane"      ,   200,  200,    0,    0 );
  playfield.profile( "sling_l"        ,     5,    0, 5000,    0 );  // Slingshots
  playfield.profile( "sling_r"        ,     5,    0, 5000,    0 );
  playfield.profile( "bumper_a"       ,     0,    0, 2000,    0 );  // Bumpers
  playfield.profile( "bumper_b"       ,     0,    0, 2000,    0 );
  playfield.profile( "bumper_c"       ,     0,    0, 2000,    0 );
  playfield.profile( "flipper"        ,     0,   10,    0,    0 );  // Flippers
  playfield.profile( "counter0"       ,     0,  200,    0,    0 );  // Counter sensors
  playfield.profile( "counter1"       ,     0,  200,    0,    0 );
  playfield.profile( "counter2"       ,     0,  200,    0,    0 );
  playfield.profile( "counter3"       ,     0,  200,    0,    0 );
  playfield.profile( "port_3x"        ,     5,    0, 5000,    0 );
  playfield.profile( "port_3o"        ,     5,    0, 5000,    0 );
  playfield.profile( "port_2x"        ,     5,    0, 5000,    0 );
  playfield.profile( "port_2o"        ,     5,    0, 5000,    0 );
  playfield.profile( "port_1x"        ,     5,    0, 5000,    0 );
  playfield.profile( "port_1o"        ,     5,    0, 5000,    0 );
  playfield.profile( "uplane_l"       ,     5,    0, 5000,    0 );
  playfield.profile( "uplane_r"       ,     5,    0, 5000,    0 );
  playfield.profile( "outlane"        ,     5,    0, 5000,    0 );
  playfield.profile( "inlane_l"       ,     5,    0, 5000,    0 );
  playfield.profile( "inlane_r"       ,     5,    0, 5000,    0 );
  playfield.profile( "ball_enter"     ,     5,    0, 5000,    0 );
  playfield.profile( "rollover"       ,     1,    0, 5000,    0 );
  playfield.profile( "target_a"       ,     2,    0,    0,    0 );
  playfield.profile( "target_b"       ,     2,    0,    0,    0 );
  playfield.profile( "target_c"       ,     2,    0,    0,    0 );
  playfield.profile( "ball_exit"      ,    50,    0, 5000,    0 );
  playfield.profile( "ball_enter"     ,   200,    0,    0,    0 );
  playfield.profile( "frontbtn"       ,     0,   50, 5000,    0 );

  Serial.println( "init devices" ); //delay( 100 );

  playfield.device( "chimes", "led_chime_grp", lib.code( "std_ledbank" ) );
  playfield.device( "counter0", "led_counter0_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "counter1", "led_counter1_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "counter2", "led_counter2_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "counter3", "led_counter3_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "bumper_a", "led_bumper_a_grp", lib.code( "std_bumper" ) );
  playfield.device( "bumper_b", "led_bumper_b_grp", lib.code( "std_bumper" ) );
  playfield.device( "bumper_c", "led_bumper_c_grp", lib.code( "std_bumper" ) );
  playfield.device( "oxo", "led_oxo_grp", lib.code( "std_tictactoe" ) );
  playfield.device( "multilane", "led_none_grp", lib.code( "std_switchbank" ) ); 
  playfield.device( "dual_target", "led_target_grp", lib.code( "std_dual_target" ) );
  playfield.device( "kicker", "led_kicker_grp", lib.code( "std_dual_kicker" ) );
  playfield.device( "uplane", "led_uplane_grp", lib.code( "std_dual_combo" ) ); 
  playfield.device( "slingshot", "led_slingshot_grp", lib.code( "std_dual_slingshot" ) );
  playfield.device( "lower", "led_none_grp", lib.code( "std_switchbank" ) ); 
  playfield.device( "flipper", "led_flipper_grp", lib.code( "std_dual_flipper" ) ); 
  playfield.device( "again", "led_again_grp", lib.code( "std_ledbank" ) );
  playfield.device( "save_gate", "coil_save_gate", lib.code( "std_ledbank" ) );
  playfield.device( "feeder", "coil_feeder", lib.code( "std_ledbank" ) );
  playfield.device( "game_over", "led_game_over", lib.code( "std_ledbank" ) );  
  playfield.device( "players", "led_players_grp", lib.code( "std_scalar" ) );
  playfield.device( "playerup", "led_playerup_grp", lib.code( "std_scalar" ) );
  playfield.device( "ballup", "led_ballup_grp", lib.code( "std_scalar" ) );
  playfield.device( "gi", "coil_gi", lib.code( "std_ledbank" ), 1 ); // default on
  playfield.device( "game", "led_game_grp", lib.code( "std_game" ), NUMBER_OF_BALLS, NUMBER_OF_PLAYERS );
  playfield.device( "animation", "led_oxo_ani_grp", lib.code( "std_animation" ) );
  
  Serial.println( "chain devices" ); //delay( 100 );

  playfield.device( "counter0" ).chain( "counter1" ).chain( "counter2" ).chain( "counter3" ); 

  Serial.println( "link devices" ); //delay( 100 );

  playfield.link( "counter0", "out_digit1", "chimes", "on0" ); // link() propagates through chain
  playfield.link( "counter0", "out_digit2", "chimes", "on1" );
  playfield.link( "counter0", "out_digit3", "chimes", "on2" );
  
  playfield.link( "oxo", "out_win_row", "kicker", "on" );
  playfield.link( "oxo", "out_win_all", "uplane", "on" );
  playfield.link( "oxo", "out_collect", "counter0", "pt1000" );
   
  playfield.link( "multilane", "out_press0", "oxo", "oxo_1o" ); 
  playfield.link( "multilane", "out_press1", "oxo", "oxo_1x" );
  playfield.link( "multilane", "out_press2", "oxo", "oxo_2o" );
  playfield.link( "multilane", "out_press3", "oxo", "oxo_2x" );
  playfield.link( "multilane", "out_press4", "oxo", "oxo_3o" ); 
  playfield.link( "multilane", "out_press5", "oxo", "oxo_3x" );
  playfield.link( "multilane", "out_score", "counter0", "pt1000" );

  playfield.link( "bumper_a", "out_score_lit", "counter0", "pt100" );
  playfield.link( "bumper_a", "out_score_unlit", "counter0", "pt10" );  
  
  playfield.link( "bumper_b", "out_score_lit", "counter0", "pt100" );
  playfield.link( "bumper_b", "out_score_unlit", "counter0", "pt10" );  
    
  playfield.link( "bumper_c", "out_score_lit", "counter0", "pt1000" );
  playfield.link( "bumper_c", "out_score_unlit", "counter0", "pt100" );  
  playfield.link( "bumper_c", "out_light_on", "save_gate", "on" );
  playfield.link( "bumper_c", "out_light_off", "save_gate", "off" );

  playfield.link( "dual_target", "out_led0_on", "bumper_a", "turn_on" );
  playfield.link( "dual_target", "out_led0_off", "bumper_a", "turn_off" );
  playfield.link( "dual_target", "out_led1_on", "bumper_b", "turn_on" );
  playfield.link( "dual_target", "out_led1_off", "bumper_b", "turn_off" );
  playfield.link( "dual_target", "out_all_on", "bumper_c", "turn_on" );  
  playfield.link( "dual_target", "out_all_off", "bumper_c", "turn_off" );
  playfield.link( "dual_target", "out_score", "counter0", "pt1000" );
  
  playfield.link( "kicker", "out_press_lit", "again", "on" );
  playfield.link( "kicker", "out_score_lit", "counter0", "pt5000" );
  playfield.link( "kicker", "out_score_unlit", "counter0", "pt500" );
  
  playfield.link( "uplane", "out_score", "counter0", "pt1000" );
  playfield.link( "uplane", "out_press_lit", "again", "on" );
  playfield.link( "uplane", "out_press0_unlit", "oxo", "oxo_4" );
  playfield.link( "uplane", "out_press1_unlit", "oxo", "oxo_6" );

  playfield.link( "slingshot", "out_score", "counter0", "pt10" );
  playfield.link( "slingshot", "out_press", "oxo", "toggle" );    

  playfield.link( "lower", "out_press0", "oxo", "oxo_5" );                  // 0 target_c
  playfield.link( "lower", "out_score0", "counter0", "pt500" );  
  playfield.link( "lower", "out_press1", "oxo", "oxo_7" );                  // 1 inlane_l
  playfield.link( "lower", "out_score1", "counter0", "pt1000" );
  playfield.link( "lower", "out_press2", "oxo", "oxo_9" );                  // 2 inlane_r
  playfield.link( "lower", "out_score2", "counter0", "pt1000" );
  playfield.link( "lower", "out_press3", "oxo", "oxo_8" );                  // 3 rollover
  playfield.link( "lower", "out_score3", "counter0", "pt500" );
  playfield.link( "lower", "out_score4", "counter0", "pt1000" );            // 4 outlane
  playfield.link( "lower", "out_press5", "game", "ball_exit" );             // 5 ball_exit 
  playfield.link( "lower", "out_press6", "dual_target", "clear" );          // 6 ball_enter 
  
  playfield.link( "game", "out_init", playfield, "pf_init" );
  playfield.link( "game", "out_enable", playfield, "pf_enable" );

  playfield.link( "game", "out_counter_reset", "counter0", "reset" );
  playfield.link( "game", "out_ball_zero", "ballup", "zero" );
  playfield.link( "game", "out_player_zero", "playerup", "zero" );
  playfield.link( "game", "out_ball_adv", "ballup", "advance" );
  playfield.link( "game", "out_player_adv", "playerup", "advance" );
  playfield.link( "game", "out_3bonus", "oxo", "triple" );
  playfield.link( "game", "out_collect", "oxo", "collect" );
  playfield.link( "game", "out_kickoff", "feeder", "on" );
  playfield.link( "game", "out_players_zero", "players", "zero" );
  playfield.link( "game", "out_players_adv", "players", "advance" );
  playfield.link( "game", "out_over", "game_over", "on" );
  playfield.link( "game", "out_disable", playfield, "pf_ready" );
  playfield.link( "game", "out_release", "flipper", "release_all" );

  playfield.link( "game_over", "out_on0", "save_gate", "off" );
