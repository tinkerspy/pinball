#include "Singularity.h"
#include "hardware.h"
#include "switches.h"
#include "leds.h"
#include "profiles.h"
#include "firmware_custom.h"
#include "freeram.hpp"

// Bug: info counter3

// Commands: attach, leds, switches, led_groups, switch_groups, invert, device, chain, link, firmware, load, save, autoload

using namespace standard_firmware;
using namespace custom_firmware; 

#define NUMBER_OF_BALLS 3
#define NUMBER_OF_PLAYERS 4

IO io;
Atm_led_matrix leds; 
Atm_switch_matrix playfield;
Library lib;

#include "CLI.h"

void setup() {
  //delay( 1000 );
  Serial.println( "Singularity Shell\r\ninit IO" );
  delay( 100 );

  Serial1.begin( 9600 );
  cmd[0].begin( Serial, cmd_buffer[0], sizeof( cmd_buffer[0] ) )
    .list( cmdlist )
    .echo( true )
    .onCommand( cmd_callback, 0 );
    
  cmd[1].begin( Serial1, cmd_buffer[1], sizeof( cmd_buffer[1] ) ) 
    .list( cmdlist )
    .echo( true )
    .onCommand( cmd_callback, 1 );
    
  Serial1.println( "Singularity OS" );

  uint16_t neo_mode = IO_Adafruit_NeoPixel::str2rgb( "neo_grbw" ) + IO_Adafruit_NeoPixel::str2freq( "neo_khz800" ); 
  Serial.printf( "NEO_MODE %X == %X\r\n", neo_mode, NEO_GRBW + NEO_KHZ800 );
  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 1 )
    .addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    .addStrip( new IO_Adafruit_NeoPixel( 12, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P2/cabinet with 8 virtual leds added 
    .addStrip( new IO_Adafruit_NeoPixel( 36, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 36 pixel SK6812 led strip on P3/headbox
    .retrigger()
    .show();

  Serial.println( "init leds" ); //delay( 100 );
  leds.begin( io );

  Serial.println( "init playfield" ); //delay( 100 );
  playfield.begin( io, leds );
     
  Serial.println( "init playfield switches" ); //delay( 100 );
  
  int32_t base_ram = FreeRam();
  Serial.println( "import firmware from flash" ); //delay( 100 );

  lib.import( "std_bumper", bumper_symbin, bumper_hexbin );
  lib.import( "std_dual_target", dual_target_symbin, dual_target_hexbin );
  lib.import( "std_game", game_symbin, game_hexbin );
  lib.import( "std_counter_em4d1w", counter_em4d1w_symbin, counter_em4d1w_hexbin );
  lib.import( "std_ledbank", ledbank_symbin, ledbank_hexbin );
  lib.import( "std_switchbank", switchbank_symbin, switchbank_hexbin );
  lib.import( "std_scalar", scalar_symbin, scalar_hexbin );
  lib.import( "std_dual_kicker", kicker_symbin, kicker_hexbin  );
  lib.import( "std_dual_combo", dual_combo_symbin, dual_combo_hexbin );
  lib.import( "std_dual_flipper", dual_flipper_symbin, dual_flipper_hexbin );
  lib.import( "std_tictactoe", tictactoe_symbin, tictactoe_hexbin );
  lib.import( "std_animation", animation_symbin, animation_hexbin );

  //lib.compile( "std_game", game_bytecode );
  //lib.compile( "std_switchbank", switchbank_bytecode );
  //lib.compile( "std_counter_em4d1w", counter_em4d1w_bytecode );
  //lib.compile( "std_animation", animation_bytecode );
  //lib.compile( "std_scalar", scalar_bytecode );
  //lib.compile( "std_ledbank", ledbank_bytecode );

//  playfield.device( "kicker" ).trigger( IN_KICKER_PERSIST ); FIXME SOMEHOW!
  
  Serial.printf( "%.2f KBytes available, %.2f KBytes used for devices, %.2f Bytes free\r\n\r\n", 
        (float) base_ram / 1024, (float)( base_ram - FreeRam() ) / 1024, (float)( FreeRam() ) );

}

void loop() {
  automaton.run(); 
}
