#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"


typedef char oxo_wins_t[8][3];

class Atm_oxo_field: public Machine {

 public:
  enum { IDLE, OXO_1X, OXO_1O, OXO_2X, OXO_2O, OXO_3X, OXO_3O, OXO_4, OXO_5, OXO_6, OXO_7, OXO_8, OXO_9, MATCH, INIT, TOGGLE }; // STATES
  enum { EVT_MATCH, EVT_1X, EVT_1O, EVT_2X, EVT_2O, EVT_3X, EVT_3O, EVT_4, EVT_5, EVT_6, EVT_7, EVT_8, EVT_9, EVT_INIT, EVT_TOGGLE, ELSE }; // EVENTS
  enum { PROFILE_COIL, PROFILE_LED }; // Standard profiles
  Atm_oxo_field( void ) : Machine() {};
  Atm_oxo_field& begin( Atm_led_scheduler& led, int16_t group_id, uint8_t oxo_profile = PROFILE_LED, uint8_t led_profile = PROFILE_LED );
  Atm_oxo_field& trace( Stream & stream );
  Atm_oxo_field& trigger( int event );
  int state( void );
  Atm_oxo_field& onMatch( Machine& machine, int event = 0 );
  Atm_oxo_field& onMatch( atm_cb_push_t callback, int idx = 0 );
  Atm_oxo_field& onSet( Machine& machine, int event = 0 );
  Atm_oxo_field& onSet( atm_cb_push_t callback, int idx = 0 );
  Atm_oxo_field& onInit( Machine& machine, int event = 0 );
  Atm_oxo_field& onInit( atm_cb_push_t callback, int idx = 0 );
  Atm_oxo_field& oxo_1x( void );
  Atm_oxo_field& oxo_1o( void );
  Atm_oxo_field& oxo_2x( void );
  Atm_oxo_field& oxo_2o( void );
  Atm_oxo_field& oxo_3x( void );
  Atm_oxo_field& oxo_3o( void );
  Atm_oxo_field& oxo_4( void );
  Atm_oxo_field& oxo_5( void );
  Atm_oxo_field& oxo_6( void );
  Atm_oxo_field& oxo_7( void );
  Atm_oxo_field& oxo_8( void );
  Atm_oxo_field& oxo_9( void );
  Atm_oxo_field& init( void );
  Atm_oxo_field& toggle( void );
  char set( int cell );
  void set( int cell, char v );
  char cell( int no );
  void set_to_active( int cell );
  Atm_oxo_field& dump( Stream & stream );
  Atm_oxo_field& select( char v );
  Atm_oxo_field& loadWins( oxo_wins_t &wins );
  Atm_oxo_field& dumpWins( Stream &stream, oxo_wins_t &wins, int idx = -1 );


 private:
  enum { ENT_1X, ENT_1O, ENT_2X, ENT_2O, ENT_3X, ENT_3O, ENT_4, ENT_5, ENT_6, ENT_7, ENT_8, ENT_9, ENT_MATCH, ENT_INIT, ENT_TOGGLE }; // ACTIONS
  enum { ON_MATCH, ON_SET, ON_INIT, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  Atm_led_scheduler *led;
  int16_t *led_map;
};
