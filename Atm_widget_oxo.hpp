#pragma once

#include <Automaton.h>
#include "Atm_apa102.hpp"


typedef char oxo_wins_t[8][3];

class Atm_widget_oxo: public Machine {

 public:
  enum { IDLE, OXO_1X, OXO_1O, OXO_2X, OXO_2O, OXO_3X, OXO_3O, OXO_4, OXO_5, OXO_6, OXO_7, OXO_8, OXO_9, MATCH, INIT, TOGGLE }; // STATES
  enum { EVT_MATCH, EVT_1X, EVT_1O, EVT_2X, EVT_2O, EVT_3X, EVT_3O, EVT_4, EVT_5, EVT_6, EVT_7, EVT_8, EVT_9, EVT_INIT, EVT_TOGGLE, ELSE }; // EVENTS
  Atm_widget_oxo( void ) : Machine() {};
  Atm_widget_oxo& begin( Atm_apa102& led, int8_t* leds );
  Atm_widget_oxo& trace( Stream & stream );
  Atm_widget_oxo& trigger( int event );
  int state( void );
  Atm_widget_oxo& onMatch( Machine& machine, int event = 0 );
  Atm_widget_oxo& onMatch( atm_cb_push_t callback, int idx = 0 );
  Atm_widget_oxo& onSet( Machine& machine, int event = 0 );
  Atm_widget_oxo& onSet( atm_cb_push_t callback, int idx = 0 );
  Atm_widget_oxo& onInit( Machine& machine, int event = 0 );
  Atm_widget_oxo& onInit( atm_cb_push_t callback, int idx = 0 );
  Atm_widget_oxo& oxo_1x( void );
  Atm_widget_oxo& oxo_1o( void );
  Atm_widget_oxo& oxo_2x( void );
  Atm_widget_oxo& oxo_2o( void );
  Atm_widget_oxo& oxo_3x( void );
  Atm_widget_oxo& oxo_3o( void );
  Atm_widget_oxo& oxo_4( void );
  Atm_widget_oxo& oxo_5( void );
  Atm_widget_oxo& oxo_6( void );
  Atm_widget_oxo& oxo_7( void );
  Atm_widget_oxo& oxo_8( void );
  Atm_widget_oxo& oxo_9( void );
  Atm_widget_oxo& init( void );
  Atm_widget_oxo& toggle( void );
  char set( int cell );
  void set( int cell, char v );
  char cell( int no );
  void set_to_active( int cell );
  Atm_widget_oxo& dump( Stream & stream );
  Atm_widget_oxo& select( char v );
  Atm_widget_oxo& loadWins( oxo_wins_t &wins );
  Atm_widget_oxo& dumpWins( Stream &stream, oxo_wins_t &wins, int idx = -1 );


 private:
  enum { ENT_1X, ENT_1O, ENT_2X, ENT_2O, ENT_3X, ENT_3O, ENT_4, ENT_5, ENT_6, ENT_7, ENT_8, ENT_9, ENT_MATCH, ENT_INIT, ENT_TOGGLE }; // ACTIONS
  enum { ON_MATCH, ON_SET, ON_INIT, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  Atm_apa102 *led;
  int8_t *led_map;
};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_widget_oxo">
    <states>
      <IDLE index="0" sleep="1">
        <EVT_1X>OXO_1X</EVT_1X>
        <EVT_1O>OXO_1O</EVT_1O>
        <EVT_2X>OXO_2X</EVT_2X>
        <EVT_2O>OXO_2O</EVT_2O>
        <EVT_3X>OXO_3X</EVT_3X>
        <EVT_3O>OXO_3O</EVT_3O>
        <EVT_4>OXO_4</EVT_4>
        <EVT_5>OXO_5</EVT_5>
        <EVT_6>OXO_6</EVT_6>
        <EVT_7>OXO_7</EVT_7>
        <EVT_8>OXO_8</EVT_8>
        <EVT_9>OXO_9</EVT_9>
      </IDLE>
      <OXO_1X index="1" on_enter="ENT_1X">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_1X>
      <OXO_1O index="2" on_enter="ENT_1O">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_1O>
      <OXO_2X index="3" on_enter="ENT_2X">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_2X>
      <OXO_2O index="4" on_enter="ENT_2O">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_2O>
      <OXO_3X index="5" on_enter="ENT_3X">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_3X>
      <OXO_3O index="6" on_enter="ENT_3O">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_3O>
      <OXO_4 index="7" on_enter="ENT_4">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_4>
      <OXO_5 index="8" on_enter="ENT_5">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_5>
      <OXO_6 index="9" on_enter="ENT_6">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_6>
      <OXO_7 index="10" on_enter="ENT_7">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_7>
      <OXO_8 index="11" on_enter="ENT_8">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_8>
      <OXO_9 index="12" on_enter="ENT_ 9">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </OXO_9>
      <MATCH index="13" on_enter="ENT_MATCH">
        <EVT_MATCH>MATCH</EVT_MATCH>
        <ELSE>IDLE</ELSE>
      </MATCH>
    </states>
    <events>
      <EVT_MATCH index="0" access="PRIVATE"/>
      <EVT_1X index="1" access="PUBLIC"/>
      <EVT_1O index="2" access="PUBLIC"/>
      <EVT_2X index="3" access="PUBLIC"/>
      <EVT_2O index="4" access="PUBLIC"/>
      <EVT_3X index="5" access="PUBLIC"/>
      <EVT_3O index="6" access="PUBLIC"/>
      <EVT_4 index="7" access="PUBLIC"/>
      <EVT_5 index="8" access="PUBLIC"/>
      <EVT_6 index="9" access="PUBLIC"/>
      <EVT_7 index="10" access="PUBLIC"/>
      <EVT_8 index="11" access="PUBLIC"/>
      <EVT_9 index="12" access="PUBLIC"/>
    </events>
    <connectors>
      <MATCH autostore="0" broadcast="0" dir="PUSH" slots="1"/>
      <SET autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

