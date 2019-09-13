#pragma once

/* Standard firmware for the Atm_led_device programmable pinball device class
 *  
 *  multilane (no leds)
 *  bumper
 *  dual flipper
 *  dual_slingshot
 *  dual_kicker
 *  dual_lane_leds (with synced leds)
 *  tictactoe
 *  single_led
 *  dual_target
 *  
 */

class std_firmware {

  public: 
  
  static int16_t multilane[];

  enum { EVT_LANE_INIT, EVT_LANE_PRESS0, EVT_LANE_RELEASE0, EVT_LANE_PRESS1, EVT_LANE_RELEASE1, EVT_LANE_PRESS2, EVT_LANE_RELEASE2, 
          EVT_LANE_PRESS3, EVT_LANE_RELEASE3, EVT_LANE_PRESS4, EVT_LANE_RELEASE4, EVT_LANE_PRESS5, EVT_LANE_RELEASE5 };
  enum { LANE_SCORE, LANE0, LANE1, LANE2, LANE3, LANE4, LANE5 };

  static int16_t bumper[];

  enum { EVT_BUMPER_INIT, EVT_BUMPER_PRESS, EVT_BUMPER_RELEASE, EVT_BUMPER_LIGHT_ON, EVT_BUMPER_LIGHT_OFF };
  enum { ARG_BUMPER_COIL, ARG_BUMPER_LED }; 
  enum { BUMPER_SCORE_LIT, BUMPER_SCORE_UNLIT, BUMPER_LIGHT_ON, BUMPER_LIGHT_OFF };

  static int16_t dual_flipper[];

  enum { EVT_FLIPPER_INIT, EVT_FLIPPER_PRESS_L, EVT_FLIPPER_RELEASE_L, EVT_FLIPPER_PRESS_R, EVT_FLIPPER_RELEASE_R };
  enum { ARG_FLIPPER_COIL_L, ARG_FLIPPER_COIL_R }; 

  static int16_t tictactoe[];

  enum { // Received events
    EVT_OXO_INIT,
    EVT_OXO_PRESS,
    EVT_OXO_RELEASE,  
    EVT_OXO_1O, EVT_OXO_1X, EVT_OXO_2O, EVT_OXO_2X, EVT_OXO_3O, EVT_OXO_3X,             
    EVT_OXO_4, EVT_OXO_5, EVT_OXO_6, EVT_OXO_7, EVT_OXO_8, EVT_OXO_9, 
    EVT_OXO_TOGGLE, 
    SUB_OXO_MATCH123, SUB_OXO_MATCH456, SUB_OXO_MATCH789,
    SUB_OXO_MATCH147, SUB_OXO_MATCH258, SUB_OXO_MATCH369,
    SUB_OXO_MATCH159, SUB_OXO_MATCH357, 
    SUB_OXO_MATCHALL,
  };
  
  enum { OXO_SET_SQUARE, OXO_MATCH_ROW, OXO_MATCH_ALL }; // Emitted events 
  
  enum { // begin() argument placeholders
    ARG_OXO_1A, ARG_OXO_1B, ARG_OXO_1C, 
    ARG_OXO_2A, ARG_OXO_2B, ARG_OXO_2C,
    ARG_OXO_3A, ARG_OXO_3B, ARG_OXO_3C,
    ARG_OXO_4A, ARG_OXO_4B, ARG_OXO_4C,
    ARG_OXO_5A, ARG_OXO_5B, ARG_OXO_5C,
    ARG_OXO_6A, ARG_OXO_6B, ARG_OXO_6C,
    ARG_OXO_7A, ARG_OXO_7B, ARG_OXO_7C,
    ARG_OXO_8A, ARG_OXO_8B, ARG_OXO_8C,
    ARG_OXO_9A, ARG_OXO_9B, ARG_OXO_9C,
    ARG_OXO_O, ARG_OXO_X,
  };
  
  static int16_t dual_slingshot[];

  enum { EVT_SLING_INIT, EVT_SLING_PRESS_L, EVT_SLING_RELEASE_L, EVT_SLING_PRESS_R, EVT_SLING_RELEASE_R };
  enum { ARG_SLING_COIL_L, ARG_SLING_COIL_R };
  enum { SLING_SCORE, SLING_KICK };
  
  static int16_t dual_kicker[];

