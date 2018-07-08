#pragma once

#include <Automaton.h>
#include "Atm_widget_oxo.hpp"

class Atm_oxo_bot: public Machine {

 public:
  enum { RANDOM_BOT, STUPID_BOT, SMART_BOT };
  enum { IDLE, WAIT, MOVE }; // STATES
  enum { EVT_TIMER, EVT_MOVE, ELSE }; // EVENTS
  Atm_oxo_bot( void ) : Machine() {};
  Atm_oxo_bot& begin( char piece, uint32_t delay, int personality = SMART_BOT );
  Atm_oxo_bot& personality( int p );
  Atm_oxo_bot& trace( Stream & stream );
  Atm_oxo_bot& trigger( int event );
  int state( void );
  Atm_oxo_bot& onMove( Machine& machine, int event = 0 );
  Atm_oxo_bot& onMove( atm_cb_push_t callback, int idx = 0 );
  Atm_oxo_bot& move( oxo_wins_t &wins );

 private:
  enum { ENT_MOVE }; // ACTIONS
  enum { ON_MOVE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id );
  int randomBit( uint16_t v );
  uint16_t availableMoves( oxo_wins_t &wins );
  uint16_t availableWins( char piece, oxo_wins_t &wins );
  int bitCount( uint16_t v );
  int findWin( char piece, oxo_wins_t &wins );
  uint16_t checkWins( oxo_wins_t &wins, char piece, int move );
  int randomBot( char piece, oxo_wins_t &wins );
  int stupidBot( char piece, oxo_wins_t &wins );
  int smartBot( char piece, oxo_wins_t &wins );
  char winner( oxo_wins_t &wins );
 
  char piece;
  int bot_personality;
  oxo_wins_t wins;
  atm_timer_millis timer;

};

/* 
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_oxo_bot">
    <states>
      <IDLE index="0" sleep="1">
        <EVT_MOVE>WAIT</EVT_MOVE>
      </IDLE>
      <WAIT index="1">
        <EVT_TIMER>MOVE</EVT_TIMER>
      </WAIT>
      <MOVE index="2" on_enter="ENT_MOVE">
        <ELSE>IDLE</ELSE>
      </MOVE>
    </states>
    <events>
      <EVT_TIMER index="0" access="PRIVATE"/>
      <EVT_MOVE index="1" access="MIXED"/>
    </events>
    <connectors>
      <MOVE autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end 
*/
