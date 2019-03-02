
IO io;
Atm_sk6812 output;
Atm_table table;

Human multipleMe[10] = { Human(20.0f,10.0f), Human(18.0f,20.0f), };

Adafruit_NeoPixel strip[] = {
	Adafruit_NeoPixel( 10, pin_data, NEO_GRBW + NEO_KHZ800),
	Adafruit_NeoPixel( 12, pin_data, NEO_GRBW + NEO_KHZ800),
};


enum { 
	LED_ONE, LED_TWO, LED_THREE,
	LED_FOUR, LED_FIVE, LED_SIX,
} leds;

enum {
	PROFILE_LED, PROFILE_BUMPER, PROFILE_SLINGSHOT, PROFILE_FLIPPER, PROFILE_KICKER
} profiles;


io.begin();
output.begin( data_pin )
	.onSelect( [] (int idx, int v, int up ) {
		io.select( v );
	})
	.add( 0, LED_ONE )
	.add( 1, LED_FOUR );

output.defineProfile( PROFILE_LED, slope_up, 127, slope_down );	// slope_up, level, slope_down
output.defineProfile( PROFILE_FLIPPER, 0, 127, 30, 64 );	// delay, kick_level, kick_time, hold_time = -1
output.defineProfile( PROFILE_SIMPLE, 127 ); // Just sets the level (delay = 0, kick_time = -1, hold_time = -1)

output.profile( LED_ONE, PROFILE_LED );
output.profile( LED_THREE, PROFILE_FLIPPER );
output.profile( LED_FOUR, PROFILE_FLIPPER );

output.on( LED_ONE );
output.pulse( LED_THREE );

table.begin( io, output );

 