  enum { EVT_KICKER_INIT, EVT_KICKER_PRESS_L, EVT_KICKER_RELEASE_L, EVT_KICKER_PRESS_R, EVT_KICKER_RELEASE_R };
  enum { ARG_KICKER_COIL_L, ARG_KICKER_COIL_R };
  enum { KICKER_SCORE, KICKER_KICK };
  
  static int16_t multilane_leds[];

  enum { EVT_LANELED_INIT, EVT_LANELED_PRESS0, EVT_LANELED_RELEASE0, EVT_LANELED_PRESS1, EVT_LANELED_RELEASE1, EVT_LANELED_ON, EVT_LANELED_OFF };
  enum { ARG_LANELED_LED0, ARG_LANELED_LED1 };
  enum { LANELED_SCORE, LANELED_SCORE_ON, LANELED_SCORE_OFF, LANELED_PRESS_ON, LANELED_PRESS_OFF };

  static int16_t single_led[];
  
  enum { EVT_SINGLE_INIT, EVT_SINGLE_ON, EVT_SINGLE_OFF };
  enum { ARG_LED };
  enum { SINGLE_LED_ON, SINGLE_LED_OFF };

  static int16_t dual_target[];
  
  enum { EVT_TARGET_INIT, EVT_TARGET_PRESS_A, EVT_TARGET_RELEASE_A, EVT_TARGET_PRESS_B, EVT_TARGET_RELEASE_B, EVT_TARGET_CLEAR };
  enum { ARG_TARGET_LED_A, ARG_TARGET_LED_B };
  enum { TARGET_LED_A_ON, TARGET_LED_B_ON, TARGET_LED_A_OFF, TARGET_LED_B_OFF, TARGET_ALL_ON, TARGET_ALL_OFF };

};


int16_t std_firmware::single_led[] {
  EVT_SINGLE_INIT, 
  EVT_SINGLE_ON, 
  EVT_SINGLE_OFF,
  -1,

  EVT_SINGLE_INIT,
  -1,

  EVT_SINGLE_ON,
  'H', -1, -1, ARG_LED,
  'T', -1, -1, SINGLE_LED_ON,
  -1,
  
  EVT_SINGLE_OFF,
  'L', -1, -1, ARG_LED,
  'T', -1, -1, SINGLE_LED_OFF,
  -1,

};

int16_t std_firmware::dual_slingshot[] {
  EVT_SLING_INIT, 
  EVT_SLING_PRESS_L, 
  EVT_SLING_RELEASE_L,
  EVT_SLING_PRESS_R, 
  EVT_SLING_RELEASE_R,
  -1,

  EVT_SLING_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  EVT_SLING_PRESS_L,
  'H', -1, -1, ARG_SLING_COIL_L,
  'T', -1, -1, SLING_SCORE,
  'T', -1, -1, SLING_KICK,
  -1,
  
  EVT_SLING_PRESS_R,
  'H', -1, -1, ARG_SLING_COIL_R,
  'T', -1, -1, SLING_SCORE,
  'T', -1, -1, SLING_KICK,
  -1,
  
};

enum { EVT_KICKER_INIT, EVT_KICKER_PRESS_L, EVT_KICKER_RELEASE_L, EVT_KICKER_PRESS_R, EVT_KICKER_RELEASE_R };
enum { ARG_KICKER_COIL_L, ARG_KICKER_COIL_R };
enum { KICKER_SCORE, KICKER_KICK };

int16_t std_firmware::dual_kicker[] {
  EVT_KICKER_INIT, 
  EVT_KICKER_PRESS_L, 
  EVT_KICKER_RELEASE_L,
  EVT_KICKER_PRESS_R, 
  EVT_KICKER_RELEASE_R,
  -1,

  EVT_KICKER_INIT,
  'P', -1, -1, 1,  // Persistent 
  -1,

  EVT_KICKER_PRESS_L,
  'H', -1, -1, ARG_KICKER_COIL_L,
  'T', -1, -1, KICKER_SCORE,
  'T', -1, -1, KICKER_KICK,
  -1,
  
  EVT_KICKER_PRESS_R,
  'H', -1, -1, ARG_KICKER_COIL_R,
  'T', -1, -1, KICKER_SCORE,
  'T', -1, -1, KICKER_KICK,
  -1,
  
};

