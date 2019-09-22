#include "Singularity.h"
#include "firmware_custom.h"
#include "mapping.h"
#include "freeram.hpp"

#define NUMBER_OF_BALLS 5

IO io;
Atm_led_scheduler leds; // IO_LED
Atm_playfield playfield; // IO_MATRIX

Atm_em_counter counter[4]; 
Atm_score score;
Atm_timer animation[3];

using namespace standard_firmware;
using namespace custom_firmware; 

void setup() {
  delay( 1000 );
  Serial.println( "start" );
  delay( 100 );
    
  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 1 )
    .addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    .addStrip( new IO_Adafruit_NeoPixel(  4, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P2/cabinet DUMMY for now!!!
    .addStrip( new IO_Adafruit_NeoPixel( 36, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 36 pixel SK6812 led strip on P3/headbox
    .invert( BALL_ENTER )
    .retrigger()
    .show();

  leds.begin( io, led_group_definition, profile_definition );

  
  Serial.println( "init playfield" ); delay( 1000 );
//  playfield.trace( Serial );
  playfield.begin( io, leds, switch_group_definition ).debounce( 20, 20, 0 );

  score.begin()
    .addCounter( counter[0].begin( playfield, COUNTER0, COIL_COUNTER0_GRP, PROFILE_COUNTER ) ) // Initialize individual score counters and link them to the score object
    .addCounter( counter[1].begin( playfield, COUNTER1, COIL_COUNTER1_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[2].begin( playfield, COUNTER2, COIL_COUNTER2_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[3].begin( playfield, COUNTER3, COIL_COUNTER3_GRP, PROFILE_COUNTER ) ) 
    //.onDigit( 0, playfield.device( CHIMES, LED_CHIME_GRP, ledbank_firmware ), IN_LBANK_ON0 ) // Link digits to chimes
    //.onDigit( 1, playfield.device( CHIMES ), IN_LBANK_ON1 ) 
    //.onDigit( 2, playfield.device( CHIMES ), IN_LBANK_ON2 )
    ; 

  playfield
    .leds()
      .profile( LED_FLASHER_GRP, PROFILE_LED )
      .profile( LED_HEADBOX_GRP, PROFILE_BRIGHT );

  leds.profile( LED_OXO_GRP, PROFILE_OXO ); // Required!
  
  automaton.delay( 1000 ); // Visible reset indicator... (GI fades off/on)
   
  // Turn on the General Illumination
  leds.profile( COIL_GI, PROFILE_GI );
  playfield.device( GI, COIL_GI, ledbank_firmware ).trigger( IN_LBANK_ON );  

  // Playfield device instantiation

  playfield.device( OXO, LED_OXO_GRP, tictactoe_firmware )
    .onEvent( OUT_OXO_WIN_ROW, playfield.device( KICKER ), IN_KICKER_ON )
    .onEvent( OUT_OXO_WIN_ALL, playfield.device( UPLANE ), IN_COMBO_ON )
    .onEvent( OUT_OXO_COLLECT, score, score.EVT_1000 );

  playfield.device( MULTILANE, -1, switchbank_firmware ) 
    .onEvent( OUT_SBANK0, playfield.device( OXO ), IN_OXO_1O )
    .onEvent( OUT_SBANK1, playfield.device( OXO ), IN_OXO_1X )
    .onEvent( OUT_SBANK2, playfield.device( OXO ), IN_OXO_2O )
    .onEvent( OUT_SBANK3, playfield.device( OXO ), IN_OXO_2X )
    .onEvent( OUT_SBANK4, playfield.device( OXO ), IN_OXO_3O )
    .onEvent( OUT_SBANK5, playfield.device( OXO ), IN_OXO_3X )
    .onEvent( OUT_SBANK_SCORE, score, score.EVT_1000 );

  leds.profile( LED_TARGET_A, PROFILE_LED );
  leds.profile( LED_TARGET_B, PROFILE_LED );
  playfield.device( DUAL_TARGET, LED_TARGET_GRP, dual_target_firmware )
    .onEvent( OUT_TARGET_LED0_ON, playfield.device( BUMPER_A ), IN_BUMPER_LIGHT_ON )
    .onEvent( OUT_TARGET_LED0_OFF, playfield.device( BUMPER_A ), IN_BUMPER_LIGHT_OFF )
    .onEvent( OUT_TARGET_LED1_ON, playfield.device( BUMPER_B ), IN_BUMPER_LIGHT_ON )
    .onEvent( OUT_TARGET_LED1_OFF, playfield.device( BUMPER_B ), IN_BUMPER_LIGHT_OFF )
    .onEvent( OUT_TARGET_ALL_ON, playfield.device( BUMPER_C ), IN_BUMPER_LIGHT_ON )  
    .onEvent( OUT_TARGET_ALL_OFF, playfield.device( BUMPER_C ), IN_BUMPER_LIGHT_OFF )
    .onEvent( OUT_TARGET_SCORE, score, score.EVT_100 );
  
  leds.profile( LED_BUMPER_A, PROFILE_LED );
  playfield.device( BUMPER_A, LED_BUMPER_A_GRP, bumper_firmware )
    .onEvent( OUT_BUMPER_SCORE_LIT, score, score.EVT_100 )
    .onEvent( OUT_BUMPER_SCORE_UNLIT, score, score.EVT_10 );  
  
  leds.profile( LED_BUMPER_B, PROFILE_LED );
  playfield.device( BUMPER_B, LED_BUMPER_B_GRP, bumper_firmware )
    .onEvent( OUT_BUMPER_SCORE_LIT, score, score.EVT_100 )
    .onEvent( OUT_BUMPER_SCORE_UNLIT, score, score.EVT_10 );  
    
  leds.profile( LED_BUMPER_C, PROFILE_LED );
  playfield.device( BUMPER_C, LED_BUMPER_C_GRP, bumper_firmware )
    .onEvent( OUT_BUMPER_SCORE_LIT, score, score.EVT_1000 )
    .onEvent( OUT_BUMPER_SCORE_UNLIT, score, score.EVT_100 )  
    .onEvent( OUT_BUMPER_LIGHT_ON, playfield.device( SAVE_GATE ), IN_LBANK_ON )
    .onEvent( OUT_BUMPER_LIGHT_OFF, playfield.device( SAVE_GATE ), IN_LBANK_OFF );

  leds.profile( COIL_KICKER_L, PROFILE_KICKER );
  leds.profile( COIL_KICKER_R, PROFILE_KICKER );
  playfield.device( KICKER, LED_KICKER_GRP, dual_kicker_firmware )
    .onEvent( OUT_KICKER_KICK_LIT, playfield.device( AGAIN ), IN_LBANK_ON )  
    .onEvent( OUT_KICKER_SCORE_LIT, score, score.EVT_5000 )
    .onEvent( OUT_KICKER_SCORE_UNLIT, score, score.EVT_500 )
    .trigger( IN_KICKER_PERSIST ); 
  
  leds.profile( LED_UPLANE_L, PROFILE_LED );
  leds.profile( LED_UPLANE_R, PROFILE_LED );
  playfield.device( UPLANE, LED_UPLANE_GRP, dual_combo_firmware )
    .onEvent( OUT_COMBO_SCORE, score, score.EVT_1000 )
    .onEvent( OUT_COMBO_PRESS_LIT, playfield.device( AGAIN ), IN_LBANK_ON )
    .onEvent( OUT_COMBO_PRESS0_UNLIT, playfield.device( OXO ), IN_OXO_4 )
    .onEvent( OUT_COMBO_PRESS1_UNLIT, playfield.device( OXO ), IN_OXO_6 );

  playfield.debounce( SLING_L, 20, 200, 0 );
  playfield.debounce( SLING_R, 20, 200, 0 );
  playfield.device( SLINGSHOT, LED_SLINGSHOT_GRP, dual_kicker_firmware )
    .onEvent( OUT_KICKER_SCORE, score, score.EVT_10 )
    .onEvent( OUT_KICKER_KICK, playfield.device( OXO ), IN_OXO_TOGGLE );    

  // Simple switches and things they trigger
  
  playfield.device( LOWER, -1, switchbank_firmware ) 
    .onEvent( OUT_SBANK0, playfield.device( OXO ), IN_OXO_5 )                   // 0 TARGET_C
    .onEvent( OUT_SBANK_SCORE0, score, score.EVT_500 )  
    .onEvent( OUT_SBANK1, playfield.device( OXO ), IN_OXO_7 )                   // 1 INLANE_L
    .onEvent( OUT_SBANK_SCORE1, score, score.EVT_1000 )
    .onEvent( OUT_SBANK2, playfield.device( OXO ), IN_OXO_9 )                   // 2 INLANE_R
    .onEvent( OUT_SBANK_SCORE2, score, score.EVT_1000 )
    .onEvent( OUT_SBANK3, playfield.device( OXO ), IN_OXO_8 )                   // 3 ROLLOVER
    .onEvent( OUT_SBANK_SCORE3, score, score.EVT_500 )
    .onEvent( OUT_SBANK_SCORE4, score, score.EVT_1000 )                         // 4 OUTLANE
    .onEvent( OUT_SBANK5, playfield, playfield.EVT_READY )                      // 5 BALL_EXIT
    .onEvent( OUT_SBANK6, playfield.device( DUAL_TARGET ), IN_TARGET_CLEAR )    // 6 BALL_ENTER (physically disabled for now)
    .onEvent( OUT_SBANK7, playfield.device( PLAYERS ), IN_SCALAR_ADVANCE );     // 7 FRONTBTN ( .trigger( IN_PERSIST7 )? )

  playfield.debounce( FLIPPER_L, 5, 0, 0 );    
  playfield.debounce( FLIPPER_R, 5, 0, 0 );    
  leds.profile( LED_FLIPPER_GRP, PROFILE_FLIPPER );
  playfield.device( FLIPPER, LED_FLIPPER_GRP, dual_flipper_firmware );    

  leds.profile( LED_AGAIN_GRP, PROFILE_LED );
  playfield.device( AGAIN, LED_AGAIN_GRP, ledbank_firmware );

  leds.profile( COIL_SAVE_GATE, PROFILE_GATE );
  playfield.device( SAVE_GATE, COIL_SAVE_GATE, ledbank_firmware );

  Serial.println( FreeRam() );

  leds.on( LED_GAME_OVER );

  leds.profile( COIL_FEEDER, PROFILE_FEEDER );

  
/*
  animation[0].begin( 500 ).onTimer( [] ( int idx, int v, int up ) { leds.toggle( LED_OXO_ANI0 ); }).repeat().start(); // leds.blink( LED_OXO_ANI0, 500 );???
  animation[1].begin( 350 ).onTimer( [] ( int idx, int v, int up ) { leds.toggle( LED_OXO_ANI1 ); }).repeat().start();
  animation[2].begin( 600 ).onTimer( [] ( int idx, int v, int up ) { leds.toggle( LED_OXO_ANI2 ); }).repeat().start();
*/
  // leds.profile( LED_GAME_OVER, PROFILE_BLINK ).on( LED_GAME_OVER );

  playfield.disable();     
  
  playfield.device( PLAYERS, LED_PLAYERS_GRP, scalar_firmware );
  playfield.device( PLAYERUP, LED_PLAYERUP_GRP, scalar_firmware );
  playfield.device( BALLUP, LED_BALLUP_GRP, scalar_firmware );

//  playfield.device( ANIMATION ).trace( Serial );
//  playfield.device( ANIMATION, LED_OXO_ANI_GRP, animation_firmware, 50 );
 // automaton.delay( 1000 );
  //playfield.device( ANIMATION ).trigger( IN_ANI_CYCLE );

  //playfield.device( ANIMATION )
   // .select( Atm_led_device::DEV_SELECT_ALL );

  automaton.delay( 500 );
  
}


void loop() {
  automaton.run(); // <<<<<<<<<< IDLE
  if ( io.isPressed( FRONTBTN ) ) {
    score.reset();
    playfield.device( PLAYERS ).trigger( IN_SCALAR_INIT );
    leds.off( LED_FLASHER_GRP );
    Serial.printf( "%d Counter reset\n", millis() );
    while ( score.state() ) automaton.run(); // <<<<<<<<<<< RESETTING COUNTERS
    automaton.delay( 1000 );
    for ( int ball = 0; ball < NUMBER_OF_BALLS; ball++ ) {      
      for ( int player = 0; player < playfield.device( PLAYERS ).state() + 1; player++ ) {
        do {
          score.select( player );
          leds.off( LED_FLASHER_GRP );
          playfield.device( BALLUP ).trigger( IN_SCALAR_SEL0 + ball );
          playfield.device( PLAYERUP ).trigger( IN_SCALAR_SEL0 + player );
          if ( ball == 4 ) {
            Serial.printf( "%d Triple bonus!\n", millis() );
            playfield.device( OXO ).trigger( IN_OXO_TRIPLE );
            automaton.delay( 100 ); // WEG!!!
          }
          Serial.printf( "%d Serve player %d, ball %d\n", millis(), player, ball );
          leds.on( COIL_FEEDER );
          playfield.enable();
          automaton.delay( 500 ); // was not needed before device conversion...
          while ( playfield.enabled() ) automaton.run(); // <<<<<<<<<< PLAYING
          Serial.printf( "%d Ball play finished, bonus collect %d\n", millis(), playfield.device( OXO ).state() );  
          playfield.device( OXO ).trigger( IN_OXO_COLLECT );
          while ( playfield.device( OXO ).state() ) automaton.run(); // <<<<<<<<< COLLECTING BONUS
          Serial.printf( "%d Bonus collect done\n", millis() );
          automaton.delay( 1000 );
          Serial.printf( "%d Delay done\n", millis() );
          playfield.device( PLAYERS ).trigger( IN_SCALAR_FREEZE );
        } while ( leds.active( LED_AGAIN0 ) ); // Extra ball
      } 
    } 
    leds.on( LED_GAME_OVER );
  }
}
