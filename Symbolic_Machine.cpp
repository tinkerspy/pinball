#include "Symbolic_Machine.hpp"


int16_t* Symbolic_Machine::compile( const char src[], int16_t dict_size ) {
  char buf[2048];
  char *token;
  Serial.println( src ); delay( 1000 );
  const char* code_start = src;
  int16_t wordcnt = dict_size;
  while ( strlen( src ) > 0 ) {
    char* ptr = strchr( src, ';' );
    strncpy( buf, src, ptr - src );
    buf[ptr - src] = '\0';
    Serial.printf( "Section %s\n", buf );
    src += ptr - src + 1;
    int16_t rescnt = 0;
    token = strtok( buf, sep );
    while( token != NULL ) {
      Serial.printf( "PASS 1: %s\n", token );
      token = strtok(NULL, sep );
      rescnt++;
    }
    if ( rescnt == 1 ) { // A label
      wordcnt += 2; 
    } else { 
      wordcnt += rescnt; 
    }
  }
  wordcnt += 2;
  Serial.printf( "%d words\n", wordcnt );

  int16_t* pcode = (int16_t *) malloc( wordcnt * 2 );
  memset( pcode, 0, wordcnt * 2 );
  
  int16_t* data_ptr = pcode;

  src = code_start;
  int16_t* porigin = pcode; 
  pcode += dict_size;
  while ( strlen( src ) > 0 ) {
    char* ptr = strchr( src, ';' );
    strncpy( buf, src, ptr - src );
    buf[ptr - src] = '\0';
    src += ptr - src + 1;
    int16_t rescnt = 0;
    int16_t res[4];
    int16_t opcode = 0;
    token = strtok( buf, sep );
    while( token != NULL ) {
      if ( rescnt == 0 && strlen( token ) == 1 ) {
        opcode = token[0]; 
      }
      res[rescnt] = findSymbol( token, -1 ); 
      if ( strlen( token ) > 1 && res[rescnt] == -1 ) Serial.printf( "Compile error: token '%s' not found\n", token );
      Serial.printf( "%s = %d\n", token, res[rescnt] ); 
      token = strtok(NULL, sep );
      rescnt++;
    }
    if ( rescnt == 1 ) { // A label
      *pcode++ = -1;
      *pcode++ = res[0];
      porigin[res[0]] = ( pcode - porigin );
      Serial.printf( "-1\n%d -> offset %d\n", res[0], (pcode - porigin) );
    } else { // A code line
      for ( int16_t i = 0; i < rescnt; i++ ) {
      *pcode++ = res[i];
      }
      Serial.printf( "%c, %d, %d, %d\n", res[0], res[1], res[2], res[3] );
    }
  }
  *pcode++ = -1;
  *pcode++ = -1;
  return data_ptr;
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
  char sep[] = " ,";
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

int16_t Symbolic_Machine::findSymbol( const char s[], int16_t def /* = 0 */ ) {
  symbolic_machine_table* p = symbols;
  if ( strlen( s ) == 0 ) return 0;
  if ( isdigit( s[0] ) || ( s[0] == '-' && isdigit( s[1] ) ) ) return atoi( s );   
  while ( p != NULL ) {
    int16_t i = findSymbolString( s, p->s );
    if ( i >= 0 ) return i;
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table ) + p->offset ) : p->next;
  }
  if ( def == 0 ) Serial.printf( "ERROR: Symbolic::findSymbol( %s ) failed\n", s );
  return def;
}

// Returns the n-th symbol in a bank or a NULL pointer if it does not exist

const char* Symbolic_Machine::findSymbol( int16_t idx, int8_t bank /* = 0 */ ) {
  symbolic_machine_table* p = symbols;
  uint8_t pcnt = 0;
  if ( idx < 0 ) {
    return null_str;
  }  
  while ( p != NULL && pcnt < bank ) {
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table ) + p->offset ) : p->next;
    pcnt++;
  }
  if ( p == NULL ) return NULL;
  uint8_t scnt = 0;
  char* s = p->s;
  while ( *s != '\0' && scnt < idx ) {
    s += strlen( s ) + 1;
    scnt++;
  }
  return *s == '\0' ? NULL : s;
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
