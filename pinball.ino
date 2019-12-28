#include "Singularity.h"
#include "hardware.h"
#include "switches.h"
#include "leds.h"
#include "profiles.h"
#include "firmware_custom.h"
#include "freeram.hpp"

// Bug: info counter3

#define PRELOAD_CONFIG
#define NUMBER_OF_BALLS 3
#define NUMBER_OF_PLAYERS 4

using namespace standard_firmware;
using namespace custom_firmware; 

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

  uint16_t neo_mode = IO_Adafruit_NeoPixel::str2int( "neo_grbw" ) + IO_Adafruit_NeoPixel::str2int( "neo_khz800" ); 
  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate );
  
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

#ifdef PRELOAD_CONFIG
  #include "load_config.h"
#endif

//  playfield.device( "kicker" ).trigger( IN_KICKER_PERSIST ); FIXME SOMEHOW!
  
  Serial.printf( "%.2f KBytes available, %.2f KBytes used for devices, %.2f Bytes free\r\n\r\n", 
        (float) base_ram / 1024, (float)( base_ram - FreeRam() ) / 1024, (float)( FreeRam() ) );

}

void loop() {
  automaton.run(); 
}
