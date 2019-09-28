#include "Neon.hpp"
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
Atm_scalar players, bonus;
Atm_timer animation[3];
Debounce debounce;

int16_t io_scan( void ) { return io.scan(); }
void io_unscan( void ) { io.unscan(); }
int16_t debounce_scan( void ) { return debounce.scan(); }

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
  
  playfield.begin( io, debounce_scan, leds ).disable();

  debounce.begin( io ).debounce( 20, 20, 0 ); // profiles );

  score.begin()
//    .addCounter( counter[0].begin( playfield, COUNTER0, COIL_COUNTER0_GRP, PROFILE_COUNTER ) ) // Initialize individual score counters and link them to the score object
    .addCounter( counter[1].begin( playfield, COUNTER1, COIL_COUNTER1_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[2].begin( playfield, COUNTER2, COIL_COUNTER2_GRP, PROFILE_COUNTER ) )
    .addCounter( counter[3].begin( playfield, COUNTER3, COIL_COUNTER3_GRP, PROFILE_COUNTER ) ) 
//    .onDigit( 0, playfield.element( CHIME0, COIL_CHIME0, -1, PROFILE_COIL ), Atm_element::EVT_KICK ) // Link digits to chimes
//    .onDigit( 1, playfield.element( CHIME1, COIL_CHIME1, -1, PROFILE_COIL ), Atm_element::EVT_KICK ) 
//    .onDigit( 2, playfield.element( CHIME2, COIL_CHIME2, -1, PROFILE_COIL ), Atm_element::EVT_KICK )
    ; 

  playfield
    .leds()
      .profile( LED_FLASHER_GRP, PROFILE_LED )
      .profile( LED_HEADBOX_GRP, PROFILE_BRIGHT );

  players.begin( leds, LED_PLAY_GRP, 0, 3 );
  bonus.begin( leds, -1, 0, 9 ) 
    .onCollect( score, score.EVT_1000 );
  
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
      .onScore( score, score.EVT_500, score.EVT_5000 )
      .persistent();

  playfield
    .element( KICKER_R, COIL_KICKER_R, LED_KICKER_GRP, PROFILE_KICKER )
      .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON ) // Extra ball
      .onScore( score, score.EVT_500, score.EVT_5000 )
      .persistent();
    
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
    .onSet( bonus, bonus.EVT_ADVANCE )
    .onMatch( playfield.element( KICKER_L ), Atm_element::EVT_ON ); // LED_KICKER_R should automatically follow (linked via LED_KICKER_GRP)

  playfield // Replace this by onFull trigger on the bonus ladder
    .watch( LED_OXO_CELLS, 9 )
      .onLight( true, playfield.element( UP_LANE_L ), Atm_element::EVT_ON ); // UP_LANE_R should automatically follow (linked via LED_UP_LANE_GRP)

  playfield
    .element( IN_LANE_L )
      .onPress( oxo, oxo.EVT_7 )
      .onScore( score, score.EVT_1000 );
    
  playfield
    .element( IN_LANE_R )
      .onPress( oxo, oxo.EVT_9 )
      .onScore( score, score.EVT_1000 );

  debounce.debounce( SLING_L, 20, 200, 0 );
  playfield
    .element( SLING_L, COIL_SLING_L, -1 )
      .onPress( oxo, oxo.EVT_TOGGLE )
      .onScore( score, score.EVT_10 );

  debounce.debounce( SLING_R, 20, 200, 0 );
  playfield
    .element( SLING_R, COIL_SLING_R, -1 )
      .onPress( oxo, oxo.EVT_TOGGLE )
      .onScore( score, score.EVT_10 );

  playfield
    .element(  ROLLOVER )
      .onPress( oxo, oxo.EVT_8 )
      .onScore( score, score.EVT_500 );

  playfield
    .element( OUT_LANE )
      .onScore( score, score.EVT_1000 );

  debounce.debounce( FLIPPER_L, 5, 0, 0 );
  playfield
    .element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER );

  debounce.debounce( FLIPPER_R, 5, 0, 0 );
  playfield  
    .element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER );
  
  playfield
    .element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );

  debounce.debounce( BALL_ENTER, 5, 0, 2000 ); 
  playfield
    .element( BALL_ENTER )
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
