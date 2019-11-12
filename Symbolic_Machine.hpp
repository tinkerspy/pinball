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

union symbol_code_pack {
    struct {
        symbolic_machine_table* symbols;
        int16_t* code;
    };
    uint64_t pack;
};

class Symbolic_Machine: public Machine {

 public:
  Symbolic_Machine( void ) : Machine() { symbols = NULL; };
  Symbolic_Machine& loadSymbols( const char s[] );
  Symbolic_Machine& linkSymbols( symbolic_machine_table* sym );
  const char* loadSymbolString( const char* s );
  int16_t findSymbolString( const char s[], const char sym[] );
  int16_t findSymbol( const char s[], int16_t def = 0 );
  int16_t findSymbol( const symbolic_machine_table* symbols, const char s[], int16_t def = 0  );
  const char* findSymbol( int16_t idx, int8_t bank = 0, const char* def = NULL );
  int16_t countSymbols( int8_t bank = 0 );
  int16_t* compileList( const char src[], int16_t dict_size, int16_t dict_offset = 0  );
  int16_t loadIntList( const symbolic_machine_table* symbols, const char src[], int16_t dst[], int16_t dict_size, int16_t dict_offset = 0, bool sparse_jumptable = false );

 protected:
  void clearComments( char s[] );
  symbolic_machine_table* symbols;
  const char null_str[2] = "";
  const char sep[3] = ", ";

};
