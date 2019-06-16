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

  counter.begin( playfield, COUNTER0, COIL_COUNTER0_GRP, PROFILE_COUNTER ); 

  // Turn on the General Illumination
  playfield.leds().profile( COIL_GI, PROFILE_GI ).on( COIL_GI );

  // Playfield element instantiation

  playfield.element( PORT_1O ).onPress( oxo, oxo.EVT_1O ).onScore( counter, counter.EVT_1000 );
  playfield.element( PORT_1X ).onPress( oxo, oxo.EVT_1X ).onScore( counter, counter.EVT_1000 );
  playfield.element( PORT_2O ).onPress( oxo, oxo.EVT_2O ).onScore( counter, counter.EVT_1000 );
  playfield.element( PORT_2X ).onPress( oxo, oxo.EVT_2X ).onScore( counter, counter.EVT_1000 );
  playfield.element( PORT_3O ).onPress( oxo, oxo.EVT_3O ).onScore( counter, counter.EVT_1000 );
  playfield.element( PORT_3X ).onPress( oxo, oxo.EVT_3X ).onScore( counter, counter.EVT_1000 );
  
  playfield.element( TARGET_A, -1, LED_TARGET_A )
    .autoLight( true )
    .onLight( true, playfield.element( BUMPER_A  ), Atm_element::EVT_ON )
    .onScore( counter, counter.EVT_100 ); 
  
  playfield.element( TARGET_B, -1, LED_TARGET_B )
    .autoLight( true )
    .onLight( true, playfield.element( BUMPER_B  ), Atm_element::EVT_ON ) 
    .onScore( counter, counter.EVT_100 ); 

  playfield.element( BUMPER_A, COIL_BUMPER_A, LED_BUMPER_A, PROFILE_BUMPER )
    .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_INPUT )
    .onScore( counter, counter.EVT_10, counter.EVT_100 ); 

  playfield.element( BUMPER_B, COIL_BUMPER_B, LED_BUMPER_B, PROFILE_BUMPER )
    .onLight( true, playfield.element( BUMPER_C ), Atm_element::EVT_INPUT )
    .onScore( counter, counter.EVT_10, counter.EVT_100 ); 

  playfield.element( BUMPER_C, COIL_BUMPER_C, LED_BUMPER_C, PROFILE_BUMPER )
    .onInput( [] ( int idx, int v, int up ) {
      if ( playfield.element( BUMPER_A ).state() && playfield.element( BUMPER_B ).state() ) {
        playfield.leds().on( LED_BUMPER_GRP );
      }
    })
    .onScore( counter, counter.EVT_100, counter.EVT_1000 ); 
  
  playfield.element( KICKER_L, COIL_KICKER_L, LED_KICKER_GRP, PROFILE_KICKER )
    .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON )
    .onScore( counter, counter.EVT_500, counter.EVT_5000 );

  playfield.element( KICKER_R, COIL_KICKER_R, LED_KICKER_GRP, PROFILE_KICKER )
    .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON )
    .onScore( counter, counter.EVT_500, counter.EVT_5000 );
    
  playfield.element( UP_LANE_L, -1, LED_UP_LANE_GRP  )
    .onPress( false, oxo, oxo.EVT_4 ) 
    .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON )
    .onScore( counter, counter.EVT_1000, counter.EVT_5000 );

  playfield.element( UP_LANE_R, -1, LED_UP_LANE_GRP  )
    .onPress( false, oxo, oxo.EVT_6 )
    .onPress( true, playfield.element( BALL_EXIT ), Atm_element::EVT_ON )
    .onScore( counter, counter.EVT_1000, counter.EVT_5000 );
    
  playfield.element(  TARGET_C )
    .onPress( oxo, oxo.EVT_5 )
    .onScore( counter, counter.EVT_500 );

  oxo.begin( playfield, LED_OXO_GRP, PROFILE_OXO )
    .onMatch( playfield.element( KICKER_L ), Atm_element::EVT_ON ); // LED_KICKER_R should automatically follow

  playfield.element( IN_LANE_L )
    .onPress( oxo, oxo.EVT_7 )
    .onScore( counter, counter.EVT_1000 );
    
  playfield.element( IN_LANE_R )
    .onPress( oxo, oxo.EVT_9 )
    .onScore( counter, counter.EVT_1000 );
    
  playfield.debounce( SLING_L, 20, 200 );
  playfield.element( SLING_L, COIL_SLING_L, -1 )
    .onPress( oxo, oxo.EVT_TOGGLE )
    .onScore( counter, counter.EVT_10 ); 
    
  playfield.debounce( SLING_R, 20, 200 );
  playfield.element( SLING_R, COIL_SLING_R, -1 )
    .onPress( oxo, oxo.EVT_TOGGLE )
    .onScore( counter, counter.EVT_10 );

  playfield.element(  ROLLOVER )
    .onPress( oxo, oxo.EVT_8 )
    .onScore( counter, counter.EVT_500 );

  playfield.element( OUT_LANE ).onScore( counter, counter.EVT_1000 );
  
  playfield.debounce( FLIPPER_L, 5, 0 );  // Tune: lowest debounce value, with sufficient flipper force
  playfield.debounce( FLIPPER_R, 5, 0 );  
  playfield.element( FLIPPER_L, COIL_FLIPPER_L, -1, PROFILE_FLIPPER );
  playfield.element( FLIPPER_R, COIL_FLIPPER_R, -1, PROFILE_FLIPPER );

  playfield.element( SAVE_GATE, COIL_SAVE_GATE, -1, PROFILE_GATE );  
  
  playfield.element( BALL_EXIT, COIL_BALL_FEEDER, LED_SHOOTS_AGAIN, PROFILE_FEEDER )
    .onPress( [] ( int idx, int v, int up ) {  // Om dit anders te doen moet je een virtual switch inrichten en daar de LED_FLASHER_GRP aan hangen
      playfield.leds().off( LED_FLASHER_GRP );
    });

  // Dat kan wellicht on-the-fly:
  //playfield.element( BALL_EXIT ).onPress( playfield.element( LED_FLASHERS, LED_FLASHER_GRP ), Atm_element::EVT_OFF );

  playfield.onPress( BALL_ENTER, [] ( int idx, int v, int up ) { 
    if ( playfield.element( BALL_EXIT ).idle( 2000 ) || playfield.element( OUT_LANE ).idle( 2000 ) ) { // Fix hardware switch instead
      playfield.leds().off( LED_BUMPER_GRP );
    }     
  });

  //playfield.element( BALL_ENTER ).onPress( playfield.element( LED_BUMPERS, LED_BUMPER_GRP ), Atm_element::EVT_OFF );

  playfield.element( FRONTBTN ).onPress( counter, counter.EVT_RESET );
  
  // end of logic

  Serial.println( FreeRam() );

}

void loop() {  
  automaton.run();
}
