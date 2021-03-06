#include "Symbolic_Machine.hpp"

void Symbolic_Machine::clearComments( char s[] ) {
  char* p = s;
  bool comment = 0;
  while ( *p != '\0' ) {
     if ( *p == '/' ) comment = 1;
     if ( *p == '\n' ) comment = 0;
     if ( comment ) *p = ' ';
     p++; 
  }
}

int16_t Symbolic_Machine::loadIntList( const symbolic_machine_table* symbols, const char src[], int16_t dst[], int16_t dict_size, int16_t dict_offset /* = 0 */, bool sparse_jumptable /* = false */ ) {
  char buf[2048];
  char *token;
  const char separator[] = ", \n\t\r";
  int16_t* pcode = dst + dict_size;
  if ( dst != NULL ) {
    for ( int16_t i = 0; i < dict_size; i++ ) {
      dst[i] = sparse_jumptable ? 0 : dict_size;
    }
  }
  while ( strlen( src ) > 0 ) {
    char* end_of_record = strchr( src, ';' );
    if ( end_of_record == NULL ) break;
    strncpy( buf, src, end_of_record - src );
    buf[end_of_record - src] = '\0';
/*
    Serial.print( ">>" );
    Serial.print( buf );
    Serial.println( "<<" );
    */
    clearComments( buf );
    /*
    Serial.print( ">>" );
    Serial.print( buf );
    Serial.println( "<<" );
    if ( sparse_jumptable ) delay( 100000000 ); // FIXME!!!
    */
    src += end_of_record - src + 1;
    token = strtok( buf, separator );
    int16_t i = findSymbol( symbols, token, -32768 ); 
    if ( strlen( token ) > 0 && i == -32768 ) Serial.printf( "Compile error: token '%s' not found\r\n", token );
    if ( dst == NULL ) {
      pcode++;
      pcode++;
    } else {  
      *pcode++ = -1; 
      *pcode++ = i;
      dst[i - dict_offset] = ( pcode - dst );
    }
    token = strtok(NULL, separator );
    while( token != NULL ) {
      int16_t i = findSymbol( symbols, token, -32768 ); 
      if ( strlen( token ) > 0 && i == -32768 ) Serial.printf( "Compile error: token '%s' not found\r\n", token );
      if ( dst == NULL ) {
        pcode++;
      } else {
        *pcode++ = i;        
      }
      token = strtok(NULL, separator );
    }
  }
  if ( dst == NULL ) {
    pcode += 2;
  } else {
    *pcode++ = -1; 
    *pcode++ = -1;     
  }
  return pcode - dst;
}


int16_t* Symbolic_Machine::compileList( const char src[], int16_t dict_size, int16_t dict_offset /* = 0 */ ) {
  int16_t data_size = loadIntList( symbols, src, NULL, dict_size, dict_offset );
  int16_t* pdata = (int16_t *) malloc( data_size * 2 );
  memset( pdata, 0, data_size * 2 );
  loadIntList( symbols, src, pdata, dict_size, dict_offset );
  /*
  for ( int16_t i = 0; i < data_size; i++ ) {
    Serial.printf( "%d: %d\n", i, pdata[i] );
  }
  */
  return pdata;
}


// Parses a string of \n separated symbol banks

Symbolic_Machine& Symbolic_Machine::loadSymbols( const char s[] ) {
  while ( *s != '\0' ) {
    s = loadSymbolString( s );
  }
  return *this; 
}

Symbolic_Machine& Symbolic_Machine::linkSymbols( symbolic_machine_table* sym ) {
  symbols = sym;    
  return *this; 
}

// Parses a string and stores the symbols in a bank added to the end of a linked list

// TODO: kan zonder buffer in two-pass: 1. bereken malloc lengte 2. kopieer karakters een voor een

