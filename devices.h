#pragma once

/* Standard firmware for the Atm_led_device programmable pinball device class
 *  
 *  multilane (no leds)
 *  bumper
 *  dual flipper
 *  dual_slingshot
 *  dual_kicker
 *  dual_lane_leds (with synced leds)
 *  single_led
 *  dual_target
 *  tictactoe
 *  
 */

class std_firmware {

  public: 
  
  static int16_t multilane[];

  enum { IN_LANE_INIT, IN_LANE_PRESS0, IN_LANE_RELEASE0, IN_LANE_PRESS1, IN_LANE_RELEASE1, IN_LANE_PRESS2, IN_LANE_RELEASE2, // Inputs
          IN_LANE_PRESS3, IN_LANE_RELEASE3, IN_LANE_PRESS4, IN_LANE_RELEASE4, IN_LANE_PRESS5, IN_LANE_RELEASE5, 
          IN_LANE_PRESS6, IN_LANE_RELEASE6, IN_LANE_PRESS7, IN_LANE_RELEASE7 };
  enum { OUT_LANE_SCORE, OUT_LANE0, OUT_LANE1, OUT_LANE2, OUT_LANE3, OUT_LANE4, OUT_LANE5, OUT_LANE6, OUT_LANE7 }; // Outputs

  static int16_t bumper[];

  enum { IN_BUMPER_INIT, IN_BUMPER_PRESS, IN_BUMPER_RELEASE, IN_BUMPER_LIGHT_ON, IN_BUMPER_LIGHT_OFF }; // Inputs
  enum { OUT_BUMPER_SCORE_LIT, OUT_BUMPER_SCORE_UNLIT, OUT_BUMPER_LIGHT_ON, OUT_BUMPER_LIGHT_OFF }; // Outputs
  enum { ARG_BUMPER_COIL, ARG_BUMPER_LED }; // Arguments (leds)

  static int16_t dual_flipper[];

  enum { IN_FLIPPER_INIT, IN_FLIPPER_PRESS_L, IN_FLIPPER_RELEASE_L, IN_FLIPPER_PRESS_R, IN_FLIPPER_RELEASE_R };
  enum { ARG_FLIPPER_COIL_L, ARG_FLIPPER_COIL_R }; 

  static int16_t dual_slingshot[];

  enum { IN_SLING_INIT, IN_SLING_PRESS_L, IN_SLING_RELEASE_L, IN_SLING_PRESS_R, IN_SLING_RELEASE_R };
  enum { OUT_SLING_SCORE, OUT_SLING_KICK };
  enum { ARG_SLING_COIL_L, ARG_SLING_COIL_R };
  
  static int16_t dual_kicker[];

  enum { IN_KICKER_INIT, IN_KICKER_PRESS_L, IN_KICKER_RELEASE_L, IN_KICKER_PRESS_R, IN_KICKER_RELEASE_R };
  enum { OUT_KICKER_SCORE, OUT_KICKER_KICK };
  enum { ARG_KICKER_COIL_L, ARG_KICKER_COIL_R };
  
  static int16_t dual_lane_leds[];

  enum { IN_LANELED_INIT, IN_LANELED_PRESS0, IN_LANELED_RELEASE0, IN_LANELED_PRESS1, IN_LANELED_RELEASE1, IN_LANELED_ON, IN_LANELED_OFF };
  enum { OUT_LANELED_SCORE, OUT_LANELED_SCORE_ON, OUT_LANELED_SCORE_OFF, OUT_LANELED_PRESS_ON, OUT_LANELED_PRESS_OFF };
  enum { ARG_LANELED_LED0, ARG_LANELED_LED1 };

  static int16_t single_led[];
  
  enum { IN_SINGLE_INIT, IN_SINGLE_ON, IN_SINGLE_OFF };
  enum { OUT_SINGLE_LED_ON, OUT_SINGLE_LED_OFF };
  enum { ARG_LED };

  static int16_t dual_target[];
  
  enum { IN_TARGET_INIT, IN_TARGET_PRESS_A, IN_TARGET_RELEASE_A, IN_TARGET_PRESS_B, IN_TARGET_RELEASE_B, IN_TARGET_CLEAR };
  enum { OUT_TARGET_LED_A_ON, OUT_TARGET_LED_B_ON, OUT_TARGET_LED_A_OFF, OUT_TARGET_LED_B_OFF, OUT_TARGET_ALL_ON, OUT_TARGET_ALL_OFF };
  enum { ARG_TARGET_LED_A, ARG_TARGET_LED_B };

  static int16_t tictactoe[];

