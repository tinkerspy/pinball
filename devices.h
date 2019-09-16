#pragma once

/* Standard firmware for the Atm_led_device programmable pinball device class
 *  
 *  single_led  ???
 *  quad_led -> ledbank
 *  dual_slingshot ???
 *  dual_kicker
 *  switchbank
 *  dual_combo_leds
 *  bumper
 *  dual_target
 *  dual flipper
 *  tictactoe
 *  
 */

namespace std_firmware {

// Ledbank: 8 leds (or coils) that can be turned on (pulsed)

enum { IN_LBANK_INIT, IN_LBANK_ON0, IN_LBANK_ON1, IN_LBANK_ON2, IN_LBANK_ON3, IN_LBANK_ON4, IN_LBANK_ON5, IN_LBANK_ON6, IN_LBANK_ON7, 
          IN_LBANK_OFF0, IN_LBANK_OFF1, IN_LBANK_OFF2, IN_LBANK_OFF3, IN_LBANK_OFF4, IN_LBANK_OFF5, IN_LBANK_OFF6, IN_LBANK_OFF7,
          IN_LBANK_ALL_ON, IN_LBANK_ALL_OFF };
enum { ARG_LBANK_LED0, ARG_LBANK_LED1, ARG_LBANK_LED2, ARG_LBANK_LED3, ARG_LBANK_LED4, ARG_LBANK_LED5, ARG_LBANK_LED6, ARG_LBANK_LED7 };
enum { OUT_LBANK_ON0, OUT_LBANK_ON1, OUT_LBANK_ON2, OUT_LBANK_ON3, OUT_LBANK_ON4, OUT_LBANK_ON5, OUT_LBANK_ON6, OUT_LBANK_ON7, 
        OUT_LBANK_OFF0, OUT_LBANK_OFF1, OUT_LBANK_OFF2, OUT_LBANK_OFF3, OUT_LBANK_OFF4, OUT_LBANK_OFF5, OUT_LBANK_OFF6, OUT_LBANK_OFF7 }; 

int16_t ledbank_firmware[] {
  IN_LBANK_INIT, 
  IN_LBANK_ON0, 
  IN_LBANK_ON1,
  IN_LBANK_ON2,
  IN_LBANK_ON3,
  IN_LBANK_ON4, 
  IN_LBANK_ON5,
  IN_LBANK_ON6,
  IN_LBANK_ON7,
  IN_LBANK_OFF0, 
  IN_LBANK_OFF1,
  IN_LBANK_OFF2,
  IN_LBANK_OFF3,
  IN_LBANK_OFF4, 
  IN_LBANK_OFF5,
  IN_LBANK_OFF6,
  IN_LBANK_OFF7,
  IN_LBANK_ALL_ON, 
  IN_LBANK_ALL_OFF,
  -1,

  IN_LBANK_INIT,
  'S', -1, -1, IN_LBANK_ALL_OFF,  
  -1,

  IN_LBANK_ALL_ON,
  'H', -1, -1, ARG_LBANK_LED0,
  'H', -1, -1, ARG_LBANK_LED1,
  'H', -1, -1, ARG_LBANK_LED2,
  'H', -1, -1, ARG_LBANK_LED3,
  'H', -1, -1, ARG_LBANK_LED4,
  'H', -1, -1, ARG_LBANK_LED5,
  'H', -1, -1, ARG_LBANK_LED6,
  'H', -1, -1, ARG_LBANK_LED7,
  -1,

  IN_LBANK_ALL_OFF,
  'L', -1, -1, ARG_LBANK_LED0,
  'L', -1, -1, ARG_LBANK_LED1,
  'L', -1, -1, ARG_LBANK_LED2,
  'L', -1, -1, ARG_LBANK_LED3,
  'L', -1, -1, ARG_LBANK_LED4,
  'L', -1, -1, ARG_LBANK_LED5,
  'L', -1, -1, ARG_LBANK_LED6,
  'L', -1, -1, ARG_LBANK_LED7,
  -1,

  IN_LBANK_ON0,
  'H', -1, -1, ARG_LBANK_LED0,
  'T', -1, -1, OUT_LBANK_ON0,
  -1,
  
  IN_LBANK_OFF0,
  'L', -1, -1, ARG_LBANK_LED0,
  'T', -1, -1, OUT_LBANK_OFF0,
  -1,
  
  IN_LBANK_ON1,
  'H', -1, -1, ARG_LBANK_LED1,
  'T', -1, -1, OUT_LBANK_ON1,
  -1,
  
  IN_LBANK_OFF1,
  'L', -1, -1, ARG_LBANK_LED1,
  'T', -1, -1, OUT_LBANK_OFF1,
  -1,
  
  IN_LBANK_ON2,
  'H', -1, -1, ARG_LBANK_LED2,
  'T', -1, -1, OUT_LBANK_ON2,
  -1,
  
  IN_LBANK_OFF2,
  'L', -1, -1, ARG_LBANK_LED2,
  'T', -1, -1, OUT_LBANK_OFF2,
  -1,
  
  IN_LBANK_ON3,
  'H', -1, -1, ARG_LBANK_LED3,
  'T', -1, -1, OUT_LBANK_ON3,
  -1,
  
  IN_LBANK_OFF3,
  'L', -1, -1, ARG_LBANK_LED3,
  'T', -1, -1, OUT_LBANK_OFF3,
  -1,
  
  IN_LBANK_ON4,
  'H', -1, -1, ARG_LBANK_LED4,
  'T', -1, -1, OUT_LBANK_ON4,
  -1,
  
  IN_LBANK_OFF4,
  'L', -1, -1, ARG_LBANK_LED4,
  'T', -1, -1, OUT_LBANK_OFF4,
  -1,
  
  IN_LBANK_ON5,
  'H', -1, -1, ARG_LBANK_LED5,
  'T', -1, -1, OUT_LBANK_ON5,
  -1,
  
  IN_LBANK_OFF5,
  'L', -1, -1, ARG_LBANK_LED5,
  'T', -1, -1, OUT_LBANK_OFF5,
  -1,
  
  IN_LBANK_ON6,
  'H', -1, -1, ARG_LBANK_LED6,
  'T', -1, -1, OUT_LBANK_ON6,
  -1,
  
  IN_LBANK_OFF6,
  'L', -1, -1, ARG_LBANK_LED6,
  'T', -1, -1, OUT_LBANK_OFF6,
  -1,
  
  IN_LBANK_ON7,
  'H', -1, -1, ARG_LBANK_LED7,
  'T', -1, -1, OUT_LBANK_ON7,
  -1,
  
  IN_LBANK_OFF7,
  'L', -1, -1, ARG_LBANK_LED7,
  'T', -1, -1, OUT_LBANK_OFF7,
  -1,

  -1,
};


// Two independent slingshots (share score & kick)

enum { IN_SLING_INIT, IN_SLING_PRESS_L, IN_SLING_RELEASE_L, IN_SLING_PRESS_R, IN_SLING_RELEASE_R };
enum { OUT_SLING_SCORE, OUT_SLING_KICK };
enum { ARG_SLING_COIL_L, ARG_SLING_COIL_R };

int16_t dual_slingshot_firmware[] {
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


// Dual kickers, same score & same kick, persistent (could work for slingshots, except non-persistent)

enum { IN_KICKER_INIT, IN_KICKER_PRESS_L, IN_KICKER_RELEASE_L, IN_KICKER_PRESS_R, IN_KICKER_RELEASE_R, IN_KICKER_ON, IN_KICKER_OFF };
enum { OUT_KICKER_SCORE_LIT, OUT_KICKER_SCORE_UNLIT, OUT_KICKER_SCORE, OUT_KICKER_KICK_LIT, OUT_KICKER_KICK_UNLIT, OUT_KICKER_KICK };
enum { ARG_KICKER_COIL_L, ARG_KICKER_COIL_R, ARG_KICKER_LED_L, ARG_KICKER_LED_R };

int16_t dual_kicker_firmware[] {
  IN_KICKER_INIT, 
  IN_KICKER_PRESS_L, 
  IN_KICKER_RELEASE_L,
  IN_KICKER_PRESS_R, 
  IN_KICKER_RELEASE_R,
  IN_KICKER_ON,
  IN_KICKER_OFF,
  -1,

  IN_KICKER_INIT,
  'P', -1, -1, 1,  // Persistent 
  'L', -1, -1, ARG_KICKER_LED_L,
  'L', -1, -1, ARG_KICKER_LED_R,
  -1,

  IN_KICKER_PRESS_L,
  'H', -1, -1, ARG_KICKER_COIL_L,
  'T', -1, -1, OUT_KICKER_KICK,
  'T', -1, -1, OUT_KICKER_SCORE,
  'J', ARG_KICKER_LED_L, 0, 3,
  'T', -1, -1, OUT_KICKER_SCORE_LIT,
  'T', -1, -1, OUT_KICKER_KICK_LIT,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_KICKER_SCORE_UNLIT,
  'T', -1, -1, OUT_KICKER_KICK_UNLIT,
  -1,
  
  IN_KICKER_PRESS_R,
  'H', -1, -1, ARG_KICKER_COIL_R,
  'T', -1, -1, OUT_KICKER_KICK,
  'T', -1, -1, OUT_KICKER_SCORE,
  'J', ARG_KICKER_LED_R, 0, 3,
  'T', -1, -1, OUT_KICKER_SCORE_LIT,
  'T', -1, -1, OUT_KICKER_KICK_LIT,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_KICKER_SCORE_UNLIT,
  'T', -1, -1, OUT_KICKER_KICK_UNLIT,
  -1,

  IN_KICKER_ON,
  'H', -1, -1, ARG_KICKER_LED_L,
  'H', -1, -1, ARG_KICKER_LED_R,
  -1,
  
  IN_KICKER_OFF,
  'L', -1, -1, ARG_KICKER_LED_L,
  'L', -1, -1, ARG_KICKER_LED_R,
  -1,
 
  -1,
};


// Basic 8-way switchbank device without leds

enum { IN_SBANK_INIT, IN_SBANK_PRESS0, IN_SBANK_RELEASE0, IN_SBANK_PRESS1, IN_SBANK_RELEASE1, IN_SBANK_PRESS2, IN_SBANK_RELEASE2, // Inputs
        IN_SBANK_PRESS3, IN_SBANK_RELEASE3, IN_SBANK_PRESS4, IN_SBANK_RELEASE4, IN_SBANK_PRESS5, IN_SBANK_RELEASE5, 
        IN_SBANK_PRESS6, IN_SBANK_RELEASE6, IN_SBANK_PRESS7, IN_SBANK_RELEASE7 };
enum { OUT_SBANK_SCORE, OUT_SBANK0, OUT_SBANK1, OUT_SBANK2, OUT_SBANK3, OUT_SBANK4, OUT_SBANK5, OUT_SBANK6, OUT_SBANK7, 
        OUT_SBANK_SCORE0, OUT_SBANK_SCORE1, OUT_SBANK_SCORE2, OUT_SBANK_SCORE3, OUT_SBANK_SCORE4, OUT_SBANK_SCORE5, OUT_SBANK_SCORE6, OUT_SBANK_SCORE7 }; // Outputs

int16_t switchbank_firmware[] = {
  IN_SBANK_INIT, 
  IN_SBANK_PRESS0, 
  IN_SBANK_RELEASE0, 
  IN_SBANK_PRESS1, 
  IN_SBANK_RELEASE1, 
  IN_SBANK_PRESS2, 
  IN_SBANK_RELEASE2, 
  IN_SBANK_PRESS3, 
  IN_SBANK_RELEASE3, 
  IN_SBANK_PRESS4, 
  IN_SBANK_RELEASE4, 
  IN_SBANK_PRESS5, 
  IN_SBANK_RELEASE5,
  IN_SBANK_PRESS6, 
  IN_SBANK_RELEASE6,
  IN_SBANK_PRESS7, 
  IN_SBANK_RELEASE7,
  -1,

  IN_SBANK_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  IN_SBANK_PRESS0,
  'T', -1, -1, OUT_SBANK0,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS1,
  'T', -1, -1, OUT_SBANK1,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS2,
  'T', -1, -1, OUT_SBANK2,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS3,
  'T', -1, -1, OUT_SBANK3,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS4,
  'T', -1, -1, OUT_SBANK4,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS5,
  'T', -1, -1, OUT_SBANK5,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS6,
  'T', -1, -1, OUT_SBANK6,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS7,
  'T', -1, -1, OUT_SBANK7,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  -1,
};


// Dual lane device with leds (both lanes share the same function)

enum { IN_COMBO_INIT, IN_COMBO_PRESS0, IN_COMBO_RELEASE0, IN_COMBO_PRESS1, IN_COMBO_RELEASE1, IN_COMBO_ON, IN_COMBO_OFF };
enum { OUT_COMBO_SCORE, OUT_COMBO_SCORE_ON, OUT_COMBO_SCORE_OFF, OUT_COMBO_PRESS_ON, OUT_COMBO_PRESS_OFF };
enum { ARG_COMBO_LED0, ARG_COMBO_LED1 };

int16_t dual_combo_firmware[] = {
  IN_COMBO_INIT, 
  IN_COMBO_PRESS0, 
  IN_COMBO_RELEASE0, 
  IN_COMBO_PRESS1, 
  IN_COMBO_RELEASE1, 
  IN_COMBO_ON, 
  IN_COMBO_OFF, 
  -1,

  IN_COMBO_INIT,
  'P', -1, -1, 0,  // Not persistent 
  -1,

  IN_COMBO_PRESS0,
  'S', -1, -1, IN_COMBO_PRESS1,
  -1,

  IN_COMBO_PRESS1,
  'J', ARG_COMBO_LED0, 0, 3, 
  'T', -1, -1, OUT_COMBO_PRESS_ON,
  'T', -1, -1, OUT_COMBO_SCORE_ON,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_COMBO_PRESS_OFF,
  'T', -1, -1, OUT_COMBO_SCORE_OFF,
  -1,

  IN_COMBO_ON,
  'H', -1, -1, ARG_COMBO_LED0, 
  'H', -1, -1, ARG_COMBO_LED1, 
  -1,

  IN_COMBO_OFF,
  'L', -1, -1, ARG_COMBO_LED0, 
  'L', -1, -1, ARG_COMBO_LED1, 
  -1,

  -1,
};


// Single bumper device with led & coil

enum { IN_BUMPER_INIT, IN_BUMPER_PRESS, IN_BUMPER_RELEASE, IN_BUMPER_LIGHT_ON, IN_BUMPER_LIGHT_OFF }; // Inputs
enum { OUT_BUMPER_SCORE_LIT, OUT_BUMPER_SCORE_UNLIT, OUT_BUMPER_LIGHT_ON, OUT_BUMPER_LIGHT_OFF }; // Outputs
enum { ARG_BUMPER_COIL, ARG_BUMPER_LED }; // Arguments (leds)

int16_t bumper_firmware[] = {
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

enum { IN_TARGET_INIT, IN_TARGET_PRESS_A, IN_TARGET_RELEASE_A, IN_TARGET_PRESS_B, IN_TARGET_RELEASE_B, IN_TARGET_CLEAR };
enum { OUT_TARGET_LED_A_ON, OUT_TARGET_LED_B_ON, OUT_TARGET_LED_A_OFF, OUT_TARGET_LED_B_OFF, OUT_TARGET_ALL_ON, OUT_TARGET_ALL_OFF };
enum { ARG_TARGET_LED_A, ARG_TARGET_LED_B };

int16_t dual_target_firmware[] = {
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


// Dual flipper device (2 independent flippers + two optional slaves)

enum { IN_FLIPPER_INIT, IN_FLIPPER_PRESS_L, IN_FLIPPER_RELEASE_L, IN_FLIPPER_PRESS_R, IN_FLIPPER_RELEASE_R };
enum { ARG_FLIPPER_COIL_L, ARG_FLIPPER_COIL_R, ARG_FLIPPER_COIL_L1, ARG_FLIPPER_COIL_R1 }; 

int16_t dual_flipper_firmware[] = {
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
  'L', -1, -1, ARG_FLIPPER_COIL_L1,
  'L', -1, -1, ARG_FLIPPER_COIL_R1,
  -1,

  IN_FLIPPER_PRESS_L,
  'H', -1, -1, ARG_FLIPPER_COIL_L,
  'H', -1, -1, ARG_FLIPPER_COIL_L1,
  -1,

  IN_FLIPPER_RELEASE_L,
  'L', -1, -1, ARG_FLIPPER_COIL_L,
  'L', -1, -1, ARG_FLIPPER_COIL_L1,
  -1,

  IN_FLIPPER_PRESS_R,
  'H', -1, -1, ARG_FLIPPER_COIL_R,
  'H', -1, -1, ARG_FLIPPER_COIL_R1,
  -1,

  IN_FLIPPER_RELEASE_R,
  'L', -1, -1, ARG_FLIPPER_COIL_R,
  'L', -1, -1, ARG_FLIPPER_COIL_R1,
  -1,

  -1,
};


// Williams OXO tic-tac-toe game

enum { // Inputs & Subs
  IN_OXO_INIT,
  IN_OXO_1O, IN_OXO_1X, IN_OXO_2O, IN_OXO_2X, IN_OXO_3O, IN_OXO_3X,             
  IN_OXO_4, IN_OXO_5, IN_OXO_6, IN_OXO_7, IN_OXO_8, IN_OXO_9, 
  IN_OXO_TOGGLE, 
  SUB_OXO_MATCH_123, SUB_OXO_MATCH_456, SUB_OXO_MATCH_789,
  SUB_OXO_MATCH_147, SUB_OXO_MATCH_258, SUB_OXO_MATCH_369,
  SUB_OXO_MATCH_159, SUB_OXO_MATCH_357, 
  SUB_OXO_MATCH_ALL,
};

enum { OUT_OXO_SCORE, OUT_OXO_WIN_ROW, OUT_OXO_WIN_ALL }; // Outputs

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

int16_t tictactoe_firmware[] = {
  IN_OXO_INIT,
  IN_OXO_1O, IN_OXO_1X, IN_OXO_2O, IN_OXO_2X, IN_OXO_3O, IN_OXO_3X,             
  IN_OXO_4, IN_OXO_5, IN_OXO_6, IN_OXO_7, IN_OXO_8, IN_OXO_9, 
  IN_OXO_TOGGLE, 
  SUB_OXO_MATCH_123, SUB_OXO_MATCH_456, SUB_OXO_MATCH_789,
  SUB_OXO_MATCH_147, SUB_OXO_MATCH_258, SUB_OXO_MATCH_369,
  SUB_OXO_MATCH_159, SUB_OXO_MATCH_357, 
  SUB_OXO_MATCH_ALL,
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
  'S', -1, -1, SUB_OXO_MATCH_123,
  'S', -1, -1, SUB_OXO_MATCH_147,
  'S', -1, -1, SUB_OXO_MATCH_159,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_1X, 
  'J', ARG_OXO_1A, -1, 0, 
  'J', ARG_OXO_1B, -1, 0, 
  'H', -1, -1, ARG_OXO_1B, 
  'S', -1, -1, SUB_OXO_MATCH_123,
  'S', -1, -1, SUB_OXO_MATCH_147,
  'S', -1, -1, SUB_OXO_MATCH_159,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_2O, 
  'J', ARG_OXO_2A, -1, 0, 
  'J', ARG_OXO_2B, -1, 0, 
  'H', -1, -1, ARG_OXO_2A, 
  'H', -1, -1, ARG_OXO_2C, 
  'S', -1, -1, SUB_OXO_MATCH_123,
  'S', -1, -1, SUB_OXO_MATCH_258,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_2X, 
  'J', ARG_OXO_2A, -1, 0, 
  'J', ARG_OXO_2B, -1, 0, 
  'H', -1, -1, ARG_OXO_2B, 
  'S', -1, -1, SUB_OXO_MATCH_123,
  'S', -1, -1, SUB_OXO_MATCH_258,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_3O, 
  'J', ARG_OXO_3A, -1, 0, 
  'J', ARG_OXO_3B, -1, 0, 
  'H', -1, -1, ARG_OXO_3A, 
  'H', -1, -1, ARG_OXO_3C, 
  'S', -1, -1, SUB_OXO_MATCH_123,
  'S', -1, -1, SUB_OXO_MATCH_369,
  'S', -1, -1, SUB_OXO_MATCH_357,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_3X, 
  'J', ARG_OXO_3A, -1, 0, 
  'J', ARG_OXO_3B, -1, 0, 
  'H', -1, -1, ARG_OXO_3B, 
  'S', -1, -1, SUB_OXO_MATCH_123,
  'S', -1, -1, SUB_OXO_MATCH_369,
  'S', -1, -1, SUB_OXO_MATCH_357,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,

  IN_OXO_4, 
  'J', ARG_OXO_4A, -1, 0, 
  'J', ARG_OXO_4B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_4B, ARG_OXO_4A, 
  'H', ARG_OXO_X, ARG_OXO_4B, ARG_OXO_4C, 
  'S', -1, -1, SUB_OXO_MATCH_147,
  'S', -1, -1, SUB_OXO_MATCH_456,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_5, 
  'J', ARG_OXO_5A, -1, 0, 
  'J', ARG_OXO_5B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_5B, ARG_OXO_5A, 
  'H', ARG_OXO_X, ARG_OXO_5B, ARG_OXO_5C, 
  'S', -1, -1, SUB_OXO_MATCH_159,
  'S', -1, -1, SUB_OXO_MATCH_258,
  'S', -1, -1, SUB_OXO_MATCH_357,
  'S', -1, -1, SUB_OXO_MATCH_456,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_6, 
  'J', ARG_OXO_6A, -1, 0, 
  'J', ARG_OXO_6B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_6B, ARG_OXO_6A, 
  'H', ARG_OXO_X, ARG_OXO_6B, ARG_OXO_6C, 
  'S', -1, -1, SUB_OXO_MATCH_369,
  'S', -1, -1, SUB_OXO_MATCH_456,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_7, 
  'J', ARG_OXO_7A, -1, 0, 
  'J', ARG_OXO_7B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_7B, ARG_OXO_7A, 
  'H', ARG_OXO_X, ARG_OXO_7B, ARG_OXO_7C, 
  'S', -1, -1, SUB_OXO_MATCH_789,
  'S', -1, -1, SUB_OXO_MATCH_147,
  'S', -1, -1, SUB_OXO_MATCH_357,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_8, 
  'J', ARG_OXO_8A, -1, 0, 
  'J', ARG_OXO_8B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_8B, ARG_OXO_8A, 
  'H', ARG_OXO_X, ARG_OXO_8B, ARG_OXO_8C, 
  'S', -1, -1, SUB_OXO_MATCH_258,
  'S', -1, -1, SUB_OXO_MATCH_789,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,
  
  IN_OXO_9, 
  'J', ARG_OXO_9A, -1, 0, 
  'J', ARG_OXO_9B, -1, 0, 
  'H', ARG_OXO_X, ARG_OXO_9B, ARG_OXO_9A, 
  'H', ARG_OXO_X, ARG_OXO_9B, ARG_OXO_9C, 
  'S', -1, -1, SUB_OXO_MATCH_159,
  'S', -1, -1, SUB_OXO_MATCH_369,
  'S', -1, -1, SUB_OXO_MATCH_789,
  'S', -1, -1, SUB_OXO_MATCH_ALL,
  -1,

  IN_OXO_TOGGLE, 
  'J', ARG_OXO_X, 0, 3, 
  'L', -1, -1, ARG_OXO_X, 
  'H', -1, -1, ARG_OXO_O, 
  'J', -1, -1, -1, 
  'L', -1, -1, ARG_OXO_O, 
  'H', -1, -1, ARG_OXO_X, 
  -1,    

  SUB_OXO_MATCH_123,
  'J', ARG_OXO_1A, 0,  2,
  'J', ARG_OXO_2A, 0, -1,
  'J', ARG_OXO_3A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_2B, 0, -1,
  'J', ARG_OXO_3B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,
  
  SUB_OXO_MATCH_456,
  'J', ARG_OXO_4A, 0,  2,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_6A, 3, -1,
  'J', ARG_OXO_4B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_6B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,

  SUB_OXO_MATCH_789,
  'J', ARG_OXO_7A, 0,  2,
  'J', ARG_OXO_8A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_7B, 0, -1,
  'J', ARG_OXO_8B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,

  SUB_OXO_MATCH_147, 
  'J', ARG_OXO_1A, 0,  2,
  'J', ARG_OXO_4A, 0, -1,
  'J', ARG_OXO_7A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_4B, 0, -1,
  'J', ARG_OXO_7B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,
  
  SUB_OXO_MATCH_258, 
  'J', ARG_OXO_2A, 0,  2,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_8A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_8B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,
  
  SUB_OXO_MATCH_369,
  'J', ARG_OXO_3A, 0,  2,
  'J', ARG_OXO_6A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_3B, 0, -1,
  'J', ARG_OXO_6B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,
  
  SUB_OXO_MATCH_159, 
  'J', ARG_OXO_1A, 0,  2,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_9A, 3, -1,
  'J', ARG_OXO_1B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_9B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,
  
  SUB_OXO_MATCH_357,
  'J', ARG_OXO_3A, 0,  2,
  'J', ARG_OXO_5A, 0, -1,
  'J', ARG_OXO_7A, 3, -1,
  'J', ARG_OXO_3B, 0, -1,
  'J', ARG_OXO_5B, 0, -1,
  'J', ARG_OXO_7B, 0, -1,
  'T', -1, -1, OUT_OXO_WIN_ROW,
  -1,

  SUB_OXO_MATCH_ALL,
  'T', -1, -1, OUT_OXO_SCORE, 
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
  'T', 9, OUT_OXO_WIN_ALL, -1,
  -1,
    
  -1,
};      

};
