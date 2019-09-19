#include "Singularity.h"
#include "mapping.h"
#include "devices.h"
#include "freeram.hpp"

#define NUMBER_OF_BALLS 5

IO io;
Atm_led_scheduler leds; // IO_LED
Atm_playfield playfield; // IO_MATRIX

Atm_em_counter counter[4]; 
Atm_score score;
Atm_scalar players, bonus;
Atm_timer animation[3];

using namespace std_firmware; 

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
    .onDigit( 0, playfield.element( CHIME0, COIL_CHIME0, -1, PROFILE_COIL ), Atm_element::EVT_KICK ) // Link digits to chimes
    .onDigit( 1, playfield.element( CHIME1, COIL_CHIME1, -1, PROFILE_COIL ), Atm_element::EVT_KICK ) 
    .onDigit( 2, playfield.element( CHIME2, COIL_CHIME2, -1, PROFILE_COIL ), Atm_element::EVT_KICK ); 

  playfield
    .leds()
      .profile( LED_FLASHER_GRP, PROFILE_LED )
      .profile( LED_HEADBOX_GRP, PROFILE_BRIGHT );

  bonus.begin( leds, -1, 0, 9 ) 
    .onCollect( score, score.EVT_1000 );
  players.begin( leds, LED_PLAY_GRP, 0, 3 );

  leds.profile( LED_OXO_GRP, PROFILE_OXO ); // Required!
  
  automaton.delay( 1000 ); // Visible reset indicator... (GI fades off/on)
   
  // Turn on the General Illumination
  leds.profile( COIL_GI, PROFILE_GI );
  playfield.device( GI, COIL_GI, ledbank_firmware ).trigger( IN_LBANK_ON );  

  // Playfield element instantiation

  playfield.device( OXO, LED_OXO_GRP, tictactoe_firmware )
    .onEvent( OUT_OXO_SCORE, bonus, bonus.EVT_ADVANCE )
    .onEvent( OUT_OXO_WIN_ROW, playfield.device( KICKER ), IN_KICKER_ON )
    .onEvent( OUT_OXO_WIN_ALL, playfield.device( UP_LANE ), IN_COMBO_ON );

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
    .onEvent( OUT_BUMPER_LIGHT_ON, playfield.element( SAVE_GATE ), Atm_element::EVT_KICK )
    .onEvent( OUT_BUMPER_LIGHT_OFF, playfield.element( SAVE_GATE ), Atm_element::EVT_RELEASE );

  leds.profile( COIL_KICKER_L, PROFILE_KICKER );
  leds.profile( COIL_KICKER_R, PROFILE_KICKER );
  playfield.device( KICKER, LED_KICKER_GRP, dual_kicker_firmware )
    .onEvent( OUT_KICKER_KICK_LIT, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) 
    .onEvent( OUT_KICKER_SCORE_LIT, score, score.EVT_5000 )
    .onEvent( OUT_KICKER_SCORE_UNLIT, score, score.EVT_500 ); 
  
  leds.profile( LED_UP_LANE_L, PROFILE_LED );
  leds.profile( LED_UP_LANE_R, PROFILE_LED );
  playfield.device( UP_LANE, LED_UP_LANE_GRP, dual_combo_firmware )
    .onEvent( OUT_COMBO_SCORE, score, score.EVT_1000 )
    .onEvent( OUT_COMBO_PRESS_LIT, playfield.element( BALL_EXIT ), Atm_element::EVT_ON )
    .onEvent( OUT_COMBO_PRESS0_UNLIT, playfield.device( OXO ), IN_OXO_4 )
    .onEvent( OUT_COMBO_PRESS1_UNLIT, playfield.device( OXO ), IN_OXO_6 );

  playfield.debounce( SLING_L, 20, 200, 0 );
  playfield.debounce( SLING_R, 20, 200, 0 );
  playfield.device( SLINGSHOT, LED_SLINGSHOT_GRP, dual_kicker_firmware )
    .onEvent( OUT_KICKER_SCORE, score, score.EVT_10 )
    .onEvent( OUT_KICKER_KICK, playfield.device( OXO ), IN_OXO_TOGGLE );    
    
  playfield
    .element(  TARGET_C )
      .onPress( playfield.device( OXO ), IN_OXO_5 )
      .onScore( score, score.EVT_500 );

  playfield
    .element( IN_LANE_L )
      .onPress( playfield.device( OXO ), IN_OXO_7 )
      .onScore( score, score.EVT_1000 );
    
  playfield
    .element( IN_LANE_R )
      .onPress( playfield.device( OXO ), IN_OXO_9 )
      .onScore( score, score.EVT_1000 );
  playfield
    .element( ROLLOVER )
      .onPress( playfield.device( OXO ), IN_OXO_8 )
      .onScore( score, score.EVT_500 );

  playfield
    .element( OUT_LANE )
      .onScore( score, score.EVT_1000 );

  playfield.debounce( FLIPPER_L, 5, 0, 0 );    
  playfield.debounce( FLIPPER_R, 5, 0, 0 );    
  leds.profile( LED_FLIPPER_GRP, PROFILE_FLIPPER );
  playfield.device( FLIPPER, LED_FLIPPER_GRP, dual_flipper_firmware );    
  
  playfield
    .element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );

  playfield
    .element( BALL_ENTER )
      .debounce( 5, 0, 2000 )
      .onPress( playfield.led( LED_BUMPER_GRP ), Atm_element::EVT_OFF ); // Mind the faulty switch hardware!

  playfield
    .element( BALL_EXIT, COIL_BALL_FEEDER, LED_AGAIN_GRP, PROFILE_FEEDER )
      .autoKick( false )
      .onPress( playfield, playfield.EVT_READY );

  playfield
    .element( TILT_PEND, LED_TILT, -1, PROFILE_BRIGHT )
      .onPress( playfield, playfield.EVT_DISABLE );

  playfield
    .element( TILT_RAMP, LED_TILT, -1, PROFILE_BRIGHT )
      .onPress( playfield, playfield.EVT_DISABLE );

  playfield 
    .watch( LED_TILT )
      .onLight( true, bonus, Atm_scalar::EVT_RESET ); 

  playfield
    .element( FRONTBTN )
      .persistent( true )
      .onPress( players, players.EVT_ADVANCE );

  Serial.println( FreeRam() );

  leds.scalar( LED_BALL_GRP, 0 );
  leds.scalar( LED_UP_GRP, 0 );
  leds.on( LED_GAME_OVER );

  animation[0].begin( 500 ).onTimer( [] ( int idx, int v, int up ) { leds.toggle( LED_OXO_ANI0 ); }).repeat().start(); // leds.blink( LED_OXO_ANI0, 500 );???
  animation[1].begin( 350 ).onTimer( [] ( int idx, int v, int up ) { leds.toggle( LED_OXO_ANI1 ); }).repeat().start();
  animation[2].begin( 600 ).onTimer( [] ( int idx, int v, int up ) { leds.toggle( LED_OXO_ANI2 ); }).repeat().start();

  // leds.profile( LED_GAME_OVER, PROFILE_BLINK ).on( LED_GAME_OVER );

  //playfield.disable();     

}


