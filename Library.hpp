#pragma once

#include "Symbolic_Machine.hpp"

#define MAX_LIBRARY_ENTRIES 16
#define MAX_LABEL_SIZE 24
 
struct library_record {
  char label[MAX_LABEL_SIZE + 1];
  symbolic_machine_table* symbols;
  int16_t* code;    
};

union lib_code_pack {
    struct {
        symbolic_machine_table* symbols;
        int16_t* code;
    };
    uint64_t pack;
};

class Library {

  public:
  enum{ SYM_INPUT, SYM_OUTPUT, SYM_ARG, SYM_REG };
  int16_t compile( const char label[], const char src[] );
  const char* cpunfold( char buf[], const char src[] );
  int16_t findString( const char s[], const char sym[] );
  int16_t findSymbol( int16_t slot, const char s[] );
  const char* findSymbol( int16_t slot, int16_t idx, int8_t bank = 0 );
  int16_t findSymbol( const char label[], const char s[] );
  int16_t countSymbols( int16_t slot, int8_t bank = 0 );
  int16_t* codePtr( int16_t idx );
  symbolic_machine_table* symbolPtr( int16_t idx );
  int16_t* codePtr( const char label[] );
  symbolic_machine_table* symbolPtr( const char label[] );
  int16_t index( const char label[] );
  Library& hexdump( Stream* stream, int16_t idx );
  Library& hexdump( Stream* stream, const char label[] );
  int16_t import( const char label[], const char symbols[], const uint16_t code[] );
  int16_t count();
  char* label( int16_t slot );
  uint64_t code( int16_t idx );
  int64_t code( const char label[] );
  int16_t findCode( const int16_t* c );


  protected:
  Library& printHexWord( Stream* stream, int16_t v, bool last = false );
  library_record lib[MAX_LIBRARY_ENTRIES];
  int16_t lib_cnt = 0;
  const char sep[3] = ", ";
  char null_str[2] = "";
  int16_t word_cnt = 0;
  uint8_t find_error = 0;
  
};
