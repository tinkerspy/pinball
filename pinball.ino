#include "Neon.h"
#include "mapping.h"
#include "devices.h"
#include "Atm_oxo_field.hpp"
#include "freeram.hpp"

#define NUMBER_OF_BALLS 5

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;

Atm_led_device oxo, multilane;
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

  leds.begin( io, group_definition, profile_definition );
  
  playfield.begin( io, leds ).debounce( 20, 20, 0 ).disable();

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

  players.begin( leds, LED_PLAY_GRP, 0, 3 );
  bonus.begin( leds, -1, 0, 9 ) 
    .onCollect( score, score.EVT_1000 );

  leds.profile( LED_OXO_GRP, PROFILE_OXO ); // Required!
  
  oxo.begin( playfield, LED_OXO_GRP, tictactoe_code )
    .onChange( OUT_OXO_SCORE, bonus, bonus.EVT_ADVANCE )
    .onChange( OUT_OXO_WIN_ROW, playfield.element( KICKER_L ), Atm_element::EVT_ON )
    .onChange( OUT_OXO_WIN_ALL, playfield.element( UP_LANE_L ), Atm_element::EVT_ON );

  multilane.begin( playfield, -1, multilane_code ) 
    .onChange( OUT_LANE0, oxo, IN_OXO_1O )
    .onChange( OUT_LANE1, oxo, IN_OXO_1X )
    .onChange( OUT_LANE2, oxo, IN_OXO_2O )
    .onChange( OUT_LANE3, oxo, IN_OXO_2X )
    .onChange( OUT_LANE4, oxo, IN_OXO_3O )
    .onChange( OUT_LANE5, oxo, IN_OXO_3X )
    .onChange( OUT_LANE_SCORE, score, score.EVT_1000 );

  playfield.onPress( PORT_1O, multilane, IN_LANE_PRESS0 );
  playfield.onPress( PORT_1X, multilane, IN_LANE_PRESS1 );
  playfield.onPress( PORT_2O, multilane, IN_LANE_PRESS2 );
  playfield.onPress( PORT_2X, multilane, IN_LANE_PRESS3 );
  playfield.onPress( PORT_3O, multilane, IN_LANE_PRESS4 );
  playfield.onPress( PORT_3X, multilane, IN_LANE_PRESS5 );
   
  // Turn on the General Illumination
  playfield
    .leds()
      .profile( COIL_GI, PROFILE_GI )
      .on( COIL_GI );

  // Playfield element instantiation

  playfield
    .element( TARGET_A, -1, LED_TARGET_A )
      .autoLight( true )
      .onLight( true, playfield.element( BUMPER_A ), Atm_element::EVT_ON )
      .onScore( score, score.EVT_100 ); 
  
  playfield
    .element( TARGET_B, -1, LED_TARGET_B )
      .autoLight( true )
      .onLight( true, playfield.element( BUMPER_B ), Atm_element::EVT_ON ) 
      .onScore( score, score.EVT_100 ); 

  playfield
    .element( BUMPER_A, COIL_BUMPER_A, LED_BUMPER_A, PROFILE_BUMPER )
      .onScore( score, score.EVT_10, score.EVT_100 ); 

  playfield
    .element( BUMPER_B, COIL_BUMPER_B, LED_BUMPER_B, PROFILE_BUMPER )
      .onScore( score, score.EVT_10, score.EVT_100 ); 

  playfield
    .element( BUMPER_C, COIL_BUMPER_C, LED_BUMPER_C, PROFILE_BUMPER )
      .onLight( true, playfield.element( SAVE_GATE ), Atm_element::EVT_KICK )
      .onScore( score, score.EVT_100, score.EVT_1000 ); 

  playfield
    .watch( LED_TARGET_GRP )
      .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_ON ); 

  playfield
    .element( KICKER_L, COIL_KICKER_L, LED_KICKER_GRP, PROFILE_KICKER )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_500, score.EVT_5000 )
      .persistent();

  playfield
    .element( KICKER_R, COIL_KICKER_R, LED_KICKER_GRP, PROFILE_KICKER )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_500, score.EVT_5000 )
      .persistent();
    
  playfield
    .element( UP_LANE_L, -1, LED_UP_LANE_GRP )
      .onPress( false, oxo, IN_OXO_4 ) 
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_1000, score.EVT_5000 );

  playfield
    .element( UP_LANE_R, -1, LED_UP_LANE_GRP )
      .onPress( false, oxo, IN_OXO_6 )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_1000, score.EVT_5000 );
    
  playfield
    .element(  TARGET_C )
      .onPress( oxo, IN_OXO_5 )
      .onScore( score, score.EVT_500 );

  playfield
    .element( IN_LANE_L )
      .onPress( oxo, IN_OXO_7 )
      .onScore( score, score.EVT_1000 );
    
  playfield
    .element( IN_LANE_R )
      .onPress( oxo, IN_OXO_9 )
      .onScore( score, score.EVT_1000 );
    
  playfield
    .element( SLING_L, COIL_SLING_L, -1 )
      .debounce( 20, 200, 0 )
      .onPress( oxo, IN_OXO_TOGGLE )
      .onScore( score, score.EVT_10 );

  playfield
    .element( SLING_R, COIL_SLING_R, -1 )
      .debounce( 20, 200, 0 )
      .onPress( oxo, IN_OXO_TOGGLE )
      .onScore( score, score.EVT_10 );

  playfield
    .element(  ROLLOVER )
      .onPress( oxo, IN_OXO_8 )
      .onScore( score, score.EVT_500 );

  playfield
    .element( OUT_LANE )
      .onScore( score, score.EVT_1000 );

  playfield
    .element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER )
      .debounce( 5, 0, 0 );

  playfield  
    .element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER )
      .debounce( 5, 0, 0 );
  
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
