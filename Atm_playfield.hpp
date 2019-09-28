#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_element.hpp"
#include "Atm_led_scheduler.hpp"

#define MAX_SWITCHES 320
#define DYNAMIC_ELEMENTS
#define STARTUP_DELAY_MS 500

class Atm_element;

typedef int16_t ( *t_scan_callback )( void );

struct switch_record { 
    uint8_t persistent : 1;
    uint8_t disabled : 1;
    bool initialized; 
#ifdef DYNAMIC_ELEMENTS    
    Atm_element *element;
#else
    Atm_element element;
#endif    
};

class Atm_playfield: public Machine { // Beter: Atm_switch_zone

 public:
  enum { IDLE, WAIT, SCAN, DISABLED, READY }; 
  enum { EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_READY, ELSE }; // EVENTS
  Atm_playfield( void ) : Machine() {};
  Atm_playfield& begin( IO& io, t_scan_callback scan, Atm_led_scheduler& led );
  Atm_playfield& trace( Stream & stream );
  Atm_playfield& trigger( int event );
  bool isPressed( int16_t n );
  int state( void );
  Atm_playfield& onPress( int sub, Machine& machine, int event = 0 );
  Atm_playfield& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_playfield& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_playfield& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_playfield& disable();
  Atm_playfield& enable();
  bool enabled();
  bool ready();
  Atm_element& element( int16_t n, int16_t coil_led = -1, int16_t light_led = -1, int8_t coil_profile = -1 , int8_t led_profile = -1 );
  Atm_element& watch( int16_t light_led, int16_t cnt = -1 );
  Atm_element& led( int16_t light_led, int8_t led_profile = -1 );
  Atm_led_scheduler& leds();
  Atm_playfield& defineProfile( uint8_t prof, uint16_t T0, uint32_t L1, uint16_t T1, uint32_t L2 = 0 );

  switch_record prof[MAX_SWITCHES + 1 ];

 protected:
  enum { ENT_SCAN, ENT_DISABLED, ENT_READY }; // ACTIONS
  enum { ON_PRESS, ON_RELEASE = MAX_SWITCHES, CONN_MAX = MAX_SWITCHES * 2 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void scan_matrix( void );
  void switch_changed( int16_t n, uint8_t v );
  
  int no_of_rows, no_of_cols;
  int8_t* cols;
  int8_t* rows;
  bool active;
  int8_t scan_col = 0;
  Atm_led_scheduler *pleds;
  uint16_t global_last_kick;
  atm_timer_millis timer;
  bool pf_enabled = false;
  t_scan_callback io_scan;
  IO* io;
};
