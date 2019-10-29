#include "Library.hpp"

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
    symbolic_machine_table* sym = (symbolic_machine_table*) malloc( sizeof( symbolic_machine_table ) + len + 1 );
    sym->next = NULL;
    sym->offset = 0;
    char* p = sym->s;
    token = strtok( buf, sep );
    while( token != NULL ) {
      strcpy( p, token );
      p += strlen( token ) + 1;   
      token = strtok(NULL, sep );
    }
    *p = '\0';  
    // TODO: Use relative offset instead of absolute pointers to make symbol tables relocatable & storable in flash
    symbolic_machine_table** psym = &(lib[lib_cnt].symbols);
    while ( *psym != NULL ) psym = &(*psym)->next;
    *psym = sym;
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
  memset( pcode, 0, wordcnt * 2 );
  
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
      if ( strlen( token ) > 1 && find_error ) Serial.printf( "Compile error: token '%s' not found\n", token );
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

int16_t Library::import( const char label[], const char symbols[], const uint16_t code[] ) {
  strcpy( lib[lib_cnt].label, label );
  lib[lib_cnt].label[strlen(label)] = '\0';
  lib[lib_cnt].symbols = (symbolic_machine_table*) symbols;
  lib[lib_cnt].code = (int16_t*) code;
  return lib_cnt++;
}

Library& Library::printHexWord( Stream* stream, int16_t v, bool last /* = false */ ) {
  if ( last ) 
    stream->printf( "0x%04X", (uint16_t)v );
  else 
    stream->printf( "0x%04X, ", (uint16_t)v );
  if ( ++word_cnt % 12 == 0 ) stream->print( "\n  " );
  return *this;
}

Library& Library::hexdump( Stream* stream, int16_t slot ) {
  char buf[1024];
  uint8_t last = 0;
  if ( slot == -1 ) return *this;
  stream->printf( "const char %s_symbin[] = {\n", label( slot ) );
  for ( int16_t b = 0; b < 4; b++ ) {
    if ( countSymbols( slot, b ) > 0 ) {
      last = b;
    }
  }
  for ( int16_t b = 0; b <= last; b++ ) {
    buf[0] = '\0';
    uint16_t cnt = 0;
    for ( int16_t i = 0; i < countSymbols( slot, b ); i++ ) {
      const char* s = findSymbol( slot, i, b );
      strcat( buf, s );
      strcat( buf, "\\0" );
      cnt += strlen( s ) + 1;
    }
    cnt++;
    if ( b < last ) {
      while ( cnt % 4 != 0 ) {
        strcat( buf, "\\0" );
        cnt++;
      }
    }
    if ( b == last ) {
      stream->printf( "  \"\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\" \"%s\\0\"\n};\n", buf );      
    } else {
      // Portability note: uint32_t byte order assumed
      stream->printf( "  \"\\x78\\x56\\x34\\x12\\x%02x\\x%02x\\x00\\x00\" \"%s\\0\"\n", 
                      cnt & 0x00FF, cnt >> 8, buf );
    }
  } 
  stream->println();
  stream->printf( "const uint16_t %s_hexbin[] = {\n  ", label( slot ) );
  int16_t* p = lib[slot].code;
  word_cnt = 0;
  while ( *p != -1 ) {
    printHexWord( stream, *p++ );
  }
  printHexWord( stream, *p++ );
  while ( *p != -1 ) {
    printHexWord( stream, *p++ );
    while ( *p != -1 ) {
      printHexWord( stream, *p++ );
      printHexWord( stream, *p++ );
      printHexWord( stream, *p++ );
      printHexWord( stream, *p++ );
    }
    printHexWord( stream, *p++ );
  }
  printHexWord( stream, *p++, true );
  stream->println( "\n};\n" );
  return *this;
}

Library& Library::hexdump(  Stream* stream, const char label[] ) {
  hexdump ( stream, index( label ) );
  return *this;
}

uint64_t Library::code( int16_t idx ) {
  lib_code_pack cp;
  cp.code = lib[idx].code;
  cp.symbols = lib[idx].symbols;
  return cp.pack;
}

int64_t Library::code( const char label[] ) {
  return code( index( label ) );
}

int16_t* Library::codePtr( int16_t idx ) {
  return lib[idx].code;
}

int16_t* Library::codePtr( const char label[] ) {
  return lib[index( label )].code;
}

symbolic_machine_table* Library::symbolPtr( int16_t idx ) {
  return lib[idx].symbols;
}

symbolic_machine_table* Library::symbolPtr( const char label[] ) {
  return lib[index(label)].symbols;
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
  find_error = 0;
  if ( strlen( s ) == 0 ) return 0;
  if ( isdigit( s[0] ) || ( s[0] == '-' && isdigit( s[1] ) ) ) return atoi( s );   
  while ( p != NULL ) {
    int16_t i = findString( s, p->s );
    if ( i >= 0 ) return i;
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table) + p->offset ) : p->next;
  }
  find_error = 1;
  return 0;
}

int16_t Library::findSymbol( const char label[], const char s[] ) {
  return findSymbol( index( label ), s );  
}

// Returns the n-th symbol in a bank or a NULL pointer if it does not exist

const char* Library::findSymbol( int16_t slot, int16_t idx, int8_t bank /* = 0 */ ) {
  symbolic_machine_table* p = lib[slot].symbols;
  uint8_t pcnt = 0;
  if ( idx < 0 ) {
    return null_str;
  }  
  while ( p != NULL && pcnt < bank ) {
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table) + p->offset ) : p->next;
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
    p = p->offset > 0 ? (symbolic_machine_table *) ( (char*) p + sizeof( symbolic_machine_table) + p->offset ) : p->next;
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

int16_t Library::count() {
  return lib_cnt;
}

char* Library::label( int16_t slot ) {
  return lib[slot].label;
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
