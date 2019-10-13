#include "Singularity.h"
#include "hardware.h"
#include "switches.h"
#include "leds.h"
#include "profiles.h"
#include "firmware_custom.h"
#include "freeram.hpp"

using namespace standard_firmware;
using namespace custom_firmware; 

#define NUMBER_OF_BALLS 3

IO io;
Atm_led_matrix leds; 
Atm_switch_matrix playfield;

void setup() {
  delay( 1000 );
  Serial.println( "Singularity framework\ninit IO" );
  delay( 100 );

  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 1 )
    .addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    .addStrip( new IO_Adafruit_NeoPixel( 12, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P2/cabinet with 8 virtual leds added 
    .addStrip( new IO_Adafruit_NeoPixel( 36, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 36 pixel SK6812 led strip on P3/headbox
    .invert( BALL_ENTER )
    .retrigger()
    .show();

  Serial.println( "init leds" ); delay( 100 );
  leds.begin( io, led_groups )
    .readProfiles( 'L', profiles );  
  
  Serial.println( "init playfield" ); delay( 1000 );
  playfield.begin( io, leds, switch_groups, LED_EXTRA )
    .readProfiles( 'S', profiles );

  Serial.printf( "Physical leds: %d (0..%d)\n", io.numberOfLeds(), io.numberOfLeds() - 1 );
  Serial.printf( "Logical leds: %d (%d..%d)\n", leds.numberOfGroups(), io.numberOfLeds(), io.numberOfLeds() + leds.numberOfGroups() - 1 );
  Serial.printf( "Physical switches: %d (1..%d)\n", io.numberOfSwitches(), io.numberOfSwitches() );
  Serial.printf( "Logical switches: %d (%d..%d)\n", playfield.numberOfGroups(), io.numberOfSwitches() + 1, io.numberOfSwitches() + playfield.numberOfGroups() );
  
  Serial.println( "init devices" ); delay( 100 );

  playfield.device( CHIMES, LED_CHIME_GRP, ledbank_firmware );
  playfield.device( COUNTER3, LED_COUNTER3_GRP, counter_em4d1w_firmware );
  playfield.device( COUNTER2, LED_COUNTER2_GRP, counter_em4d1w_firmware ).chain( COUNTER3 );
  playfield.device( COUNTER1, LED_COUNTER1_GRP, counter_em4d1w_firmware ).chain( COUNTER2 );
  playfield.device( COUNTER, LED_COUNTER0_GRP, counter_em4d1w_firmware ).chain( COUNTER1 )
  //   .onEvent( OUT_CTR_DIGIT1, CHIMES, IN_LBANK_ON0 )
  //   .onEvent( OUT_CTR_DIGIT2, CHIMES, IN_LBANK_ON1 )
  //   .onEvent( OUT_CTR_DIGIT3, CHIMES, IN_LBANK_ON2 )
  ;

  automaton.delay( 1000 ); // Visible reset indicator... (GI fades off/on)
  
  playfield.device( OXO, LED_OXO_GRP, tictactoe_firmware )
    .onEvent( OUT_OXO_WIN_ROW, KICKER, IN_KICKER_ON )
    .onEvent( OUT_OXO_WIN_ALL, UPLANE, IN_COMBO_ON )
    .onEvent( OUT_OXO_COLLECT, COUNTER, IN_CTR_PT1000 );

  playfield.device( MULTILANE, -1, switchbank_firmware ) 
    .onEvent( OUT_SBANK_PRESS0, OXO, IN_OXO_1O )
    .onEvent( OUT_SBANK_PRESS1, OXO, IN_OXO_1X )
    .onEvent( OUT_SBANK_PRESS2, OXO, IN_OXO_2O )
    .onEvent( OUT_SBANK_PRESS3, OXO, IN_OXO_2X )
    .onEvent( OUT_SBANK_PRESS4, OXO, IN_OXO_3O )
    .onEvent( OUT_SBANK_PRESS5, OXO, IN_OXO_3X )
    .onEvent( OUT_SBANK_SCORE, COUNTER, IN_CTR_PT1000 );

  playfield.device( BUMPER_A, LED_BUMPER_A_GRP, bumper_firmware )
    .onEvent( OUT_BUMPER_SCORE_LIT, COUNTER, IN_CTR_PT100 )
    .onEvent( OUT_BUMPER_SCORE_UNLIT, COUNTER, IN_CTR_PT10 );  
  
  playfield.device( BUMPER_B, LED_BUMPER_B_GRP, bumper_firmware )
    .onEvent( OUT_BUMPER_SCORE_LIT, COUNTER, IN_CTR_PT100 )
    .onEvent( OUT_BUMPER_SCORE_UNLIT, COUNTER, IN_CTR_PT10 );  
    
  playfield.device( BUMPER_C, LED_BUMPER_C_GRP, bumper_firmware )
    .onEvent( OUT_BUMPER_SCORE_LIT, COUNTER, IN_CTR_PT1000 )
    .onEvent( OUT_BUMPER_SCORE_UNLIT, COUNTER, IN_CTR_PT100 )  
    .onEvent( OUT_BUMPER_LIGHT_ON, SAVE_GATE, IN_LBANK_ON )
    .onEvent( OUT_BUMPER_LIGHT_OFF, SAVE_GATE, IN_LBANK_OFF );

  playfield.device( DUAL_TARGET, LED_TARGET_GRP, dual_target_firmware )
    .onEvent( OUT_TARGET_LED0_ON, BUMPER_A, IN_BUMPER_LIGHT_ON )
    .onEvent( OUT_TARGET_LED0_OFF, BUMPER_A, IN_BUMPER_LIGHT_OFF )
    .onEvent( OUT_TARGET_LED1_ON, BUMPER_B, IN_BUMPER_LIGHT_ON )
    .onEvent( OUT_TARGET_LED1_OFF, BUMPER_B, IN_BUMPER_LIGHT_OFF )
    .onEvent( OUT_TARGET_ALL_ON, BUMPER_C, IN_BUMPER_LIGHT_ON )  
    .onEvent( OUT_TARGET_ALL_OFF, BUMPER_C, IN_BUMPER_LIGHT_OFF )
    .onEvent( OUT_TARGET_SCORE, COUNTER, IN_CTR_PT1000 );
  
  playfield.device( KICKER, LED_KICKER_GRP, dual_kicker_firmware )
    .onEvent( OUT_KICKER_PRESS_LIT, AGAIN, IN_LBANK_ON )  
    .onEvent( OUT_KICKER_SCORE_LIT, COUNTER, IN_CTR_PT5000 )
    .onEvent( OUT_KICKER_SCORE_UNLIT, COUNTER, IN_CTR_PT500 );
  
  playfield.device( UPLANE, LED_UPLANE_GRP, dual_combo_firmware )
    .onEvent( OUT_COMBO_SCORE, COUNTER, IN_CTR_PT1000 )
    .onEvent( OUT_COMBO_PRESS_LIT, AGAIN, IN_LBANK_ON )
    .onEvent( OUT_COMBO_PRESS0_UNLIT, OXO, IN_OXO_4 )
    .onEvent( OUT_COMBO_PRESS1_UNLIT, OXO, IN_OXO_6 );

  playfield.device( SLINGSHOT, LED_SLINGSHOT_GRP, dual_kicker_firmware )
    .onEvent( OUT_KICKER_SCORE, COUNTER, IN_CTR_PT10 )
    .onEvent( OUT_KICKER_PRESS, OXO, IN_OXO_TOGGLE );    

  playfield.device( LOWER, -1, switchbank_firmware ) 
    .onEvent( OUT_SBANK_PRESS0, OXO, IN_OXO_5 )                  // 0 TARGET_C
    .onEvent( OUT_SBANK_SCORE0, COUNTER, IN_CTR_PT500 )  
    .onEvent( OUT_SBANK_PRESS1, OXO, IN_OXO_7 )                  // 1 INLANE_L
    .onEvent( OUT_SBANK_SCORE1, COUNTER, IN_CTR_PT1000 )
    .onEvent( OUT_SBANK_PRESS2, OXO, IN_OXO_9 )                  // 2 INLANE_R
    .onEvent( OUT_SBANK_SCORE2, COUNTER, IN_CTR_PT1000 )
    .onEvent( OUT_SBANK_PRESS3, OXO, IN_OXO_8 )                  // 3 ROLLOVER
    .onEvent( OUT_SBANK_SCORE3, COUNTER, IN_CTR_PT500 )
    .onEvent( OUT_SBANK_SCORE4, COUNTER, IN_CTR_PT1000 )         // 4 OUTLANE
    .onEvent( OUT_SBANK_PRESS5, playfield, playfield.EVT_READY ) // 5 BALL_EXIT
    .onEvent( OUT_SBANK_PRESS6, DUAL_TARGET, IN_TARGET_CLEAR )   // 6 BALL_ENTER 
    .onEvent( OUT_SBANK_PRESS7, PLAYERS, IN_SCALAR_ADVANCE );    // 7 FRONTBTN

  playfield.device( FLIPPER, LED_FLIPPER_GRP, dual_flipper_firmware );    
  playfield.device( AGAIN, LED_AGAIN_GRP, ledbank_firmware );
  playfield.device( SAVE_GATE, COIL_SAVE_GATE, ledbank_firmware );
  playfield.device( FEEDER, COIL_FEEDER, ledbank_firmware );
  playfield.device( GAME_OVER, LED_GAME_OVER, ledbank_firmware );  
  playfield.device( PLAYERS, LED_PLAYERS_GRP, scalar_firmware );
  playfield.device( PLAYERUP, LED_PLAYERUP_GRP, scalar_firmware );
  playfield.device( BALLUP, LED_BALLUP_GRP, scalar_firmware );
  playfield.device( GI, COIL_GI, ledbank_firmware, 1 );   // Default ON
  
  playfield.device( KICKER ).trigger( IN_KICKER_PERSIST );
  playfield.device( GAME_OVER ).trigger( IN_LBANK_ON );
  Serial.println( FreeRam() );
  
  playfield.disable();     

/*
  // FRONTBTN, LED_PLAYERS_GRP, AGAIN0, VLED_COUNTER0, VLED_COUNTER1, VLED_COUNTER2, VLED_COUNTER3, EXTRA_LED, VLED_COLLECTING
  playfield.device( GAME, LED_GAME_GRP, game_firmware )
    .onEvent( OUT_GAME_INIT, playfield, playfield.EVT_INIT )
    .onEvent( OUT_GAME_ENABLE, playfield, playfield.EVT_ENABLE )
    .onEvent( OUT_GAME_RESET, COUNTER, IN_CTR_RESET )
    .onEvent( OUT_GAME_PLAYERS_ZERO, PLAYERS, IN_SCALAR_ZERO )
    .onEvent( OUT_GAME_BALL_ZERO, BALLUP, IN_SCALAR_ZERO )
    .onEvent( OUT_GAME_PLAYER_ZERO, PLAYERUP, IN_SCALAR )
    .onEvent( OUT_GAME_BALL_ADV, BALLUP, IN_SCALAR_ADVANCE )
    .onEvent( OUT_GAME_PLAYER_ADV, PLAYERUP, IN_SCALAR_ADVANCE )
    .onEvent( OUT_GAME_3BONUS, OXO, IN_OXO_TRIPLE )
    .onEvent( OUT_GAME_COLLECT, OXO, IN_OXO_COLLECT )
    .onEvent( OUT_GAME_KICKOFF, FEEDER, IN_LBANK_ON )
    .onEvent( OUT_GAME_PLAYERS_FIX, PLAYERS, IN_SCALAR_ZERO )
    .onEvent( OUT_GAME_OVER, GAME_OVER, IN_LBANK_ON );
*/    

}

void loop() {
  automaton.run(); 
  if ( playfield.isPressed( FRONTBTN ) ) {
    playfield.trigger( playfield.EVT_INIT );    
    playfield.device( COUNTER ).trigger( IN_CTR_RESET );
    playfield.device( PLAYERS ).trigger( IN_SCALAR_ZERO );
    while ( playfield.device( COUNTER ).state() ) automaton.run();
    automaton.delay( 500 );
    for ( int ball = 0; ball < NUMBER_OF_BALLS; ball++ ) {      
      for ( int player = 0; player < playfield.device( PLAYERS ).state( 1 ) + 1; player++ ) {
        do {
          //automaton.delay( 500 ); // replaces the other two
          playfield.trigger( playfield.EVT_INIT );
          playfield.device( BALLUP ).trigger( IN_SCALAR_SEL0 + ball );
          playfield.device( PLAYERUP ).trigger( IN_SCALAR_SEL0 + player );
          playfield.device( OXO ).trigger( ball == NUMBER_OF_BALLS - 1 ? IN_OXO_TRIPLE : IN_OXO_SINGLE );
          playfield.device( FEEDER ).trigger( IN_LBANK_ON );
          automaton.delay( 500 ); 
          playfield.trigger( playfield.EVT_ENABLE );
          while ( playfield.enabled() ) automaton.run();            
          playfield.device( OXO ).trigger( IN_OXO_COLLECT );
          while ( playfield.device( OXO ).state() ) automaton.run(); 
          automaton.delay( 500 );
          playfield.device( PLAYERS ).trigger( IN_SCALAR_FREEZE );
        } while ( playfield.device( AGAIN ).state() ); 
      } 
    } 
    playfield.device( GAME_OVER ).trigger( IN_LBANK_ON );
  }
}
