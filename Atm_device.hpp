#pragma once

#include <Automaton.h>
#include "Atm_led_matrix.hpp"
#include "Atm_switch_matrix.hpp"
#include "Symbolic_Machine.hpp"

// Number of outputs is also limited by the 32 bits in trigger_flags 

#define MAX_OUTPUTS 20
#define MAX_REGISTERS 16
#define STACK_SIZE 16
#define DYNAMIC_ELEMENTS

namespace instruction_set {

  enum { JmpL = 'J', JmpLA = 'A', JmpRE = 'C', Prim = '0', LedOn = 'H', LedOff = 'L', GoSub = 'S', Inc = 'I', Dec = 'D', Trig = 'T', Pers = 'P', Reg = 'R', Yield = 'Y' };

};

class Atm_switch_matrix;

class Atm_device: public Symbolic_Machine {

 public:
  enum { IDLE, NOTIFY, YIELD, RESUME }; // STATES
  enum { EVT_NOTIFY, EVT_TIMER, EVT_YIELD, ELSE }; // EVENTS
  Atm_device( void ) : Symbolic_Machine() { };
  Atm_device& begin( Atm_switch_matrix* playfield, int16_t switch_group, int16_t led_group, const int16_t* device_script, 
          int16_t r0 = 0, int16_t r1 = 0, int16_t r2 = 0, int16_t r3 = 0, int16_t r4 = 0, int16_t r5 = 0, int16_t r6 = 0, int16_t r7 = 0 );
  Atm_device& set_script( const int16_t* script );
  Atm_device& set_led( int16_t led_group );
  int16_t switchGroup( void);
  int16_t ledGroup( void);
  int16_t handler( int16_t e );
  Atm_device& trace( Stream & stream );
  Atm_device& trace( void );
  Atm_device& traceCode( Stream & stream, uint8_t bitmap = 1 );
  Atm_device& dump( Stream & streamint16_t, int16_t offset = 0 );
  Atm_device& trigger( int event );
  Atm_device& trigger( int event, uint32_t sel );
  int state( void );
  int state( uint32_t sel );
  Atm_device& onEvent( int sub, Machine& machine, int event = 0 );
  Atm_device& onEvent( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_device& onEvent( int sub, int sw, int event );
  Atm_device& onEvent( const char sub_str[], const char sw_str[], const char event_str[] );
  Atm_device& init( void );
  Atm_device& init( uint32_t sel );
  Atm_device& press( void );
  Atm_device& release( void );
  Atm_device& reg( uint8_t r, int16_t v );
  int16_t reg( uint8_t r );
  Atm_device& chain( Atm_device& next );
  Atm_device& chain( int16_t sw );
  Atm_device& chain( const char s[] );
  Atm_device& select( uint32_t mask );
  Machine* outputPtr( int16_t n );
  int16_t outputEvent( int16_t n );
  Atm_device& loadSymbols( const char s[] );
  Atm_device& linkSymbols( symbolic_machine_table* sym );
  Atm_device& dumpCode( Stream* stream, uint8_t event, bool clean = 0 );
  uint8_t sleep( int8_t v = -1 );

  static const uint32_t SELECT_ALL = 0xFFFFFFFF;
  const int16_t* script;

 protected:
  enum { ENT_NOTIFY, ENT_RESUME }; // ACTIONS
  enum { ON_EVENT, CONN_MAX = MAX_OUTPUTS }; // CONNECTORS
  atm_connector connectors[CONN_MAX+1];
  int event( int id ); 
  void action( int id ); 
  void start_code( int16_t e );
  void run_code( void );
  int16_t parse_code( const int16_t* device_script );
  int16_t led_index( int16_t led_group, int16_t selector );
  bool led_active( int16_t led_group, int16_t selector );
  void led_on( int16_t led_group, int16_t selector );
  void led_off( int16_t led_group, int16_t selector );
  void decompile( uint16_t ip, char* s, bool hide_ptr = 0 );
  Atm_device& update_switch( int event );

  uint32_t trigger_flags; 
  Atm_led_matrix *leds;
  Atm_switch_matrix *playfield;
  int16_t registers[MAX_REGISTERS];
  int16_t led_group = -1;
  int16_t numberOfInputs;
  atm_timer_millis timer;
  bool enabled;
  uint32_t xctr;
  Stream* tc_stream;
  uint8_t trace_code = 0;
  int16_t switch_group = 0;
  Atm_device* next;
  uint32_t event_map, switch_map;
  int16_t code_ptr;
  uint8_t reg_ptr;
  uint8_t stack_ptr;
  int16_t stack[STACK_SIZE];  

};
