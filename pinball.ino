#include "Singularity.h"
#include "hardware.h"
#include "switches.h"
#include "leds.h"
#include "profiles.h"
#include "firmware_custom.h"
#include "freeram.hpp"

using namespace standard_firmware;
using namespace custom_firmware; 

#define NUMBER_OF_BALLS 3
#define NUMBER_OF_PLAYERS 4

IO io;
Atm_led_matrix leds; 
Atm_switch_matrix playfield;
Library lib;

char cmd_buffer[80];
Atm_my_command cmd[2]; 

enum { CMD_PS, CMD_LL, CMD_LO, CMD_HD, CMD_STATS, CMD_TS, CMD_TC, CMD_TR, CMD_PRESS, CMD_RELEASE, CMD_INIT, CMD_INFO, CMD_REBOOT };
const char cmdlist[] = "ps ll lo hd stats ts tc tr press release init info reboot";

void cmd_callback( int idx, int v, int up ) {
  switch ( v ) {
    case CMD_PS:  // TODO add firmware label: 00 SLEEPING 58L    ledbank  game_over
      {           // Better call it PS (because it lists tasks)
        uint8_t map[32];
        uint8_t cnt = 0;
        memset( map, 0, sizeof( map ) );
        cmd[idx].stream->printf( "## State    Sw#               Device             Bytecode\n" ); 
        for ( int16_t n = io.numberOfSwitches() + playfield.numberOfGroups(); n > 0; n-- ) {
          if ( playfield.exists( n + 1 ) == 1 ) {
            Atm_device* dev = &playfield.device( n + 1 );
            uint8_t addr = ( (uint32_t)dev & 0xFFFF ) >> 8;
            if ( ( map[addr >> 3] & ( 1 << ( addr & B111 ) ) ) == 0 ) {
              cmd[idx].stream->printf( "%02d %s %02d%c %20s %20s\n", cnt, 
                dev->sleep() ? "SLEEPING" : "RUNNING ", 
                n + 1, 
                n > io.numberOfSwitches() ? 'L' : 'P', 
                playfield.findSymbol( dev->switchGroup(), 1 ),
                lib.label( lib.findCode( dev->script ) )
                );
              map[addr >> 3] |= ( 1 << ( addr & B111 ) ); 
              cnt++;
            }
          }     
        }
        cmd[idx].stream->println(); 
      }
      return;
    case CMD_LL: // List library entries
      {  
        for ( int16_t i = 0; i < lib.count(); i++ ) {
          cmd[idx].stream->printf( "%02d: %s\n", i, lib.label( i ) );
        }
        cmd[idx].stream->println(); 
      }
      return;
    case CMD_LO: // List leds that are active
      {  
        for ( int16_t i = 0; i < io.numberOfLeds(); i++ ) {
          if ( leds.active( i ) ) {
            cmd[idx].stream->printf( "Led %d: %s %s\n", i, leds.findSymbol( i ), leds.active( i ) ? "ON" : "OFF" );
          } 
        }
        cmd[idx].stream->println(); 
      }
      return;
    case CMD_HD: // Hexdump firmware by firmware or device label
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ), -1 ) ;
        if ( sw > -1 ) {
          lib.hexdump( cmd[idx].stream, lib.findCode( playfield.device( sw ).script ) );      
        } else {
          lib.hexdump( cmd[idx].stream, cmd[idx].arg( 1 ) );      
        }
        cmd[idx].stream->println(); 
      }
      return;
    case CMD_STATS:
      cmd[idx].stream->printf( "Runtime: %02d:%02d:%02d (h:m:s)\n", millis() / 3600000L, ( millis() / 60000L ) % 60, ( millis() / 1000L ) % 60 );
      cmd[idx].stream->printf( "Physical leds: %d (0..%d)\n", io.numberOfLeds(), io.numberOfLeds() - 1 );
      cmd[idx].stream->printf( "Logical leds: %d (%d..%d)\n", leds.numberOfGroups(), io.numberOfLeds(), io.numberOfLeds() + leds.numberOfGroups() - 1 );
      cmd[idx].stream->printf( "Physical switches: %d (1..%d)\n", io.numberOfSwitches(), io.numberOfSwitches() );
      cmd[idx].stream->printf( "Logical switches: %d (%d..%d)\n", playfield.numberOfGroups(), io.numberOfSwitches() + 1, io.numberOfSwitches() + playfield.numberOfGroups() );
      cmd[idx].stream->printf( "Devices: ??\n" );
      cmd[idx].stream->printf( "Firmware: %d\n", lib.count() );
      cmd[idx].stream->printf( "Free RAM: %d\n", FreeRam() );
      cmd[idx].stream->println();
      return;
    case CMD_TS:
      playfield.traceSwitches( *cmd[idx].stream, atoi( cmd[idx].arg( 1 ) ) );
      cmd[idx].stream->printf( "Trace switches: %d\n", atoi( cmd[idx].arg( 1 ) ) );
      return;    
    case CMD_TC:
      { 
        if ( strlen( cmd[idx].arg( 1 ) ) == 0 ) {
          for ( int16_t n = io.numberOfSwitches() + playfield.numberOfGroups(); n > 0; n-- ) {
            if ( playfield.exists( n + 1 ) == 1 ) {
              playfield.device( n + 1 ).traceCode( *cmd[idx].stream, 0 );
            }
          }          
          cmd[idx].stream->println( "Tracing interrupted\n" );
          return;
        }
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t mode = atoi( cmd[idx].arg( 2 ) );
        if ( strlen( cmd[idx].arg( 2 ) ) == 0 ) mode = 1;
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).traceCode( *cmd[idx].stream, mode ); 
          cmd[idx].stream->printf( "Trace code: device %d -> %d\n", sw, mode );
        } else {
          cmd[idx].stream->printf( "Trace code: device %d not found\n", sw );
        }
      }
      return;    
    case CMD_TR:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t sel = atoi( cmd[idx].arg( 3 ) );
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          int16_t e = dev->findSymbol( cmd[idx].arg( 2 ) );
          if ( sel ) {
            dev->trigger( e, sel ); 
          } else {
            dev->trigger( e );             
          }
          cmd[idx].stream->printf( "Trigger: device %d -> %d (%d)\n", sw, e, sel );
        } else {
          cmd[idx].stream->printf( "Trigger: device %d not found\n", sw );
        }
      }
      return;    
    case CMD_PRESS:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t n = atoi( cmd[idx].arg( 2 ) );
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).trigger( ( n * 2 ) + 1, 1 ); 
          cmd[idx].stream->printf( "Press device %d, switch %d -> event %d\n", sw, n, ( n * 2 ) + 1 );
        } else {
          cmd[idx].stream->printf( "Press device %d not found\n", sw );
        }
      }
      return;    
    case CMD_RELEASE:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t n = atoi( cmd[idx].arg( 2 ) );
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).trigger( ( n * 2 ) + 2, 1 ); 
          cmd[idx].stream->printf( "Release device %d -> switch %d, event %d\n", sw, n, ( n * 2 ) + 2 );
        } else {
          cmd[idx].stream->printf( "Release device %d not found\n", sw );
        }
      }
      return;    
    case CMD_INIT:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t n = atoi( cmd[idx].arg( 2 ) );
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).trigger( ( n * 2 ), 1 ); 
          cmd[idx].stream->printf( "Init device %d -> %d\n", sw, n );
        } else {
          cmd[idx].stream->printf( "Init device %d not found\n", sw );
        }
      }
      return;    
    case CMD_REBOOT:
      _reboot_Teensyduino_();
      return;
    case CMD_INFO: // TODO show firmware label & running/sleeping state & device next in chain...
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );        
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          cmd[idx].stream->printf( "Device info for %d: %s [%X:%s] %s\n", sw, playfield.findSymbol( sw, 1 ), dev, dev->sleep() ? "SLEEPING" : "RUNNING", lib.label( lib.findCode( dev->script ) ) );
          for ( int i = 0; i < 80; i++ ) cmd[idx].stream->print( "=" );
          cmd[idx].stream->println();
          for ( uint16_t in = 0; in < dev->countSymbols( 0 ); in++ ) {
            cmd[idx].stream->printf( "In[%02d]  %20s  %04X  %s", in, dev->findSymbol( in, 0 ), dev->handler( in ), playfield.findSymbol( playfield.index( dev->switchGroup(), ( ( in + 1 ) >> 1 ) - 1 ), 1 ) );
            uint8_t map[32];
            uint8_t cnt = 0;
            memset( map, 0, sizeof( map ) );
            for ( int16_t n = io.numberOfSwitches() + playfield.numberOfGroups(); n > 0; n-- ) {
              if ( playfield.exists( n + 1 ) == 1 ) { 
                Atm_device* d = &playfield.device( n + 1 );
                uint8_t addr = ( (uint32_t)d & 0xFFFF ) >> 8;
                if ( ( map[addr >> 3] & ( 1 << ( addr & B111 ) ) ) == 0 ) {
                  for ( int16_t i = 0; i < d->countSymbols( 1 ); i++ ) {
                    if ( dev == d->outputPtr( i ) && in == d->outputEvent( i ) ) { 
                      cmd[idx].stream->printf( "%s:%s ", playfield.findSymbol( d->switchGroup(), 1 ), d->findSymbol( i, 1 ) );
                    }
                  }
                  map[addr >> 3] |= ( 1 << ( addr & B111 ) ); 
                  cnt++;
                }
              }     
            }            
            Serial.println();        
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 1 ); i++ ) {
            Machine* machine = dev->outputPtr( i );
            if ( machine == &playfield ) {
              cmd[idx].stream->printf( "Out[%02d] %20s  %s::%s\n", i, dev->findSymbol( i, 1 ), "playfield", playfield.findSymbol( dev->outputEvent( i ), 0 ) );              
            } else {
              Atm_device* dest = ( Atm_device* ) machine;
              const char* dest_dev_str = playfield.findSymbol( dest->switchGroup(), 1 );
              cmd[idx].stream->printf( "Out[%02d] %20s  ", i, dev->findSymbol( i, 1 ) );
              if ( strlen( dest_dev_str ) > 0 ) { 
                const char* dest_event_str = dest->findSymbol( dev->outputEvent( i ) );
                cmd[idx].stream->printf( "%s", dest_dev_str, dev->outputEvent( i ) );
                if ( dest_event_str == NULL ) {
                  cmd[idx].stream->printf( "::%d", dev->outputEvent( i ) );
                } else {
                  cmd[idx].stream->printf( "::%s", dest_event_str );
                }
              }
              cmd[idx].stream->println();
            }
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 2 ); i++ ) {
            const char* arg_str = leds.findSymbol( leds.index( dev->ledGroup(), i ) );
            if ( arg_str[0] == '\0' ) {
              cmd[idx].stream->printf( "Arg[%02d] %20s\n", i, dev->findSymbol( i, 2 ) );
            } else {
              cmd[idx].stream->printf( "Arg[%02d] %20s  %s %s\n", i, dev->findSymbol( i, 2 ), leds.findSymbol( leds.index( dev->ledGroup(), i ) ), leds.active( leds.index( dev->ledGroup(), i ) ) ? "ON" : "OFF" );              
            }
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 3 ); i++ ) {
            cmd[idx].stream->printf( "Reg[%02d] %20s  %d\n", i, dev->findSymbol( i, 3 ), dev->reg( i ) );
          }
        } else {
          cmd[idx].stream->printf( "Init device %d not found\n", sw );
        }
      }
      cmd[idx].stream->println();
      return;    
  }
} 

