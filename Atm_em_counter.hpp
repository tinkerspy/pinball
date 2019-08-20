#pragma once

#include <Automaton.h>
#include "Atm_led_scheduler.hpp"
#include "Atm_playfield.hpp"

#define DIGIT_DELAY_MS 120
#define RECURSIVE true

/*  

 4 + 1 digit electromagnetic counter with 4 coils, uses three 'nine' contacts and one 'zero' wired in parallel/series for feedback.
    
0|1|2|3|X  <-  Reel numbering (X=dummy reel)
  0: 10000's
  1: 1000's
  2: 100's
  3: 10's

 Switch wiring, c0 is normally closed, c1..c3 are normally open:
 
                              /-> c1 (9) >-\  
 matrix strobe -> c0 (!0) >==<--> c2 (9) >-->==> matrix input line
                              \-> c3 (9) >-/ 

 Worst case: 10000 (49 pulses), best case: 99880 (6 pulses)

 WARNING: For the 'switch_sensor' to operate some simultaneously running process must be actively                    
 polling the switch matrix to refresh the buffer. Usually this is done by the Atm_playfield object.
*/

class Atm_em_counter: public Machine {

 public:
  enum { IDLE, CHECK, DIG0, DIG1, DIG2, DIG3, ZERO, RESET, PRE0, PRE0W, PRE1, PRE1W, FRST, FRST1, FRST1W, FRST2, FRST2W, FRST3, FRST3W, 
    SCND, SCND1, SCND1W, SCND2, SCND2W, SCND3, SCND3W, THRD, THRD1, THRD1W, THRD2, THRD2W, THRD3, THRD3W, FRTH, FRTH0, FRTHW, FFTH }; // STATES
  enum { EVT_CLEAN, EVT_LO, EVT_HI, EVT_DIG3, EVT_DIG2, EVT_DIG1, EVT_DIG0, EVT_RESET, EVT_ZERO, EVT_TIMER, EVT_CHANGE, ELSE, EVT_10, EVT_100, EVT_500, EVT_1000, EVT_5000 }; // EVENTS
  Atm_em_counter( void ) : Machine() {};
  Atm_em_counter& begin( Atm_playfield& playfield, int16_t sensor_switch,  int16_t led_group_id, int profile );
  Atm_em_counter& trace( Stream & stream );
  Atm_em_counter& trigger( int event );
  int state( void );
  Atm_em_counter& onDigit( int sub, Machine& machine, int event = 0 );
  Atm_em_counter& onDigit( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_em_counter& reset( void );
  Atm_em_counter& zero( void );
  uint16_t value( void );
  Atm_em_counter& set( uint16_t v );
  Atm_em_counter& add( int16_t v );  

 private:
  enum { ENT_DIG0, ENT_DIG1, ENT_DIG2, ENT_DIG3, ENT_ZERO, ENT_RESET, ENT_PULS0, ENT_PULS1, ENT_PULS2, ENT_PULS3, ENT_FRST, ENT_SCND, ENT_THRD, ENT_FRTH, ENT_FFTH }; // ACTIONS
  enum { ON_DIGIT, CONN_MAX = 3 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  Atm_em_counter& pulse( int8_t reel, uint8_t force = 0, uint8_t recursive = 0 );
  int sensor( void ); 

  Atm_playfield* playfield;
  int coil[4];
  int current_value;
  uint8_t ist[4], soll[4];
  int16_t sensor_switch;
  uint8_t last_pulse;
  uint8_t solved[4];
  bool resetting, touched;

  atm_timer_millis timer;
    
};