  enum { // Inputs & Subs
    IN_OXO_INIT,
    IN_OXO_1O, IN_OXO_1X, IN_OXO_2O, IN_OXO_2X, IN_OXO_3O, IN_OXO_3X,             
    IN_OXO_4, IN_OXO_5, IN_OXO_6, IN_OXO_7, IN_OXO_8, IN_OXO_9, 
    IN_OXO_TOGGLE, 
    SUB_OXO_MATCH123, SUB_OXO_MATCH456, SUB_OXO_MATCH789,
    SUB_OXO_MATCH147, SUB_OXO_MATCH258, SUB_OXO_MATCH369,
    SUB_OXO_MATCH159, SUB_OXO_MATCH357, 
    SUB_OXO_MATCHALL,
  };
  
  enum { OUT_OXO_SET_SQUARE, OUT_OXO_MATCH_ROW, OUT_OXO_MATCH_ALL }; // Outputs
  
  enum { // Arguments (leds)
    ARG_OXO_1A, ARG_OXO_1B, ARG_OXO_1C, 
    ARG_OXO_2A, ARG_OXO_2B, ARG_OXO_2C,
    ARG_OXO_3A, ARG_OXO_3B, ARG_OXO_3C,
    ARG_OXO_4A, ARG_OXO_4B, ARG_OXO_4C,
    ARG_OXO_5A, ARG_OXO_5B, ARG_OXO_5C,
    ARG_OXO_6A, ARG_OXO_6B, ARG_OXO_6C,
    ARG_OXO_7A, ARG_OXO_7B, ARG_OXO_7C,
    ARG_OXO_8A, ARG_OXO_8B, ARG_OXO_8C,
    ARG_OXO_9A, ARG_OXO_9B, ARG_OXO_9C,
    ARG_OXO_X, ARG_OXO_O,
  };
  
};


// A single led (or coil) that can be turned on and off

int16_t std_firmware::single_led[] {
  IN_SINGLE_INIT, 
  IN_SINGLE_ON, 
  IN_SINGLE_OFF,
  -1,

  IN_SINGLE_INIT,
  -1,

  IN_SINGLE_ON,
  'H', -1, -1, ARG_LED,
  'T', -1, -1, OUT_SINGLE_LED_ON,
  -1,
  
  IN_SINGLE_OFF,
  'L', -1, -1, ARG_LED,
  'T', -1, -1, OUT_SINGLE_LED_OFF,
  -1,

  -1,
};


// Two independent slingshots (share score & kick)

int16_t std_firmware::dual_slingshot[] {
  IN_SLING_INIT, 
  IN_SLING_PRESS_L, 
  IN_SLING_RELEASE_L,
  IN_SLING_PRESS_R, 
  IN_SLING_RELEASE_R,
  -1,

  IN_SLING_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  IN_SLING_PRESS_L,
  'H', -1, -1, ARG_SLING_COIL_L,
  'T', -1, -1, OUT_SLING_SCORE,
  'T', -1, -1, OUT_SLING_KICK,
  -1,
  
  IN_SLING_PRESS_R,
  'H', -1, -1, ARG_SLING_COIL_R,
  'T', -1, -1, OUT_SLING_SCORE,
  'T', -1, -1, OUT_SLING_KICK,
  -1,
  
  -1,
};


// Dual kickers, same score & same kick, persistent

int16_t std_firmware::dual_kicker[] {
  IN_KICKER_INIT, 
  IN_KICKER_PRESS_L, 
  IN_KICKER_RELEASE_L,
  IN_KICKER_PRESS_R, 
  IN_KICKER_RELEASE_R,
  -1,

  IN_KICKER_INIT,
  'P', -1, -1, 1,  // Persistent 
  -1,

  IN_KICKER_PRESS_L,
  'H', -1, -1, ARG_KICKER_COIL_L,
  'T', -1, -1, OUT_KICKER_SCORE,
  'T', -1, -1, OUT_KICKER_KICK,
  -1,
  
  IN_KICKER_PRESS_R,
  'H', -1, -1, ARG_KICKER_COIL_R,
  'T', -1, -1, OUT_KICKER_SCORE,
  'T', -1, -1, OUT_KICKER_KICK,
  -1,
  
  -1,
};


// Basic 8-way multilane device without leds

