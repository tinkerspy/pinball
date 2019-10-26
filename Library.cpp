#include "Library.hpp"

/*
library->add( "bumper_bytecode", bumper_bytecode );
dev.loadSymbols( library->symbols( "bumper_bytecode" ) );
dev.loadCode( library->code( "bumper_bytecode" ) );
*/


const char* Library::cpunfold( char buf[], const char src[] ) {
  const char* ps = src;
  char* pb = buf;
  int escape = 0;
  while ( *ps != '\n' || escape ) {
    if ( *ps == '\\' ) {
      escape++;
    } else {
      if ( escape ) {
        if ( *ps == '\n' ) escape = 0;
      } else {
        *pb++ = *ps;
      }
    }
    ps++;
  }
  *pb='\0';
  return ps;  
}

int16_t Library::compile( const char label[], const char src[] ) {
  char buf[2048];
  char *token;
  strcpy( lib[lib_cnt].label, label );
  lib[lib_cnt].label[strlen(label)] = '\0';
  lib[lib_cnt].symbols = NULL;
  do {
    const char * newsrc = cpunfold( buf, src );
    if ( strlen( buf ) == 0 ) src = newsrc + 1; 
  } while ( strlen( buf ) == 0 ); 
  
  for ( int i = 0; i < 4; i++ ) {
    uint16_t len = 0;
    token = strtok( buf, sep );
    while( token != NULL ) {
      len += strlen( token ) + 1;
      token = strtok(NULL, sep );
    }
    src = cpunfold( buf, src ) + 1;
    
    symbolic_machine_table* sym = (symbolic_machine_table*) malloc( 4 + len + 1 );
    sym->next = NULL;
    char* p = sym->s;
    token = strtok( buf, sep );
    while( token != NULL ) {
      strcpy( p, token );
      p += strlen( token ) + 1;   
      token = strtok(NULL, sep );
    }
    *p = '\0';  
    // TOD: store in natural order (add to the end of the list!)
    sym->next = lib[lib_cnt].symbols;
    lib[lib_cnt].symbols = sym;
    cpunfold( buf, src );
  }

  const char* code_start = src;
  int16_t wordcnt = countSymbols( lib_cnt, SYM_INPUT );
  while ( strlen( src ) > 0 ) {
    char* ptr = strchr( src, '\n' );
    strncpy( buf, src, ptr - src );
    buf[ptr - src] = '\0';
    src += ptr - src + 1;
    int16_t rescnt = 0;
    token = strtok( buf, sep );
    while( token != NULL ) {
      token = strtok(NULL, sep );
      rescnt++;
    }
    if ( rescnt == 1 ) { // A label
      wordcnt += 2; 
    } else if ( rescnt == 4 ) { // A code line
      wordcnt += 4; 
    }
  }
  wordcnt += 2;
  int16_t* pcode = (int16_t *) malloc( wordcnt * 2 );
  memset( pcode, 0, wordcnt * sizeof( pcode[0] ) );
  
  lib[lib_cnt].code = pcode;

  src = code_start;
  int16_t* porigin = pcode;
  pcode += countSymbols( lib_cnt, SYM_INPUT );
  while ( strlen( src ) > 0 ) {
    char* ptr = strchr( src, '\n' );
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
      res[rescnt] = findSymbol( lib_cnt, token );
      //Serial.printf( "%s = %d\n", token, res[rescnt] ); 
      token = strtok(NULL, sep );
      rescnt++;
    }
    if ( rescnt == 1 ) { // A label
      *pcode++ = -1;
      *pcode++ = res[0];
      porigin[res[0]] = ( pcode - porigin );
      //Serial.printf( "-1\n%d -> offset %d\n", res[0], (pcode - porigin) );
    } else if ( rescnt == 4 ) { // A code line
      *pcode++ = opcode;
      *pcode++ = res[1];
      *pcode++ = res[2];
      *pcode++ = res[3];
      //Serial.printf( "%c, %d, %d, %d\n", opcode, res[1], res[2], res[3] );
    }
  }
  *pcode++ = -1;
  *pcode++ = -1;
  return lib_cnt++;
}

int16_t* Library::codePtr( int16_t idx ) {
  return lib[idx].code;
}

symbolic_machine_table* Library::symbolPtr( int16_t idx ) {
  return lib[idx].symbols;
}

int16_t Library::index( const char label[] ) {
  for ( int16_t i = 0; i < lib_cnt; i++ ) {
    if ( strcasecmp( label, lib[i].label ) == 0 ) {
      return i;
    }
  }
  return -1;
}

// Returns the index of a symbol string in any of the banks
// If the argument string looks like a number return that number

int16_t Library::findSymbol( int16_t slot, const char s[] ) {
  symbolic_machine_table* p = lib[slot].symbols;
  if ( strlen( s ) == 0 ) return 0;
  if ( isdigit( s[0] ) || ( s[0] == '-' && isdigit( s[1] ) ) ) return atoi( s );   
  while ( p != NULL ) {
    int16_t i = findString( s, p->s );
    if ( i >= 0 ) return i;
    p = p->next;
  }
  return 0;
}

// Returns the n-th symbol in a bank or a NULL pointer if it does not exist

char* Library::findSymbol( int16_t slot, int16_t idx, int8_t bank /* = 0 */ ) {
  symbolic_machine_table* p = lib[slot].symbols;
  uint8_t pcnt = 0;
  if ( idx < 0 ) {
    return null_str;
  }  
  while ( p != NULL && pcnt < bank ) {
    p = p->next;
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

int16_t Library::countSymbols( int16_t slot, int8_t bank /* = 0 */ ) {
  symbolic_machine_table* p = lib[slot].symbols;
  uint8_t pcnt = 0;
  while ( p != NULL && pcnt < bank ) {
    p = p->next;
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

int16_t Library::findString( const char s[], const char sym[] ) {
  const char* p = sym;
  int16_t cnt = 0;
  while ( *p != '\0' && strcasecmp( s, p ) != 0 ) {
    p += strlen( p ) + 1;    
    cnt++;
  }
  return *p == '\0' ? -1 : cnt;
}

/*

const char bumper_bytecode[] = R""""(
init, press, release, turn_on, turn_off
out_score, out_score_lit, out_score_unlit, out_light_on, out_light_off
arg_coil, arg_led

init
P, -1, -1, 0                   
L, -1, -1, arg_coil     
L, -1, -1, arg_led      

press
H, -1, -1, arg_coil     
T, -1, -1, out_score
J, arg_led, 0, 2     
T, -1, -1, out_score_lit
J, -1, -1, -1
T, -1, -1, out_score_unlit

turn_on
H, -1, -1, arg_led
T, -1, -1, out_light_on

turn_off
L, -1, -1, arg_led
T, -1, -1, out_light_off
 
)"""";



 */