const char* Symbolic_Machine::loadSymbolString( const char* s ) { 
  char buf[2048];
  char sep[] = " ,;";
  char* b = buf;
  int16_t char_cnt = 0;
  while ( *s != '\0' && *s != '\n' ) {
    while ( *s != '\0' && *s != '\n' && strchr( sep, *s ) != NULL ) {
      s++;      
    }
    while ( *s != '\0' && *s != '\n' && strchr( sep, *s ) == NULL ) {
      char_cnt++;
      *b++ = tolower( *s );
      s++;      
    }
    while ( *s != '\0' && *s != '\n' && strchr( sep, *s ) != NULL ) {
      s++;      
    }
    *b++ = '\0';
  }
  *b = '\0'; 
  s++;
  // Add new tables to the end of the linked list
  if ( char_cnt > 0 ) {
    symbolic_machine_table** p = &symbols;
    while ( *p != NULL ) p = &(*p)->next;
    *p = (symbolic_machine_table*) malloc( sizeof( symbolic_machine_table ) + ( b - buf ) + 1 );
    memcpy( (*p)->s, buf, ( b - buf ) + 1 );
    (*p)->next = NULL;
    (*p)->offset = 0;
  }
  return s;
}

// Returns the index of a symbol string in any of the banks
// If the argument string looks like a number return that number
// If the argument string is a single character return its ascii value

int16_t Symbolic_Machine::findSymbol( const char s[], int16_t def /* = 0 */ ) {
  symbolic_machine_table* p = symbols;
  if ( strlen( s ) == 0 ) return 0;
  if ( isdigit( s[0] ) || ( s[0] == '-' && isdigit( s[1] ) ) ) return atoi( s );   
  if ( strlen( s ) == 1 ) return s[0];
  while ( p != NULL ) {
    int16_t i = findSymbolString( s, p->s );
    if ( i >= 0 ) return i;
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table ) + p->offset ) : p->next;
  }
  if ( def == 0 ) Serial.printf( "ERROR: Symbolic::findSymbol( %s ) failed\r\n", s );
  return def;
}

int16_t Symbolic_Machine::findSymbol( const symbolic_machine_table* symbols, const char s[], int16_t def /* = 0 */ ) {
  const symbolic_machine_table* p = symbols;
  if ( strlen( s ) == 0 ) return 0;
  if ( isdigit( s[0] ) || ( s[0] == '-' && isdigit( s[1] ) ) ) return atoi( s );   
  if ( strlen( s ) == 1 ) return s[0];
  while ( p != NULL ) {
    int16_t i = findSymbolString( s, p->s );
    if ( i >= 0 ) return i;
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table ) + p->offset ) : p->next;
  }
  if ( def == 0 ) Serial.printf( "ERROR: Symbolic::findSymbol( %s ) failed\r\n", s );
  return def;
}

// Returns the n-th symbol in a bank or a NULL pointer if it does not exist

const char* Symbolic_Machine::findSymbol( int16_t idx, int8_t bank /* = 0 */, const char* def /* = NULL */ ) {
  symbolic_machine_table* p = symbols;
  uint8_t pcnt = 0;
  if ( idx < 0 ) {
    return def == NULL ? null_str : def;
  }  
  while ( p != NULL && pcnt < bank ) {
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table ) + p->offset ) : p->next;
    pcnt++;
  }
  if ( p == NULL ) return def;
  uint8_t scnt = 0;
  char* s = p->s;
  while ( *s != '\0' && scnt < idx ) {
    s += strlen( s ) + 1;
    scnt++;
  }
  return *s == '\0' ? def : s;
}

// Returns number of symbols in a bank

int16_t Symbolic_Machine::countSymbols( int8_t bank /* = 0 */ ) {
  symbolic_machine_table* p = symbols;
  uint8_t pcnt = 0;
  while ( p != NULL && pcnt < bank ) {
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table ) + p->offset ) : p->next;
    pcnt++;
  }
  if ( p == NULL ) return 0;
  uint8_t scnt = 0;
  char* s = p->s;
  while ( *s != '\0' ) {
    s += strlen( s ) + 1;
    scnt++;
  }
  return scnt;
}

// Find a string in a char array of null terminated strings (terminated by an empty string -- double \0 )
// Returns string index or -1 if not found

int16_t Symbolic_Machine::findSymbolString( const char s[], const char sym[] ) {
  const char* p = sym;
  int16_t cnt = 0;
  while ( *p != '\0' && strcasecmp( s, p ) != 0 ) {
    p += strlen( p ) + 1;    
    cnt++;
  }
  return *p == '\0' ? -1 : cnt;
}
