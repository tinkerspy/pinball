#pragma once

/* Standard firmware for the Atm_device programmable pinball device class
 *  
 *  ledbank
 *  dual_kicker
 *  switchbank
 *  dual_combo
 *  bumper
 *  dual_target
 *  dual flipper
 *  scalar
 *  counter_em4d1w
 *  
 *  playfield.device( <switch-id> ); // Instantiates object
 *  playfield.device( <switch-id>, <led-id>, <firmware> ); // Initializes object
 */

using namespace instruction_set;

namespace standard_firmware {

/*
 * Firmware: ledbank_firmware
 * Maximum switches: 0
 * Led arguments: LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7
 * Description: 
 *   8 leds (or coils) that can be turned on/off (or pulsed) 
 */

enum { IN_LBANK_INIT, IN_LBANK_ON0, IN_LBANK_ON1, IN_LBANK_ON2, IN_LBANK_ON3, IN_LBANK_ON4, IN_LBANK_ON5, IN_LBANK_ON6, IN_LBANK_ON7, 
          IN_LBANK_OFF0, IN_LBANK_OFF1, IN_LBANK_OFF2, IN_LBANK_OFF3, IN_LBANK_OFF4, IN_LBANK_OFF5, IN_LBANK_OFF6, IN_LBANK_OFF7,
          IN_LBANK_ON, IN_LBANK_OFF };
enum { ARG_LBANK_LED0, ARG_LBANK_LED1, ARG_LBANK_LED2, ARG_LBANK_LED3, ARG_LBANK_LED4, ARG_LBANK_LED5, ARG_LBANK_LED6, ARG_LBANK_LED7 };
enum { OUT_LBANK_ON0, OUT_LBANK_ON1, OUT_LBANK_ON2, OUT_LBANK_ON3, OUT_LBANK_ON4, OUT_LBANK_ON5, OUT_LBANK_ON6, OUT_LBANK_ON7, 
        OUT_LBANK_OFF0, OUT_LBANK_OFF1, OUT_LBANK_OFF2, OUT_LBANK_OFF3, OUT_LBANK_OFF4, OUT_LBANK_OFF5, OUT_LBANK_OFF6, OUT_LBANK_OFF7 }; 
enum { REG_LBANK_STATE, REG_LBANK_DEFAULT };

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
  IN_LBANK_ON, 
  IN_LBANK_OFF,
  -1,

  IN_LBANK_INIT,
  'P', -1, -1, 1,
  'X',  1,  0, 4, // Skip the rest if not on 1st INIT
  'R', -1, -1, REG_LBANK_STATE,
  'C',  0,  2, 0,
  'R', -1, -1, REG_LBANK_DEFAULT,
  'I', -1, -1, 1,
  'R', -1, -1, REG_LBANK_STATE,   
  'I', -1, -1, -1,
  'S', -1, -1, IN_LBANK_OFF,  
  'R', -1, -1, REG_LBANK_DEFAULT,
  'C',  0,  1, 0,
  'S', -1, -1, IN_LBANK_ON,    
  -1,

  IN_LBANK_ON,
  'I', -1, -1, 1,
  'H', -1, -1, ARG_LBANK_LED0,
  'H', -1, -1, ARG_LBANK_LED1,
  'H', -1, -1, ARG_LBANK_LED2,
  'H', -1, -1, ARG_LBANK_LED3,
  'H', -1, -1, ARG_LBANK_LED4,
  'H', -1, -1, ARG_LBANK_LED5,
  'H', -1, -1, ARG_LBANK_LED6,
  'H', -1, -1, ARG_LBANK_LED7,
  -1,

  IN_LBANK_OFF,
  'I', -1, -1, -1,
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
  'I', -1, -1, 1,
  'H', -1, -1, ARG_LBANK_LED0,
  'T', -1, -1, OUT_LBANK_ON0,
  -1,
  