int16_t std_firmware::multilane[] = {
  EVT_LANE_INIT, 
  EVT_LANE_PRESS0, 
  EVT_LANE_RELEASE0, 
  EVT_LANE_PRESS1, 
  EVT_LANE_RELEASE1, 
  EVT_LANE_PRESS2, 
  EVT_LANE_RELEASE2, 
  EVT_LANE_PRESS3, 
  EVT_LANE_RELEASE3, 
  EVT_LANE_PRESS4, 
  EVT_LANE_RELEASE4, 
  EVT_LANE_PRESS5, 
  EVT_LANE_RELEASE5,
  -1,

  EVT_LANE_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  EVT_LANE_PRESS0,
  'T', -1, -1, LANE0,
  'T', -1, -1, LANE_SCORE,
  -1,

  EVT_LANE_PRESS1,
  'T', -1, -1, LANE1,
  'T', -1, -1, LANE_SCORE,
  -1,

  EVT_LANE_PRESS2,
  'T', -1, -1, LANE2,
  'T', -1, -1, LANE_SCORE,
  -1,

  EVT_LANE_PRESS3,
  'T', -1, -1, LANE3,
  'T', -1, -1, LANE_SCORE,
  -1,

  EVT_LANE_PRESS4,
  'T', -1, -1, LANE4,
  'T', -1, -1, LANE_SCORE,
  -1,

  EVT_LANE_PRESS5,
  'T', -1, -1, LANE5,
  'T', -1, -1, LANE_SCORE,
  -1,

  -1,
};



int16_t std_firmware::multilane_leds[] = {
  EVT_LANELED_INIT, 
  EVT_LANELED_PRESS0, 
  EVT_LANELED_RELEASE0, 
  EVT_LANELED_PRESS1, 
  EVT_LANELED_RELEASE1, 
  EVT_LANELED_ON, 
  EVT_LANELED_OFF, 
  -1,

  EVT_LANELED_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  EVT_LANELED_PRESS0,
  'S', -1, -1, EVT_LANELED_PRESS1,
  -1,

  EVT_LANELED_PRESS1,
  'J', ARG_LANELED_LED0, 0, 3, 
  'T', -1, -1, LANELED_PRESS_ON,
  'T', -1, -1, LANELED_SCORE_ON,
  'J', -1, -1, -1,
  'T', -1, -1, LANELED_PRESS_OFF,
  'T', -1, -1, LANELED_SCORE_OFF,
  -1,

  EVT_LANELED_ON,
  'H', -1, -1, ARG_LANELED_LED0, 
  'H', -1, -1, ARG_LANELED_LED1, 
  -1,

  EVT_LANELED_OFF,
  'L', -1, -1, ARG_LANELED_LED0, 
  'L', -1, -1, ARG_LANELED_LED1, 
  -1,

  -1,
};


int16_t std_firmware::bumper[] = {
  EVT_BUMPER_INIT,                  // Event jump table (initialized by parser)
  EVT_BUMPER_PRESS,
  EVT_BUMPER_RELEASE,  
  EVT_BUMPER_LIGHT_ON, 
  EVT_BUMPER_LIGHT_OFF,
  -1,

  EVT_BUMPER_INIT,
  'P', -1, -1, 0,                   // Not a persistent device
  'L', -1, -1, ARG_BUMPER_COIL,     // Turn off coil
  'L', -1, -1, ARG_BUMPER_LED,      // Turn off led
  -1,

  EVT_BUMPER_PRESS,
  'H', -1, -1, ARG_BUMPER_COIL,     // Pulse the bumper coil
  'J', ARG_BUMPER_LED, 0, 2,        // Check bumper light
  'T', -1, -1, BUMPER_SCORE_LIT,    // Emit lit score event
  'J', -1, -1, -1,                  // Exit
  'T', -1, -1, BUMPER_SCORE_UNLIT,  // Emit unlit score event
  -1,

  EVT_BUMPER_LIGHT_ON,
  'H', -1, -1, ARG_BUMPER_LED,      // Light the bumper
  'T', -1, -1, BUMPER_LIGHT_ON,     // Emit bumper light on event
  -1,

  EVT_BUMPER_LIGHT_OFF,
  'L', -1, -1, ARG_BUMPER_LED,      // Turn off the bumper light
  'T', -1, -1, BUMPER_LIGHT_OFF,    // Emit bumper light off event
  -1,

  -1,
};


