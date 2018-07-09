#include "Atm_oxo_bot.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_oxo_bot& Atm_oxo_bot::begin( int personality /* = SMART_BOT */, uint32_t delay /* = 0 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER  EVT_MOVE  ELSE */
    /*  IDLE */       -1, ATM_SLEEP,      -1,        -1,     WAIT,   -1,
    /*  WAIT */       -1,        -1,      -1,      MOVE,       -1,   -1,
    /*  MOVE */ ENT_MOVE,        -1,      -1,        -1,       -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->bot_personality = personality;
  timer.set( delay );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_oxo_bot::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_MOVE:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_MOVE, 0, <v>, <up> );
 */

void Atm_oxo_bot::action( int id ) {
  switch ( id ) {
    case ENT_MOVE:
      int m = 0;
      switch ( bot_personality ) {
        case RANDOM_BOT: 
          m = randomBot( piece, wins );
          break;
        case STUPID_BOT: 
          m = stupidBot( piece, wins );
          break;
        case SMART_BOT: 
          m = smartBot( piece, wins );
          break;
      }
      push( connectors, ON_MOVE, 0, m, 0 );
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_oxo_bot& Atm_oxo_bot::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_oxo_bot::state( void ) {
  return Machine::state();
}

char Atm_oxo_bot::winner( oxo_wins_t &wins ) { // Returns O, X or 0
  for ( int w = 0; w < 8; w++ ) 
    if ( wins[w][0] > 9 && wins[w][0] == wins[w][1] && wins[w][1] == wins[w][2] ) {
      return wins[w][0]; 
    }
  return 0;
}

int Atm_oxo_bot::randomBit( uint16_t v ) { // Selects a random 1 bit from the arg and returns its number (or -1 if arg = 0 )
  int cnt = 0;
  for ( int c = 0; c < 16; c++ )
    if ( v & ( 1 << c ) ) cnt++;
  int r = random( 0, cnt );
  for ( int c = 0; c < 16; c++ )
    if ( v & ( 1 << c ) )
      if ( !r-- ) return c;
  return -1;
}



uint16_t Atm_oxo_bot::availableMoves( oxo_wins_t &wins ) { // Returns bitmap with free squares, 1 = bit1, 2 = bit2, etc. (1..9)
  uint16_t r = 0;
  for ( int w = 0; w < 3; w++ ) 
    for ( int p = 0; p < 3; p++ )   
      if ( wins[w][p] <= 9 ) r |= 1 << ( w * 3 + p + 1);
  return r;
}

/*
 * Remise: availableWins( 'O', wins ) == 0 && availableWins( 'X', wins ) == 0
 * Bot can't win: availableWins( 'O', wins ) == 0
 */

uint16_t Atm_oxo_bot::availableWins( char piece, oxo_wins_t &wins ) { // Returns bitmap of possible wins (0..7) for piece
  uint16_t r = 0;
  for ( int w = 0; w < 3; w++ ) {
    bool winnable = true;
    for ( int p = 0; p < 3; p++ ) {  
      if ( wins[w][p] > 9 && wins[w][p] != piece ) winnable = false;
    }
    if ( winnable ) r |= 1 << w;
  }
  return r;
}

int Atm_oxo_bot::bitCount( uint16_t v ) { // Returns the number of high bits in the argument
  int r = 0;
  while ( v )  {
    if ( v & 1 ) r++;
    v >>= 1;
  }
  return r;
}

int Atm_oxo_bot::findWin( char piece, oxo_wins_t &wins ) {
  uint16_t available = availableMoves( wins );
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      for ( int w = 0; w < 8; w++ ) {
        bool success = true;
        for ( int p = 0; p < 3; p++ ) {
          if ( wins[w][p] != piece && wins[w][p] != c ) success = false; 
        }
        if ( success ) return c;
      }
    }
  }
  return 0;
}

