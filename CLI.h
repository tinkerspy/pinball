
char cmd_buffer[2][256];
Atm_my_command cmd[2]; 
char accumulator[3600]; // shared between serial devices!
char accu_mode = 0;

const char runstate_str[3][9] = { "RUNNING ", "SLEEPING", "WAITING " };

enum { CMD_PS, CMD_PF, CMD_LL, CMD_L, CMD_LO, CMD_HD, CMD_STATS, CMD_TS, CMD_TC, CMD_TR, CMD_DC, CMD_DCC, 
        CMD_DDC, CMD_PRESS, CMD_RELEASE, CMD_INIT, CMD_INFO, CMD_REBOOT, CMD_LINK, CMD_INVERT, CMD_DEVICE, CMD_CHAIN, 
        CMD_PROFILE, CMD_IO, CMD_ECHO, CMD_FC, CMD_LEDS, CMD_SWITCHES, CMD_LEDGROUPS, CMD_SWITCHGROUPS, CMD_DS, CMD_DL };

const char cmdlist[] = "ps pf ll l lo hd stats ts tc tr dc dcc ddc press release init info reboot link invert "
                        "device chain profile io echo fc leds switches ledgroups switchgroups ds dl";

void trim(char * s) {
    char * p = s;
    int l = strlen(p);
    while(!isalnum(p[l - 1])) p[--l] = 0;
    while(* p && !isalnum(* p)) ++p, --l;
    memmove(s, p, l + 1);
}   

void dumpSymbols( Symbolic_Machine* machine, int16_t bank = -1 ) {
  for ( int16_t b = 0; b < 8; b++ ) {
    for ( int16_t i = 0; i < machine->countSymbols( b ); i++ ) {
      if ( bank == -1 || bank == b ) {
        Serial.printf( "%d %d %s -> %d\r\n", b, i, machine->findSymbol( i, b ), machine->findSymbol( machine->findSymbol( i, b ) ) );
      }
    }
  }
}