void loop() {
  automaton.run(); // <<<<<<<<<< IDLE
  if ( io.isPressed( FRONTBTN ) ) {
    score.reset();
    players.reset();
    leds.off( LED_FLASHER_GRP );
    Serial.printf( "%d Counter reset\n", millis() );
    while ( score.state() ) automaton.run(); // <<<<<<<<<<< RESETTING COUNTERS
    for ( int ball = 0; ball < NUMBER_OF_BALLS; ball++ ) {      
      for ( int player = 0; player < players.state() + 1; player++ ) {
        do {
          bonus.reset();
          score.select( player );
          leds.off( LED_FLASHER_GRP );
          leds.scalar( LED_UP_GRP, player );
          leds.scalar( LED_BALL_GRP, ball );
          if ( ball == 4 ) {
            bonus.multiplier( 3 );
            leds.on( LED_TRIPLE_BONUS );
            Serial.printf( "%d Triple bonus!\n", millis() );
          }
          Serial.printf( "%d Serve player %d, ball %d\n", millis(), player, ball );
          leds.on( COIL_BALL_FEEDER );
          playfield.enable();
          while ( playfield.enabled() ) automaton.run(); // <<<<<<<<<< PLAYING
          Serial.printf( "%d Ball play finished, bonus collect %d\n", millis(),  bonus.state() );     
          bonus.collect(); 
          while ( bonus.state() ) automaton.run(); // <<<<<<<<< COLLECTING BONUS
          Serial.printf( "%d Bonus collect done\n", millis() );
          automaton.delay( 500 );
          players.lock();
        } while ( leds.active( LED_AGAIN0 ) ); // Extra ball
      } 
    } 
    leds.on( LED_GAME_OVER );
  }
}


