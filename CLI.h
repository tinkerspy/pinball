
char cmd_buffer[80];
Atm_my_command cmd[2]; 

const char runstate_str[3][9] = { "RUNNING ", "SLEEPING", "WAITING " };

enum { CMD_PS, CMD_PF, CMD_LL, CMD_L, CMD_LO, CMD_HD, CMD_STATS, CMD_TS, CMD_TC, CMD_TR, CMD_DC, CMD_DCC, 
        CMD_DDC, CMD_PRESS, CMD_RELEASE, CMD_INIT, CMD_INFO, CMD_REBOOT, CMD_LINK, CMD_DEVICE, CMD_CHAIN };

const char cmdlist[] = "ps pf ll l lo hd stats ts tc tr dc dcc ddc press release init info reboot link device chain";

void cmd_callback( int idx, int v, int up ) {
  switch ( v ) {
    case CMD_CHAIN:
      {
        Atm_device* dev = &playfield.device( cmd[idx].arg( 1 ) );
        dev->chain( cmd[idx].arg( 2 ) );
      }
      return;
    case CMD_DEVICE:
      playfield.device( cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), lib.code( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ), atoi( cmd[idx].arg( 6 ) ), atoi( cmd[idx].arg( 7 ) ), atoi( cmd[idx].arg( 8 ) ) );    
      return;
    case CMD_LINK:
      if ( strcasecmp( cmd[idx].arg( 3 ), "playfield" ) == 0 ) {
        playfield.link( cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), playfield, cmd[idx].arg( 4 ) );
      } else {
        playfield.link( cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), cmd[idx].arg( 3 ), cmd[idx].arg( 4 ) );          
      }
      cmd[idx].stream->printf( "link %s %s %s %s\n", cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), cmd[idx].arg( 3 ), cmd[idx].arg( 4 ) );
      return;
    case CMD_PS:  
      {           
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
                runstate_str[dev->sleep()], 
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
    case CMD_PF:  
      playfield.trigger( playfield.findSymbol( cmd[idx].arg( 1 ), 0 ) ); // PF_DISABLE, PF_ENABLE, PF_READY, PF_INIT
      return;
    case CMD_LL: // List library entries
      {  
        for ( int16_t i = 0; i < lib.count(); i++ ) {
          cmd[idx].stream->printf( "%02d: %s %s\n", i, lib.location( i ) == 'R' ? "RAM  " : "FLASH", lib.label( i ) );
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
    case CMD_L: // Toggle led (or group)
      {  
        int16_t ledno = leds.findSymbol( cmd[idx].arg( 1 ), -1 ) ;
        leds.on( ledno );
        cmd[idx].stream->printf( "Led %d: %s %s\n", ledno, leds.findSymbol( ledno ), leds.active( ledno ) ? "ON" : "OFF" );
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
          cmd[idx].stream->printf( "Trigger: device %d -> %d (%d)\n", sw, e, sel );
          if ( sel ) {
            dev->trigger( e, sel ); 
          } else {
            dev->trigger( e );             
          }
        } else {
          cmd[idx].stream->printf( "Trigger: device %d not found\n", sw );
        }
      }
      return;    
    case CMD_DC:
    case CMD_DCC:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          int16_t e = dev->findSymbol( cmd[idx].arg( 2 ) );
          dev->dumpCode( cmd[idx].stream, e, v == CMD_DCC );
        } else {
          cmd[idx].stream->printf( "Dump code: device %d not found\n", sw );
        }
      }
      return;    
    case CMD_DDC:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        cmd[idx].stream->printf( "// Device code: %s\n", cmd[idx].arg( 1 ) );
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          for ( int16_t i = 0; i < dev->countSymbols( 0 ); i++ ) {
            dev->dumpCode( cmd[idx].stream, i, true );            
          }
          cmd[idx].stream->println();
        } else {
          cmd[idx].stream->printf( "Dump device code: device %d not found\n", sw );
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
          cmd[idx].stream->printf( "Device info for %d: %s [%X:%s] %s\n", 
                sw, playfield.findSymbol( sw, 1 ), dev, runstate_str[dev->sleep()], lib.label( lib.findCode( dev->script ) ) );
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
            cmd[idx].stream->println();        
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 1 ); i++ ) {
            Machine* machine = dev->outputPtr( i );
            if ( machine == &playfield ) {
              cmd[idx].stream->printf( "Out[%02d] %20s  %s::%s\n", i, dev->findSymbol( i, 1 ), "playfield", playfield.findSymbol( dev->outputEvent( i ), 0 ) );              
            } else if ( machine == NULL ) {
              cmd[idx].stream->printf( "Out[%02d] %20s\n", i, dev->findSymbol( i, 1 ) );                            
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
