#pragma once

#include <Automaton.h>
#include "FastLED.h"

#define MAX_LEDS 100

#define MOSI0 11
#define SCK0 13
#define MOSI1 21
#define SCK1 20

#define GLOBAL_BRIGHTNESS 255 

enum{ SPI_11_13, SPI_11_20, SPI_21_13, SPI_21_20 };

struct led_meta_data {
    uint32_t rgb; 
    uint16_t last_millis; 
    uint16_t pulse_millis; 
    uint8_t status : 1;
    uint8_t pulsing : 1;
    
    uint8_t changed : 1;
    uint8_t fade : 1;
    uint8_t step;
};


class Atm_apa102: public Machine {

 public:
  enum { IDLE, WAITING, RUNNING, UPDATING }; // STATES
  enum { EVT_DONE, EVT_RUN, EVT_UPDATE, EVT_MILLI, ELSE }; // EVENTS
  Atm_apa102( void ) : Machine() {};
  Atm_apa102& begin( int number_of_leds, int idx = 0 );
  Atm_apa102& trace( Stream & stream );
  Atm_apa102& trigger( int event );
  int state( void );
  Atm_apa102& done( void );
  Atm_apa102& run( void );
  Atm_apa102& update( void );
  Atm_apa102& level( uint8_t v );
  Atm_apa102& level( uint8_t ledno, uint8_t v );
  Atm_apa102& rgb( uint32_t rgb );
  Atm_apa102& rgb( int ledno, uint32_t rgb );
  Atm_apa102& rgb( int ledno, uint8_t r, uint8_t g, uint8_t b );
  Atm_apa102& set( int ledno, uint32_t rgb );
  Atm_apa102& set( int ledno, uint8_t r, uint8_t g, uint8_t b );
  Atm_apa102& on( int ledno );
  Atm_apa102& off( int ledno );
  Atm_apa102& pulse( int ledno, uint16_t duration );
  int active( int ledno );
  uint8_t debug;

 private:
  enum { ENT_RUNNING, ENT_UPDATING, ENT_IDLE }; // ACTIONS
  int event( int id ); 
  void action( int id ); 
  CRGB leds[MAX_LEDS];
  led_meta_data meta[MAX_LEDS];
  int number_of_leds;
  uint8_t refresh, running;
  uint8_t last_milli;
  CLEDController *controller;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_apa102">
    <states>
      <IDLE index="0" sleep="1">
        <EVT_RUN>RUNNING</EVT_RUN>
        <EVT_UPDATE>UPDATING</EVT_UPDATE>
      </IDLE>
      <RUNNING index="1" on_enter="ENT_RUNNING">
        <EVT_DONE>IDLE</EVT_DONE>
        <EVT_UPDATE>UPDATING</EVT_UPDATE>
      </RUNNING>
      <UPDATING index="2" on_enter="ENT_UPDATING">
        <EVT_DONE>IDLE</EVT_DONE>
        <EVT_RUN>RUNNING</EVT_RUN>
        <ELSE>IDLE</ELSE>
      </UPDATING>
    </states>
    <events>
      <EVT_DONE index="0" access="MIXED"/>
      <EVT_RUN index="1" access="MIXED"/>
      <EVT_UPDATE index="2" access="MIXED"/>
    </events>
    <connectors>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