  IN_LBANK_OFF0,
  'I', -1, -1, -1,
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

/*
 * Firmware: dual_kicker_firmware
 * Maximum switches: 2
 * Led arguments: COIL_L, COIL_R, LED_L, LED_R
 * Description: 
 *   Dual kickers, same score & same kick, can be made persistent 
 *   could work for slingshots as well
 */

enum { IN_KICKER_INIT, IN_KICKER_PRESS_L, IN_KICKER_RELEASE_L, IN_KICKER_PRESS_R, IN_KICKER_RELEASE_R, IN_KICKER_ON, IN_KICKER_OFF, IN_KICKER_PERSIST };
enum { OUT_KICKER_SCORE_LIT, OUT_KICKER_SCORE_UNLIT, OUT_KICKER_SCORE, OUT_KICKER_PRESS_LIT, OUT_KICKER_PRESS_UNLIT, OUT_KICKER_PRESS };
enum { ARG_KICKER_COIL_L, ARG_KICKER_COIL_R, ARG_KICKER_LED_L, ARG_KICKER_LED_R };

int16_t dual_kicker_firmware[] {
  IN_KICKER_INIT, 
  IN_KICKER_PRESS_L, 
  IN_KICKER_RELEASE_L, 
  IN_KICKER_PRESS_R, 
  IN_KICKER_RELEASE_R, 
  IN_KICKER_ON, 
  IN_KICKER_OFF, 
  IN_KICKER_PERSIST,
  -1,

  IN_KICKER_INIT,
  'P', -1, -1, 0,  // Default not persistent 
  'L', -1, -1, ARG_KICKER_LED_L,
  'L', -1, -1, ARG_KICKER_LED_R,
  -1,

  IN_KICKER_PERSIST,
  'P', -1, -1, 1,  // Persistent 
  -1,

  IN_KICKER_PRESS_L,
  'H', -1, -1, ARG_KICKER_COIL_L,
  'T', -1, -1, OUT_KICKER_PRESS,
  'T', -1, -1, OUT_KICKER_SCORE,
  'J', ARG_KICKER_LED_L, 0, 3,
  'T', -1, -1, OUT_KICKER_SCORE_LIT,
  'T', -1, -1, OUT_KICKER_PRESS_LIT,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_KICKER_SCORE_UNLIT,
  'T', -1, -1, OUT_KICKER_PRESS_UNLIT,
  -1,
  
  IN_KICKER_PRESS_R,
  'H', -1, -1, ARG_KICKER_COIL_R,
  'T', -1, -1, OUT_KICKER_PRESS,
  'T', -1, -1, OUT_KICKER_SCORE,
  'J', ARG_KICKER_LED_R, 0, 3,
  'T', -1, -1, OUT_KICKER_SCORE_LIT,
  'T', -1, -1, OUT_KICKER_PRESS_LIT,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_KICKER_SCORE_UNLIT,
  'T', -1, -1, OUT_KICKER_PRESS_UNLIT,
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

/*
 * Firmware: switchbank_firmware
 * Maximum switches: 8
 * Led arguments: <none>
 * Description: 
 *   Basic 8-way switchbank device without leds
 */

enum { IN_SBANK_INIT, IN_SBANK_PRESS0, IN_SBANK_RELEASE0, IN_SBANK_PRESS1, IN_SBANK_RELEASE1, IN_SBANK_PRESS2, IN_SBANK_RELEASE2, // Inputs
        IN_SBANK_PRESS3, IN_SBANK_RELEASE3, IN_SBANK_PRESS4, IN_SBANK_RELEASE4, IN_SBANK_PRESS5, IN_SBANK_RELEASE5, 
        IN_SBANK_PRESS6, IN_SBANK_RELEASE6, IN_SBANK_PRESS7, IN_SBANK_RELEASE7 };
enum { OUT_SBANK_SCORE, OUT_SBANK_PRESS0, OUT_SBANK_PRESS1, OUT_SBANK_PRESS2, OUT_SBANK_PRESS3, OUT_SBANK_PRESS4, OUT_SBANK_PRESS5, OUT_SBANK_PRESS6, OUT_SBANK_PRESS7, 
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
  'T', -1, -1, OUT_SBANK_PRESS0,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE0,
  -1,

  IN_SBANK_PRESS1,
  'T', -1, -1, OUT_SBANK_PRESS1,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE1,
  -1,

  IN_SBANK_PRESS2,
  'T', -1, -1, OUT_SBANK_PRESS2,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE2,
  -1,

  IN_SBANK_PRESS3,
  'T', -1, -1, OUT_SBANK_PRESS3,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE3,
  -1,

  IN_SBANK_PRESS4,
  'T', -1, -1, OUT_SBANK_PRESS4,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE4,
  -1,

  IN_SBANK_PRESS5,
  'T', -1, -1, OUT_SBANK_PRESS5,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE5,
  -1,

  IN_SBANK_PRESS6,
  'T', -1, -1, OUT_SBANK_PRESS6,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE6,
  -1,

  IN_SBANK_PRESS7,
  'T', -1, -1, OUT_SBANK_PRESS7,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE7,
  -1,

  -1,
};

/*
 * Firmware: dual_combo_firmware
 * Maximum switches: 2
 * Led arguments: LED0, LED1
 * Description: 
 *   Dual lane device with leds (both lanes share the same function)
 */

enum { IN_COMBO_INIT, IN_COMBO_PRESS0, IN_COMBO_RELEASE0, IN_COMBO_PRESS1, IN_COMBO_RELEASE1, IN_COMBO_ON, IN_COMBO_OFF };
enum { OUT_COMBO_SCORE, OUT_COMBO_SCORE_LIT, OUT_COMBO_SCORE_UNLIT, OUT_COMBO_PRESS_LIT, OUT_COMBO_PRESS_UNLIT, OUT_COMBO_PRESS0_LIT, OUT_COMBO_PRESS0_UNLIT, OUT_COMBO_PRESS1_LIT, OUT_COMBO_PRESS1_UNLIT };
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
  'S', -1, -1, IN_COMBO_OFF,
  -1,

  IN_COMBO_PRESS0,
  'T', -1, -1, OUT_COMBO_SCORE, 
  'J', ARG_COMBO_LED0, 0, 4, 
  'T', -1, -1, OUT_COMBO_PRESS_LIT,
  'T', -1, -1, OUT_COMBO_PRESS0_LIT,
  'T', -1, -1, OUT_COMBO_SCORE_LIT,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_COMBO_PRESS_UNLIT,
  'T', -1, -1, OUT_COMBO_PRESS0_UNLIT,
  'T', -1, -1, OUT_COMBO_SCORE_UNLIT,
  -1,

  IN_COMBO_PRESS1,
  'T', -1, -1, OUT_COMBO_SCORE, 
  'J', ARG_COMBO_LED0, 0, 4, 
  'T', -1, -1, OUT_COMBO_PRESS_LIT,
  'T', -1, -1, OUT_COMBO_PRESS1_LIT,
  'T', -1, -1, OUT_COMBO_SCORE_LIT,
  'J', -1, -1, -1,
  'T', -1, -1, OUT_COMBO_PRESS_UNLIT,
  'T', -1, -1, OUT_COMBO_PRESS1_UNLIT,
  'T', -1, -1, OUT_COMBO_SCORE_UNLIT,
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


/*
 * Firmware: bumper_firmware
 * Maximum switches: 1
 * Led arguments: COIL, LED
 * Description: 
 *   Single bumper device with led & coil
 */

enum { IN_BUMPER_INIT, IN_BUMPER_PRESS, IN_BUMPER_RELEASE, IN_BUMPER_LIGHT_ON, IN_BUMPER_LIGHT_OFF }; // Inputs
enum { OUT_BUMPER_SCORE, OUT_BUMPER_SCORE_LIT, OUT_BUMPER_SCORE_UNLIT, OUT_BUMPER_LIGHT_ON, OUT_BUMPER_LIGHT_OFF }; // Outputs
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
  'T', -1, -1, OUT_BUMPER_SCORE,        // Emit score event
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


/*
 * Firmware: dual_target_firmware
 * Maximum switches: 2
 * Led arguments: LED0, LED1
 * Description: 
 *   Dual target device
 */

enum { IN_TARGET_INIT, IN_TARGET_PRESS0, IN_TARGET_RELEASE0, IN_TARGET_PRESS1, IN_TARGET_RELEASE1, IN_TARGET_CLEAR };
enum { OUT_TARGET_LED0_ON, OUT_TARGET_LED1_ON, OUT_TARGET_LED0_OFF, OUT_TARGET_LED1_OFF, OUT_TARGET_ALL_ON, OUT_TARGET_ALL_OFF, OUT_TARGET_SCORE };
enum { ARG_TARGET_LED0, ARG_TARGET_LED1 };

int16_t dual_target_firmware[] = {
  IN_TARGET_INIT,    
  IN_TARGET_PRESS0,
  IN_TARGET_RELEASE0,  
  IN_TARGET_PRESS1,
  IN_TARGET_RELEASE1,  
  IN_TARGET_CLEAR,  
  -1,

  IN_TARGET_INIT,
  'P', -1, -1, 0,
  'S', -1, -1, IN_TARGET_CLEAR,
  -1,
  
  IN_TARGET_PRESS0,
  'T', -1, -1, OUT_TARGET_SCORE,
  'J', ARG_TARGET_LED0, -1, 0,
  'H', -1, -1, ARG_TARGET_LED0,
  'T', -1, -1, OUT_TARGET_LED0_ON,
  'J', ARG_TARGET_LED1, 0, -1,
  'T', -1, -1, OUT_TARGET_ALL_ON,
  -1,
   
  IN_TARGET_PRESS1,
  'T', -1, -1, OUT_TARGET_SCORE,
  'J', ARG_TARGET_LED1, -1, 0,
  'H', -1, -1, ARG_TARGET_LED1,
  'T', -1, -1, OUT_TARGET_LED1_ON,
  'J', ARG_TARGET_LED0, 0, -1,
  'T', -1, -1, OUT_TARGET_ALL_ON,
  -1,

  IN_TARGET_CLEAR, // Only send off events if leds were on in the first place?
  'L', -1, -1, ARG_TARGET_LED0,
  'L', -1, -1, ARG_TARGET_LED1,
  'T', -1, -1, OUT_TARGET_LED0_OFF,
  'T', -1, -1, OUT_TARGET_LED1_OFF,
  'T', -1, -1, OUT_TARGET_ALL_OFF,
  -1,
  
  -1,
};


/*
 * Firmware: dual_flipper_firmware
 * Maximum switches: 2
 * Led arguments: COIL_L, COIL_R
 * Description: 
 *   Dual flipper device)
 */

enum { IN_FLIPPER_INIT, IN_FLIPPER_PRESS_L, IN_FLIPPER_RELEASE_L, IN_FLIPPER_PRESS_R, IN_FLIPPER_RELEASE_R };
enum { ARG_FLIPPER_COIL_L, ARG_FLIPPER_COIL_R }; 

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

/*
 * Firmware: scalar_firmware
 * Maximum switches: 0
 * Led arguments: LED0, LED1, LED2, LED3, LED4, BACKSTOP
 * Description: 
 *   Led position indicator with 5 places (0..4), used for # of players, # of ball, # of player up
 *   Advances until ARG_BACKTOP led is lit
 *   or until IN_SCALAR_FREEZE is called
 */

enum { IN_SCALAR_INIT, IN_SCALAR_ZERO, IN_SCALAR_ADVANCE, IN_SCALAR_FREEZE, IN_SCALAR_SEL0, IN_SCALAR_SEL1, IN_SCALAR_SEL2, IN_SCALAR_SEL3, IN_SCALAR_SEL4, SUB_SCALAR_CLEAR };
enum { ARG_LED0, ARG_LED1, ARG_LED2, ARG_LED3, ARG_LED4, ARG_BACKSTOP }; 
enum { REG_SCALAR_STATE, REG_SCALAR_BLOCK, REG_SCALAR };

int16_t scalar_firmware[] = {
  IN_SCALAR_INIT, 
  IN_SCALAR_ZERO, 
  IN_SCALAR_ADVANCE, 
  IN_SCALAR_FREEZE, 
  IN_SCALAR_SEL0, 
  IN_SCALAR_SEL1, 
  IN_SCALAR_SEL2, 
  IN_SCALAR_SEL3, 
  IN_SCALAR_SEL4, 
  SUB_SCALAR_CLEAR,  
  -1,

  IN_SCALAR_INIT,
  'P', -1, -1, 1,  // Persistent
  'X',  1, -1, -1, // Skip the rest if not 1st INIT
  'R', -1, -1, REG_SCALAR_BLOCK,
  'I', -1, -1, -1,
  'S', -1, -1, IN_SCALAR_ZERO,
  -1,

  IN_SCALAR_ZERO,
  'H', -1, -1, ARG_LED0,
  'L', -1, -1, ARG_LED1,
  'L', -1, -1, ARG_LED2,
  'L', -1, -1, ARG_LED3,
  'L', -1, -1, ARG_LED4,
  'R', -1, -1, REG_SCALAR_BLOCK,
  'I', -1, -1, -1,
  -1,

  IN_SCALAR_ADVANCE, 
  'R', -1, -1, REG_SCALAR_BLOCK,
  'C', 1, -1, 0,
  'J', ARG_BACKSTOP, -1, 0,
  'R', -1, -1, REG_SCALAR_STATE,
  'J', ARG_LED0, 0, 2,
  'S', -1, -1, IN_SCALAR_SEL1,
  'J', -1, -1, -1,
  'J', ARG_LED1, 0, 2,
  'S', -1, -1, IN_SCALAR_SEL2,
  'J', -1, -1, -1,
  'J', ARG_LED2, 0, 2,
  'S', -1, -1, IN_SCALAR_SEL3,
  'J', -1, -1, -1,
  'J', ARG_LED3, 0, -1,
  'S', -1, -1, IN_SCALAR_SEL4,
  -1,

  IN_SCALAR_FREEZE,
  'R', -1, -1, REG_SCALAR_BLOCK,
  'I', -1, -1, -1,
  'I', -1, -1, 1,
  -1,   

  IN_SCALAR_SEL0,
  'I', -1, -1, -1,
  'I', -1, -1, 0,
  'S', -1, -1, SUB_SCALAR_CLEAR,  
  'H', -1, -1, ARG_LED0,
  -1,

  IN_SCALAR_SEL1,
  'I', -1, -1, -1,
  'I', -1, -1, 1,
  'S', -1, -1, SUB_SCALAR_CLEAR,  
  'H', -1, -1, ARG_LED1,
  -1,

  IN_SCALAR_SEL2,
  'I', -1, -1, -1,
  'I', -1, -1, 2,
  'S', -1, -1, SUB_SCALAR_CLEAR,  
  'H', -1, -1, ARG_LED2,
  -1,

  IN_SCALAR_SEL3,
  'I', -1, -1, -1,
  'I', -1, -1, 3,
  'S', -1, -1, SUB_SCALAR_CLEAR,  
  'H', -1, -1, ARG_LED3,
  -1,

  IN_SCALAR_SEL4,
  'I', -1, -1, -1,
  'I', -1, -1, 4,
  'S', -1, -1, SUB_SCALAR_CLEAR,  
  'H', -1, -1, ARG_LED4,
  -1,

  SUB_SCALAR_CLEAR,
  'L', -1, -1, ARG_LED0,
  'L', -1, -1, ARG_LED1,
  'L', -1, -1, ARG_LED2,
  'L', -1, -1, ARG_LED3,
  'L', -1, -1, ARG_LED4,
  -1,

  -1,
};


/*
 * Firmware: counter_em4d1w_firmware
 * Maximum switches: 1 (sensor wire)
 * Led arguments: COIL0, COIL1, COIL2, COIL3
 * Description: 
 *   4 digit electromechanical counter device with 1 sensor wire
 */

enum { IN_CTR_INIT, IN_CTR_PRESS, IN_CTR_RELEASE, IN_CTR_RESET,
  IN_CTR_PT10, IN_CTR_PT100, IN_CTR_PT1000, IN_CTR_PT500, IN_CTR_PT5000,
  SUB_CTR_PULSE_10, SUB_CTR_PULSE_100, SUB_CTR_PULSE_1K, SUB_CTR_PULSE_10K,
  SUB_CTR_MOVE_START, SUB_CTR_SOLVE_POS1, SUB_CTR_SOLVE_POS2_12, SUB_CTR_SOLVE_POS2_13, SUB_CTR_SOLVE_POS2_23, 
  SUB_CTR_SOLVE_POS3_1, SUB_CTR_SOLVE_POS3_2, SUB_CTR_SOLVE_POS3_3, SUB_CTR_SOLVE_REEL0,
};
enum { ARG_CTR_10K, ARG_CTR_1K, ARG_CTR_100, ARG_CTR_10, ARG_CTR_UP, ARG_CTR_DIRTY }; 
enum { OUT_CTR_DIGIT1, OUT_CTR_DIGIT2, OUT_CTR_DIGIT3 };
enum { REG_CTR_STATE, REG_CTR_SENSOR, REG_CTR_10K, REG_CTR_1K, REG_CTR_100, REG_CTR_10, REG_CTR_WAIT };
enum { VAR_CTR_LOW = 0, VAR_CTR_HIGH = 1, VAR_CTR_DELAY = 120 };
 
int16_t counter_em4d1w_firmware[] = {
  IN_CTR_INIT, IN_CTR_PRESS, IN_CTR_RELEASE, IN_CTR_RESET,
  IN_CTR_PT10, IN_CTR_PT100, IN_CTR_PT1000, IN_CTR_PT500, IN_CTR_PT5000,
  SUB_CTR_PULSE_10, SUB_CTR_PULSE_100, SUB_CTR_PULSE_1K, SUB_CTR_PULSE_10K,
  SUB_CTR_MOVE_START, SUB_CTR_SOLVE_POS1, SUB_CTR_SOLVE_POS2_12, SUB_CTR_SOLVE_POS2_13, SUB_CTR_SOLVE_POS2_23, 
  SUB_CTR_SOLVE_POS3_1, SUB_CTR_SOLVE_POS3_2, SUB_CTR_SOLVE_POS3_3, SUB_CTR_SOLVE_REEL0,
  -1,

  IN_CTR_INIT,
  'P', -1, -1,  1,                      // Persistent
  'X',  1,  0,  -1,
  'H', -1, -1, ARG_CTR_DIRTY,           // Set dirty led on 1st init
  'I', -1, -1, -1,
  'I', -1, -1,  1,
  -1,

  IN_CTR_PRESS,
  'R', -1, -1, REG_CTR_SENSOR,
  'I', -1, -1, -1,
  'I', -1, -1, 1,                      // Set sensor flag
  -1,
  
  IN_CTR_RELEASE,
  'R', -1, -1, REG_CTR_SENSOR,
  'I', -1, -1, -1,                     // Clear sensor flag
  -1,

  IN_CTR_RESET, 
  '0', -1,  0, -1,                      // Force primary core
  'C',  0, -1,  0,                      // Only when dirty
  'S', -1, -1, SUB_CTR_MOVE_START,
  'S', -1, -1, SUB_CTR_SOLVE_POS1,  
  'R', -1, -1, REG_CTR_STATE,
  'I', -1, -1, -1,                      // Clean!
  'L', -1, -1, ARG_CTR_DIRTY,           
  'R', -1, -1, REG_CTR_10,              // Reset digit counter registers to 0
  'I', -1, -1, -1,
  'R', -1, -1, REG_CTR_100,
  'I', -1, -1, -1,
  'R', -1, -1, REG_CTR_1K,
  'I', -1, -1, -1,
  'R', -1, -1, REG_CTR_10K,
  'I', -1, -1, -1,
  -1,

  SUB_CTR_MOVE_START,                   // Pulse 0 & 1 while sensor is low
  'R', -1, -1, REG_CTR_SENSOR,          // Select sensor register   
  'C', VAR_CTR_LOW,  0, -1,
  'H', -1, -1, ARG_CTR_10K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, -1,
  'H', -1, -1, ARG_CTR_1K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, -1,
  'A', -1, -1, SUB_CTR_MOVE_START,       // >> LOOP 
  -1,

  SUB_CTR_SOLVE_POS1,                    // Pulse 1 & 2 & 3 while sensor is high (store pos of 1ST)
  'R', -1, -1, REG_CTR_SENSOR,           // Select sensor register   
  'H', -1, -1, ARG_CTR_1K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_HIGH,  0, 7,
  'H', -1, -1, ARG_CTR_100,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_HIGH,  0, 5,
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_HIGH,  0, 3,
  'A', -1, -1, SUB_CTR_SOLVE_POS1,      // >> LOOP
  'A', -1, -1, SUB_CTR_SOLVE_POS2_23,  
  'A', -1, -1, SUB_CTR_SOLVE_POS2_13,  
  'A', -1, -1, SUB_CTR_SOLVE_POS2_12,  
  -1,  

  SUB_CTR_SOLVE_POS2_23,
  'R', -1, -1, REG_CTR_SENSOR,
  'H', -1, -1, ARG_CTR_100,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, 4,
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, 4,  
  'A', -1, -1, SUB_CTR_SOLVE_POS2_23,      // >> LOOP
  'H', -1, -1, ARG_CTR_100,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_3,
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_2,
  -1, 

  SUB_CTR_SOLVE_POS2_13,
  'R', -1, -1, REG_CTR_SENSOR,
  'H', -1, -1, ARG_CTR_10K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, 4,
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, 4,  
  'A', -1, -1, SUB_CTR_SOLVE_POS2_13,      // >> LOOP
  'H', -1, -1, ARG_CTR_1K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_3,
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_1,
  -1, 

  SUB_CTR_SOLVE_POS2_12,
  'R', -1, -1, REG_CTR_SENSOR,
  'H', -1, -1, ARG_CTR_10K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, 4,
  'H', -1, -1, ARG_CTR_100,
  'Y', -1, -1, VAR_CTR_DELAY,
  'C', VAR_CTR_LOW,  0, 4,  
  'A', -1, -1, SUB_CTR_SOLVE_POS2_12,      // >> LOOP
  'H', -1, -1, ARG_CTR_1K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_2,
  'H', -1, -1, ARG_CTR_100,        // We know 100 & 10 (23 -> 1)
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_1,
  -1, 

  SUB_CTR_SOLVE_POS3_1,
  'R', -1, -1, REG_CTR_SENSOR,
  'C', VAR_CTR_LOW,  0, 3,
  'H', -1, -1, ARG_CTR_1K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_1,  // >> LOOP
  'S', -1, -1, SUB_CTR_SOLVE_REEL0,  
  'H', -1, -1, ARG_CTR_1K,
  'Y', -1, -1, VAR_CTR_DELAY,
  -1,
  
  SUB_CTR_SOLVE_POS3_2,
  'R', -1, -1, REG_CTR_SENSOR,
  'C', VAR_CTR_LOW,  0, 3,
  'H', -1, -1, ARG_CTR_100,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_2,  // >> LOOP
  'S', -1, -1, SUB_CTR_SOLVE_REEL0,  
  'H', -1, -1, ARG_CTR_100,
  'Y', -1, -1, VAR_CTR_DELAY,
  -1,
  
  SUB_CTR_SOLVE_POS3_3,
  'R', -1, -1, REG_CTR_SENSOR,
  'C', VAR_CTR_LOW,  0, 3,
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_POS3_3,  // >> LOOP
  'S', -1, -1, SUB_CTR_SOLVE_REEL0,  
  'H', -1, -1, ARG_CTR_10,
  'Y', -1, -1, VAR_CTR_DELAY,
  -1,
  
  SUB_CTR_SOLVE_REEL0,
  'R', -1, -1, REG_CTR_SENSOR,          
  'C', VAR_CTR_HIGH, 0, -1,
  'H', -1, -1, ARG_CTR_10K,
  'Y', -1, -1, VAR_CTR_DELAY,
  'A', -1, -1, SUB_CTR_SOLVE_REEL0,  
  -1,

  IN_CTR_PT10, 
  '0', -1,  0, -1,                      // Force primary core
  'J', ARG_CTR_UP, 0, -1,
  'I', -1, -1,  1,                      
  'H', -1, -1, ARG_CTR_DIRTY,          
  'T', -1, -1, OUT_CTR_DIGIT3,
  'S', -1, -1, SUB_CTR_PULSE_10,
  -1,
  
  IN_CTR_PT100, 
  '0', -1,  0, -1,                      // Force primary core
  'J', ARG_CTR_UP, 0, -1,
  'I', -1, -1,  1,                      
  'H', -1, -1, ARG_CTR_DIRTY,          
  'T', -1, -1, OUT_CTR_DIGIT2,
  'S', -1, -1, SUB_CTR_PULSE_100,
  -1,
  
  IN_CTR_PT1000,
  '0', -1,  0, -1,                      // Force primary core
  'J', ARG_CTR_UP, 0, -1,
  'I', -1, -1,  1,                      
  'H', -1, -1, ARG_CTR_DIRTY,          
  'T', -1, -1, OUT_CTR_DIGIT1,
  'S', -1, -1, SUB_CTR_PULSE_1K,
  -1,
   
  IN_CTR_PT500, 
  '0', -1,  0, -1,                      // Force primary core
  'J', ARG_CTR_UP, 0, -1,
  'I', -1, -1,  1,                      
  'H', -1, -1, ARG_CTR_DIRTY,          
  'T', -1, -1, OUT_CTR_DIGIT2,
  'S', -1, -1, SUB_CTR_PULSE_100,
  'T', -1, -1, OUT_CTR_DIGIT2,
  'S', -1, -1, SUB_CTR_PULSE_100,
  'T', -1, -1, OUT_CTR_DIGIT2,
  'S', -1, -1, SUB_CTR_PULSE_100,
  'T', -1, -1, OUT_CTR_DIGIT2,
  'S', -1, -1, SUB_CTR_PULSE_100,
  'T', -1, -1, OUT_CTR_DIGIT2,
  'S', -1, -1, SUB_CTR_PULSE_100,
  -1,
  
  IN_CTR_PT5000,
  '0', -1,  0, -1,                      // Force primary core
  'J', ARG_CTR_UP, 0, -1,
  'I', -1, -1,  1,                      
  'H', -1, -1, ARG_CTR_DIRTY,          
  'T', -1, -1, OUT_CTR_DIGIT1,
  'S', -1, -1, SUB_CTR_PULSE_1K,
  'T', -1, -1, OUT_CTR_DIGIT1,
  'S', -1, -1, SUB_CTR_PULSE_1K,
  'T', -1, -1, OUT_CTR_DIGIT1,
  'S', -1, -1, SUB_CTR_PULSE_1K,
  'T', -1, -1, OUT_CTR_DIGIT1,
  'S', -1, -1, SUB_CTR_PULSE_1K,
  'T', -1, -1, OUT_CTR_DIGIT1,
  'S', -1, -1, SUB_CTR_PULSE_1K,
  -1,

  SUB_CTR_PULSE_10,
  'R', -1, -1, REG_CTR_10,
  'H', -1, -1, ARG_CTR_10,
  'C',  9,  3, 0,                      // Check rollover
  'I', -1, -1, 1,                      // If not just increment & yield
  'Y', -1, -1, VAR_CTR_DELAY,
  'J', -1, -1, -1,
  'I', -1, -1, -1,                     // Else reset digit counter register
  'S', -1, -1, SUB_CTR_PULSE_100,      // And rollover
  -1,

  SUB_CTR_PULSE_100,
  'R', -1, -1, REG_CTR_100,
  'H', -1, -1, ARG_CTR_100,
  'C',  9,  3, 0,
  'I', -1, -1, 1,
  'Y', -1, -1, VAR_CTR_DELAY,
  'J', -1, -1, -1,
  'I', -1, -1, -1,
  'S', -1, -1, SUB_CTR_PULSE_1K,  
  -1,

  SUB_CTR_PULSE_1K,
  'R', -1, -1, REG_CTR_1K,
  'H', -1, -1, ARG_CTR_1K,
  'C',  9,  3, 0,
  'I', -1, -1, 1,
  'Y', -1, -1, VAR_CTR_DELAY,
  'J', -1, -1, -1,
  'I', -1, -1, -1,
  'S', -1, -1, SUB_CTR_PULSE_10K,  
  -1,

  SUB_CTR_PULSE_10K,
  'R', -1, -1, REG_CTR_10K,
  'H', -1, -1, ARG_CTR_10K,
  'I', -1, -1, 1,
  'Y', -1, -1, VAR_CTR_DELAY,
  -1,

  -1,
};


//  playfield.device( BUMPER_A )
//    .onEvent( "SCORE_LIT", COUNTER0, "PT1000" );

// src => intermediate
// 120 if ( led( LIGHT ) or led( LIGHT2 ) ) then output SCORE_LIT
// [120] => "IF (LED(VAR)|LED(3)) THEN OUTPUT 1"
// "IF", "(LED(VAR)|LED(3))", "THEN", "OUTPUT", "(1)" 

// INFIX expressie > prepared RPN, alleen functies/arrays/variables nog resolven 

char bumper_basic[] = R""""(
INIT, PRESS, RELEASE, LIGHT_ON, LIGHT_OFF
SCORE, SCORE_LIT, SCORE_UNLIT, LIGHT_ON, LIGHT_OFF
COIL, LIGHT
  
000 on PRESS call 100
001 on LIGHT_ON call 300
002 on LIGHT_OFF call 400
003 persist OFF, OFF
004 led COIL, OFF
005 led LIGHT, OFF
006 end  

100 led COIL, ON
110 output SCORE
120 if (  led( LIGHT ) ) then output SCORE_LIT
130 if ( !led( LIGHT ) ) then output SCORE_UNLIT
140 end

300 led LIGHT, ON
310 output LIGHT_ON
320 end

400 led LIGHT, OFF
410 output LIGHT_OFF
420 end

)"""";

};