int16_t std_firmware::dual_target[] = {
  EVT_TARGET_INIT,    
  EVT_TARGET_PRESS_A,
  EVT_TARGET_RELEASE_A,  
  EVT_TARGET_PRESS_B,
  EVT_TARGET_RELEASE_B,  
  EVT_TARGET_CLEAR,  
  -1,

  EVT_TARGET_INIT,
  'P', -1, -1, 0,
  'S', -1, -1, EVT_TARGET_CLEAR,
  -1,
  
  EVT_TARGET_PRESS_A,
  'J', ARG_TARGET_LED_A, -1, 0,
  'H', -1, -1, ARG_TARGET_LED_A,
  'T', -1, -1, TARGET_LED_A_ON,
  'J', ARG_TARGET_LED_B, 0, -1,
  'T', -1, -1, TARGET_ALL_ON,
  -1,
   
  EVT_TARGET_PRESS_B,
  'J', ARG_TARGET_LED_B, -1, 0,
  'H', -1, -1, ARG_TARGET_LED_B,
  'T', -1, -1, TARGET_LED_B_ON,
  'J', ARG_TARGET_LED_A, 0, -1,
  'T', -1, -1, TARGET_ALL_ON,
  -1,

  EVT_TARGET_CLEAR, // Only send off events if leds were on in the first place?
  'L', -1, -1, ARG_TARGET_LED_A,
  'L', -1, -1, ARG_TARGET_LED_B,
  'T', -1, -1, TARGET_LED_A_OFF,
  'T', -1, -1, TARGET_LED_B_OFF,
  'T', -1, -1, TARGET_ALL_OFF,
  -1,
  
  -1,
};

int16_t std_firmware::dual_flipper[] = {
  EVT_FLIPPER_INIT,
  EVT_FLIPPER_PRESS_L,
  EVT_FLIPPER_RELEASE_L,  
  EVT_FLIPPER_PRESS_R,
  EVT_FLIPPER_RELEASE_R,  
  -1,

  EVT_FLIPPER_INIT,
  'P', -1, -1, 0,  // Not persistent
  'L', -1, -1, ARG_FLIPPER_COIL_L,
  'L', -1, -1, ARG_FLIPPER_COIL_R,
  -1,

  EVT_FLIPPER_PRESS_L,
  'H', -1, -1, ARG_FLIPPER_COIL_L,
  -1,

  EVT_FLIPPER_RELEASE_L,
  'L', -1, -1, ARG_FLIPPER_COIL_L,
  -1,

  EVT_FLIPPER_PRESS_R,
  'H', -1, -1, ARG_FLIPPER_COIL_R,
  -1,

  EVT_FLIPPER_RELEASE_R,
  'L', -1, -1, ARG_FLIPPER_COIL_R,
  -1,

  -1,
};

