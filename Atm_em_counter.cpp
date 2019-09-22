#include "Atm_em_counter.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */
 

Atm_em_counter& Atm_em_counter::begin( Atm_playfield& playfield, int16_t sensor_switch, int16_t led_group_id, int profile ) {
  
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER    ON_LOOP  ON_EXIT  EVT_CLEAN EVT_LO  EVT_HI  EVT_DIG3  EVT_DIG2  EVT_DIG1  EVT_DIG0  EVT_RESET  EVT_ZERO  EVT_TIMER, EVT_CHANGE,  ELSE */
    /*  IDLE */       -1, ATM_SLEEP,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,     RESET,     ZERO,        -1,      CHECK,    -1,
    /* CHECK */       -1,        -1,      -1,        -1,    -1,     -1,     DIG3,     DIG2,     DIG1,     DIG0,        -1,       -1,        -1,         -1,  IDLE,
    /*  DIG0 */ ENT_DIG0,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     CHECK,         -1,    -1,
    /*  DIG1 */ ENT_DIG1,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     CHECK,         -1,    -1,
    /*  DIG2 */ ENT_DIG2,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     CHECK,         -1,    -1,
    /*  DIG3 */ ENT_DIG3,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     CHECK,         -1,    -1,
    /*  ZERO */ ENT_ZERO,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, CHECK,
    /* RESET */ENT_RESET,        -1,      -1,      IDLE,    -1,   FRST,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1,  PRE0,
// Move to start position (digitalRead() == high) by pulsing reels 0 & 1
    /* PRE0 */ ENT_PULS0,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     PRE0W,         -1,    -1,
    /* PRE0W */       -1,        -1,      -1,        -1,    -1,   FRST,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1,  PRE1,
    /* PRE1 */ ENT_PULS1,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     PRE1W,         -1,    -1,
    /* PRE1W */       -1,        -1,      -1,        -1,    -1,   FRST,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1,  PRE0,
// Solve first reel (first that goes low) by pulsing all three
    /* FRST  */ ENT_FRST,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, FRST1,
    /* FRST1 */ENT_PULS1,        -1,      -1,        -1,    -1,     -1,       -1,       -1,    FRST2,       -1,        -1,       -1,    FRST1W,         -1,    -1,
    /* FRST1W */      -1,        -1,      -1,        -1,  SCND,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, FRST2,
    /* FRST2 */ENT_PULS2,        -1,      -1,        -1,    -1,     -1,       -1,    FRST3,       -1,       -1,        -1,       -1,    FRST2W,         -1,    -1,
    /* FRST2W */      -1,        -1,      -1,        -1,  SCND,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, FRST3,
    /* FRST3 */ENT_PULS3,        -1,      -1,        -1,    -1,     -1,    FRST1,       -1,       -1,       -1,        -1,       -1,    FRST3W,         -1,    -1,
    /* FRST3W */      -1,        -1,      -1,        -1,  SCND,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, FRST1,
// Solve second reel (first that goes high) by pulsing the remaining 2
    /* SCND  */ ENT_SCND,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, SCND1,
    /* SCND1 */ENT_PULS1,        -1,      -1,        -1,    -1,     -1,       -1,       -1,    SCND2,       -1,        -1,       -1,    SCND1W,         -1,    -1,
    /* SCND1W */      -1,        -1,      -1,        -1,    -1,   THRD,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, SCND2,
    /* SCND2 */ENT_PULS2,        -1,      -1,        -1,    -1,     -1,       -1,    SCND3,       -1,       -1,        -1,       -1,    SCND2W,         -1,    -1,
    /* SCND2W */      -1,        -1,      -1,        -1,    -1,   THRD,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, SCND3,
    /* SCND3 */ENT_PULS3,        -1,      -1,        -1,    -1,     -1,    SCND1,       -1,       -1,       -1,        -1,       -1,    SCND3W,         -1,    -1,
    /* SCND3W */      -1,        -1,      -1,        -1,    -1,   THRD,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, SCND1,
// Solve third reel (first that goes high, and keep it at 9 to solve reel 0)
    /* THRD  */ ENT_THRD,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     THRD1,         -1,    -1,
    /* THRD1 */ENT_PULS1,        -1,      -1,        -1,    -1,     -1,       -1,       -1,    THRD2,       -1,        -1,       -1,    THRD1W,         -1,    -1,
    /* THRD1W */      -1,        -1,      -1,        -1,    -1,   FRTH,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, THRD2,
    /* THRD2 */ENT_PULS2,        -1,      -1,        -1,    -1,     -1,       -1,    THRD3,       -1,       -1,        -1,       -1,    THRD2W,         -1,    -1,
    /* THRD2W */      -1,        -1,      -1,        -1,    -1,   FRTH,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, THRD3,
    /* THRD3 */ENT_PULS3,        -1,      -1,        -1,    -1,     -1,    THRD1,       -1,       -1,       -1,        -1,       -1,    THRD3W,         -1,    -1,
    /* THRD3W */      -1,        -1,      -1,        -1,    -1,   FRTH,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, THRD1,    
// Solve reel 0 (pulse until line goes low)         
    /* FRTH */  ENT_FRTH,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, FRTH0,
    /* FRTH0 */ENT_PULS0,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,     FRTHW,         -1,    -1,
    /* FRTHW */       -1,        -1,      -1,        -1,  FFTH,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1, FRTH0,
// Move third reel to 0                             
    /* FFTH  */ ENT_FFTH,        -1,      -1,        -1,    -1,     -1,       -1,       -1,       -1,       -1,        -1,       -1,        -1,         -1,  IDLE,    
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->playfield = &playfield;
  this->sensor_switch = sensor_switch;
  const int16_t* p = playfield.leds().group( led_group_id );
  playfield.leds().profile( this->coil[0] = p[0], profile );
  playfield.leds().profile( this->coil[1] = p[1], profile );
  playfield.leds().profile( this->coil[2] = p[2], profile );
  playfield.leds().profile( this->coil[3] = p[3], profile );
  current_value = 0;
  memset( soll, 0, sizeof( soll ) );
  memset( ist, 0, sizeof( ist ) );
  memset( solved, 0, sizeof( solved ) );
  timer.set( DIGIT_DELAY_MS );
  resetting = false;
  touched = true;
  enabled = true;
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_em_counter::event( int id ) {
  switch ( id ) {
    case EVT_CLEAN:
      return !touched;
    case EVT_LO:
      return !sensor();
    case EVT_HI:
      return sensor();
    case EVT_DIG0:      
      return resetting ? solved[0] : ( ist[0] != soll[0] );
    case EVT_DIG1:
      return resetting ? solved[1] : ( ist[1] != soll[1] );
    case EVT_DIG2:
      return resetting ? solved[2] : ( ist[2] != soll[2] );
    case EVT_DIG3:
      return resetting ? solved[3] : ( ist[3] != soll[3] );
    case EVT_RESET:
      return 0;
    case EVT_TIMER:
      return timer.expired( this );
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_DIGIT, <sub>, <v>, <up> );
 */

void Atm_em_counter::action( int id ) {
  switch ( id ) {
    case ENT_DIG0:
      pulse( 0, true );
      return;
    case ENT_DIG1:
      pulse( 1, true, RECURSIVE );
      push( connectors, ON_DIGIT, 2, 1000, 1 );
      return;
    case ENT_DIG2:
      pulse( 2, true, RECURSIVE );
      push( connectors, ON_DIGIT, 1, 100, 1 );
      return;
    case ENT_DIG3:
      pulse( 3, true, RECURSIVE );
      push( connectors, ON_DIGIT, 0, 10, 1 );
      return;
    case ENT_ZERO:
      set( 0 );
      return;
    case ENT_RESET:
      memset( soll, 0, sizeof( soll ) );
      memset( ist, 0, sizeof( ist ) );
      memset( solved, 0, sizeof( solved ) );
      return;
    case ENT_PULS0:
      resetting = true;
      pulse( 0 );
      return;
    case ENT_PULS1:
      pulse( 1 );
      return;
    case ENT_PULS2:
      pulse( 2 );
      return;
    case ENT_PULS3:
      pulse( 3 );
      return;
    case ENT_FRST:
      resetting = true;
      return;
    case ENT_SCND:
      solved[last_pulse] = 1;
      return;
    case ENT_THRD:
      solved[last_pulse] = 1;
      pulse( last_pulse, true );
      return;
    case ENT_FRTH:
      solved[last_pulse] = 2; // 2 signals extra pulse is required
      return;
    case ENT_FFTH:
      for ( int i = 0; i < 4; i++ ) {
        if ( solved[i] == 2 ) pulse( i, true );
        ist[i] = 0;      
      }
      memset( soll, 0, sizeof( soll ) );
      memset( ist, 0, sizeof( ist ) );
      memset( solved, 0, sizeof( solved ) );
      resetting = false;
      touched = false;
      current_value = 0;
      return;
  }
}

Atm_em_counter& Atm_em_counter::chain( Atm_em_counter& next ) {
  this->next = &next;  
  return *this;
}

Atm_em_counter& Atm_em_counter::select( uint32_t mask ) {
  this->enabled = mask & 1;
  if ( next ) {
    next->select( mask >> 1 );
  }
  return *this;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_em_counter& Atm_em_counter::trigger( int event ) {
  if ( next ) {
    next->trigger( event );
  }
  if ( this->enabled ) {
    switch ( event ) {
      case EVT_10:
        set( current_value + 1 );
        return *this;
      case EVT_100:
        set( current_value + 10 );
        return *this;
      case EVT_500:
        set( current_value + 50 );
        return *this;
      case EVT_1000:
        set( current_value + 100 );
        return *this;
      case EVT_5000:
        set( current_value + 500 );
        return *this;
    }
    Machine::trigger( event );
  }
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_em_counter::state( void ) {
  return Machine::state();
}

uint16_t Atm_em_counter::value( void ) {
  return current_value * 10;
}

Atm_em_counter& Atm_em_counter::set( uint16_t v ) {
  current_value = v;
  soll[3] = v % 10;
  soll[2] = v % 100 / 10;
  soll[1] = v % 1000 / 100;
  soll[0] = v % 10000 / 1000;
//  Serial.print( "set: " );
//  Serial.println( current_value );
  trigger( EVT_CHANGE );
  return *this;
}

Atm_em_counter& Atm_em_counter::add( int16_t v ) {
  set( current_value + v );
  return *this;
}

Atm_em_counter& Atm_em_counter::pulse( int8_t reel, uint8_t force /* = 0 */, uint8_t recursive /* = 0 */ ) {
  if ( reel >= 0 ) {
    if ( force || solved[reel] == 0 ) {
      ist[reel] = ( ist[reel] + 1 ) % 10;
      playfield->leds().on( coil[reel] );
      if ( ist[reel] == 0 && recursive ) pulse( reel - 1, force, recursive ); 
      last_pulse = reel;
      touched = true;
    }
  }
  return *this;
}

int Atm_em_counter::sensor( void ) {
  return playfield->isPressed( sensor_switch );
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_em_counter& Atm_em_counter::reset() {
  trigger( EVT_RESET );
  return *this;
}

Atm_em_counter& Atm_em_counter::zero() {
  trigger( EVT_ZERO );
  return *this;
}

/*
 * onScore() push connector variants ( slots 3, autostore 0, broadcast 0 )
 */

Atm_em_counter& Atm_em_counter::onDigit( int sub, Machine& machine, int event ) {
   if ( next ) next->onDigit( sub, machine, event );    
  if ( enabled ) onPush( connectors, ON_DIGIT, sub, 3, 0, machine, event );
  return *this;
}

Atm_em_counter& Atm_em_counter::onDigit( int sub, atm_cb_push_t callback, int idx ) {
  if ( next ) next->onDigit( sub, callback, idx );    
  if ( enabled ) onPush( connectors, ON_DIGIT, sub, 3, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_em_counter& Atm_em_counter::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "EM_COUNTER\0EVT_CLEAN\0EVT_LO\0EVT_HI\0EVT_DIG3\0EVT_DIG2\0EVT_DIG1\0EVT_DIG0\0EVT_RESET\0EVT_ZERO\0EVT_TIMER\0EVT_CHANGE\0ELSE\0"
    "IDLE\0CHECK\0DIG0\0DIG1\0DIG2\0DIG3\0ZERO\0RESET\0PRE0\0PRE0W\0PRE1\0PRE1W\0FRST\0FRST1\0FRST1W\0FRST2\0FRST2W\0FRST3\0FRST3W\0"
    "SCND\0SCND1\0SCND1W\0SCND2\0SCND2W\0SCND3\0SCND3W\0THRD\0THRD1\0THRD1W\0THRD2\0THRD2W\0THRD3\0THRD3W\0FRTH\0FRTH0\0FRTHW\0FFTH" );
  return *this;
}
