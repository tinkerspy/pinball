#pragma once

#include <Automaton.h>
#include "Symbolic_Machine.hpp"
#include "IO.hpp"

#define MAX_LEDS 320 // Physical leds

struct led_meta_data {
  uint32_t rgbw; 
  uint32_t last_millis; 
  uint8_t state;
  uint16_t watch = 0;
  uint32_t T0, L1, T1, L2;
};

struct led_watcher {
  Machine* machine;
  int16_t event;
};

/*
 * Scheduler functions:
 * 
 * T0 Waiting period before led is switched on (ms)
 * L1 Initial led RGBW level
 * T1 Pulse length (period L1 is applied in ms)
 * L2 Hold level (when hold level zero, led is in pulse mode: off() is ignored)
 */

class Atm_led_matrix: public Symbolic_Machine {

 public:
  enum { IDLE, WAITING, RUNNING, UPDATING }; // STATES
  enum { EVT_DONE, EVT_RUN, EVT_UPDATE, EVT_MILLI, ELSE }; // EVENTS
  enum { LED_STATE_IDLE, LED_STATE_DELAY, LED_STATE_RGBW1, LED_STATE_RGBW2 };
  enum { PROFILE_COIL, PROFILE_LED }; // Standard profiles
  
  Atm_led_matrix( void ) : Symbolic_Machine() {};
  Atm_led_matrix& begin( IO &io );
  Atm_led_matrix& trace( Stream & stream );
  Atm_led_matrix& trace();
  Atm_led_matrix& trigger( int event );
  int state( void );
  Atm_led_matrix& update( void );

  Atm_led_matrix& on( int ledno, bool no_update = false ); 
  Atm_led_matrix& toggle( int ledno, int v = -1 ); 
  Atm_led_matrix& off( int ledno, bool no_update = false ); 
  Atm_led_matrix& off( void );
  Atm_led_matrix& set( int16_t ledno, uint32_t c ); 
  Atm_led_matrix& readProfiles(  char label, const int16_t* profile_def );
  Atm_led_matrix& profile( int16_t ledno, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 = 0  );
  Atm_led_matrix& profile( const char* led_group_str, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 = 0  );

  int active( int ledno );
  int16_t count( int16_t ledno, int8_t led_active = -1 );
  int16_t index( int16_t led_group, int16_t idx );
  int16_t numberOfGroups( void );
  
  uint8_t debug;
  Atm_led_matrix& dump( Stream& stream );
  Atm_led_matrix& dump_meta( Stream& stream );
  Atm_led_matrix& loadSymbols( const char leds[] );
  Atm_led_matrix& loadGroups( const char groups[] );

protected:
  int16_t* parseGroups( int16_t* group_def );
  enum { ENT_RUNNING, ENT_UPDATING, ENT_IDLE }; // ACTIONS
  int event( int id ); 
  void action( int id ); 
  led_meta_data meta[MAX_LEDS];
  int numOfLeds, numOfGroups;
  uint8_t refresh, running;
  uint8_t last_milli;
  IO *io;
  int16_t* group_def;
  
};