int16_t std_firmware::tictactoe[] = {

  EVT_OXO_INIT,
  EVT_OXO_PRESS,
  EVT_OXO_RELEASE,  
  EVT_OXO_1O, EVT_OXO_1X, EVT_OXO_2O, EVT_OXO_2X, EVT_OXO_3O, EVT_OXO_3X,             
  EVT_OXO_4, EVT_OXO_5, EVT_OXO_6, EVT_OXO_7, EVT_OXO_8, EVT_OXO_9, 
  EVT_OXO_TOGGLE, 
  SUB_OXO_MATCH123, SUB_OXO_MATCH456, SUB_OXO_MATCH789,
  SUB_OXO_MATCH147, SUB_OXO_MATCH258, SUB_OXO_MATCH369,
  SUB_OXO_MATCH159, SUB_OXO_MATCH357, 
  SUB_OXO_MATCHALL,
  -1,

  EVT_OXO_INIT,
  'H', -1, -1, ARG_OXO_O, 
  'H', -1, -1, ARG_OXO_X, 
  -1,

  EVT_OXO_1O, 
  'J', ARG_OXO_1A, -1, 0, 
  'J', ARG_OXO_1B, -1, 0, 
  'H', -1, -1, ARG_OXO_1A, 
  'H', -1, -1, ARG_OXO_1C, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_1X, 
  'J', ARG_OXO_1A, -1, 0, 
  'J', ARG_OXO_1B, -1, 0, 
  'H', -1, -1, ARG_OXO_1B, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_2O, 
  'J', ARG_OXO_2A, -1, 0, 
  'J', ARG_OXO_2B, -1, 0, 
  'H', -1, -1, ARG_OXO_2A, 
  'H', -1, -1, ARG_OXO_2C, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_2X, 
  'J', ARG_OXO_2A, -1, 0, 
  'J', ARG_OXO_2B, -1, 0, 
  'H', -1, -1, ARG_OXO_2B, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_3O, 
  'J', ARG_OXO_3A, -1, 0, 
  'J', ARG_OXO_3B, -1, 0, 
  'H', -1, -1, ARG_OXO_3A, 
  'H', -1, -1, ARG_OXO_3C, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_3X, 
  'J', ARG_OXO_3A, -1, 0, 
  'J', ARG_OXO_3B, -1, 0, 
  'H', -1, -1, ARG_OXO_3B, 
  'S', -1, -1, SUB_OXO_MATCH123,
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,

  EVT_OXO_4, 
  'J', ARG_OXO_4A, -1, 0, 
  'J', ARG_OXO_4B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_4B, ARG_OXO_4A, 
  'H', ARG_OXO_X, ARG_OXO_4B, ARG_OXO_4C, 
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH456,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_5, 
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
  
  EVT_OXO_6, 
  'J', ARG_OXO_6A, -1, 0, 
  'J', ARG_OXO_6B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_6B, ARG_OXO_6A, 
  'H', ARG_OXO_X, ARG_OXO_6B, ARG_OXO_6C, 
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH456,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_7, 
  'J', ARG_OXO_7A, -1, 0, 
  'J', ARG_OXO_7B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_7B, ARG_OXO_7A, 
  'H', ARG_OXO_X, ARG_OXO_7B, ARG_OXO_7C, 
  'S', -1, -1, SUB_OXO_MATCH789,
  'S', -1, -1, SUB_OXO_MATCH147,
  'S', -1, -1, SUB_OXO_MATCH357,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_8, 
  'J', ARG_OXO_8A, -1, 0, 
  'J', ARG_OXO_8B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_8B, ARG_OXO_8A, 
  'H', ARG_OXO_X, ARG_OXO_8B, ARG_OXO_8C, 
  'S', -1, -1, SUB_OXO_MATCH258,
  'S', -1, -1, SUB_OXO_MATCH789,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,
  
  EVT_OXO_9, 
  'J', ARG_OXO_9A, -1, 0, 
  'J', ARG_OXO_9B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_9B, ARG_OXO_9A, 
  'H', ARG_OXO_X, ARG_OXO_9B, ARG_OXO_9C, 
  'S', -1, -1, SUB_OXO_MATCH159,
  'S', -1, -1, SUB_OXO_MATCH369,
  'S', -1, -1, SUB_OXO_MATCH789,
  'S', -1, -1, SUB_OXO_MATCHALL,
  -1,

  EVT_OXO_TOGGLE, 
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
  'T', -1, -1, OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH456,
  'J', ARG_OXO_4A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_6A, 3, -1,
  'J', ARG_OXO_4B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_6B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,

  SUB_OXO_MATCH789,
  'J', ARG_OXO_7A, 0,  4,
  'J', ARG_OXO_8A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_7B, 0, -1,
  'J', ARG_OXO_8B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,

  SUB_OXO_MATCH147, 
  'J', ARG_OXO_1A, 0,  4,
  'J', ARG_OXO_4A, 0, -1,
  'J', ARG_OXO_7A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_4B, 0, -1,
  'J', ARG_OXO_7B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH258, 
  'J', ARG_OXO_2A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_8A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_8B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH369,
  'J', ARG_OXO_3A, 0,  4,
  'J', ARG_OXO_6A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_3B, 0, -1,
  'J', ARG_OXO_6B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH159, 
  'J', ARG_OXO_1A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,
  
  SUB_OXO_MATCH357,
  'J', ARG_OXO_3A, 0,  4,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_7A, 3, -1,
  'J', ARG_OXO_3B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_7B, 0, -1,
  'T', -1, -1, OXO_MATCH_ROW,
  -1,

  SUB_OXO_MATCHALL,
  'T', -1, -1, OXO_SET_SQUARE, 
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
  'T', 9, OXO_MATCH_ALL, -1,
  -1,
    
  -1,
};      

  
