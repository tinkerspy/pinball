#include "Singularity.h"
#include "hardware.h"
#include "switches.h"
#include "leds.h"
#include "profiles.h"
#include "firmware_custom.h"
#include "freeram.hpp"

#define PRELOAD_CONFIG
#undef COMPILE_FW
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
  delay( 100 );
  Serial.println( "Singularity Shell\r\ninit IO" );

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

  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate );
  
  int32_t base_ram = FreeRam();

#ifdef COMPILE_FW
  Serial.println( "compile firmware from flash" ); //delay( 100 );
  lib.compile( "std_bumper", bumper_bytecode );
  lib.compile( "std_dual_target", dual_target_bytecode );
  lib.compile( "std_counter_em4d1w", counter_em4d1w_bytecode );
  lib.compile( "std_ledbank", ledbank_bytecode );
  lib.compile( "std_switchbank", switchbank_bytecode );
  lib.compile( "std_scalar", scalar_bytecode );
  lib.compile( "std_dual_kicker", dual_kicker_bytecode  );
  lib.compile( "std_dual_slingshot", dual_slingshot_bytecode  );
  lib.compile( "std_dual_combo", dual_combo_bytecode );
  lib.compile( "std_dual_flipper", dual_flipper_bytecode );

  lib.compile( "std_game", game_bytecode );
  lib.compile( "std_tictactoe", tictactoe_bytecode ); // phucked! it's just too big for loadintlist!!!
  lib.compile( "std_animation", animation_bytecode );
#else 
  Serial.println( "import firmware from flash" ); //delay( 100 );
  lib.import( "std_bumper", std_bumper_symbin, std_bumper_hexbin );
  lib.import( "std_dual_target", std_dual_target_symbin, std_dual_target_hexbin );
  lib.import( "std_counter_em4d1w", std_counter_em4d1w_symbin, std_counter_em4d1w_hexbin );
  lib.import( "std_ledbank", std_ledbank_symbin, std_ledbank_hexbin );
  lib.import( "std_switchbank", std_switchbank_symbin, std_switchbank_hexbin );
  lib.import( "std_scalar", std_scalar_symbin, std_scalar_hexbin );
  lib.import( "std_dual_kicker", std_dual_kicker_symbin, std_dual_kicker_hexbin  );
  lib.import( "std_dual_slingshot", std_dual_slingshot_symbin, std_dual_slingshot_hexbin  );
  lib.import( "std_dual_combo", std_dual_combo_symbin, std_dual_combo_hexbin );
  lib.import( "std_dual_flipper", std_dual_flipper_symbin, std_dual_flipper_hexbin );
  
  lib.import( "std_game", std_game_symbin, std_game_hexbin );
  lib.import( "std_tictactoe", std_tictactoe_symbin, std_tictactoe_hexbin );
  lib.import( "std_animation", std_animation_symbin, std_animation_hexbin );
#endif


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
