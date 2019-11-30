#pragma once

#include <Automaton.h>

class Atm_my_command : public Machine {
 public:
  enum { IDLE, READCHAR, SEND };
  enum { EVT_EOL, EVT_INPUT, ELSE };

  Atm_my_command( void ) : Machine(){};
  Atm_my_command& begin( Stream& stream, char buffer[], int size );
  Atm_my_command& trace( Stream& stream );
  Atm_my_command& onCommand( atm_cb_push_t callback, int idx = 0 );
  Atm_my_command& list( const char* cmds );
  Atm_my_command& echo( int8_t v );
  bool echo( void );
  Atm_my_command& fc( int8_t v );
  bool fc( void );

  Atm_my_command& separator( const char sep[] );
  int lookup( int id, const char* cmdlist );
  char* arg( int id );
  Stream* stream;

 private:
  enum { ENT_READCHAR, ENT_SEND };
  atm_connector oncommand;
  char* buffer;
  int bufsize, bufptr;
  char eol, lastch;
  const char* separatorChar;
  const char* commands;
  bool remote_echo = 0;
  bool flow_control = 0;

  int event( int id );
  void action( int id );
};
