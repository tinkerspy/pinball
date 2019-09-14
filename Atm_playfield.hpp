#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_element.hpp"
#include "Atm_led_device.hpp"
#include "Atm_led_scheduler.hpp"

#define MAX_SWITCHES 320
#define DYNAMIC_ELEMENTS
#define STARTUP_DELAY_MS 500

class Atm_element;
class Atm_led_device;

struct switch_record { 
    uint8_t switch_state : 1;
    uint8_t persistent : 1;
    uint8_t disabled : 1;
    uint8_t make_wait : 1;
    uint8_t break_delay;
    uint16_t make_delay; 
    uint16_t retrigger_delay;
    uint16_t last_change;
    bool initialized, device_initialized; 
#ifdef DYNAMIC_ELEMENTS    
    Atm_element *element;
    Atm_led_device *device;
#else
    Atm_element element;
    Atm_led_device device;
#endif    
};

class Atm_playfield: public Machine { // Beter: Atm_switch_zone

 public:
  enum { IDLE, WAIT, SCAN, DISABLED, READY }; 
  enum { EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_READY, ELSE }; // EVENTS
  Atm_playfield( void ) : Machine() {};
  Atm_playfield& begin( IO& io, Atm_led_scheduler& led );
  Atm_playfield& trace( Stream & stream );
  Atm_playfield& trigger( int event );
  int state( void );
  bool isPressed( int16_t n );
  Atm_playfield& onPress( int sub, Machine& machine, int event = 0 );
  Atm_playfield& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_playfield& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_playfield& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_playfield& debounce( uint8_t b, uint16_t r, uint16_t m );
  Atm_playfield& debounce( int16_t n, uint8_t b, uint16_t r, uint16_t m );  
  Atm_playfield& disable();
  Atm_playfield& enable();
  bool enabled();
  bool ready();
  Atm_element& element( int16_t n, int16_t coil_led = -1, int16_t light_led = -1, int8_t coil_profile = -1 , int8_t led_profile = -1 );
  Atm_led_device& device( int16_t n, int16_t led_group = -1, int16_t* device_script = NULL );
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
  IO *io;
};