uint16_t Atm_oxo_bot::checkWins( oxo_wins_t &wins, char piece, int move ) { // Returns bitmap of possible wins (0..8)

  uint16_t r = 0;
  for ( int w = 0; w < 8; w++ ) {
    bool his = false;
    bool mine = false;
    bool mover = false;
    for ( int p = 0; p < 3; p++ ) {
      if ( wins[w][p] == move ) mover = true;
      if ( wins[w][p] == piece ) mine = true;
      if ( wins[w][p] > 9 && wins[w][p] != piece ) his = true;
    }
    if ( mover && mine && !his ) r |= 1 << w;
  }
  return r;
}

int Atm_oxo_bot::randomBot( char piece, oxo_wins_t &wins ) { // Makes a random valid move
  if ( winner( wins ) ) return 0; // Give up when all is lost
  int r = randomBit( availableMoves( wins ) );
  return r == -1 ? 0 : r;
}

int Atm_oxo_bot::stupidBot( char piece, oxo_wins_t &wins ) { // Takes wins, blocks opponent wins 
  if ( winner( wins ) ) return 0; // Give up when all is lost
  uint16_t available = availableMoves( wins );
  if ( available & ( 1 << 5 ) ) return 5; // If cell 5 is free, take it
  char opp_piece = piece == 'X' ? 'O' : 'X'; 
  // Try to find a winning square for the bot
  int m = 0;
  if ( ( m = findWin( piece, wins ) ) ) return m;
  if ( ( m = findWin( opp_piece, wins ) ) ) return m;
  // Fall back to random  
  m = randomBit( available );
  return m > 0 ? m : 0;
}

// TODO: bot concedes defeat: no more wins possible

int Atm_oxo_bot::smartBot( char piece, oxo_wins_t &wins ) { // Takes wins, blocks opponent wins, blocks opponent forks 
  if ( winner( wins ) ) return 0; // Give up when all is lost
  uint16_t available = availableMoves( wins );
  if ( available & ( 1 << 5 ) ) return 5; // If cell 5 is free, take it
  char opp_piece = piece == 'X' ? 'O' : 'X'; 
  int m = 0;
  // Try to find a winning square for the bot
  if ( ( m = findWin( piece, wins ) ) ) return m;
  if ( ( m = findWin( opp_piece, wins ) ) ) return m;
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      if ( bitCount( checkWins( wins, piece, c ) ) > 1 ) {
        m = c;
      }
    }
  }
  if ( m ) return m;
  for ( int c = 1; c < 10; c++ ) {
    if ( available & ( 1 << c ) ) {
      uint16_t wmap = checkWins( wins, piece, c );
      for ( int w = 0; w < 8; w++ ) {
        if ( wmap & 1 << w ) {
          for ( int p = 0; p < 3; p++ ) {
            if ( wins[w][p] != piece && wins[w][p] != c ) {
              if ( bitCount( checkWins( wins, opp_piece, wins[w][p] ) ) < 2 ) {
                m = c;
              }
            }
          }
        }        
      }      
    }
  }  
  if ( m ) return m;
  // Fall back to random
  if ( bitCount( available ) == 8 ) {
    m = randomBit( 0b1010001010 ); // Second move: only corners!
  } else {
    m = randomBit( available );
  }
  return m > 0 ? m : 0;
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_oxo_bot& Atm_oxo_bot::move( char piece, oxo_wins_t &wins ) {

  memcpy( this->wins, wins, sizeof( oxo_wins_t ) );
  this->piece = piece;
  trigger( EVT_MOVE );
  return *this;
}

/*
 * onMove() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_oxo_bot& Atm_oxo_bot::onMove( Machine& machine, int event ) {
  onPush( connectors, ON_MOVE, 0, 1, 1, machine, event );
  return *this;
}

Atm_oxo_bot& Atm_oxo_bot::onMove( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_MOVE, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_oxo_bot& Atm_oxo_bot::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "OXO_BOT\0EVT_TIMER\0EVT_MOVE\0ELSE\0IDLE\0WAIT\0MOVE" );
  return *this;
}

