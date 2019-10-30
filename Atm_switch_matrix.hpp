#pragma once

// EVT_START toevoegen om in stijl te blijven met Atm_timer e.d.???

#include <Automaton.h>
#include "Atm_device.hpp"
#include "Atm_led_matrix.hpp"
#include "Symbolic_Machine.hpp"

#define MAX_SWITCHES 320
#define DYNAMIC_ELEMENTS
#define STARTUP_DELAY_MS 500

#define ATMSM_TRACE_PRESS 1
#define ATMSM_TRACE_RELEASE 2

class Atm_device;

struct switch_record { 
    int8_t device_index; 
#ifdef DYNAMIC_ELEMENTS    
    Atm_device *device;
#else
    Atm_device device;
#endif    
};

class Atm_switch_matrix: public Symbolic_Machine { 

 public:
  enum { IDLE, WAIT, SCAN, DISABLED, READY, INIT }; 
  enum { EVT_DISABLE, EVT_ENABLE, EVT_TIMER, EVT_READY, EVT_INIT, ELSE }; // EVENTS
  Atm_switch_matrix( void ) : Symbolic_Machine() {};
  Atm_switch_matrix& begin( IO& io, Atm_led_matrix& leds, int16_t* group_definition = NULL, int16_t status_led = -1 );
  Atm_switch_matrix& trace( Stream & stream );
  Atm_switch_matrix& trace( void );
  Atm_switch_matrix& traceSwitches( Stream & stream, uint8_t bitmap = 1 );
  Atm_switch_matrix& traceSwitches( Stream* stream, uint8_t bitmap = 1 );
  Atm_switch_matrix& trigger( int event );
  int16_t index( int16_t swno, int16_t n );
  int16_t exists( int16_t n );
  int state( void );
  bool isPressed( int16_t n );
  Atm_switch_matrix& disable();
  Atm_switch_matrix& enable();
  bool enabled();
  bool ready();
  Atm_device& device( int16_t n, int16_t led_group = -1, int16_t* device_script = NULL,
        int16_t r0 = 0, int16_t r1 = 0, int16_t r2 = 0, int16_t r3 = 0, int16_t r4 = 0, int16_t r5 = 0, int16_t r6 = 0, int16_t r7 = 0 );
  Atm_device& device( const char dev_str[], int16_t led_group = -1, int16_t* device_script = NULL,
        int16_t r0 = 0, int16_t r1 = 0, int16_t r2 = 0, int16_t r3 = 0, int16_t r4 = 0, int16_t r5 = 0, int16_t r6 = 0, int16_t r7 = 0 );
  Atm_device& device( const char dev_str[], const char led_group_str[], int64_t code_pack,
        int16_t r0 = 0, int16_t r1 = 0, int16_t r2 = 0, int16_t r3 = 0, int16_t r4 = 0, int16_t r5 = 0, int16_t r6 = 0, int16_t r7 = 0 );
  Atm_switch_matrix& profile( int16_t n, int16_t press_ticks, int16_t release_ticks, int16_t throttle_ticks, int16_t separate_ticks );
  Atm_switch_matrix& readProfiles( char label, const int16_t* profile_def );
  Atm_device& link( const char src_str[], const char out_str[], const char dest_str[], const char in_str[] );
  Atm_device& link( const char src_str[], const char out_str[], Machine& dest, int16_t dest_input );

  Atm_led_matrix* leds( void );
  int16_t numberOfGroups( void );
  Atm_switch_matrix& loadSymbols( const char s[] );
  int64_t* codePack( int16_t idx );

  switch_record prof[MAX_SWITCHES + 1 ];
 
 protected:
  enum { ENT_SCAN, ENT_DISABLED, ENT_READY, ENT_INIT }; // ACTIONS
  char event_symbols[48] = "PF_DISABLE,PF_ENABLE,PF_TIMER,PF_READY,PF_INIT\n";
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
  int16_t status_led;
  uint8_t trace_switches = 0;
  Stream* ts_stream;
};
