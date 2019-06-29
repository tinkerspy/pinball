#include "Atm_pinball.hpp"
#include "mapping.hpp"
#include "Atm_oxo_field.hpp"
#include "freeram.hpp"

#define NUMBER_OF_BALLS 5

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;
Atm_oxo_field oxo;
Atm_em_counter counter;
Atm_timer timer;
int number_of_players;

// TODO: spook flipperacties bij opstarten systeem lijken te komen doordat onterecht een button press wordt gedetecteert.
// Oplossen door tijdelijk button presses te negeren. (ontdekt doordat ook de counter reset (front button) spontaan begin te lopen)

// Debounce uitbreiden, we hebben nu delay_break & delay_retrigger, toevoegen: delay_make (pas een press registreren als die aanhoudt)

// Atm_em_counter, Atm_analog of Atm_comparator gebruiken om sensorlijn te filteren/debouncen

void setup() {
  delay( 1000 );
  Serial.println( "start" );
  delay( 100 );
    
  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 1 )
    .addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    .addStrip( new IO_Adafruit_NeoPixel(  4, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P2/cabinet DUMMY!!!
    .addStrip( new IO_Adafruit_NeoPixel(  4, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P3/headbox
    .invert( BALL_ENTER )
    .retrigger()
    .show();

  leds.begin( io, group_definition, profile_definition );
  
  playfield.begin( io, leds ).debounce( 20, 20 );

  counter.begin( playfield, COUNTER0, COIL_COUNTER0_GRP, PROFILE_COUNTER ); 

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
      .onScore( counter, counter.EVT_1000 );

  playfield
    .element( PORT_1X )
      .onPress( oxo, oxo.EVT_1X )
      .onScore( counter, counter.EVT_1000 );
      
  playfield
    .element( PORT_2O )
      .onPress( oxo, oxo.EVT_2O )
      .onScore( counter, counter.EVT_1000 );
      
  playfield
    .element( PORT_2X )
      .onPress( oxo, oxo.EVT_2X )
      .onScore( counter, counter.EVT_1000 );
      
  playfield
    .element( PORT_3O )
      .onPress( oxo, oxo.EVT_3O )
      .onScore( counter, counter.EVT_1000 );
      
  playfield
    .element( PORT_3X )
      .onPress( oxo, oxo.EVT_3X )
      .onScore( counter, counter.EVT_1000 );
  
  playfield
    .element( TARGET_A, -1, LED_TARGET_A )
      .autoLight( true )
      .onLight( true, playfield.element( BUMPER_A ), Atm_element::EVT_ON )
      .onScore( counter, counter.EVT_100 ); 
  
  playfield
    .element( TARGET_B, -1, LED_TARGET_B )
      .autoLight( true )
      .onLight( true, playfield.element( BUMPER_B ), Atm_element::EVT_ON ) 
      .onScore( counter, counter.EVT_100 ); 

  playfield
    .element( BUMPER_A, COIL_BUMPER_A, LED_BUMPER_A, PROFILE_BUMPER )
      .onScore( counter, counter.EVT_10, counter.EVT_100 ); 

  playfield
    .element( BUMPER_B, COIL_BUMPER_B, LED_BUMPER_B, PROFILE_BUMPER )
      .onScore( counter, counter.EVT_10, counter.EVT_100 ); 

  playfield
    .element( BUMPER_C, COIL_BUMPER_C, LED_BUMPER_C, PROFILE_BUMPER )
      .onLight( true, playfield.element( SAVE_GATE ), Atm_element::EVT_KICK )
      .onScore( counter, counter.EVT_100, counter.EVT_1000 ); 

  playfield
    .watch( LED_TARGET_GRP )
      .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_ON ); 

  playfield
    .element( KICKER_L, COIL_KICKER_L, LED_KICKER_GRP, PROFILE_KICKER )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( counter, counter.EVT_500, counter.EVT_5000 );

  playfield
    .element( KICKER_R, COIL_KICKER_R, LED_KICKER_GRP, PROFILE_KICKER )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( counter, counter.EVT_500, counter.EVT_5000 );
    
  playfield
    .element( UP_LANE_L, -1, LED_UP_LANE_GRP )
      .onPress( false, oxo, oxo.EVT_4 ) 
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( counter, counter.EVT_1000, counter.EVT_5000 );

  playfield
    .element( UP_LANE_R, -1, LED_UP_LANE_GRP )
      .onPress( false, oxo, oxo.EVT_6 )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( counter, counter.EVT_1000, counter.EVT_5000 );
    
  playfield
    .element(  TARGET_C )
      .onPress( oxo, oxo.EVT_5 )
      .onScore( counter, counter.EVT_500 );

  oxo.begin( playfield, LED_OXO_GRP, PROFILE_OXO )
    .onMatch( playfield.element( KICKER_L ), Atm_element::EVT_ON ); // LED_KICKER_R should automatically follow

  playfield
    .watch( LED_OXO_CELLS, 9 )
      .onLight( true, playfield.element( UP_LANE_L ), Atm_element::EVT_ON ); // UP_LANE_R should automatically follow

  playfield
    .element( IN_LANE_L )
      .onPress( oxo, oxo.EVT_7 )
      .onScore( counter, counter.EVT_1000 );
    
  playfield
    .element( IN_LANE_R )
      .onPress( oxo, oxo.EVT_9 )
      .onScore( counter, counter.EVT_1000 );
    
  playfield
    .element( SLING_L, COIL_SLING_L, -1 )
      .debounce( 20, 200 )
      .onPress( oxo, oxo.EVT_TOGGLE )
      .onScore( counter, counter.EVT_10 );

  playfield
    .element( SLING_R, COIL_SLING_R, -1 )
      .debounce( 20, 200 )
      .onPress( oxo, oxo.EVT_TOGGLE )
      .onScore( counter, counter.EVT_10 );

  playfield
    .element(  ROLLOVER )
      .onPress( oxo, oxo.EVT_8 )
      .onScore( counter, counter.EVT_500 );

  playfield
    .element( OUT_LANE )
      .onScore( counter, counter.EVT_1000 );

  playfield
    .element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER )
      .debounce( 5, 0 );

  playfield  
    .element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER )
      .debounce( 5, 0 );
  
  playfield
    .element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );

  playfield
    .element( BALL_ENTER )
      .onPress( playfield.led( LED_BUMPER_GRP ), Atm_element::EVT_OFF ); // Mind the faulty switch hardware!

  playfield
    .element( BALL_EXIT, COIL_BALL_FEEDER, LED_SHOOTS_AGAIN, PROFILE_FEEDER )
      //.onPress( playfield.led( LED_FLASHER_GRP ), Atm_element::EVT_OFF )
      ;
      

