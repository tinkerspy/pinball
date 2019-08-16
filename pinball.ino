#include "Atm_pinball.hpp"
#include "mapping.hpp"
#include "Atm_oxo_field.hpp"
#include "freeram.hpp"

#define NUMBER_OF_BALLS 5

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;
Atm_oxo_field oxo;
Atm_em_counter counter[4]; 
Atm_score score;
Atm_scalar player, ball, up, bonus;

void setup() {
  delay( 1000 );
  Serial.println( "start" );
  delay( 100 );
    
  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 1 )
    .addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    .addStrip( new IO_Adafruit_NeoPixel(  4, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P2/cabinet DUMMY!!!
    .addStrip( new IO_Adafruit_NeoPixel( 36, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 36 pixel SK6812 led strip on P3/headbox
    .invert( BALL_ENTER )
    .retrigger()
    .show();

  leds.begin( io, group_definition, profile_definition );
  
  playfield.begin( io, leds ).debounce( 20, 20, 0 );

  score.begin()
    .addCounter( counter[0].begin( playfield, COUNTER0, COIL_COUNTER0_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[1].begin( playfield, COUNTER1, COIL_COUNTER1_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[2].begin( playfield, COUNTER2, COIL_COUNTER2_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[3].begin( playfield, COUNTER3, COIL_COUNTER3_GRP, PROFILE_COUNTER ) ); 

  playfield
    .leds()
      .profile( LED_FLASHER_GRP, PROFILE_LED );

  // Turn on the General Illumination
  playfield
    .leds()
      .profile( COIL_GI, PROFILE_GI )
      .on( COIL_GI );

  // Playfield element instantiation

  playfield
    .element( PORT_1O )
      .onPress( oxo, oxo.EVT_1O )
      .onScore( score, score.EVT_1000 );

  playfield
    .element( PORT_1X )
      .onPress( oxo, oxo.EVT_1X )
      .onScore( score, score.EVT_1000 );
      
  playfield
    .element( PORT_2O )
      .onPress( oxo, oxo.EVT_2O )
      .onScore( score, score.EVT_1000 );
      
  playfield
    .element( PORT_2X )
      .onPress( oxo, oxo.EVT_2X )
      .onScore( score, score.EVT_1000 );
      
  playfield
    .element( PORT_3O )
      .onPress( oxo, oxo.EVT_3O )
      .onScore( score, score.EVT_1000 );
      
  playfield
    .element( PORT_3X )
      .onPress( oxo, oxo.EVT_3X )
      .onScore( score, score.EVT_1000 );
  
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
      .onScore( score, score.EVT_500, score.EVT_5000 );

  playfield
    .element( KICKER_R, COIL_KICKER_R, LED_KICKER_GRP, PROFILE_KICKER )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_500, score.EVT_5000 );
    
  playfield
    .element( UP_LANE_L, -1, LED_UP_LANE_GRP )
      .onPress( false, oxo, oxo.EVT_4 ) 
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_1000, score.EVT_5000 );

  playfield
    .element( UP_LANE_R, -1, LED_UP_LANE_GRP )
      .onPress( false, oxo, oxo.EVT_6 )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_1000, score.EVT_5000 );
    
  playfield
    .element(  TARGET_C )
      .onPress( oxo, oxo.EVT_5 )
      .onScore( score, score.EVT_500 );

  oxo.begin( playfield, LED_OXO_GRP, PROFILE_OXO )
    .onMatch( playfield.element( KICKER_L ), Atm_element::EVT_ON ); // LED_KICKER_R should automatically follow

  playfield
    .watch( LED_OXO_CELLS, 9 )
      .onLight( true, playfield.element( UP_LANE_L ), Atm_element::EVT_ON ); // UP_LANE_R should automatically follow

  playfield
    .element( IN_LANE_L )
      .onPress( oxo, oxo.EVT_7 )
      .onScore( score, score.EVT_1000 );
    
  playfield
    .element( IN_LANE_R )
      .onPress( oxo, oxo.EVT_9 )
      .onScore( score, score.EVT_1000 );
    
  playfield
    .element( SLING_L, COIL_SLING_L, -1 )
      .debounce( 20, 200, 0 )
      .onPress( oxo, oxo.EVT_TOGGLE )
      .onScore( score, score.EVT_10 );

  playfield
    .element( SLING_R, COIL_SLING_R, -1 )
      .debounce( 20, 200, 0 )
      .onPress( oxo, oxo.EVT_TOGGLE )
      .onScore( score, score.EVT_10 );

  playfield
    .element(  ROLLOVER )
      .onPress( oxo, oxo.EVT_8 )
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
    .element( BALL_EXIT, COIL_BALL_FEEDER, LED_SHOOTS_AGAIN, PROFILE_FEEDER )
      .onPress( playfield.led( LED_FLASHER_GRP ), Atm_element::EVT_OFF );

  playfield
    .element( FRONTBTN )
      .onPress( score, score.EVT_RESET );

  Serial.println( FreeRam() );

  leds.profile( LED_HEADBOX_GRP, PROFILE_BRIGHT );
  leds.on( LED_HEADBOX_GRP );
    
  /*
  TODO:
  - Atm_bonus bonus;
  - Atm_game game;
  - playfield.enable( 0 ); playfield.persistent( sw1, sw2, sw3 );

  game.begin( LED_NO_OF_PLAYERS, LED_ACTIVE_PLAYER, LED_ACTIVE_BALL )
    .add( counter[0] )
    .add( counter[1] )
    .add( counter[2] )
    .add( counter[3] );

  game.reset(); // EVT
  game.add_player(); // EVT
  game.advance(); // EVT
  EVT_10..EVT_5000 -> counter
  
  game.state();   
  game.onScore();
  game.ball();
  game.player();
  game.touched(); // ball scored > 0
        
   */

}

void loop() {
/*  
  if ( isPressed( FRONTBTN ) ) {
    Serial.println( "Resetting counters" );
    game.reset();
    while ( game.state() ) automaton.run();
    Serial.println( "Initializing game (add players, wait for game activity)" );
    game.initBall();
    playfield.enable( 1 );
    while ( !game.touched() ) automaton.run();
    Serial.println( "Game started" );
    do {
      Serial.println( "Ball start" );
      game.initBall();
      playfield.enable( 1 );
      Serial.println( "Ball play in progress" );
      while ( !io.isPressed( BALL_EXIT ) ) automaton.run();
      Serial.println( "Ball play finished" );
      if ( game.touched() ) {
        Serial.println( "Collecting bonus" );
        playfield.enable( 0 );
        bonus.collect( leds.active( TRIPLE_BONUS ) ? 1000 : 3000 );
        while ( bonus.state() ) automaton.run();
      }
      if ( !leds.active( SHOOTS_AGAIN ) ) { 
        Serial.println( "Advance game ball/player" );
        game.advance();
      }
      Serial.println( "Clear playfield" );
      leds.off( LED_GROUP_FLASHERS );
    } while ( game.state() );  
  }

  if ( io.isPressed( FRONTBTN ) ) {
    score.reset();
    balls.reset();
    players.reset();
    Serial.println( "Counter reset started" );
    while ( score.state() ) automaton.run();
    Serial.println( "Counter reset finished" );
    // Wait for the first score activity??? if ( score.touched() );
    for ( int p = 0; p < players.state(); p++ ) {
      for ( int b = 0; b < 5; b++ ) {
        do {
          up.select( p );
          balls.select( b );
          leds.off( LED_FLASHER_GRP );
          Serial.println( "Serve ball" );
          playfield.element( BALL_EXIT ).kick();
          Serial.println( "Ball play in progress" );
          while ( !io.isPressed( BALL_EXIT ) ) automaton.run();
          Serial.println( "Ball play finished" );      
        while ( leds.active( SHOOTS_AGAIN ) );
      }
    }
  }
  */
  automaton.run();
}
