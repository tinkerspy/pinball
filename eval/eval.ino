
int16_t ram[1024];

struct var_table { /* Memory lookup table: simple vars & arrays */
  char label[20];
  uint16_t ptr;
  uint8_t len, size;  
} var[32] = {
   "AAA", 0, 0, 1,
   "BBB", 1, 0, 10,
};
uint8_t addr_cnt = 2;
uint8_t ram_cnt = 11;

int16_t get_addr( char label[] ) {
  for ( uint8_t i = 0; i < addr_cnt; i++ ) {
    if ( strcasecmp( label, var[i].label ) == 0 )
      return i;
  }
  return -1;  
}

int16_t get_var( char label[] ) {
  int a = get_addr( label );
  if ( a != -1 ) {
    return ram[var[a].ptr];
  }  
  return 0;  
}

int16_t get_length( char label[] ) {
  int a = get_addr( label );
  if ( a != -1 ) {
    return var[a].len;
  }
  return -1;  
}

int16_t get_size( char label[] ) {
  int a = get_addr( label );
  if ( a != -1 ) {
    return var[a].size;
  }
  return -1;  
}

void create_var( char label[], uint8_t size, uint8_t len ) {
  int a = get_addr( label );
  if ( a == -1 ) {
    a = addr_cnt++; // Check overflow
    memset( ram + ram_cnt, 0, size );
    var[a].size = size;    
    var[a].ptr = ram_cnt;
    var[a].len = len;
    strcpy( var[a].label, label );
    ram_cnt += size;
  }
}

void set_var( char label[], uint8_t index, int16_t v ) {
  int a = get_addr( label );
  if ( a != -1 ) {
    if ( index < var[a].size ) {
      if ( index >= var[a].len ) {
        var[a].len = index + 1;
      }
      ram[var[a].ptr + index] = v;
    }
  } else {
    create_var( label, index, index );
    set_var( label, index, v );
  }
}

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  Serial.setTimeout( 100000 );
  memset( ram, 0, sizeof( ram ) );
  ram[0] = 123;
  ram[1] = 256;
  
}

void preprocess( char s[] ) {
  Serial.print( "Variable found: " );
  Serial.println( get_var( s ) );
  
  
}


char buffer[81];

void loop() {
  Serial.readBytesUntil( '\n', buffer, 80 );
  Serial.print( "Received: " );
  Serial.println( buffer );
  preprocess( buffer );
  Serial.print( "Prepped: " );
  Serial.println( buffer );
  
}
