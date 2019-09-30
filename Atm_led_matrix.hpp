#pragma once

#include <Automaton.h>
#include "IO.hpp"

#define MAX_LEDS 320 // Physical + Logical
#define MAX_PROFILES 32
#define MAX_GROUPS 32 // Logical groups
#define MAX_WATCHERS 16 

struct led_meta_data {
  uint32_t rgbw; 
  uint8_t profile = 0;
  uint32_t last_millis; 
  uint8_t state;
  uint16_t watch = 0;
};

struct led_watcher {
  Machine* machine;
  int16_t event;
};

/*
 * Scheduler functions:
 * - led envelope timing (wait duration, pulse level, pulse duration, hold level) per led
 * - led logical groups
 * - led state keeping: active()
 * - analog lamp simulation (fade in/out) TODO
 * 
 * T0 Waiting period before led is switched on (ms)
 * L1 Initial led RGBW level
 * T1 Pulse length (period L1 is applied in ms)
 * L2 Hold level (when hold level zero, led is in pulse mode: off() is ignored)
 */

struct led_profile_record {
  uint32_t T0, L1, T1, L2;
};

class Atm_led_matrix: public Machine {

 public:
  enum { IDLE, WAITING, RUNNING, UPDATING }; // STATES
  enum { EVT_DONE, EVT_RUN, EVT_UPDATE, EVT_MILLI, ELSE }; // EVENTS
  enum { LED_STATE_IDLE, LED_STATE_DELAY, LED_STATE_RGBW1, LED_STATE_RGBW2 };
  enum { PROFILE_COIL, PROFILE_LED }; // Standard profiles
  
  Atm_led_matrix( void ) : Machine() {};
  Atm_led_matrix& begin( IO &io, const int16_t* group_definition, const int16_t* profile_definition );
  Atm_led_matrix& trace( Stream & stream );
  Atm_led_matrix& trigger( int event );
  int state( void );
  Atm_led_matrix& update( void );

  Atm_led_matrix& on( int ledno, bool no_update = false ); 
  Atm_led_matrix& scalar( int16_t ledno, int8_t index, bool fill = false ); 
  int16_t scalar( int16_t ledno ); 
  Atm_led_matrix& toggle( int ledno, int v = -1 ); 
  Atm_led_matrix& off( int ledno, bool no_update = false ); 
  Atm_led_matrix& off( void );
  Atm_led_matrix& set( int16_t ledno, uint32_t c ); 
  Atm_led_matrix& defineProfile( uint8_t prof, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 = 0 );
  Atm_led_matrix& profile( int16_t ledno, uint8_t prof );
  const int16_t* group( int16_t gid );
  int active( int ledno );
  Atm_led_matrix& onWatch( int16_t ledno, Machine& machine, int16_t event );
  Atm_led_matrix& onWatch( int16_t ledno, Machine* machine, int16_t event );
  Atm_led_matrix& led_register( int16_t ledno, uint8_t idx );
  int16_t count( int16_t ledno, int8_t led_active = -1 );
  int16_t index( int16_t led_group, int16_t idx );
  
  uint8_t debug;
  Atm_led_matrix& dump( Stream& stream );
  Atm_led_matrix& dump_meta( Stream& stream );

protected:
  Atm_led_matrix& parseGroups( const int16_t* group_def ); 
  Atm_led_matrix& parseProfiles( const int16_t* profile_def ); 
  Atm_led_matrix& group_set( int16_t ledno, uint32_t c ); 
  Atm_led_matrix& group_on( int ledno );
  Atm_led_matrix& group_off( int ledno ); 
  Atm_led_matrix& group_toggle( int ledno, int v = -1 );
  Atm_led_matrix& group_profile( int16_t ledno, uint8_t prof );
  int group_active( int ledno );
  Atm_led_matrix& led_notify( int16_t ledno );
  led_profile_record led_profile[MAX_PROFILES];
  enum { ENT_RUNNING, ENT_UPDATING, ENT_IDLE }; // ACTIONS
  int event( int id ); 
  void action( int id ); 
  led_meta_data meta[MAX_LEDS];
  int number_of_leds;
  uint8_t refresh, running;
  uint8_t last_milli;
  IO *io;
  const int16_t* led_group[MAX_GROUPS];
  led_watcher watchers[MAX_WATCHERS];
  uint8_t watcher_cnt = 0;
  
};