int16_t std_firmware::multilane[] = {
  IN_LANE_INIT, 
  IN_LANE_PRESS0, 
  IN_LANE_RELEASE0, 
  IN_LANE_PRESS1, 
  IN_LANE_RELEASE1, 
  IN_LANE_PRESS2, 
  IN_LANE_RELEASE2, 
  IN_LANE_PRESS3, 
  IN_LANE_RELEASE3, 
  IN_LANE_PRESS4, 
  IN_LANE_RELEASE4, 
  IN_LANE_PRESS5, 
  IN_LANE_RELEASE5,
  IN_LANE_PRESS6, 
  IN_LANE_RELEASE6,
  IN_LANE_PRESS7, 
  IN_LANE_RELEASE7,
  -1,

  IN_LANE_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  IN_LANE_PRESS0,
  'T', -1, -1, OUT_LANE0,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS1,
  'T', -1, -1, OUT_LANE1,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS2,
  'T', -1, -1, OUT_LANE2,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS3,
  'T', -1, -1, OUT_LANE3,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS4,
  'T', -1, -1, OUT_LANE4,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS5,
  'T', -1, -1, OUT_LANE5,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS6,
  'T', -1, -1, OUT_LANE6,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  IN_LANE_PRESS7,
  'T', -1, -1, OUT_LANE7,
  'T', -1, -1, OUT_LANE_SCORE,
  -1,

  -1,
};


// Dual lane device with leds (both lanes share the same function)

int16_t std_firmware::dual_lane_leds[] = {
  IN_LANELED_INIT, 
  IN_LANELED_PRESS0, 
  IN_LANELED_RELEASE0, 
  IN_LANELED_PRESS1, 
  IN_LANELED_RELEASE1, 
  IN_LANELED_ON, 
  IN_LANELED_OFF, 
  -1,

  IN_LANELED_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  IN_LANELED_PRESS0,
  'S', -1, -1, IN_LANELED_PRESS1,
  -1,

  IN_LANELED_PRESS1,
  'J', ARG_LANELED_LED0, 0, 3, 
  'T', -1, -1, OUT_LANELED_PRESS_ON,
  'T', -1, -1, OUT_LANELED_SCORE_ON,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_LANELED_PRESS_OFF,
  'T', -1, -1, OUT_LANELED_SCORE_OFF,
  -1,

  IN_LANELED_ON,
  'H', -1, -1, ARG_LANELED_LED0, 
  'H', -1, -1, ARG_LANELED_LED1, 
  -1,

  IN_LANELED_OFF,
  'L', -1, -1, ARG_LANELED_LED0, 
  'L', -1, -1, ARG_LANELED_LED1, 
  -1,

  -1,
};


// Single bumper device with led & coil

int16_t std_firmware::bumper[] = {
  IN_BUMPER_INIT,                       // Input event jump table (initialized by parser)
  IN_BUMPER_PRESS,
  IN_BUMPER_RELEASE,  
  IN_BUMPER_LIGHT_ON, 
  IN_BUMPER_LIGHT_OFF,
  -1,

  IN_BUMPER_INIT,
  'P', -1, -1, 0,                       // Not a persistent device
  'L', -1, -1, ARG_BUMPER_COIL,         // Turn off coil
  'L', -1, -1, ARG_BUMPER_LED,          // Turn off led
  -1,

  IN_BUMPER_PRESS,
  'H', -1, -1, ARG_BUMPER_COIL,         // Pulse the bumper coil
  'J', ARG_BUMPER_LED, 0, 2,            // Check bumper light
  'T', -1, -1, OUT_BUMPER_SCORE_LIT,    // Emit lit score event
  'J', -1, -1, -1,                      // Exit
  'T', -1, -1, OUT_BUMPER_SCORE_UNLIT,  // Emit unlit score event
  -1,

  IN_BUMPER_LIGHT_ON,
  'H', -1, -1, ARG_BUMPER_LED,          // Light the bumper 
  'T', -1, -1, OUT_BUMPER_LIGHT_ON,     // Emit bumper light on event
  -1,

  IN_BUMPER_LIGHT_OFF,
  'L', -1, -1, ARG_BUMPER_LED,          // Turn off the bumper light
  'T', -1, -1, OUT_BUMPER_LIGHT_OFF,    // Emit bumper light off event
  -1,

  -1,
};


// Dual target device