void cmd_callback( int idx, int v, int up ) {
  if ( accu_mode != 0 ) {
    uint8_t arg_idx = 0;
    while ( strlen( cmd[idx].arg( arg_idx ) ) > 0 ) {
      if ( strchr( cmd[idx].arg( arg_idx ), '"' ) != NULL ) {
        strcat( accumulator, cmd[idx].arg( arg_idx ) );
        trim( accumulator );
        strcat( accumulator, ";" );
        Serial.printf( "FINAL: >>%s<<\r\n", accumulator ); // call whatever function we're in
        switch ( accu_mode ) {
          case CMD_LEDS:
            leds.loadSymbols( accumulator );
            break;
          case CMD_SWITCHES:
            playfield.loadSymbols( accumulator );
            break;
          case CMD_LEDGROUPS:
            leds.loadGroups( accumulator );
            break;
          case CMD_SWITCHGROUPS:
            playfield.loadGroups( accumulator );
            break;
        }
        accu_mode = 0;
        return;        
      }
      strcat( accumulator, cmd[idx].arg( arg_idx ) );
      strcat( accumulator, " " );
      arg_idx++;
    }
  //  Serial.printf( "ACCU: >>%s<<\r\n", accumulator );
    return;
  }
  switch ( v ) {
    //    io.addStrip( new IO_Adafruit_NeoPixel( 53, pin_data, NEO_GRBW + NEO_KHZ800 ) ) // 53 pixel SK6812 led strip on P1/playfield
    case CMD_DS:
      dumpSymbols( &playfield, 1 );
      break;
    case CMD_DL:
      dumpSymbols( &leds, 0 );
      break;
    case CMD_LEDS:
    case CMD_SWITCHES:
    case CMD_LEDGROUPS:
    case CMD_SWITCHGROUPS:
      {
        accumulator[0] = '\0';
        accu_mode = v; 
        uint8_t arg_idx = 0 ;
        while ( strlen( cmd[idx].arg( arg_idx ) ) > 0 ) {
          if ( strchr( cmd[idx].arg( arg_idx ), '"' ) != NULL ) {
            accumulator[0] = '\0';
          }
          strcat( accumulator, cmd[idx].arg( arg_idx ) );
          strcat( accumulator, " " );
//          Serial.printf( "ACCU: >>%s<<\r\n", accumulator );
          arg_idx++;
        }
      }
      break;
    case CMD_IO:
      {
//        io.addStrip( new IO_Adafruit_NeoPixel( atoi( cmd[idx].arg( 2 ), pin_date, atoi( cmd[idx].arg( 3 ) );
        Serial.printf( "IO added interface: %d, %d, %d, %d\r\n", cmd[idx].arg( 1 ), atoi( cmd[idx].arg( 2 ) ), atoi( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ) );        
      }
      break;
    case CMD_INVERT:
      {
        io.invert( playfield.findSymbol( cmd[idx].arg( 1 ) ) );
        Serial.printf( "Inverted switch: %s\r\n", cmd[idx].arg( 1 ) );        
      }
      break;
    case CMD_ECHO:
      {
        if ( strcmp( cmd[idx].arg(1), "" ) != 0 ) cmd[idx].echo( strcasecmp( cmd[idx].arg(1), "ON" ) == 0 ? 1 : atoi( cmd[idx].arg( 1 ) ) );
        cmd[idx].stream->printf( "ECHO is %s\r\n", cmd[idx].echo() ? "ON" : "OFF" );
      }
      break;
    case CMD_FC:
      {
        if ( strcmp( cmd[idx].arg(1), "" ) != 0 ) cmd[idx].fc( strcasecmp( cmd[idx].arg(1), "ON" ) == 0 ? 1 : atoi( cmd[idx].arg( 1 ) ) );
        cmd[idx].stream->printf( "FLOW CONTROL is %s\r\n", cmd[idx].fc() ? "ON" : "OFF" );
      }
      break;
    case CMD_PROFILE:
      {
        if ( leds.findSymbol( cmd[idx].arg( 1 ), -1 ) >= 0 ) {
          cmd[idx].stream->printf( "Led %s profile: %d, %d, %d, %d\r\n", cmd[idx].arg( 1 ), atoi( cmd[idx].arg( 2 ) ), atoi( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ) );
          leds.profile( cmd[idx].arg( 1 ), atoi( cmd[idx].arg( 2 ) ), atoi( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ) );
        } else {          
          cmd[idx].stream->printf( "Switch %s profile: %d, %d, %d, %d\r\n", cmd[idx].arg( 1 ), atoi( cmd[idx].arg( 2 ) ), atoi( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ) );
          playfield.profile( cmd[idx].arg( 1 ), atoi( cmd[idx].arg( 2 ) ), atoi( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ) );
        }
      }
      break;
    case CMD_CHAIN:
      {
        Atm_device* dev = &playfield.device( cmd[idx].arg( 1 ) );
        dev->chain( cmd[idx].arg( 2 ) );
        cmd[idx].stream->printf( "\rChained device %s to %s\r\n", cmd[idx].arg( 1 ), cmd[idx].arg( 2 ) );
      }
      break;
    case CMD_DEVICE:
      playfield.device( cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), lib.code( cmd[idx].arg( 3 ) ), atoi( cmd[idx].arg( 4 ) ), atoi( cmd[idx].arg( 5 ) ), atoi( cmd[idx].arg( 6 ) ), atoi( cmd[idx].arg( 7 ) ), atoi( cmd[idx].arg( 8 ) ) );    
        cmd[idx].stream->printf( "\rCreated device %s::%s (%s)\r\n", cmd[idx].arg( 3 ), cmd[idx].arg( 1 ), cmd[idx].arg( 2 ) );
      break;
    case CMD_LINK:
      if ( strcasecmp( cmd[idx].arg( 3 ), "playfield" ) == 0 ) {
        playfield.link( cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), playfield, cmd[idx].arg( 4 ) );
      } else {
        playfield.link( cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), cmd[idx].arg( 3 ), cmd[idx].arg( 4 ) );          
      }
      cmd[idx].stream->printf( "\rLinked device %s::%s -> %s::%s\r\n", cmd[idx].arg( 1 ), cmd[idx].arg( 2 ), cmd[idx].arg( 3 ), cmd[idx].arg( 4 ) );
      break;
    case CMD_PS:  
      {           
        uint8_t map[32];
        uint8_t cnt = 0;
        memset( map, 0, sizeof( map ) );
        cmd[idx].stream->printf( "## State    Sw#               Device             Bytecode\r\n" ); 
        for ( int16_t n = io.numberOfSwitches() + playfield.numberOfGroups(); n > 0; n-- ) {
          if ( playfield.exists( n + 1 ) == 1 ) {
            Atm_device* dev = &playfield.device( n + 1 );
            uint8_t addr = ( (uint32_t)dev & 0xFFFF ) >> 8;
            if ( ( map[addr >> 3] & ( 1 << ( addr & B111 ) ) ) == 0 ) {
              cmd[idx].stream->printf( "%02d %s %02d%c %20s %20s\r\n", cnt, 
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
      break;
    case CMD_PF:  
      playfield.trigger( playfield.findSymbol( cmd[idx].arg( 1 ), 0 ) ); // PF_DISABLE, PF_ENABLE, PF_READY, PF_INIT
      break;
    case CMD_LL: // List library entries
      {  
        for ( int16_t i = 0; i < lib.count(); i++ ) {
          cmd[idx].stream->printf( "%02d: %s %s\r\n", i, lib.location( i ) == 'R' ? "RAM  " : "FLASH", lib.label( i ) );
        }
        cmd[idx].stream->println(); 
      }
      break;
    case CMD_LO: // List leds that are active
      {  
        for ( int16_t i = 0; i < io.numberOfLeds(); i++ ) {
          if ( leds.active( i ) ) {
            cmd[idx].stream->printf( "Led %d: %s %s\r\n", i, leds.findSymbol( i ), leds.active( i ) ? "ON" : "OFF" );
          } 
        }
        cmd[idx].stream->println(); 
      }
      break;
    case CMD_L: // Toggle led (or group)
      {  
        int16_t ledno = leds.findSymbol( cmd[idx].arg( 1 ), -1 ) ;
        leds.on( ledno );
        cmd[idx].stream->printf( "Led %d: %s %s\r\n", ledno, leds.findSymbol( ledno ), leds.active( ledno ) ? "ON" : "OFF" );
        cmd[idx].stream->println(); 
      }
      break;
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
      break;
    case CMD_STATS:
      cmd[idx].stream->printf( "Runtime: %02d:%02d:%02d (h:m:s)\r\n", millis() / 3600000L, ( millis() / 60000L ) % 60, ( millis() / 1000L ) % 60 );
      cmd[idx].stream->printf( "Physical leds: %d (0..%d)\r\n", io.numberOfLeds(), io.numberOfLeds() - 1 );
      cmd[idx].stream->printf( "Logical leds: %d (%d..%d)\r\n", leds.numberOfGroups(), io.numberOfLeds(), io.numberOfLeds() + leds.numberOfGroups() - 1 );
      cmd[idx].stream->printf( "Physical switches: %d (1..%d)\r\n", io.numberOfSwitches(), io.numberOfSwitches() );
      cmd[idx].stream->printf( "Logical switches: %d (%d..%d)\r\n", playfield.numberOfGroups(), io.numberOfSwitches() + 1, io.numberOfSwitches() + playfield.numberOfGroups() );
      cmd[idx].stream->printf( "Devices: ??\r\n" );
      cmd[idx].stream->printf( "Firmware: %d\r\n", lib.count() );
      cmd[idx].stream->printf( "Free RAM: %d\r\n", FreeRam() );
      cmd[idx].stream->println();
      break;
    case CMD_TS:
      playfield.traceSwitches( *cmd[idx].stream, atoi( cmd[idx].arg( 1 ) ) );
      cmd[idx].stream->printf( "Trace switches: %d\r\n", atoi( cmd[idx].arg( 1 ) ) );
      break;    
    case CMD_TC:
      { 
        if ( strlen( cmd[idx].arg( 1 ) ) == 0 ) {
          for ( int16_t n = io.numberOfSwitches() + playfield.numberOfGroups(); n > 0; n-- ) {
            if ( playfield.exists( n + 1 ) == 1 ) {
              playfield.device( n + 1 ).traceCode( *cmd[idx].stream, 0 );
            }
          }          
          cmd[idx].stream->println( "Tracing interrupted\r\n" );
          break;
        }
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t mode = atoi( cmd[idx].arg( 2 ) );
        if ( strlen( cmd[idx].arg( 2 ) ) == 0 ) mode = 1;
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).traceCode( *cmd[idx].stream, mode ); 
          cmd[idx].stream->printf( "Trace code: device %d -> %d\r\n", sw, mode );
        } else {
          cmd[idx].stream->printf( "Trace code: device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_TR:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t sel = atoi( cmd[idx].arg( 3 ) );
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          int16_t e = dev->findSymbol( cmd[idx].arg( 2 ) );
          cmd[idx].stream->printf( "Trigger: device %d -> %d (%d)\r\n", sw, e, sel );
          if ( sel ) {
            dev->trigger( e, sel ); 
          } else {
            dev->trigger( e );             
          }
        } else {
          cmd[idx].stream->printf( "Trigger: device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_DC:
    case CMD_DCC:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          int16_t e = dev->findSymbol( cmd[idx].arg( 2 ) );
          dev->dumpCode( cmd[idx].stream, e, v == CMD_DCC );
        } else {
          cmd[idx].stream->printf( "Dump code: device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_DDC:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        cmd[idx].stream->printf( "// Device code: %s\r\n", cmd[idx].arg( 1 ) );
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          for ( int16_t i = 0; i < dev->countSymbols( 0 ); i++ ) {
            dev->dumpCode( cmd[idx].stream, i, true );            
          }
          cmd[idx].stream->println();
        } else {
          cmd[idx].stream->printf( "Dump device code: device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_PRESS:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t n = atoi( cmd[idx].arg( 2 ) );
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).trigger( ( n * 2 ) + 1, 1 ); 
          cmd[idx].stream->printf( "Press device %d, switch %d -> event %d\r\n", sw, n, ( n * 2 ) + 1 );
        } else {
          cmd[idx].stream->printf( "Press device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_RELEASE:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t n = atoi( cmd[idx].arg( 2 ) );
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).trigger( ( n * 2 ) + 2, 1 ); 
          cmd[idx].stream->printf( "Release device %d -> switch %d, event %d\r\n", sw, n, ( n * 2 ) + 2 );
        } else {
          cmd[idx].stream->printf( "Release device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_INIT:
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );
        int16_t n = atoi( cmd[idx].arg( 2 ) );
        if ( playfield.exists( sw ) ) {
          playfield.device( sw ).trigger( ( n * 2 ), 1 ); 
          cmd[idx].stream->printf( "Init device %d -> %d\r\n", sw, n );
        } else {
          cmd[idx].stream->printf( "Init device %d not found\r\n", sw );
        }
      }
      break;    
    case CMD_REBOOT:
      _reboot_Teensyduino_();
      break;
    case CMD_INFO: // TODO show firmware label & running/sleeping state & device next in chain...
      {
        int16_t sw = playfield.findSymbol( cmd[idx].arg( 1 ) );        
        if ( playfield.exists( sw ) ) {
          Atm_device* dev = &( playfield.device( sw ) );
          cmd[idx].stream->printf( "Device info for %d: %s [%X:%s] %s\r\n", 
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
              cmd[idx].stream->printf( "Out[%02d] %20s  %s::%s\r\n", i, dev->findSymbol( i, 1 ), "playfield", playfield.findSymbol( dev->outputEvent( i ), 0 ) );              
            } else if ( machine == NULL ) {
              cmd[idx].stream->printf( "Out[%02d] %20s\r\n", i, dev->findSymbol( i, 1 ) );                            
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
              cmd[idx].stream->printf( "Arg[%02d] %20s\r\n", i, dev->findSymbol( i, 2 ) );
            } else {
              cmd[idx].stream->printf( "Arg[%02d] %20s  %s %s\r\n", i, dev->findSymbol( i, 2 ), leds.findSymbol( leds.index( dev->ledGroup(), i ) ), leds.active( leds.index( dev->ledGroup(), i ) ) ? "ON" : "OFF" );              
            }
          }
          for ( uint16_t i = 0; i < dev->countSymbols( 3 ); i++ ) {
            cmd[idx].stream->printf( "Reg[%02d] %20s  %d\r\n", i, dev->findSymbol( i, 3 ), dev->reg( i ) );
          }
        } else {
          cmd[idx].stream->printf( "Init device %d not found\r\n", sw );
        }
      }
      cmd[idx].stream->println();
      break;    
  }
} 
