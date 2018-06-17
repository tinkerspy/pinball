#pragma once

#include <Automaton.h>
#include "Atm_apa102.hpp"

class Atm_element: public Machine {

 public:
  enum { IDLE, DELAY, KICKING, DISABLED, INIT, INPUTTING, RELEASE, LIGHT_ON, LIGHT_OFF, TOGGLE }; // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_KICK, EVT_RELEASE, EVT_INPUT, EVT_INIT, EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_LIT, ELSE }; // EVENTS
  Atm_element( void ) : Machine() {};
  Atm_element& begin( Atm_apa102& led, int light, int coil, int pulse_time = -1, int delay = 0 );
  Atm_element& trace( Stream & stream );
  Atm_element& trigger( int event );
  int state( void );
  Atm_element& onInit( Machine& machine, int event = 0 );
  Atm_element& onInit( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onInput( Machine& machine, int event = 0 );
  Atm_element& onInput( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onInput( int sub, Machine& machine, int event = 0 );
  Atm_element& onInput( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& onKick( Machine& machine, int event = 0 );
  Atm_element& onKick( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onKick( int sub, Machine& machine, int event = 0 );
  Atm_element& onKick( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& onLight( Machine& machine, int event = 0 );
  Atm_element& onLight( atm_cb_push_t callback, int idx = 0 );
  Atm_element& onLight( int sub, Machine& machine, int event = 0 );
  Atm_element& onLight( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_element& on( void );
  Atm_element& off( void );
  Atm_element& kick( void );
  Atm_element& release( void );
  Atm_element& input( void );
  Atm_element& init( void );
  Atm_element& disable( void );
  Atm_element& enable( void );
  Atm_element& toggle( void );
  Atm_element& autoLite( int v = 1 );

 private:
  enum { ENT_KICKING, ENT_INIT, ENT_INPUT, ENT_RELEASE, ENT_LIGHT_ON, ENT_LIGHT_OFF }; // ACTIONS
  enum { ON_INIT, ON_INPUT, ON_KICK = 4, ON_LIGHT = 7, CONN_MAX = 10 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  atm_timer_millis timer;
  int light_led, coil_led, autolite;
  uint16_t pulse_time;
  Atm_apa102 *led;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_element">
    <states>
      <IDLE index="0" sleep="1">
        <EVT_ON>LIGHT_ON</EVT_ON>
        <EVT_OFF>LIGHT_OFF</EVT_OFF>
        <EVT_KICK>DELAY</EVT_KICK>
        <EVT_RELEASE>RELEASE</EVT_RELEASE>
        <EVT_INPUT>INPUT</EVT_INPUT>
        <EVT_INIT>INIT</EVT_INIT>
        <EVT_DISABLE>DISABLED</EVT_DISABLE>
      </IDLE>
      <DELAY index="1">
        <EVT_RELEASE>IDLE</EVT_RELEASE>
        <EVT_TIMER>KICKING</EVT_TIMER>
      </DELAY>
      <KICKING index="2" on_enter="ENT_KICKING">
        <EVT_TIMER>IDLE</EVT_TIMER>
      </KICKING>
      <DISABLED index="3" sleep="1">
        <EVT_ENABLE>IDLE</EVT_ENABLE>
      </DISABLED>
      <INIT index="4" sleep="1" on_enter="ENT_INIT">
        <ELSE>IDLE</ELSE>
      </INIT>
      <INPUT index="5" on_enter="ENT_INPUT">
        <ELSE>IDLE</ELSE>
      </INPUT>
      <RELEASE index="6" on_enter="ENT_RELEASE">
        <ELSE>IDLE</ELSE>
      </RELEASE>
      <LIGHT_ON index="7" on_enter="ENT_LIGHT_ON">
        <ELSE>IDLE</ELSE>
      </LIGHT_ON>
      <LIGHT_OFF index="8" on_enter="ENT_LIGHT_OFF">
        <ELSE>IDLE</ELSE>
      </LIGHT_OFF>
    </states>
    <events>
      <EVT_ON index="0" access="PUBLIC"/>
      <EVT_OFF index="1" access="PUBLIC"/>
      <EVT_KICK index="2" access="PUBLIC"/>
      <EVT_RELEASE index="3" access="PUBLIC"/>
      <EVT_INPUT index="4" access="PUBLIC"/>
      <EVT_INIT index="5" access="PUBLIC"/>
      <EVT_DISABLE index="6" access="PUBLIC"/>
      <EVT_ENABLE index="7" access="PUBLIC"/>
      <EVT_TIMER index="8" access="PRIVATE"/>
    </events>
    <connectors>
      <INIT autostore="0" broadcast="0" dir="PUSH" slots="3"/>
      <INPUT autostore="0" broadcast="0" dir="PUSH" slots="3"/>
      <KICK autostore="0" broadcast="0" dir="PUSH" slots="3"/>
      <LIGHT autostore="0" broadcast="0" dir="PUSH" slots="3"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