int16_t std_firmware::dual_target[] = {
  IN_TARGET_INIT,    
  IN_TARGET_PRESS_A,
  IN_TARGET_RELEASE_A,  
  IN_TARGET_PRESS_B,
  IN_TARGET_RELEASE_B,  
  IN_TARGET_CLEAR,  
  -1,

  IN_TARGET_INIT,
  'P', -1, -1, 0,
  'S', -1, -1, IN_TARGET_CLEAR,
  -1,
  
  IN_TARGET_PRESS_A,
  'J', ARG_TARGET_LED_A, -1, 0,
  'H', -1, -1, ARG_TARGET_LED_A,
  'T', -1, -1, OUT_TARGET_LED_A_ON,
  'J', ARG_TARGET_LED_B, 0, -1,
  'T', -1, -1, OUT_TARGET_ALL_ON,
  -1,
   
  IN_TARGET_PRESS_B,
  'J', ARG_TARGET_LED_B, -1, 0,
  'H', -1, -1, ARG_TARGET_LED_B,
  'T', -1, -1, OUT_TARGET_LED_B_ON,
  'J', ARG_TARGET_LED_A, 0, -1,
  'T', -1, -1, OUT_TARGET_ALL_ON,
  -1,

  IN_TARGET_CLEAR, // Only send off events if leds were on in the first place?
  'L', -1, -1, ARG_TARGET_LED_A,
  'L', -1, -1, ARG_TARGET_LED_B,
  'T', -1, -1, OUT_TARGET_LED_A_OFF,
  'T', -1, -1, OUT_TARGET_LED_B_OFF,
  'T', -1, -1, OUT_TARGET_ALL_OFF,
  -1,
  
  -1,
};


// Dual flipper device (2 independent flippers)

int16_t std_firmware::dual_flipper[] = {
  IN_FLIPPER_INIT,
  IN_FLIPPER_PRESS_L,
  IN_FLIPPER_RELEASE_L,  
  IN_FLIPPER_PRESS_R,
  IN_FLIPPER_RELEASE_R,  
  -1,

  IN_FLIPPER_INIT,
  'P', -1, -1, 0,  // Not persistent
  'L', -1, -1, ARG_FLIPPER_COIL_L,
  'L', -1, -1, ARG_FLIPPER_COIL_R,
  -1,

  IN_FLIPPER_PRESS_L,
  'H', -1, -1, ARG_FLIPPER_COIL_L,
  -1,

  IN_FLIPPER_RELEASE_L,
  'L', -1, -1, ARG_FLIPPER_COIL_L,
  -1,

  IN_FLIPPER_PRESS_R,
  'H', -1, -1, ARG_FLIPPER_COIL_R,
  -1,

  IN_FLIPPER_RELEASE_R,
  'L', -1, -1, ARG_FLIPPER_COIL_R,
  -1,

  -1,
};


// Williams OXO tic-tac-toe game

