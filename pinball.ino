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
#undef SYMBOLS

IO io;
Atm_led_matrix leds; 
Atm_switch_matrix playfield;
Library library;

char cmd_buffer[80];
Atm_my_command cmd[2]; 

enum { CMD_LS, CMD_STAT, CMD_TS, CMD_TC, CMD_TR, CMD_PRESS, CMD_RELEASE, CMD_INIT, CMD_INFO };
const char cmdlist[] = "ls stat ts tc tr press release init info";

void cmd_callback( int idx, int v, int up ) {
  switch ( v ) {
    case CMD_LS: 
      {  
        uint8_t map[32];
        uint8_t cnt = 0;
        memset( map, 0, sizeof( map ) );
        for ( int16_t n = io.numberOfSwitches() + playfield.numberOfGroups(); n > 0; n-- ) {
          if ( playfield.exists( n + 1 ) == 1 ) {
            Atm_device* dev = &playfield.device( n + 1 );
            uint8_t addr = ( (uint32_t)dev & 0xFFFF ) >> 8;
            if ( ( map[addr >> 3] & ( 1 << ( addr & B111 ) ) ) == 0 ) {
              cmd[idx].stream->printf( "%02d %s %02d%c %s\n", cnt, 
                dev->sleep() ? "SLEEPING" : "RUNNING ", n + 1, n > io.numberOfSwitches() ? 'L' : 'P', playfield.findSymbol( dev->switchGroup(), 1 ) );
              map[addr >> 3] |= ( 1 << ( addr & B111 ) ); 
              cnt++;
            }
          }     
        }
        cmd[idx].stream->println(); 
      }
      return;
    case CMD_STAT:
      cmd[idx].stream->printf( "Physical leds: %d (0..%d)\n", io.numberOfLeds(), io.numberOfLeds() - 1 );
      cmd[idx].stream->printf( "Logical leds: %d (%d..%d)\n", leds.numberOfGroups(), io.numberOfLeds(), io.numberOfLeds() + leds.numberOfGroups() - 1 );
      cmd[idx].stream->printf( "Physical switches: %d (1..%d)\n", io.numberOfSwitches(), io.numberOfSwitches() );
      cmd[idx].stream->printf( "Logical switches: %d (%d..%d)\n", playfield.numberOfGroups(), io.numberOfSwitches() + 1, io.numberOfSwitches() + playfield.numberOfGroups() );
      cmd[idx].stream->printf( "Free RAM: %d\n", FreeRam() );
      cmd[idx].stream->println();
      return;
    case CMD_TS:
      playfield.traceSwitches( *cmd[idx].stream, atoi( cmd[idx].arg( 1 ) ) );
      cmd[idx].stream->printf( "Trace switches: %d\n", atoi( cmd[idx].arg( 1 ) ) );
      return;    
    case CMD_TC:
      {
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
    case CMD_INFO:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );        
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          Serial.printf( "Device info for: %s\n", playfield.findSymbol( sw, 1 ) );
          for ( int i = 0; i < 80; i++ ) Serial.print( "=" );
          Serial.println();
          for ( uint16_t i = 0; i < dev->countSymbols( 0 ); i++ ) {
            cmd[idx].stream->printf( "In[%02d]  %30s  %04X  %s\n", i, dev->findSymbol( i, 0 ), dev->handler( i ), playfield.findSymbol( playfield.index( dev->switchGroup(), ( ( i + 1 ) >> 1 ) - 1 ), 1 ) );
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 1 ); i++ ) {
            Machine* machine = dev->outputPtr( i );
            if ( machine == &playfield ) {
              cmd[idx].stream->printf( "Out[%02d] %30s  %s::%s\n", i, dev->findSymbol( i, 1 ), "playfield", playfield.findSymbol( dev->outputEvent( i ), 0 ) );              
            } else {
              Atm_device* dest = ( Atm_device* ) machine;
              char* dest_dev_str = playfield.findSymbol( dest->switchGroup(), 1 );
              cmd[idx].stream->printf( "Out[%02d] %30s  ", i, dev->findSymbol( i, 1 ) );
              if ( dest_dev_str ) { 
                char* dest_event_str = dest->findSymbol( dev->outputEvent( i ) );
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
            char* arg_str = leds.findSymbol( leds.index( dev->ledGroup(), i ) );
            if ( arg_str[0] == '\0' ) {
              cmd[idx].stream->printf( "Arg[%02d] %30s\n", i, dev->findSymbol( i, 2 ) );
            } else {
              cmd[idx].stream->printf( "Arg[%02d] %30s  %s %s\n", i, dev->findSymbol( i, 2 ), leds.findSymbol( leds.index( dev->ledGroup(), i ) ), leds.active( leds.index( dev->ledGroup(), i ) ) ? "ON" : "OFF" );              
            }
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 3 ); i++ ) {
            cmd[idx].stream->printf( "Reg[%02d] %30s  %d\n", i, dev->findSymbol( i, 3 ), dev->reg( i ) );
          }
        } else {
          cmd[idx].stream->printf( "Init device %d not found\n", sw );
        }
      }
      cmd[idx].stream->println();
      return;    
  }
} 