/*
  playfield
    .element( FRONTBTN )
      .onPress( counter, counter.EVT_RESET );
*/
  Serial.println( FreeRam() );
  
  
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
        bonus.trigger( leds.active( TRIPLE_BONUS ) ? bonus.EVT_TRIPLE : bonus.EVT_SINGLE );
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
  */
  if ( io.isPressed( FRONTBTN ) ) {
    counter.reset();
//    game.players( 1 );
    Serial.print( millis() / 1000 );
    Serial.println( " Counter reset started" );
    while ( counter.state() ) automaton.run();
    Serial.print( millis() / 1000 );
    Serial.println( " Counter reset finished" );
    for ( int p = 0; p < 1; p++ ) {
      for ( int b = 0; b < NUMBER_OF_BALLS; b++ ) {
        do {
  //        game.select( p, b );
          leds.off( LED_FLASHER_GRP );
          Serial.print( millis() / 1000 );
          Serial.println( " Serve next ball" );
          if ( io.isPressed( BALL_EXIT ) ) playfield.element( BALL_EXIT ).kick();
          automaton.delay( 500 );
          Serial.print( millis() / 1000 );
          Serial.println( " Ball play in progress" );
          while ( !io.isPressed( BALL_EXIT ) ) automaton.run();
          Serial.print( millis() / 1000 );
          Serial.println( " Ball play finished" );      
        } while ( leds.active( LED_SHOOTS_AGAIN ) );
      }
    }
  }
  automaton.run();
}
