#pragma once

#include "Symbolic_Machine.hpp"

#define MAX_LIBRARY_ENTRIES 16
#define MAX_LABEL_SIZE 24
 
struct library_record {
  char label[MAX_LABEL_SIZE + 1];
  symbolic_machine_table* symbols;
  int16_t* code;    
};

class Library {

  public:
  int16_t compile( const char label[], const char src[] );
  const char* cpunfold( char buf[], const char src[] );
  int16_t findString( const char s[], const char sym[] );
  int16_t findSymbol( int16_t slot, const char s[] );
  char* findSymbol( int16_t slot, int16_t idx, int8_t bank = 0 );
  int16_t countSymbols( int16_t slot, int8_t bank = 0 );
  int16_t* codePtr( int16_t idx );
  symbolic_machine_table* symbolPtr( int16_t idx );
  int16_t index( const char label[] );

  protected:
  enum{ SYM_REG, SYM_ARG, SYM_OUTPUT, SYM_INPUT };
  library_record lib[MAX_LIBRARY_ENTRIES];
  int16_t lib_cnt = 0;
  const char sep[3] = ", ";
  char null_str[2] = "";

};
