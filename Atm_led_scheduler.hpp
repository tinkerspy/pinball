#pragma once

#include <Automaton.h>
#include "IO.hpp"

#define MAX_LEDS 5 * 256
#define MAX_PROFILES 16

struct led_meta_data {
  uint32_t rgbw; 
  uint8_t profile = 0;
  uint32_t last_millis; 
  uint8_t state;
};

struct led_profile_record {
  uint32_t delay_time;
  uint32_t rgbw1; 
  uint32_t rgbw1_time;
  uint32_t rgbw2;  // 0? immediate auto-off (single pulse) 
};

class Atm_led_scheduler: public Machine {

 public:
  enum { IDLE, WAITING, RUNNING, UPDATING }; // STATES
  enum { EVT_DONE, EVT_RUN, EVT_UPDATE, EVT_MILLI, ELSE }; // EVENTS
  enum { LED_STATE_IDLE, LED_STATE_DELAY, LED_STATE_RGBW1, LED_STATE_RGBW2 };
  enum { PROFILE_DEFAULT_COIL, PROFILE_DEFAULT_LED }; // Standard profiles
  
  Atm_led_scheduler( void ) : Machine() {};
  Atm_led_scheduler& begin( IO &io );
  Atm_led_scheduler& trace( Stream & stream );
  Atm_led_scheduler& trigger( int event );
  int state( void );
  Atm_led_scheduler& update( void );

  Atm_led_scheduler& on( int ledno );
  Atm_led_scheduler& toggle( int ledno, int v = -1 );
  Atm_led_scheduler& off( int ledno, bool no_update = false );
  Atm_led_scheduler& off( void );
  Atm_led_scheduler& set( int16_t n, uint32_t c );
  Atm_led_scheduler& defineProfile( uint8_t prof, uint16_t delay_time, uint32_t rgbw1, uint16_t rgbw1_time, uint32_t rgbw2 = 0 );
  Atm_led_scheduler& profile( int16_t n, uint8_t prof );
  
  int active( int ledno );
  uint8_t debug;
  Atm_led_scheduler& dump( Stream& stream );
  Atm_led_scheduler& dump_meta( Stream& stream );

// private:
  led_profile_record led_profile[MAX_PROFILES];
  enum { ENT_RUNNING, ENT_UPDATING, ENT_IDLE }; // ACTIONS
  int event( int id ); 
  void action( int id ); 
  led_meta_data meta[MAX_LEDS];
  int number_of_leds;
  uint8_t refresh, running;
  uint8_t last_milli;
  IO *io;
  
};
