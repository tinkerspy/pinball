#pragma once

#include <Automaton.h>

struct symbolic_machine_table {
  symbolic_machine_table* next;
  char s[];
};

class Symbolic_Machine: public Machine {

 public:
  Symbolic_Machine( void ) : Machine() { symbols = NULL; };
  Symbolic_Machine& loadSymbols( char s[] );
  char* loadString( char* s );
  int16_t findString( const char s[], const char sym[] );
  int16_t findSymbol( const char s[] );
  char* findSymbol( int16_t idx, int8_t bank = 0 );
  int16_t countSymbols( int8_t bank = 0 );
 
 protected:
  symbolic_machine_table* symbols;
  char null_str[2] = "";

};
