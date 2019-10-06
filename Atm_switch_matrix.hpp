#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_device.hpp"
#include "Atm_led_matrix.hpp"

#define MAX_SWITCHES 320
#define DYNAMIC_ELEMENTS
#define STARTUP_DELAY_MS 500

class Atm_device;

struct switch_record { 
    int8_t device_index; 
#ifdef DYNAMIC_ELEMENTS    
    Atm_device *device;
#else
    Atm_device device;
#endif    
};

class Atm_switch_matrix: public Machine { // Beter: Atm_switch_zone

 public:
  enum { IDLE, WAIT, SCAN, DISABLED, READY }; 
  enum { EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_READY, ELSE }; // EVENTS
  Atm_switch_matrix( void ) : Machine() {};
  Atm_switch_matrix& begin( IO& io, Atm_led_matrix& leds, int16_t* group_definition = NULL );
  Atm_switch_matrix& trace( Stream & stream );
  Atm_switch_matrix& trigger( int event );
  int state( void );
  bool isPressed( int16_t n );
  Atm_switch_matrix& onPress( int sub, Machine& machine, int event = 0 );
  Atm_switch_matrix& onPress( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_switch_matrix& onRelease( int sub, Machine& machine, int event = 0 );
  Atm_switch_matrix& onRelease( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_switch_matrix& disable();
  Atm_switch_matrix& enable();
  bool enabled();
  bool ready();
  Atm_device& device( int16_t n, int16_t led_group = -1, int16_t* device_script = NULL,
        int16_t r0 = 0, int16_t r1 = 0, int16_t r2 = 0, int16_t r3 = 0, int16_t r4 = 0, int16_t r5 = 0, int16_t r6 = 0, int16_t r7 = 0 );
  Atm_switch_matrix& profile( int16_t n, int16_t press_100us, int16_t release_100us, int16_t throttle_100us );
  Atm_switch_matrix& readProfiles( char label, const int16_t* profile_def );
  Atm_led_matrix* leds( void );
  int16_t numberOfGroups( void );

  switch_record prof[MAX_SWITCHES + 1 ];
 
 protected:
  enum { ENT_SCAN, ENT_DISABLED, ENT_READY }; // ACTIONS
  enum { ON_PRESS, ON_RELEASE = MAX_SWITCHES, CONN_MAX = MAX_SWITCHES * 2 }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  void scan_matrix( void );
  int16_t* parseGroups( int16_t* group_def );

  uint16_t numOfSwitches, numOfGroups;
  int16_t* group_def;
  bool active;
  int8_t scan_col = 0;
  Atm_led_matrix *pleds;
  uint16_t global_last_kick;
  atm_timer_millis timer;
  bool pf_enabled = false;
  IO *io;
};
