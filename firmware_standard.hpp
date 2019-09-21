#pragma once

/* Standard firmware for the Atm_led_device programmable pinball device class
 *  
 *  ledbank
 *  dual_kicker
 *  switchbank
 *  dual_combo_leds
 *  bumper
 *  dual_target
 *  dual flipper
 *  
 *  playfield.device( <switch-id> ); // Instantiates object
 *  playfield.device( <switch-id>, <led-id>, <firmware> ); // Initializes object
 */

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
  'P', -1, -1, 0,
  'I', -1, -1, -1,
  'S', -1, -1, IN_LBANK_OFF,  
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
  'P', -1, -1, 0,  // Default not persistent 
  'L', -1, -1, ARG_KICKER_LED_L,
  'L', -1, -1, ARG_KICKER_LED_R,
  -1,

  IN_KICKER_PERSIST,
  'P', -1, -1, 1,  // Persistent 
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
  'T', -1, -1, OUT_SBANK_SCORE1,
  -1,

  IN_SBANK_PRESS2,
  'T', -1, -1, OUT_SBANK2,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE2,
  -1,

  IN_SBANK_PRESS3,
  'T', -1, -1, OUT_SBANK3,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE3,
  -1,

  IN_SBANK_PRESS4,
  'T', -1, -1, OUT_SBANK4,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE4,
  -1,

  IN_SBANK_PRESS5,
  'T', -1, -1, OUT_SBANK5,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE5,
  -1,

  IN_SBANK_PRESS6,
  'T', -1, -1, OUT_SBANK6,
  'T', -1, -1, OUT_SBANK_SCORE,
  'T', -1, -1, OUT_SBANK_SCORE6,
  -1,

  IN_SBANK_PRESS7,
  'T', -1, -1, OUT_SBANK7,
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
 * Led arguments: COIL_L, COIL_R, COIL_L1, COIL_R1
 * Description: 
 *   Dual flipper device (2 independent flippers + two optional slaves)
 */

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

/*
 * Firmware: scalar_firmware
 * Maximum switches: 0
 * Led arguments: LED0, LED1, LED2, LED3, LED4, BACKSTOP
 * Description: 
 *   Led position indicator with 5 places (0..4), used for # of players, # of ball, # of player up
 *   Advances until ARG_BACKTOP led is lit
 *   or until IN_SCALAR_BLOCK is called
 */

enum { IN_SCALAR_INIT, IN_SCALAR_ADVANCE, IN_SCALAR_BLOCK, IN_SCALAR_SEL0, IN_SCALAR_SEL1, IN_SCALAR_SEL2, IN_SCALAR_SEL3, IN_SCALAR_SEL4, SUB_SCALAR_CLEAR };
enum { ARG_LED0, ARG_LED1, ARG_LED2, ARG_LED3, ARG_LED4, ARG_BACKSTOP }; 
enum { REG_SCALAR_STATE, REG_SCALAR_BLOCK };

int16_t scalar_firmware[] = {
  IN_SCALAR_INIT, 
  IN_SCALAR_ADVANCE, 
  IN_SCALAR_BLOCK, 
  IN_SCALAR_SEL0, 
  IN_SCALAR_SEL1, 
  IN_SCALAR_SEL2, 
  IN_SCALAR_SEL3, 
  IN_SCALAR_SEL4, 
  SUB_SCALAR_CLEAR,  
  -1,

  IN_SCALAR_INIT,
  'P', -1, -1, 1,  // Persistent
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

  IN_SCALAR_BLOCK,
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

};
