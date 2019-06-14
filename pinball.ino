#include "Atm_pinball.hpp"
#include "mapping.hpp"
#include "Atm_oxo_field.hpp"
#include "freeram.hpp"

IO io;
Atm_led_scheduler leds;
Atm_playfield playfield;
Atm_oxo_field oxo;
Atm_em_counter counter;

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

  // Playfield element instantiation
  playfield.element( TARGET_A, -1, LED_TARGET_A );
  playfield.element( TARGET_B, -1, LED_TARGET_B );
  playfield.element( BUMPER_A, COIL_BUMPER_A, LED_BUMPER_A, PROFILE_BUMPER ); 
  playfield.element( BUMPER_B, COIL_BUMPER_B, LED_BUMPER_B, PROFILE_BUMPER );
  playfield.element( BUMPER_C, COIL_BUMPER_C, LED_BUMPER_C, PROFILE_BUMPER );
  playfield.element( KICKER_L, COIL_KICKER_L, LED_KICKER_GRP, PROFILE_KICKER );
  playfield.element( KICKER_R, COIL_KICKER_R, LED_KICKER_GRP, PROFILE_KICKER );
  playfield.element( SLING_L, COIL_SLING_L, -1 );
  playfield.element( SLING_R, COIL_SLING_R, -1 );
  playfield.element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER );
  playfield.element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER );
  playfield.element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );  
  playfield.element( BALL_EXIT, COIL_BALL_FEEDER, LED_SHOOTS_AGAIN, PROFILE_FEEDER );
  playfield.element( UP_LANE_L, -1, LED_UP_LANE_GRP );
  playfield.element( UP_LANE_R, -1, LED_UP_LANE_GRP );

  // Turn on the General Illumination
  playfield.leds().profile( COIL_GI, PROFILE_GI ).on( COIL_GI );

  // Custom debounce settings
  playfield.debounce( FLIPPER_L, 5, 0 );  // Tune: lowest debounce value, with sufficient flipper force
  playfield.debounce( FLIPPER_R, 5, 0 );  
  playfield.debounce( SLING_L, 20, 200 );
  playfield.debounce( SLING_R, 20, 200 );

  // Start OXO widget and connect to the proper switches 
  oxo.begin( playfield, LED_OXO_GRP, PROFILE_OXO );

  //counter.begin( playfield, COUNTER0_SENSE, COIL_COUNTER0_GRP, PROFILE_COUNTER ); 
  
  playfield.onPress(   PORT_1O, oxo, oxo.EVT_1O );
  playfield.onPress(   PORT_1X, oxo, oxo.EVT_1X );
  playfield.onPress(   PORT_2O, oxo, oxo.EVT_2O );
  playfield.onPress(   PORT_2X, oxo, oxo.EVT_2X );
  playfield.onPress(   PORT_3O, oxo, oxo.EVT_3O );
  playfield.onPress(   PORT_3X, oxo, oxo.EVT_3X );
  playfield.element( UP_LANE_L ).onPress( oxo, oxo.EVT_4 );
  playfield.onPress(  TARGET_C, oxo, oxo.EVT_5 );
  playfield.element( UP_LANE_R ).onPress( oxo, oxo.EVT_6 );
  playfield.onPress( IN_LANE_L, oxo, oxo.EVT_7 );
  playfield.onPress(  ROLLOVER, oxo, oxo.EVT_8 );
  playfield.onPress( IN_LANE_R, oxo, oxo.EVT_9 );

  // Toggle O -> X -> O
  playfield.element( SLING_L ).onPress( oxo, oxo.EVT_TOGGLE ); // Replace with playfield.OR( SW_SLING_GRP ).onPress( oxo, oxo::EVT_TOGGLE );
  playfield.element( SLING_R ).onPress( oxo, oxo.EVT_TOGGLE );

  // OXO tix-tac-toe match lites 'extra ball' on kickers
  // TODO all nine fields lights 'special' on upper lanes
  oxo.onMatch( playfield.element( LED_KICKER_L ), Atm_element::EVT_ON ); // LED_KICKER_R should automatically follow

  // Light the same player shoots again led
  playfield.element(  KICKER_L ).onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON );
  playfield.element(  KICKER_R ).onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON );
  playfield.element( UP_LANE_L ).onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON );
  playfield.element( UP_LANE_R ).onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON );
  
  // TARGET -> BUMPER -> GATE logic

  // Replace with playfield.OR( SW_TARGET_GRP ).onPress( oxo, oxo::EVT_TOGGLE );
  // Specially configured Atm_element object.

  playfield.element( TARGET_A )
    .autoLight()
    .onLight( true, playfield.element( BUMPER_A ), Atm_element::EVT_ON ); 
    
  playfield.element( TARGET_B )
    .autoLight()
    .onLight( true, playfield.element( BUMPER_B ), Atm_element::EVT_ON ); 

  playfield.element( BUMPER_A )
    .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_INPUT );

  playfield.element( BUMPER_B )
    .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_INPUT );

  playfield.element( BUMPER_C )
    .onInput( [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_A ).state() && playfield.element( BUMPER_B ).state() ) {
        playfield.leds().on( LED_BUMPER_GRP );
      }
    });
    
  playfield.onPress( BALL_ENTER, [] ( int idx, int v, int up ) { 
    if ( playfield.element( BALL_EXIT ).idle( 2000 ) || playfield.element( OUT_LANE ).idle( 2000 ) ) { // Fix hardware switch instead
      playfield.leds().off( LED_BUMPER_GRP );
    }     
  });

  playfield.onPress( BALL_EXIT, [] ( int idx, int v, int up ) {  // Om dit anders te doen moet je een virtual switch inrichten en daar de LED_FLASHER_GRP aan hangen
    playfield.leds().off( LED_FLASHER_GRP );
  });

  // end of logic

  Serial.println( FreeRam() );
}

void loop() {  
  automaton.run();
}
