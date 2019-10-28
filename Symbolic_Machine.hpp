#pragma once

#include <Automaton.h>

// Jump to the next record in the list? (mix of relative/absolute addressing)
// Offset should contain the size of the symbol data
// Careful: next must be NULL for the last bank and !NULL for the others

struct symbolic_machine_table {
  symbolic_machine_table* next;
  uint32_t offset;
  char s[];
};

class Symbolic_Machine: public Machine {

 public:
  Symbolic_Machine( void ) : Machine() { symbols = NULL; };
  Symbolic_Machine& loadSymbols( const char s[] );
  Symbolic_Machine& linkSymbols( symbolic_machine_table* sym );
  const char* loadString( const char* s );
  int16_t findString( const char s[], const char sym[] );
  int16_t findSymbol( const char s[] );
  const char* findSymbol( int16_t idx, int8_t bank = 0 );
  int16_t countSymbols( int8_t bank = 0 );
 
 protected:
  symbolic_machine_table* symbols;
  const char null_str[2] = "";

};
