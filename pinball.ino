#include "Atm_pinball.hpp"
#include "mapping.hpp"
#include "Atm_oxo_field.hpp"
#include "freeram.hpp"

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;
Atm_oxo_field oxo;
Atm_em_counter counter;
Atm_timer timer;

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
    .element( BALL_EXIT, COIL_BALL_FEEDER, LED_SHOOTS_AGAIN, PROFILE_FEEDER )
      .onPress( playfield.led( LED_FLASHER_GRP ), Atm_element::EVT_OFF );

  playfield
    .element( BALL_ENTER )
      .onPress( playfield.led( LED_BUMPER_GRP ), Atm_element::EVT_OFF ); // Mind the faulty switch hardware!

  playfield
    .element( FRONTBTN )
      .onPress( counter, counter.EVT_RESET );

  Serial.println( FreeRam() );

  /*
  TODO:
  - Atm_bonus
  - Atm_score
  - playfield.enable( 0 );

  score.begin( LED_NO_OF_PLAYERS, LED_ACTIVE_PLAYER, LED_ACTIVE_BALL )
    .add( counter[0] )
    .add( counter[1] )
    .add( counter[2] )
    .add( counter[3] );

  score.reset(); // EVT
  score.add_player(); // EVT
  score.advance(); // EVT
  EVT_10..EVT_5000 -> counter
  
  score.state();   
  score.onScore();
  score.ball();
  score.player();
  score.touched(); // ball scored > 0
        
   */

}

void loop() {  
    // playfield.enable( 0 ); // playfield should be disabled by default!, disabled means all switches are ignored
    // while ( !isPressed( FRONTBTN ) ) automaton.run();
    // start game, score.reset();
    // wait for counters to reset
    // init game, playfield.enable();
    // while ( !score.touched() ) ) automaton.run();
    // do {
      // init ball, playfield.enable( 1 )
      // ball_active = 1;
      // while ( ball_active ) automaton.run() // ball_active is set to false by BALL_EXIT onPress
      // if ( score.touched() ) {
        // if ( playfield.enabled() ) {
        //   playfield.enable( 0 );
        //   bonus.trigger( leds.active( TRIPLE_BONUS ) ? bonus.EVT_TRIPLE : bonus.EVT_SINGLE );
        //   while ( bonus.state() ) automaton.run();
        // }
        // if ( !leds.active( SHOOTS_AGAIN ) ) { 
        //   score.advance();
        // }
      //}
    //} while ( score.state() ); // End-Of-Game
  automaton.run();
}