void setup() {
  delay( 1000 );
  Serial.println( "Singularity OS\ninit IO" );
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
  Serial.println( "init devices" ); delay( 100 );

  Serial.printf( "Compile -> %d\n", library.compile( "bumper", bumper_bytecode ) );
  Serial.printf( "Compile -> %d\n", library.compile( "dual_target", dual_target_bytecode ) );

#ifdef SYMBOLS
  playfield.device( CHIMES, LED_CHIME_GRP, ledbank_firmware ).loadSymbols( ledbank_symbols );
  playfield.device( COUNTER, LED_COUNTER0_GRP, counter_em4d1w_firmware ).loadSymbols( counter_em4d1w_symbols );
  playfield.device( COUNTER1, LED_COUNTER1_GRP, counter_em4d1w_firmware ).loadSymbols( counter_em4d1w_symbols );
  playfield.device( COUNTER2, LED_COUNTER2_GRP, counter_em4d1w_firmware ).loadSymbols( counter_em4d1w_symbols );
  playfield.device( COUNTER3, LED_COUNTER3_GRP, counter_em4d1w_firmware ).loadSymbols( counter_em4d1w_symbols );
  playfield.device( OXO, LED_OXO_GRP, tictactoe_firmware ).loadSymbols( tictactoe_symbols );
  playfield.device( MULTILANE, -1, switchbank_firmware ).loadSymbols( switchbank_symbols ); 
  playfield.device( BUMPER_A, LED_BUMPER_A_GRP, bumper_firmware ).loadSymbols( bumper_symbols );
  playfield.device( BUMPER_B, LED_BUMPER_B_GRP, bumper_firmware ).loadSymbols( bumper_symbols );
  playfield.device( BUMPER_C, LED_BUMPER_C_GRP, bumper_firmware ).loadSymbols( bumper_symbols );
  playfield.device( DUAL_TARGET, LED_TARGET_GRP, dual_target_firmware ).loadSymbols( dual_target_symbols );
  playfield.device( KICKER, LED_KICKER_GRP, dual_kicker_firmware ).loadSymbols( dual_kicker_symbols );
  playfield.device( UPLANE, LED_UPLANE_GRP, dual_combo_firmware ).loadSymbols( dual_combo_symbols );
  playfield.device( SLINGSHOT, LED_SLINGSHOT_GRP, dual_kicker_firmware ).loadSymbols( dual_kicker_symbols );
  playfield.device( LOWER, -1, switchbank_firmware ).loadSymbols( switchbank_symbols ); 
  playfield.device( FLIPPER, LED_FLIPPER_GRP, dual_flipper_firmware ).loadSymbols( dual_flipper_symbols );    
  playfield.device( AGAIN, LED_AGAIN_GRP, ledbank_firmware ).loadSymbols( ledbank_symbols );
  playfield.device( SAVE_GATE, COIL_SAVE_GATE, ledbank_firmware ).loadSymbols( ledbank_symbols );
  playfield.device( FEEDER, COIL_FEEDER, ledbank_firmware ).loadSymbols( ledbank_symbols );
  playfield.device( GAME_OVER, LED_GAME_OVER, ledbank_firmware ).loadSymbols( ledbank_symbols );  
  playfield.device( PLAYERS, LED_PLAYERS_GRP, scalar_firmware ).loadSymbols( scalar_symbols );
  playfield.device( PLAYERUP, LED_PLAYERUP_GRP, scalar_firmware ).loadSymbols( scalar_symbols );
  playfield.device( BALLUP, LED_BALLUP_GRP, scalar_firmware ).loadSymbols( scalar_symbols );
  playfield.device( GI, COIL_GI, ledbank_firmware, 1 ).loadSymbols( ledbank_symbols ); // Default ON
  playfield.device( FRONTBTN, LED_GAME_GRP, game_firmware, NUMBER_OF_BALLS, NUMBER_OF_PLAYERS ).loadSymbols( game_symbols );
#else   
  playfield.device( CHIMES, LED_CHIME_GRP, ledbank_firmware );
  playfield.device( COUNTER, LED_COUNTER0_GRP, counter_em4d1w_firmware ); //.loadSymbols( counter_em4d1w_symbols );
  playfield.device( COUNTER1, LED_COUNTER1_GRP, counter_em4d1w_firmware );
  playfield.device( COUNTER2, LED_COUNTER2_GRP, counter_em4d1w_firmware );
  playfield.device( COUNTER3, LED_COUNTER3_GRP, counter_em4d1w_firmware );
  playfield.device( OXO, LED_OXO_GRP, tictactoe_firmware );
  playfield.device( MULTILANE, -1, switchbank_firmware ); 
  playfield.device( BUMPER_A, LED_BUMPER_A_GRP, library.codePtr( library.index( "dual_target" ) ) ).loadSymbols( library.symbolPtr( library.index( "dual_target" ) ) );
  playfield.device( BUMPER_B, LED_BUMPER_B_GRP, library.codePtr( library.index( "dual_target" ) ) );
  playfield.device( BUMPER_C, LED_BUMPER_C_GRP, library.codePtr( library.index( "dual_target" ) ) );
  playfield.device( DUAL_TARGET, LED_TARGET_GRP, dual_target_firmware );
  playfield.device( KICKER, LED_KICKER_GRP, dual_kicker_firmware );
  playfield.device( UPLANE, LED_UPLANE_GRP, dual_combo_firmware );
  playfield.device( SLINGSHOT, LED_SLINGSHOT_GRP, dual_kicker_firmware );
  playfield.device( LOWER, -1, switchbank_firmware ); 
  playfield.device( FLIPPER, LED_FLIPPER_GRP, dual_flipper_firmware );    
  playfield.device( AGAIN, LED_AGAIN_GRP, ledbank_firmware );
  playfield.device( SAVE_GATE, COIL_SAVE_GATE, ledbank_firmware );
  playfield.device( FEEDER, COIL_FEEDER, ledbank_firmware );
  playfield.device( GAME_OVER, LED_GAME_OVER, ledbank_firmware );  
  playfield.device( PLAYERS, LED_PLAYERS_GRP, scalar_firmware );
  playfield.device( PLAYERUP, LED_PLAYERUP_GRP, scalar_firmware );
  playfield.device( BALLUP, LED_BALLUP_GRP, scalar_firmware );
  playfield.device( GI, COIL_GI, ledbank_firmware, 1 ); // Default ON
  playfield.device( FRONTBTN, LED_GAME_GRP, game_firmware, NUMBER_OF_BALLS, NUMBER_OF_PLAYERS ); //.loadSymbols( game_symbols );
#endif


  //symbolic_machine_table* sym = lib.symbolPtr( idx );
  int16_t* code = library.codePtr( library.index( "dual_target" ) );

  for ( int16_t i = 0; i < 67; i++ ) {
    //Serial.printf( "%d\n", code[i] );
  }
  
  Serial.println( "chain devices" ); delay( 100 );

  playfield.device( COUNTER ).chain( COUNTER1 );
  playfield.device( COUNTER1 ).chain( COUNTER2 );
  playfield.device( COUNTER2 ).chain( COUNTER3 );

  automaton.delay( 1000 ); // Visible reset indicator... (GI fades off/on)

  Serial.println( "link devices" ); delay( 100 );

  playfield.device( COUNTER ).onEvent( OUT_CTR_DIGIT1, CHIMES, IN_LBANK_ON0 );
  playfield.device( COUNTER ).onEvent( OUT_CTR_DIGIT2, CHIMES, IN_LBANK_ON1 );
  playfield.device( COUNTER ).onEvent( OUT_CTR_DIGIT3, CHIMES, IN_LBANK_ON2 );
  
  playfield.device( OXO, LED_OXO_GRP).onEvent( OUT_OXO_WIN_ROW, KICKER, IN_KICKER_ON );
  playfield.device( OXO, LED_OXO_GRP).onEvent( OUT_OXO_WIN_ALL, UPLANE, IN_COMBO_ON );
  playfield.device( OXO, LED_OXO_GRP).onEvent( OUT_OXO_COLLECT, COUNTER, IN_CTR_PT1000 );
  
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_PRESS0, OXO, IN_OXO_1O );
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_PRESS1, OXO, IN_OXO_1X );
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_PRESS2, OXO, IN_OXO_2O );
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_PRESS3, OXO, IN_OXO_2X );
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_PRESS4, OXO, IN_OXO_3O );
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_PRESS5, OXO, IN_OXO_3X );
  playfield.device( MULTILANE ).onEvent( OUT_SBANK_SCORE, COUNTER, IN_CTR_PT1000 );
  
  playfield.device( BUMPER_A ).onEvent( OUT_BUMPER_SCORE_LIT, COUNTER, IN_CTR_PT100 );
  playfield.device( BUMPER_A ).onEvent( OUT_BUMPER_SCORE_UNLIT, COUNTER, IN_CTR_PT10 );  
  
  playfield.device( BUMPER_B ).onEvent( OUT_BUMPER_SCORE_LIT, COUNTER, IN_CTR_PT100 );
  playfield.device( BUMPER_B ).onEvent( OUT_BUMPER_SCORE_UNLIT, COUNTER, IN_CTR_PT10 );  
    
  playfield.device( BUMPER_C ).onEvent( OUT_BUMPER_SCORE_LIT, COUNTER, IN_CTR_PT1000 );
  playfield.device( BUMPER_C ).onEvent( OUT_BUMPER_SCORE_UNLIT, COUNTER, IN_CTR_PT100 );  
  playfield.device( BUMPER_C ).onEvent( OUT_BUMPER_LIGHT_ON, SAVE_GATE, IN_LBANK_ON );
  playfield.device( BUMPER_C ).onEvent( OUT_BUMPER_LIGHT_OFF, SAVE_GATE, IN_LBANK_OFF );

  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_LED0_ON, BUMPER_A, IN_BUMPER_LIGHT_ON );
  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_LED0_OFF, BUMPER_A, IN_BUMPER_LIGHT_OFF );
  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_LED1_ON, BUMPER_B, IN_BUMPER_LIGHT_ON );
  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_LED1_OFF, BUMPER_B, IN_BUMPER_LIGHT_OFF );
  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_ALL_ON, BUMPER_C, IN_BUMPER_LIGHT_ON );  
  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_ALL_OFF, BUMPER_C, IN_BUMPER_LIGHT_OFF );
  playfield.device( DUAL_TARGET ).onEvent( OUT_TARGET_SCORE, COUNTER, IN_CTR_PT1000 );
  
  playfield.device( KICKER ).onEvent( OUT_KICKER_PRESS_LIT, AGAIN, IN_LBANK_ON );
  playfield.device( KICKER ).onEvent( OUT_KICKER_SCORE_LIT, COUNTER, IN_CTR_PT5000 );
  playfield.device( KICKER ).onEvent( OUT_KICKER_SCORE_UNLIT, COUNTER, IN_CTR_PT500 );
  
  playfield.device( UPLANE ).onEvent( OUT_COMBO_SCORE, COUNTER, IN_CTR_PT1000 );
  playfield.device( UPLANE ).onEvent( OUT_COMBO_PRESS_LIT, AGAIN, IN_LBANK_ON );
  playfield.device( UPLANE ).onEvent( OUT_COMBO_PRESS0_UNLIT, OXO, IN_OXO_4 );
  playfield.device( UPLANE ).onEvent( OUT_COMBO_PRESS1_UNLIT, OXO, IN_OXO_6 );

  playfield.device( SLINGSHOT ).onEvent( OUT_KICKER_SCORE, COUNTER, IN_CTR_PT10 );
  playfield.device( SLINGSHOT ).onEvent( OUT_KICKER_PRESS, OXO, IN_OXO_TOGGLE );    

  playfield.device( LOWER ).onEvent( OUT_SBANK_PRESS0, OXO, IN_OXO_5 );                  // 0 TARGET_C
  playfield.device( LOWER ).onEvent( OUT_SBANK_SCORE0, COUNTER, IN_CTR_PT500 );  
  playfield.device( LOWER ).onEvent( OUT_SBANK_PRESS1, OXO, IN_OXO_7 );                  // 1 INLANE_L
  playfield.device( LOWER ).onEvent( OUT_SBANK_SCORE1, COUNTER, IN_CTR_PT1000 );
  playfield.device( LOWER ).onEvent( OUT_SBANK_PRESS2, OXO, IN_OXO_9 );                  // 2 INLANE_R
  playfield.device( LOWER ).onEvent( OUT_SBANK_SCORE2, COUNTER, IN_CTR_PT1000 );
  playfield.device( LOWER ).onEvent( OUT_SBANK_PRESS3, OXO, IN_OXO_8 );                  // 3 ROLLOVER
  playfield.device( LOWER ).onEvent( OUT_SBANK_SCORE3, COUNTER, IN_CTR_PT500 );
  playfield.device( LOWER ).onEvent( OUT_SBANK_SCORE4, COUNTER, IN_CTR_PT1000 );         // 4 OUTLANE
  playfield.device( LOWER ).onEvent( OUT_SBANK_PRESS5, playfield, playfield.EVT_READY ); // 5 BALL_EXIT
  playfield.device( LOWER ).onEvent( OUT_SBANK_PRESS6, DUAL_TARGET, IN_TARGET_CLEAR );   // 6 BALL_ENTER 
  
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_INIT, playfield, playfield.EVT_INIT );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_ENABLE, playfield, playfield.EVT_ENABLE );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_COUNTER_RESET, COUNTER, IN_CTR_RESET );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_BALL_ZERO, BALLUP, IN_SCALAR_ZERO );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_PLAYER_ZERO, PLAYERUP, IN_SCALAR_ZERO );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_BALL_ADV, BALLUP, IN_SCALAR_ADVANCE );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_PLAYER_ADV, PLAYERUP, IN_SCALAR_ADVANCE );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_3BONUS, OXO, IN_OXO_TRIPLE );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_COLLECT, OXO, IN_OXO_COLLECT );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_KICKOFF, FEEDER, IN_LBANK_ON );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_PLAYERS_ZERO, PLAYERS, IN_SCALAR_ZERO );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_PLAYERS_ADV, PLAYERS, IN_SCALAR_ADVANCE );
  playfield.device( FRONTBTN ).onEvent( OUT_GAME_OVER, GAME_OVER, IN_LBANK_ON );

  playfield.device( KICKER ).trigger( IN_KICKER_PERSIST );
  
  Serial.printf( "%.2f KBytes available, %.2f KBytes used for devices, %.2f KBytes free\n\n", 
        (float) base_ram / 1024, (float)( base_ram - FreeRam() ) / 1024, (float)( FreeRam() / 1024 ) );

}

void loop() {
  automaton.run(); 
}
