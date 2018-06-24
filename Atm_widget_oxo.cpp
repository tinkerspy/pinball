#include "Atm_widget_oxo.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

/* TODO: OXO-Bot mode
 *  toggle disabled (player is O)
 *  OXO_1X, OXO_2X, AOX_3X select corresponding O event
 *  First event is ignored (extra wait state?)
 */


Atm_widget_oxo& Atm_widget_oxo::begin( Atm_apa102& led, int8_t* led_map ) { // Expects 9 * 3 + 2 leds
  // clang-format off
  const static state_t state_table[] PROGMEM = { 
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_MATCH  EVT_1X  EVT_1O  EVT_2X  EVT_2O  EVT_3X  EVT_3O  EVT_4  EVT_5  EVT_6  EVT_7  EVT_8  EVT_9  EVT_INIT  EVT_TOGGLE  ELSE */
    /*   IDLE */         -1, ATM_SLEEP,      -1,        -1, OXO_1X, OXO_1O, OXO_2X, OXO_2O, OXO_3X, OXO_3O, OXO_4, OXO_5, OXO_6, OXO_7, OXO_8, OXO_9,     INIT,     TOGGLE,   -1,
    /* OXO_1X */     ENT_1X,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /* OXO_1O */     ENT_1O,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /* OXO_2X */     ENT_2X,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /* OXO_2O */     ENT_2O,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /* OXO_3X */     ENT_3X,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /* OXO_3O */     ENT_3O,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  OXO_4 */      ENT_4,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  OXO_5 */      ENT_5,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  OXO_6 */      ENT_6,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  OXO_7 */      ENT_7,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  OXO_8 */      ENT_8,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  OXO_9 */      ENT_9,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /*  MATCH */  ENT_MATCH, ATM_SLEEP,      -1,        -1, OXO_1X, OXO_1O, OXO_2X, OXO_2O, OXO_3X, OXO_3O, OXO_4, OXO_5, OXO_6, OXO_7, OXO_8, OXO_9,     INIT,     TOGGLE,   -1,
    /*   INIT */   ENT_INIT,        -1,      -1,        -1,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
    /* TOGGLE */ ENT_TOGGLE,        -1,      -1,     MATCH,     -1,     -1,     -1,     -1,     -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,       -1,         -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->led_map = led_map;
  this->led = &led;
  led.on( led_map[27] ); // Default selector: X
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_widget_oxo::event( int id ) {
  switch ( id ) {
    case EVT_MATCH:
      char s1 = set( 1 ), s2 = set( 2 ), s3 = set( 3 );
      char s4 = set( 4 ), s5 = set( 5 ), s6 = set( 6 );
      char s7 = set( 7 ), s8 = set( 8 ), s9 = set( 9 );
      if ( s1 && s1 == s2 && s2 == s3 ) return 1; // Horizontal
      if ( s4 && s4 == s5 && s5 == s6 ) return 1;
      if ( s7 && s7 == s8 && s8 == s9 ) return 1;
      if ( s1 && s1 == s4 && s4 == s7 ) return 1; // Vertical
      if ( s2 && s2 == s5 && s5 == s8 ) return 1;
      if ( s3 && s3 == s6 && s6 == s9 ) return 1;
      if ( s1 && s1 == s5 && s5 == s9 ) return 1; // Diagonal
      if ( s7 && s7 == s5 && s5 == s3 ) return 1;
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_MATCH, 0, <v>, <up> );
 *   push( connectors, ON_SET, 0, <v>, <up> );
 */

void Atm_widget_oxo::action( int id ) {
  switch ( id ) {
    case ENT_1X:
      if ( !set( 1 ) ) {
        set( 1, 'X' );
        push( connectors, ON_SET, 0, 1, 1 );
      }
      return;
    case ENT_1O:
      if ( !set( 1 ) ) {
        set( 1, 'O' );
        push( connectors, ON_SET, 0, 2, 0 );
      }
      return;
    case ENT_2X:
      if ( !set( 2 ) ) {
        set( 2, 'X' );
        push( connectors, ON_SET, 0, 2, 1 );
      }
      return;
    case ENT_2O:
      if ( !set( 2 ) ) {
        set( 2, 'O' );
        push( connectors, ON_SET, 0, 2, 0 );
      }
      return;
    case ENT_3X:
      if ( !set( 3 ) ) {
        set( 3, 'X' );
        push( connectors, ON_SET, 0, 3, 1 );
      }
      return;
    case ENT_3O:
      if ( !set( 3 ) ) {
        set( 3, 'O' );
        push( connectors, ON_SET, 0, 3, 0 );
      }
      return;
    case ENT_4:
      if ( !set( 4 ) ) {
        set_to_active( 4 );
        push( connectors, ON_SET, 0, 4, led->active( led_map[ 27 ] ) ? 1 : 0 );
      }
      return;
    case ENT_5:
      if ( !set( 5 ) ) {
        set_to_active( 5 );
        push( connectors, ON_SET, 0, 5, led->active( led_map[ 27 ] ) ? 1 : 0 );
      }
      return;
    case ENT_6:
      if ( !set( 6 ) ) {
        set_to_active( 6 );
        push( connectors, ON_SET, 0, 6, led->active( led_map[ 27 ] ) ? 1 : 0 );
      }
      return;
    case ENT_7:
      if ( !set( 7 ) ) {
        set_to_active( 7 );
        push( connectors, ON_SET, 0, 7, led->active( led_map[ 27 ] ) ? 1 : 0 );
      }
      return;
    case ENT_8:
      if ( !set( 8 ) ) {
        set_to_active( 8 );
        push( connectors, ON_SET, 0, 8, led->active( led_map[ 27 ] ) ? 1 : 0 );
      }
      return;
    case ENT_9:
      if ( !set( 9 ) ) {
        set_to_active( 9 );
        push( connectors, ON_SET, 0, 9, led->active( led_map[ 27 ] ) ? 1 : 0 );
      }
      return;
    case ENT_MATCH:
      push( connectors, ON_MATCH, 0, 1, 1 ); // FIXME: This fires multiple times per game
      return;
    case ENT_INIT:
      for ( int i = 0; i < 9; i++ ) {
        set( i + 1, 0 );
      }
      push( connectors, ON_INIT, 0, 9, led->active( led_map[ 27 ] ) ? 1 : 0 );
      return;
    case ENT_TOGGLE:
      if ( led->active( led_map[ 27 ] ) ) {
        led->off( led_map[ 27 ] ).on( led_map[ 28 ] );
      } else {
        led->off( led_map[ 28 ] ).on( led_map[ 27 ] );
      }
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_widget_oxo& Atm_widget_oxo::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_widget_oxo::state( void ) {
  return current == MATCH ? 1 : 0;
}

void Atm_widget_oxo::set( int cell, char v ) {
  switch ( v ) {
    case 'X':
      led->on( led_map[ ( cell - 1 ) * 3 + 1 ] );
      return;
    case 'O':
      led->on( led_map[ ( cell - 1 ) * 3 + 0 ] );
      led->on( led_map[ ( cell - 1 ) * 3 + 2 ] );
      return;
  }
  led->off( led_map[ ( cell - 1 ) * 3 + 1 ] );
  led->off( led_map[ ( cell - 1 ) * 3 + 0 ] );
  led->off( led_map[ ( cell - 1 ) * 3 + 2 ] );
}

void Atm_widget_oxo::set_to_active( int cell ) {
  set( cell, led->active( led_map[27] ) ? 'X' : 'O' );
}

char Atm_widget_oxo::set( int cell ) {
  if ( led->active( led_map[ ( cell - 1 ) * 3 + 0 ] ) ) return 'O';
  if ( led->active( led_map[ ( cell - 1 ) * 3 + 1 ] ) ) return 'X';
  return 0;
}

Atm_widget_oxo& Atm_widget_oxo::dump( Stream & stream ) {
  stream.print( set( 1 ) ? set( 1 ) : '-' );
  stream.print( set( 2 ) ? set( 2 ) : '-' );
  stream.print( set( 3 ) ? set( 3 ) : '-' );
  stream.println();
  stream.print( set( 4 ) ? set( 4 ) : '-' );
  stream.print( set( 5 ) ? set( 5 ) : '-' );
  stream.print( set( 6 ) ? set( 6 ) : '-' );
  stream.println();
  stream.print( set( 7 ) ? set( 7 ) : '-' );
  stream.print( set( 8 ) ? set( 8 ) : '-' );
  stream.print( set( 9 ) ? set( 9 ) : '-' );
  stream.println();
  return *this;  
}

uint16_t Atm_widget_oxo::loadWins( oxo_wins_t &wins ) {
  // Winning patterns 123 456 789 147 258 369 159 357
  uint16_t r = 0;
  wins[0][0] = wins[3][0] = wins[6][0] = cell( 1 );
  wins[0][1] = wins[4][0] = cell( 2 );
  wins[0][2] = wins[7][0] = wins[5][0] = cell( 3 );
  wins[1][0] = wins[3][1] = cell( 4 );
  wins[6][1] = wins[7][1] = wins[4][1] = wins[1][1] = cell( 5 );
  wins[1][2] = wins[5][1] = cell( 6 );
  wins[3][2] = wins[2][0] = wins[7][2] = cell( 7 );
  wins[2][1] = wins[4][2] = cell( 8 );  
  wins[2][2] = wins[6][2] = wins[5][2] = cell( 9 );  
  if ( wins[0][0] <= 9 ) r |= 1 << 1 ;
  if ( wins[0][1] <= 9 ) r |= 1 << 2 ;
  if ( wins[0][2] <= 9 ) r |= 1 << 3 ;
  if ( wins[1][0] <= 9 ) r |= 1 << 4 ;
  if ( wins[1][1] <= 9 ) r |= 1 << 5 ;
  if ( wins[1][2] <= 9 ) r |= 1 << 6 ;
  if ( wins[2][0] <= 9 ) r |= 1 << 7 ;
  if ( wins[2][1] <= 9 ) r |= 1 << 8 ;
  if ( wins[2][2] <= 9 ) r |= 1 << 9 ;
  return r;  
}

Atm_widget_oxo& Atm_widget_oxo::dumpWins( Stream &stream, oxo_wins_t &wins, int idx ) {
  for ( int win = 0; win < 8; win++ ) {
    if ( idx == -1 || idx == win ) {
      for ( int pos = 0; pos < 3; pos++ ) {
        if ( wins[win][pos] > 9 ) {
          stream.print( wins[win][pos] ); 
        } else {
          stream.print( int(wins[win][pos]) );        
        }
      }
      stream.print( ' ' );
    }
  }
  stream.println();
  return *this;  
}

Atm_widget_oxo& Atm_widget_oxo::select( char v ) {
  if ( v == 'O' ) led->on( 28 ).off( 27 );
  if ( v == 'X' ) led->on( 27 ).off( 28 );
  return *this;  
}


char Atm_widget_oxo::cell( int no ) {
  char c = set( no );
  return c ? c : no; 
}


/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_widget_oxo& Atm_widget_oxo::oxo_1x() {
  trigger( EVT_1X );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_1o() {
  trigger( EVT_1O );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_2x() {
  trigger( EVT_2X );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_2o() {
  trigger( EVT_2O );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_3x() {
  trigger( EVT_3X );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_3o() {
  trigger( EVT_3O );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_4() {
  trigger( EVT_4 );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_5() {
  trigger( EVT_5 );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_6() {
  trigger( EVT_6 );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_7() {
  trigger( EVT_7 );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_8() {
  trigger( EVT_8 );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::oxo_9() {
  trigger( EVT_9 );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::toggle() {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::init() {
  trigger( EVT_INIT );
  return *this;
}

/*
 * onMatch() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_widget_oxo& Atm_widget_oxo::onMatch( Machine& machine, int event ) {
  onPush( connectors, ON_MATCH, 0, 1, 1, machine, event );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::onMatch( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_MATCH, 0, 1, 1, callback, idx );
  return *this;
}

/*
 * onset() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_widget_oxo& Atm_widget_oxo::onSet( Machine& machine, int event ) {
  onPush( connectors, ON_SET, 0, 1, 1, machine, event );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::onSet( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_SET, 0, 1, 1, callback, idx );
  return *this;
}

/*
 * onSet() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_widget_oxo& Atm_widget_oxo::onInit( Machine& machine, int event ) {
  onPush( connectors, ON_INIT, 0, 1, 1, machine, event );
  return *this;
}

Atm_widget_oxo& Atm_widget_oxo::onInit( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_INIT, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_widget_oxo& Atm_widget_oxo::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "WIDGET_OXO\0EVT_MATCH\0EVT_1X\0EVT_1O\0EVT_2X\0EVT_2O\0EVT_3X\0EVT_3O\0EVT_4\0EVT_5\0EVT_6\0EVT_7\0EVT_8\0EVT_9\0EVT_INIT\0EVT_TOGGLE\0ELSE\0IDLE\0OXO_1X\0OXO_1O\0OXO_2X\0OXO_2O\0OXO_3X\0OXO_3O\0OXO_4\0OXO_5\0OXO_6\0OXO_7\0OXO_8\0OXO_9\0MATCH\0INIT\0TOGGLE" );
  return *this;
}



