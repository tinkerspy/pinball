#include "Atm_my_command.hpp"

/*
 * Modifications:
 * - Added ; as a command separator (TODO: make optional, configurable, does it work???)
 * - Made stream pointer public
 * - Changed the EVT_INPUT/EVT_EOL order (important!!!)
 * - Added remote echo (TODO: make optional)
 */

Atm_my_command& Atm_my_command::begin( Stream& stream, char buffer[], int size ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER    ON_LOOP    ON_EXIT  EVT_INPUT   EVT_EOL   ELSE */
    /* IDLE     */            -1,        -1,        -1,        -1, READCHAR,    -1,
    /* READCHAR */  ENT_READCHAR,        -1,        -1,      SEND, READCHAR,    -1,
    /* SEND     */      ENT_SEND,        -1,        -1,        -1,       -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->stream = &stream;
  this->buffer = buffer;
  bufsize = size;
  bufptr = 0;
  separatorChar = " ";
  lastch = '\0';
  remote_echo = 0;
  flow_control = 0;
  return *this;
}

int Atm_my_command::event( int id ) {
  switch ( id ) {
    case EVT_INPUT:
      return stream->available();
    case EVT_EOL:
      return buffer[bufptr - 1] == '\n' || buffer[bufptr - 1] == '\r' || bufptr >= bufsize; //  || buffer[bufptr - 1] == ';'
  }
  return 0;
}

void Atm_my_command::action( int id ) {
  switch ( id ) {
    case ENT_READCHAR:
      if ( stream->available() ) {
        char ch = stream->read();
        if ( remote_echo ) stream->printf( "%c", ch );
        if ( strchr( separatorChar, ch ) == NULL ) {
          buffer[bufptr++] = ch;
          lastch = ch;
        } else {
          if ( lastch != '\0' ) buffer[bufptr++] = '\0';
          lastch = '\0';
          return;
        }
      }
      return;
    case ENT_SEND:
      buffer[--bufptr] = '\0';
      if ( remote_echo ) stream->printf( "\r\n" );
      if ( flow_control ) {
        Serial.printf( "%c", 19 );
        Serial.flush();
      }
      oncommand.push( lookup( 0, commands ) );
      if ( flow_control ) {
        Serial.printf( "%c", 17 );
        Serial.flush();
      }
      lastch = '\0';
      bufptr = 0;
      return;
  }
}

Atm_my_command& Atm_my_command::onCommand( atm_cb_push_t callback, int idx /* = 0 */ ) {
  oncommand.set( callback, idx );
  return *this;
}

Atm_my_command& Atm_my_command::list( const char* cmds ) {
  commands = cmds;
  return *this;
}

Atm_my_command& Atm_my_command::echo( int8_t v ) {
  if ( v > -1 ) remote_echo = v;
  return *this;
}

Atm_my_command& Atm_my_command::fc( int8_t v ) {
  if ( v > -1 ) flow_control = v;
  return *this;
}

bool Atm_my_command::echo( void ) {
  return remote_echo > 0;
}

bool Atm_my_command::fc( void ) {
  return flow_control > 0;
}

Atm_my_command& Atm_my_command::separator( const char sep[] ) {
  separatorChar = sep;
  return *this;
}

char* Atm_my_command::arg( int id ) {
  int cnt = 0;
  int i;
  if ( id == 0 ) return buffer;
  for ( i = 0; i < bufptr; i++ ) {
    if ( buffer[i] == '\0' ) {
      if ( ++cnt == id ) {
        i++;
        break;
      }
    }
  }
  return &buffer[i];
}

int Atm_my_command::lookup( int id, const char* cmdlist ) {
  int cnt = 0;
  char* arg = this->arg( id );
  char* a = arg;
  while ( cmdlist[0] != '\0' ) {
    while ( cmdlist[0] != '\0' && toupper( cmdlist[0] ) == toupper( a[0] ) ) {
      cmdlist++;
      a++;
    }
    if ( a[0] == '\0' && ( cmdlist[0] == ' ' || cmdlist[0] == '\0' ) ) return cnt;
    while ( cmdlist[0] != ' ' && cmdlist[0] != '\0' ) cmdlist++;
    cmdlist++;
    a = arg;
    cnt++;
  }
  return -1;
}

Atm_my_command& Atm_my_command::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace, "COMMAND\0EVT_EOL\0EVT_INPUT\0ELSE\0IDLE\0READCHAR\0SEND" );
  return *this;
}