void dumpSymbols( int16_t d, int16_t bank = -1 ) {
  Atm_device* dev = &playfield.device( d );
  for ( int16_t b = 0; b < 4; b++ ) {
    for ( int16_t i = 0; i < dev->countSymbols( b ); i++ ) {
      if ( bank == -1 || bank == b ) {
        Serial.printf( "%d %d %s -> %d\n", b, i, dev->findSymbol( i, b ), dev->findSymbol( dev->findSymbol( i, b ) ) );
      }
    }
  }
}

void setup() {
  delay( 1000 );
  Serial.println( "Singularity Shell\ninit IO" );
  delay( 100 );

  Serial1.begin( 9600 );
  cmd[0].begin( Serial, cmd_buffer, sizeof( cmd_buffer ) )
    .list( cmdlist )
    .onCommand( cmd_callback, 0 );
    
  cmd[1].begin( Serial1, cmd_buffer, sizeof( cmd_buffer ) )
    .list( cmdlist )
    .onCommand( cmd_callback, 1 );
  Serial1.println( "Singularity OS" );

  io.begin( pin_clock, pin_latch, addr, shift_inputs, gate )
    .switchMap( 3, 1, 1 )
    .addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    .addStrip( new IO_Adafruit_NeoPixel( 12, pin_data, NEO_GRBW + NEO_KHZ800 ) ) //  4 pixel SK6812 led strip on P2/cabinet with 8 virtual leds added 
    .addStrip( new IO_Adafruit_NeoPixel( 36, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 36 pixel SK6812 led strip on P3/headbox
    .invert( BALL_ENTER )
    .retrigger()
    .show();

  Serial.println( "init leds" ); delay( 100 );
  leds.begin( io, led_groups )
    .loadSymbols( led_symbols )
    .readProfiles( 'L', profiles );
  
  Serial.println( "init playfield" ); delay( 1000 );
  playfield.begin( io, leds, switch_groups, LED_EXTRA ) // TODO LED_EXTRA moet hier weg: separation of concerns!!!
    .loadSymbols( switch_symbols )
    .readProfiles( 'S', profiles );

  

  int32_t base_ram = FreeRam();
  Serial.println( "import firmware from flash" ); delay( 100 );

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

  playfield.compileList( switch_group_list, playfield.countSymbols( 1 ) - io.numberOfSwitches(), io.numberOfSwitches() + 1 );
  lib.compile( "cust_dual_flipper", dual_flipper_bytecode );

  Serial.println( "init devices" ); delay( 100 );

  playfield.device( "chimes", "led_chime_grp", lib.code( "std_ledbank" ) );
  playfield.device( "counter", "led_counter0_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "counter1", "led_counter1_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "counter2", "led_counter2_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "counter3", "led_counter3_grp", lib.code( "std_counter_em4d1w" ) );
  playfield.device( "bumper_a", "led_bumper_a_grp", lib.code( "std_bumper" ) );
  playfield.device( "bumper_b", "led_bumper_b_grp", lib.code( "std_bumper" ) );
  playfield.device( "bumper_c", "led_bumper_c_grp", lib.code( "std_bumper" ) );
  playfield.device( "oxo", "led_oxo_grp", lib.code( "std_tictactoe" ) );
  playfield.device( "multilane", "led_none_grp", lib.code( "std_switchbank" ) ); 
  playfield.device( "dual_target", "led_target_grp", lib.code( "std_dual_target" ) );
  playfield.device( "kicker", "led_kicker_grp", lib.code( "std_dual_kicker" ) );
  playfield.device( "uplane", "led_uplane_grp", lib.code( "std_dual_combo" ) ); 
  playfield.device( "slingshot", "led_slingshot_grp", lib.code( "std_dual_kicker" ) );
  playfield.device( "lower", "led_none_grp", lib.code( "std_switchbank" ) ); 
  playfield.device( "flipper", "led_flipper_grp", lib.code( "std_dual_flipper" ) ); 
  playfield.device( "again", "led_again_grp", lib.code( "std_ledbank" ) );
  playfield.device( "save_gate", "coil_save_gate", lib.code( "std_ledbank" ) );
  playfield.device( "feeder", "coil_feeder", lib.code( "std_ledbank" ) );
  playfield.device( "game_over", "led_game_over", lib.code( "std_ledbank" ) );  
  playfield.device( "players", "led_players_grp", lib.code( "std_scalar" ) );
  playfield.device( "playerup", "led_playerup_grp", lib.code( "std_scalar" ) );
  playfield.device( "ballup", "led_ballup_grp", lib.code( "std_scalar" ) );
  playfield.device( "gi", "coil_gi", lib.code( "std_ledbank" ), 1 ); // default on
  playfield.device( "frontbtn", "led_game_grp", lib.code( "std_game" ), NUMBER_OF_BALLS, NUMBER_OF_PLAYERS );
  
  Serial.println( "chain devices" ); delay( 100 );

  playfield.device( "counter" ).chain( "counter1" ).chain( "counter2" ).chain( "counter3" );

  automaton.delay( 1000 ); // Visible reset indicator... (GI fades off/on)

  Serial.println( "link devices" ); delay( 100 );

  playfield.link( "counter","out_digit1", "chimes", "on0" );
  playfield.link( "counter", "out_digit2", "chimes", "on1" );
  playfield.link( "counter", "out_digit3", "chimes", "on2" );
  
  playfield.link( "oxo", "out_win_row", "kicker", "on" );
  playfield.link( "oxo", "out_win_all", "uplane", "on" );
  playfield.link( "oxo", "out_collect", "counter", "pt1000" );
   
  playfield.link( "multilane", "out_press0", "oxo", "oxo_1o" ); 
  playfield.link( "multilane", "out_press1", "oxo", "oxo_1x" );
  playfield.link( "multilane", "out_press2", "oxo", "oxo_2o" );
  playfield.link( "multilane", "out_press3", "oxo", "oxo_2x" );
  playfield.link( "multilane", "out_press4", "oxo", "oxo_3o" ); 
  playfield.link( "multilane", "out_press5", "oxo", "oxo_3x" );
  playfield.link( "multilane", "out_score", "counter", "pt1000" );

  playfield.link( "bumper_a", "out_score_lit", "counter", "pt100" );
  playfield.link( "bumper_a", "out_score_unlit", "counter", "pt10" );  
  
  playfield.link( "bumper_b", "out_score_lit", "counter", "pt100" );
  playfield.link( "bumper_b", "out_score_unlit", "counter", "pt10" );  
    
  playfield.link( "bumper_c", "out_score_lit", "counter", "pt1000" );
  playfield.link( "bumper_c", "out_score_unlit", "counter", "pt100" );  
  playfield.link( "bumper_c", "out_light_on", "save_gate", "on" );
  playfield.link( "bumper_c", "out_light_off", "save_gate", "off" );

  playfield.link( "dual_target", "out_led0_on", "bumper_a", "turn_on" );
  playfield.link( "dual_target", "out_led0_off", "bumper_a", "turn_off" );
  playfield.link( "dual_target", "out_led1_on", "bumper_b", "turn_on" );
  playfield.link( "dual_target", "out_led1_off", "bumper_b", "turn_off" );
  playfield.link( "dual_target", "out_all_on", "bumper_c", "turn_on" );  
  playfield.link( "dual_target", "out_all_off", "bumper_c", "turn_off" );
  playfield.link( "dual_target", "out_score", "counter", "pt1000" );
  
  playfield.link( "kicker", "out_press_lit", "again", "on" );
  playfield.link( "kicker", "out_score_lit", "counter", "pt5000" );
  playfield.link( "kicker", "out_score_unlit", "counter", "pt500" );
  
  playfield.link( "uplane", "out_score", "counter", "pt1000" );
  playfield.link( "uplane", "out_press_lit", "again", "on" );
  playfield.link( "uplane", "out_press0_unlit", "oxo", "oxo_4" );
  playfield.link( "uplane", "out_press1_unlit", "oxo", "oxo_6" );

  playfield.link( "slingshot", "out_score", "counter", "pt10" );
  playfield.link( "slingshot", "out_press", "oxo", "toggle" );    

  playfield.link( "lower", "out_press0", "oxo", "oxo_5" );                  // 0 target_c
  playfield.link( "lower", "out_score0", "counter", "pt500" );  
  playfield.link( "lower", "out_press1", "oxo", "oxo_7" );                  // 1 inlane_l
  playfield.link( "lower", "out_score1", "counter", "pt1000" );
  playfield.link( "lower", "out_press2", "oxo", "oxo_9" );                  // 2 inlane_r
  playfield.link( "lower", "out_score2", "counter", "pt1000" );
  playfield.link( "lower", "out_press3", "oxo", "oxo_8" );                  // 3 rollover
  playfield.link( "lower", "out_score3", "counter", "pt500" );
  playfield.link( "lower", "out_score4", "counter", "pt1000" );             // 4 outlane
  playfield.link( "lower", "out_press5", playfield, "pf_ready" );
  playfield.link( "lower", "out_press6", "dual_target", "clear" );          // 6 ball_enter 
  
  playfield.link( "frontbtn", "out_init", playfield, "pf_init" );
  playfield.link( "frontbtn", "out_enable", playfield, "pf_enable" );

  playfield.link( "frontbtn", "out_counter_reset", "counter", "reset" );
  playfield.link( "frontbtn", "out_ball_zero", "ballup", "zero" );
  playfield.link( "frontbtn", "out_player_zero", "playerup", "zero" );
  playfield.link( "frontbtn", "out_ball_adv", "ballup", "advance" );
  playfield.link( "frontbtn", "out_player_adv", "playerup", "advance" );
  playfield.link( "frontbtn", "out_3bonus", "oxo", "triple" );
  playfield.link( "frontbtn", "out_collect", "oxo", "collect" );
  playfield.link( "frontbtn", "out_kickoff", "feeder", "on" );
  playfield.link( "frontbtn", "out_players_zero", "players", "zero" );
  playfield.link( "frontbtn", "out_players_adv", "players", "advance" );
  playfield.link( "frontbtn", "out_over", "game_over", "on" );

  playfield.device( "kicker" ).trigger( IN_KICKER_PERSIST );
  
  Serial.printf( "%.2f KBytes available, %.2f KBytes used for devices, %.2f Bytes free\n\n", 
        (float) base_ram / 1024, (float)( base_ram - FreeRam() ) / 1024, (float)( FreeRam() ) );

}

void loop() {
  automaton.run(); 
}