int16_t std_firmware::tictactoe[] = {

  IN_OXO_INIT,
  IN_OXO_1O, IN_OXO_1X, IN_OXO_2O, IN_OXO_2X, IN_OXO_3O, IN_OXO_3X,             
  IN_OXO_4, IN_OXO_5, IN_OXO_6, IN_OXO_7, IN_OXO_8, IN_OXO_9, 
  IN_OXO_TOGGLE, 
  SUB_OXO_MATCH123, SUB_OXO_MATCH456, SUB_OXO_MATCH789,
  SUB_OXO_MATCH147, SUB_OXO_MATCH258, SUB_OXO_MATCH369,
  SUB_OXO_MATCH159, SUB_OXO_MATCH357, 
  SUB_OXO_MATCHALL,
  -1,

  IN_OXO_INIT,
  'H', -1, -1, ARG_OXO_O, 
  'L', -1, -1, ARG_OXO_X, 
  -1,

  IN_OXO_1O, 
  'J', ARG_OXO_1A, -1, 0, 
  'J', ARG_OXO_1B, -1, 0, 
  'H', -1, -1, ARG_OXO_1A, 
  'H', -1, -1, ARG_OXO_1C, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_1X, 
  'J', ARG_OXO_1A, -1, 0, 
  'J', ARG_OXO_1B, -1, 0, 
  'H', -1, -1, ARG_OXO_1B, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_2O, 
  'J', ARG_OXO_2A, -1, 0, 
  'J', ARG_OXO_2B, -1, 0, 
  'H', -1, -1, ARG_OXO_2A, 
  'H', -1, -1, ARG_OXO_2C, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_2X, 
  'J', ARG_OXO_2A, -1, 0, 
  'J', ARG_OXO_2B, -1, 0, 
  'H', -1, -1, ARG_OXO_2B, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_3O, 
  'J', ARG_OXO_3A, -1, 0, 
  'J', ARG_OXO_3B, -1, 0, 
  'H', -1, -1, ARG_OXO_3A, 
  'H', -1, -1, ARG_OXO_3C, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_3X, 
  'J', ARG_OXO_3A, -1, 0, 
  'J', ARG_OXO_3B, -1, 0, 
  'H', -1, -1, ARG_OXO_3B, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,

  IN_OXO_4, 
  'J', ARG_OXO_4A, -1, 0, 
  'J', ARG_OXO_4B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_4B, ARG_OXO_4A, 
  'H', ARG_OXO_X, ARG_OXO_4B, ARG_OXO_4C, 
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH456,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_5, 
  'J', ARG_OXO_5A, -1, 0, 
  'J', ARG_OXO_5B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_5B, ARG_OXO_5A, 
  'H', ARG_OXO_X, ARG_OXO_5B, ARG_OXO_5C, 
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCH456,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_6, 
  'J', ARG_OXO_6A, -1, 0, 
  'J', ARG_OXO_6B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_6B, ARG_OXO_6A, 
  'H', ARG_OXO_X, ARG_OXO_6B, ARG_OXO_6C, 
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH456,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_7, 
  'J', ARG_OXO_7A, -1, 0, 
  'J', ARG_OXO_7B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_7B, ARG_OXO_7A, 
  'H', ARG_OXO_X, ARG_OXO_7B, ARG_OXO_7C, 
  'S', -1, -1, SUB_OXO_MATCH789,
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_8, 
  'J', ARG_OXO_8A, -1, 0, 
  'J', ARG_OXO_8B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_8B, ARG_OXO_8A, 
  'H', ARG_OXO_X, ARG_OXO_8B, ARG_OXO_8C, 
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCH789,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  IN_OXO_9, 
  'J', ARG_OXO_9A, -1, 0, 
  'J', ARG_OXO_9B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_9B, ARG_OXO_9A, 
  'H', ARG_OXO_X, ARG_OXO_9B, ARG_OXO_9C, 
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH789,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,

  IN_OXO_TOGGLE, 
  'J', ARG_OXO_X, 0, 3, 
  'L', -1, -1, ARG_OXO_X, 
  'H', -1, -1, ARG_OXO_O, 
  'J', -1, -1, -1, 
  'L', -1, -1, ARG_OXO_O, 
  'H', -1, -1, ARG_OXO_X, 
  -1,    

  SUB_OXO_MATCH123,
  'J', ARG_OXO_1A, 0,  4,
  'J', ARG_OXO_2A, 0, -1,
  'J', ARG_OXO_3A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_2B, 0, -1,
  'J', ARG_OXO_3B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH456,
  'J', ARG_OXO_4A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_6A, 3, -1,
  'J', ARG_OXO_4B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_6B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,

  SUB_OXO_MATCH789,
  'J', ARG_OXO_7A, 0,  4,
  'J', ARG_OXO_8A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_7B, 0, -1,
  'J', ARG_OXO_8B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,

  SUB_OXO_MATCH147, 
  'J', ARG_OXO_1A, 0,  4,
  'J', ARG_OXO_4A, 0, -1,
  'J', ARG_OXO_7A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_4B, 0, -1,
  'J', ARG_OXO_7B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH258, 
  'J', ARG_OXO_2A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_8A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_8B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH369,
  'J', ARG_OXO_3A, 0,  4,
  'J', ARG_OXO_6A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_3B, 0, -1,
  'J', ARG_OXO_6B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH159, 
  'J', ARG_OXO_1A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH357,
  'J', ARG_OXO_3A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_7A, 3, -1,
  'J', ARG_OXO_3B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_7B, 0, -1,
  'T', -1, -1, OUT_OXO_MATCH_ROW,
  -1,

  SUB_OXO_MATCHALL,
  'T', -1, -1, OUT_OXO_SET_SQUARE, 
  'I', -1, -1, -1,
  'I', ARG_OXO_1A, 1, 0,
  'I', ARG_OXO_2A, 1, 0,
  'I', ARG_OXO_3A, 1, 0,
  'I', ARG_OXO_4A, 1, 0,
  'I', ARG_OXO_5A, 1, 0,
  'I', ARG_OXO_6A, 1, 0,
  'I', ARG_OXO_7A, 1, 0,
  'I', ARG_OXO_8A, 1, 0,
  'I', ARG_OXO_9A, 1, 0,
  'I', ARG_OXO_1B, 1, 0,
  'I', ARG_OXO_2B, 1, 0,
  'I', ARG_OXO_3B, 1, 0,
  'I', ARG_OXO_4B, 1, 0,
  'I', ARG_OXO_5B, 1, 0,
  'I', ARG_OXO_6B, 1, 0,
  'I', ARG_OXO_7B, 1, 0,
  'I', ARG_OXO_8B, 1, 0,
  'I', ARG_OXO_9B, 1, 0,
  'T', 9, OUT_OXO_MATCH_ALL, -1,
  -1,
    
  -1,
};      

  
