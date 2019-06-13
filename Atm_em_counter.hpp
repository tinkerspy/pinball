#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

/*  
0|1|2|3|X  <-  Reel numbering (X=dummy reel)
  0: 10000's
  1: 1000's
  2: 100's
  3: 10's

 Switch wiring, c0 is normally closed, c1..c3 are normally open:
                    /-> c1 (9) >-\  
 GND -> c0 (!0) >==<--> c2 (9) >-->==> input (pullup)
                    \-> c3 (9) >-/ 

 Worst case: 10000 (49 pulses), best case: 99880 (6 pulses)

 WARNING: For the 'switch_sensor' to operate some simultaneously running process must be actively                    
 polling the switch matrix to refresh the buffer. Usually this is done by the Atm_playfield object.
*/

class Atm_em_counter: public Machine {

 public:
  enum { IDLE, CHECK, DIG0, DIG1, DIG2, DIG3, ZERO, RESET, PRE0, PRE0W, PRE1, PRE1W, FRST, FRST1, FRST1W, FRST2, FRST2W, FRST3, FRST3W, 
    SCND, SCND1, SCND1W, SCND2, SCND2W, SCND3, SCND3W, THRD, THRD1, THRD1W, THRD2, THRD2W, THRD3, THRD3W, FRTH, FRTH0, FRTHW, FFTH }; // STATES
  enum { EVT_LO, EVT_HI, EVT_DIG3, EVT_DIG2, EVT_DIG1, EVT_DIG0, EVT_RESET, EVT_ZERO, EVT_TIMER, EVT_CHANGE, ELSE, EVT_10, EVT_100, EVT_500, EVT_1000, EVT_5000 }; // EVENTS
  Atm_em_counter( void ) : Machine() {};
  Atm_em_counter& begin( Atm_playfield& playfield, int16_t sensor_switch,  int16_t group_id, int profile );
  Atm_em_counter& trace( Stream & stream );
  Atm_em_counter& trigger( int event );
  int state( void );
  Atm_em_counter& onScore( Machine& machine, int event = 0 ); // This is actually onPulse() (ENT_DIG0..ENT_DIG3) 
  Atm_em_counter& onScore( atm_cb_push_t callback, int idx = 0 );
  Atm_em_counter& onScore( int sub, Machine& machine, int event = 0 );
  Atm_em_counter& onScore( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_em_counter& reset( void );
  Atm_em_counter& zero( void );
  Atm_em_counter& set( uint16_t v );
  Atm_em_counter& set_hw( uint16_t v ); // mag later weg
  Atm_em_counter& add( int16_t v );
  Atm_em_counter& dump_soll( Stream & stream ); // Mag later weg
  Atm_em_counter& dump_ist( Stream & stream, int plus = 1 ); // Mag later weg
  


 private:
  enum { ENT_DIG0, ENT_DIG1, ENT_DIG2, ENT_DIG3, ENT_ZERO, ENT_RESET, ENT_PULS0, ENT_PULS1, ENT_PULS2, ENT_PULS3, ENT_FRST, ENT_SCND, ENT_THRD, ENT_FRTH, ENT_FFTH }; // ACTIONS
  enum { ON_SCORE, CONN_MAX = 3 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  Atm_em_counter& pulse( uint8_t reel, uint8_t force = 0 );
  int sensor( void ); // Sensor (simulation)

  Atm_playfield* playfield;
  int coil[4];
  int value;
  uint8_t ist[4], soll[4];
  int16_t sensor_switch;
  uint8_t last_pulse;
  uint8_t solved[4];

  atm_timer_millis timer;
    
};

/* 
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_em_counter">
    <states>
      <IDLE index="0" sleep="1">
        <EVT_RESET>RESET</EVT_RESET>
        <EVT_ZERO>ZERO</EVT_ZERO>
      </IDLE>
      <CHECK index="1">
        <EVT_DIG0>DIG0</EVT_DIG0>
        <EVT_DIG1>DIG1</EVT_DIG1>
        <EVT_DIG2>DIG2</EVT_DIG2>
        <EVT_DIG3>DIG3</EVT_DIG3>
        <ELSE>IDLE</ELSE>
      </CHECK>
      <DIG0 index="2" on_enter="ENT_DIG0">
        <EVT_TIMER>CHECK</EVT_TIMER>
      </DIG0>
      <DIG1 index="3" on_enter="ENT_DIG1">
        <EVT_TIMER>CHECK</EVT_TIMER>
      </DIG1>
      <DIG2 index="4" on_enter="ENT_DIG2">
        <EVT_TIMER>CHECK</EVT_TIMER>
      </DIG2>
      <DIG3 index="5" on_enter="ENT_DIG3">
        <EVT_TIMER>CHECK</EVT_TIMER>
      </DIG3>
      <RESET index="6">
      </RESET>
      <ZERO index="7">
      </ZERO>
    </states>
    <events>
      <EVT_LO index="0" access="PRIVATE"/>
      <EVT_HI index="1" access="PRIVATE"/>
      <EVT_DIG0 index="2" access="PRIVATE"/>
      <EVT_DIG1 index="3" access="PRIVATE"/>
      <EVT_DIG2 index="4" access="PRIVATE"/>
      <EVT_DIG3 index="5" access="PRIVATE"/>
      <EVT_RESET index="6" access="MIXED"/>
      <EVT_ZERO index="7" access="MIXED"/>
      <EVT_TIMER index="8" access="PRIVATE"/>
    </events>
    <connectors>
      <SCORE autostore="0" broadcast="0" dir="PUSH" slots="3"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end 
*/
