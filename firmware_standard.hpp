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

const char ledbank_bytecode[] = R""""(
init, on0, off0, on1, off1, on2, off2,\
on3, off3, on4, off4, on5, off5, on6,\
off6, on7, off7, on, off
out_on0, out_on1, out_on2, out_on3, out_on4, out_on5, out_on6, out_on7,\
out_off0, out_off1, out_off2, out_off3, out_off4, out_off5, out_off6, out_off7
arg_led0, arg_led1, arg_led2, arg_led3, arg_led4, arg_led5, arg_led6, arg_led7
reg_state, reg_default


init
P, -1, -1, 1
X,  1,  0, 4
R, -1, -1, reg_state
=,  0,  2, 0
R, -1, -1, reg_default
Z, -1, -1, 1
R, -1, -1, reg_state
Z, -1, -1, 0
S, -1, -1, off
R, -1, -1, reg_default
=,  0,  1, 0
S, -1, -1, on;

on
Z, -1, -1, 1
S, -1, -1, on0
S, -1, -1, on1
S, -1, -1, on2
S, -1, -1, on3
S, -1, -1, on4
S, -1, -1, on5
S, -1, -1, on6
S, -1, -1, on7;

off
Z, -1, -1, 0
S, -1, -1, off0
S, -1, -1, off1
S, -1, -1, off2
S, -1, -1, off3
S, -1, -1, off4
S, -1, -1, off5
S, -1, -1, off6
S, -1, -1, off7;

on0
Z, -1, -1, 1
H, -1, -1, arg_led0
T, -1, -1, out_on0;

off0
Z, -1, -1, 0
L, -1, -1, arg_led0
T, -1, -1, out_off0;

on1
H, -1, -1, arg_led1
T, -1, -1, out_on1;

off1
L, -1, -1, arg_led1
T, -1, -1, out_off1;

on2
H, -1, -1, arg_led2
T, -1, -1, out_on2;

off2
L, -1, -1, arg_led2
T, -1, -1, out_off2;

on3
H, -1, -1, arg_led3
T, -1, -1, out_on3;

off3
L, -1, -1, arg_led3
T, -1, -1, out_off3;

on4
H, -1, -1, arg_led4
T, -1, -1, out_on4;

off4
L, -1, -1, arg_led4
T, -1, -1, out_off4;

on5
H, -1, -1, arg_led5
T, -1, -1, out_on5;

off5
L, -1, -1, arg_led5
T, -1, -1, out_off5;

on6
H, -1, -1, arg_led6
T, -1, -1, out_on6;

off6
L, -1, -1, arg_led6
T, -1, -1, out_off6;

on7
H, -1, -1, arg_led7
T, -1, -1, out_on7;

off7
L, -1, -1, arg_led7
T, -1, -1, out_off7;

)"""";

const char ledbank_symbin[] = { 
  "\x78\x56\x34\x12\x58\x00\x00\x00" "init\0on0\0off0\0on1\0off1\0on2\0off2\0on3\0off3\0on4\0off4\0on5\0off5\0on6\0off6\0on7\0off7\0on\0off\0\0\0\0\0"
  "\x78\x56\x34\x12\x8c\x00\x00\x00" "out_on0\0out_on1\0out_on2\0out_on3\0out_on4\0out_on5\0out_on6\0out_on7\0out_off0\0out_off1\0out_off2\0out_off3\0out_off4\0out_off5\0out_off6\0out_off7\0\0\0\0\0"
  "\x78\x56\x34\x12\x4c\x00\x00\x00" "arg_led0\0arg_led1\0arg_led2\0arg_led3\0arg_led4\0arg_led5\0arg_led6\0arg_led7\0\0\0\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "reg_state\0reg_default\0\0"
};

const uint16_t ledbank_hexbin[] = {
  0x0015, 0x0093, 0x00A1, 0x00AF, 0x00B9, 0x00C3, 0x00CD, 0x00D7, 0x00E1, 0x00EB, 0x00F5, 0x00FF, 
  0x0109, 0x0113, 0x011D, 0x0127, 0x0131, 0x0047, 0x006D, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 0xFFFF, 
  0x0001, 0x0058, 0x0001, 0x0000, 0x0004, 0x0052, 0xFFFF, 0xFFFF, 0x0000, 0x003D, 0x0000, 0x0002, 
  0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x005A, 0xFFFF, 0xFFFF, 0x0001, 0x0052, 0xFFFF, 0xFFFF, 
  0x0000, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x0012, 0x0052, 0xFFFF, 0xFFFF, 
  0x0001, 0x003D, 0x0000, 0x0001, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x0011, 0xFFFF, 0x0011, 0x005A, 
  0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 
  0xFFFF, 0xFFFF, 0x0002, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0004, 0x0048, 
  0xFFFF, 0xFFFF, 0x0005, 0x0048, 0xFFFF, 0xFFFF, 0x0006, 0x0048, 0xFFFF, 0xFFFF, 0x0007, 0xFFFF, 
  0x0012, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 
  0x0001, 0x004C, 0xFFFF, 0xFFFF, 0x0002, 0x004C, 0xFFFF, 0xFFFF, 0x0003, 0x004C, 0xFFFF, 0xFFFF, 
  0x0004, 0x004C, 0xFFFF, 0xFFFF, 0x0005, 0x004C, 0xFFFF, 0xFFFF, 0x0006, 0x004C, 0xFFFF, 0xFFFF, 
  0x0007, 0xFFFF, 0x0001, 0x005A, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 
  0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0002, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 
  0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0008, 0xFFFF, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 
  0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0004, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 
  0x0009, 0xFFFF, 0x0005, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0xFFFF, 
  0x0006, 0x004C, 0xFFFF, 0xFFFF, 0x0002, 0x0054, 0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0x0007, 0x0048, 
  0xFFFF, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0x0008, 0x004C, 0xFFFF, 0xFFFF, 
  0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x000B, 0xFFFF, 0x0009, 0x0048, 0xFFFF, 0xFFFF, 0x0004, 0x0054, 
  0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0x000A, 0x004C, 0xFFFF, 0xFFFF, 0x0004, 0x0054, 0xFFFF, 0xFFFF, 
  0x000C, 0xFFFF, 0x000B, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0005, 0xFFFF, 
  0x000C, 0x004C, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x000D, 0xFFFF, 0x000D, 0x0048, 
  0xFFFF, 0xFFFF, 0x0006, 0x0054, 0xFFFF, 0xFFFF, 0x0006, 0xFFFF, 0x000E, 0x004C, 0xFFFF, 0xFFFF, 
  0x0006, 0x0054, 0xFFFF, 0xFFFF, 0x000E, 0xFFFF, 0x000F, 0x0048, 0xFFFF, 0xFFFF, 0x0007, 0x0054, 
  0xFFFF, 0xFFFF, 0x0007, 0xFFFF, 0x0010, 0x004C, 0xFFFF, 0xFFFF, 0x0007, 0x0054, 0xFFFF, 0xFFFF, 
  0x000F, 0xFFFF, 0xFFFF
};

/*
 * Firmware: dual_kicker_firmware
 * Maximum switches: 2
 * Led arguments: COIL_L, COIL_R, LED_L, LED_R
 * Description: 
 *   Dual kickers, same score & same kick, can be made persistent 
 *   could work for slingshots as well
 */

const char dual_kicker_bytecode[] = R""""(
init, press_l, release_l, press_r, release_r, on, off, persist
out_score_lit, out_score_unlit, out_score, out_press_lit, out_press_unlit, out_press  
arg_coil_l, arg_coil_r, arg_led_l, arg_led_r


 
init
P, -1, -1, 0
L, -1, -1, arg_led_l
L, -1, -1, arg_led_r;

persist
P, -1, -1, 1;

press_l
H, -1, -1, arg_coil_l
T, -1, -1, out_press
T, -1, -1, out_score
J, arg_led_l, 0, 3
T, -1, -1, out_score_lit
T, -1, -1, out_press_lit
J, -1, -1, -1
T, -1, -1, out_score_unlit
T, -1, -1, out_press_unlit;

press_r
H, -1, -1, arg_coil_r
T, -1, -1, out_press
T, -1, -1, out_score
J, arg_led_r, 0, 3
T, -1, -1, out_score_lit
T, -1, -1, out_press_lit
J, -1, -1, -1
T, -1, -1, out_score_unlit
T, -1, -1, out_press_unlit;

on
H, -1, -1, arg_led_l
H, -1, -1, arg_led_r;

off
L, -1, -1, arg_led_l
L, -1, -1, arg_led_r;

)"""";

const char kicker_symbin[] = {
  "\x78\x56\x34\x12\x3c\x00\x00\x00" "init\0press_l\0release_l\0press_r\0release_r\0on\0off\0persist\0\0\0\0\0"
  "\x78\x56\x34\x12\x54\x00\x00\x00" "out_score_lit\0out_score_unlit\0out_score\0out_press_lit\0out_press_unlit\0out_press\0\0\0\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "arg_coil_l\0arg_coil_r\0arg_led_l\0arg_led_r\0\0"
};

const uint16_t kicker_hexbin[] = {
  0x000A, 0x001E, 0x0000, 0x0044, 0x0000, 0x006A, 0x0074, 0x0018, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 
  0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0002, 0x004C, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0x0007, 
  0x0050, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 
  0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0x004A, 0x0002, 0x0000, 0x0003, 0x0054, 0xFFFF, 
  0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x0054, 0xFFFF, 
  0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 
  0x0054, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0x004A, 0x0003, 0x0000, 0x0003, 
  0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 
  0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0x0005, 0x0048, 0xFFFF, 
  0xFFFF, 0x0002, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0x0006, 0x004C, 0xFFFF, 0xFFFF, 0x0002, 
  0x004C, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0xFFFF
};

/*
 * Firmware: switchbank_firmware
 * Maximum switches: 8
 * Led arguments: <none>
 * Description: 
 *   Basic 8-way switchbank device without leds
 */

const char switchbank_bytecode[] = R""""(
init, press0, release0, press1, release1, press2, release2,\
press3, release3, press4, release4, press5, release5,\
press6, release6, press7, release7
out_score, out_press0, out_press1, out_press2, out_press3, out_press4, out_press5, out_press6, out_press7,\
out_score0, out_score1, out_score2, out_score3, out_score4, out_score5, out_score6, out_score7



init
P, -1, -1, 0;

press0
T, -1, -1, out_press0
T, -1, -1, out_score
T, -1, -1, out_score0;

press1
T, -1, -1, out_press1
T, -1, -1, out_score
T, -1, -1, out_score1;

press2
T, -1, -1, out_press2
T, -1, -1, out_score
T, -1, -1, out_score2;

press3
T, -1, -1, out_press3
T, -1, -1, out_score
T, -1, -1, out_score3;

press4
T, -1, -1, out_press4
T, -1, -1, out_score
T, -1, -1, out_score4;

press5
T, -1, -1, out_press5
T, -1, -1, out_score
T, -1, -1, out_score5;

press6
T, -1, -1, out_press6
T, -1, -1, out_score
T, -1, -1, out_score6;

press7
T, -1, -1, out_press7
T, -1, -1, out_score
T, -1, -1, out_score7;

)"""";

const char switchbank_symbin[] = { 
  "\x78\x56\x34\x12\x88\x00\x00\x00" "init\0press0\0release0\0press1\0release1\0press2\0release2\0press3\0release3\0press4\0release4\0press5\0release5\0press6\0release6\0press7\0release7\0\0\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "out_score\0out_press0\0out_press1\0out_press2\0out_press3\0out_press4\0out_press5\0out_press6\0out_press7\0out_score0\0out_score1\0out_score2\0out_score3\0out_score4\0out_score5\0out_score6\0out_score7\0\0"
};

const uint16_t switchbank_hexbin[] = {
  0x0013, 0x0019, 0x0000, 0x0027, 0x0000, 0x0035, 0x0000, 0x0043, 0x0000, 0x0051, 0x0000, 0x005F, 
  0x0000, 0x006D, 0x0000, 0x007B, 0x0000, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 
  0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 
  0x0009, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 
  0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 
  0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x000B, 0xFFFF, 0x0007, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0x0054, 
  0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x000C, 0xFFFF, 0x0009, 0x0054, 0xFFFF, 0xFFFF, 
  0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x000D, 0xFFFF, 0x000B, 0x0054, 
  0xFFFF, 0xFFFF, 0x0006, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x000E, 0xFFFF, 
  0x000D, 0x0054, 0xFFFF, 0xFFFF, 0x0007, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 
  0x000F, 0xFFFF, 0x000F, 0x0054, 0xFFFF, 0xFFFF, 0x0008, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 
  0xFFFF, 0xFFFF, 0x0010, 0xFFFF, 0xFFFF
};

/*
 * Firmware: dual_combo_firmware
 * Maximum switches: 2
 * Led arguments: LED0, LED1
 * Description: 
 *   Dual lane device with leds (both lanes share the same function)
 */

const char dual_combo_bytecode[] = R""""(
init, press0, release0, press1, release1, on, off
out_score, out_score_lit, out_score_unlit, out_press_lit, out_press_unlit, out_press0_lit, out_press0_unlit, out_press1_lit, out_press1_unlit
arg_led0, arg_led1



init
P, -1, -1, 0
S, -1, -1, off;

press0
T, -1, -1, out_score // TODO: replace whole sub with: A, -1, -1, press1
J, arg_led0, 0, 4
T, -1, -1, out_press_lit
T, -1, -1, out_press0_lit
T, -1, -1, out_score_lit
J, -1, -1, -1
T, -1, -1, out_press_unlit
T, -1, -1, out_press0_unlit
T, -1, -1, out_score_unlit;

press1
T, -1, -1, out_score
J, arg_led0, 0, 4
T, -1, -1, out_press_lit
T, -1, -1, out_press1_lit
T, -1, -1, out_score_lit
J, -1, -1, -1
T, -1, -1, out_press_unlit
T, -1, -1, out_press1_unlit
T, -1, -1, out_score_unlit;

on
H, -1, -1, arg_led0
H, -1, -1, arg_led1;

off
L, -1, -1, arg_led0
L, -1, -1, arg_led1;

)"""";

const char dual_combo_symbin[] = {
  "\x78\x56\x34\x12\x30\x00\x00\x00" "init\0press0\0release0\0press1\0release1\0on\0off\0\0\0\0\0"
  "\x78\x56\x34\x12\x88\x00\x00\x00" "out_score\0out_score_lit\0out_score_unlit\0out_press_lit\0out_press_unlit\0out_press0_lit\0out_press0_unlit\0out_press1_lit\0out_press1_unlit\0\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "arg_led0\0arg_led1\0\0"
};

const uint16_t dual_combo_hexbin[] = {
  0x0009, 0x0013, 0x0000, 0x0039, 0x0000, 0x005F, 0x0069, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 0xFFFF, 
  0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x0006, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x004A, 
  0x0000, 0x0000, 0x0004, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 
  0xFFFF, 0xFFFF, 0x0001, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0x0054, 
  0xFFFF, 0xFFFF, 0x0006, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 
  0x0000, 0x004A, 0x0000, 0x0000, 0x0004, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 
  0x0007, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x0054, 0xFFFF, 0xFFFF, 
  0x0004, 0x0054, 0xFFFF, 0xFFFF, 0x0008, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0xFFFF, 0x0005, 0x0048, 
  0xFFFF, 0xFFFF, 0x0000, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0006, 0x004C, 0xFFFF, 0xFFFF, 
  0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0xFFFF
};

/*
 * Firmware: bumper_firmware
 * Maximum switches: 1
 * Led arguments: COIL, LED
 * Description: 
 *   Single bumper device with led & coil
 */

const char bumper_bytecode[] = R""""(
init, press, release, turn_on, turn_off
out_score, out_score_lit, out_score_unlit, out_light_on, out_light_off
arg_coil, arg_led



init
P, -1, -1, 0                   
L, -1, -1, arg_coil     
L, -1, -1, arg_led;      

press
H, -1, -1, arg_coil     
T, -1, -1, out_score
J, arg_led, 0, 2     
T, -1, -1, out_score_lit
J, -1, -1, -1
T, -1, -1, out_score_unlit;

turn_on
H, -1, -1, arg_led
T, -1, -1, out_light_on;

turn_off
L, -1, -1, arg_led
T, -1, -1, out_light_off;
 
)"""";

const uint16_t bumper_hexbin[] = {
  0x0007, 0x0015, 0x0000, 0x002F, 0x0039, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 0xFFFF, 0x0000, 0x004C, 
  0xFFFF, 0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 
  0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x004A, 0x0001, 0x0000, 0x0002, 0x0054, 0xFFFF, 0xFFFF, 
  0x0001, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0xFFFF, 0x0003, 0x0048, 
  0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0x0004, 0x004C, 0xFFFF, 0xFFFF, 
  0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0xFFFF
};

const char bumper_symbin[] = { 
  "\x78\x56\x34\x12\x28\x00\x00\x00" "init\0press\0release\0turn_on\0turn_off\0\0\0\0\0"
  "\x78\x56\x34\x12\x44\x00\x00\x00" "out_score\0out_score_lit\0out_score_unlit\0out_light_on\0out_light_off\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "arg_coil\0arg_led\0\0"
};


/*
 * Firmware: dual_target_firmware
 * Maximum switches: 2
 * Led arguments: LED0, LED1
 * Description: 
 *   Dual target device
 */

const char dual_target_bytecode[] = R""""(
init, press0, release0, press1, release1, clear
out_led0_on, out_led1_on, out_led0_off, out_led1_off, out_all_on, out_all_off, out_score
arg_led0, arg_led1



init
P, -1, -1, 0
S, -1, -1, clear;

press0
T, -1, -1, out_score
J, arg_led0, -1, 0
H, -1, -1, arg_led0
T, -1, -1, out_led0_on
J, arg_led1, 0, -1
T, -1, -1, out_all_on;
 
press1
T, -1, -1, out_score
J, arg_led1, -1, 0
H, -1, -1, arg_led1
T, -1, -1, out_led1_on
J, arg_led0, 0, -1
T, -1, -1, out_all_on;

clear
L, -1, -1, arg_led0
L, -1, -1, arg_led1
T, -1, -1, out_led0_off
T, -1, -1, out_led1_off
T, -1, -1, out_all_off;

)"""";

const uint16_t dual_target_hexbin[] = {
  0x0008, 0x0012, 0x0000, 0x002C, 0x0000, 0x0046, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 0xFFFF, 0x0000, 
  0x0053, 0xFFFF, 0xFFFF, 0x0005, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0006, 0x004A, 0x0000, 
  0xFFFF, 0x0000, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x004A, 0x0001, 
  0x0000, 0xFFFF, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x0006, 
  0x004A, 0x0001, 0xFFFF, 0x0000, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 
  0x004A, 0x0000, 0x0000, 0xFFFF, 0x0054, 0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0x0005, 0x004C, 0xFFFF, 
  0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0x0054, 0xFFFF, 
  0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x0005, 0xFFFF, 0xFFFF
};

const char dual_target_symbin[] = { 
  "\x78\x56\x34\x12\x2c\x00\x00\x00" "init\0press0\0release0\0press1\0release1\0clear\0\0"
  "\x78\x56\x34\x12\x54\x00\x00\x00" "out_led0_on\0out_led1_on\0out_led0_off\0out_led1_off\0out_all_on\0out_all_off\0out_score\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "arg_led0\0arg_led1\0\0"
};

/*
 * Firmware: dual_flipper_firmware
 * Maximum switches: 2
 * Led arguments: COIL_L, COIL_R
 * Description: 
 *   Dual flipper device)
 */

const char dual_flipper_bytecode[] = R""""(
init, press_l, release_l, press_r, release_r

arg_coil_l, arg_coil_r


init
P, -1, -1, 0
L, -1, -1, arg_coil_l
L, -1, -1, arg_coil_r;

press_l
H, -1, -1, arg_coil_l;

release_l
L, -1, -1, arg_coil_l;

press_r
H, -1, -1, arg_coil_r;

release_r
L, -1, -1, arg_coil_r;

)"""";

const char dual_flipper_symbin[] = {
  "\x78\x56\x34\x12\x2c\x00\x00\x00" "init\0press_l\0release_l\0press_r\0release_r\0\0\0\0"
  "\x78\x56\x34\x12\x04\x00\x00\x00" "\0\0\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "arg_coil_l\0arg_coil_r\0\0"
};

const uint16_t dual_flipper_hexbin[] = {
  0x0007, 0x0015, 0x001B, 0x0021, 0x0027, 0xFFFF, 0x0000, 0x0050, 0xFFFF, 0xFFFF, 0x0000, 0x004C, 
  0xFFFF, 0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 
  0x0000, 0xFFFF, 0x0002, 0x004C, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 
  0x0001, 0xFFFF, 0x0004, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0xFFFF
};

/*
 * Firmware: scalar_firmware
 * Maximum switches: 0
 * Led arguments: LED0, LED1, LED2, LED3, LED4, BACKSTOP
 * Description: 
 *   Led position indicator with 5 places (0..4), used for # of players, # of ball, # of player up
 *   Advances until arg_backstop led is lit
 *   or until freeze is called
 */

const char scalar_bytecode[] = R""""(
init, zero, advance, freeze, sel0, sel1, sel2, sel3, sel4, sub_clear
out_sel0, out_sel1, out_sel2, out_sel3, out_sel4
arg_led0, arg_led1, arg_led2, arg_led3, arg_led4, arg_backstop
reg_state, reg_freeze, reg_scalar


init
P, -1, -1, 1
X,  1,  0, -1
A, -1, -1, zero;

zero
H, -1, -1, arg_led0
L, -1, -1, arg_led1
L, -1, -1, arg_led2
L, -1, -1, arg_led3
L, -1, -1, arg_led4
R, -1, -1, reg_freeze
Z, -1, -1, 0;

advance
R, -1, -1, reg_freeze
=, 1, -1, 0
J, arg_backstop, -1, 0
R, -1, -1, reg_state
J, arg_led0, 0, 2
S, -1, -1, sel1
J, -1, -1, -1
J, arg_led1, 0, 2
S, -1, -1, sel2
J, -1, -1, -1
J, arg_led2, 0, 2
S, -1, -1, sel3
J, -1, -1, -1
J, arg_led3, 0, -1
S, -1, -1, sel4;

freeze
R, -1, -1, reg_freeze
Z, -1, -1, 1;

sel0
Z, -1, -1, 0
S, -1, -1, sub_clear
H, -1, -1, arg_led0
T, -1, -1, out_sel0;

sel1
Z, -1, -1, 1
S, -1, -1, sub_clear
H, -1, -1, arg_led1
T, -1, -1, out_sel1;

sel2
Z, -1, -1, 2
S, -1, -1, sub_clear
H, -1, -1, arg_led2
T, -1, -1, out_sel2;

sel3
Z, -1, -1, 3
S, -1, -1, sub_clear
H, -1, -1, arg_led3
T, -1, -1, out_sel3;

sel4
Z, -1, -1, 4
S, -1, -1, sub_clear
H, -1, -1, arg_led4
T, -1, -1, out_sel4;

sub_clear
L, -1, -1, arg_led0
L, -1, -1, arg_led1
L, -1, -1, arg_led2
L, -1, -1, arg_led3
L, -1, -1, arg_led4;

)"""";

const char scalar_symbin[] = { 
  "\x78\x56\x34\x12\x40\x00\x00\x00" "init\0zero\0advance\0freeze\0sel0\0sel1\0sel2\0sel3\0sel4\0sub_clear\0\0\0\0\0"
  "\x78\x56\x34\x12\x04\x00\x00\x00" "\0\0\0\0"
  "\x78\x56\x34\x12\x3c\x00\x00\x00" "arg_led0\0arg_led1\0arg_led2\0arg_led3\0arg_led4\0arg_backstop\0\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "reg_state\0reg_block\0reg_scalar\0\0"
};

const uint16_t scalar_hexbin[] = {
  0x000C, 0x0022, 0x0040, 0x007E, 0x0088, 0x0096, 0x00A4, 0x00B2, 0x00C0, 0x00CE, 0xFFFF, 0x0000, 
  0x0050, 0xFFFF, 0xFFFF, 0x0001, 0x0058, 0x0001, 0xFFFF, 0xFFFF, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 
  0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 
  0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0x004C, 0xFFFF, 0xFFFF, 0x0002, 0x004C, 0xFFFF, 
  0xFFFF, 0x0003, 0x004C, 0xFFFF, 0xFFFF, 0x0004, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x005A, 0xFFFF, 
  0xFFFF, 0x0000, 0xFFFF, 0x0002, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0001, 0xFFFF, 0x0000, 
  0x004A, 0x0005, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0000, 0x004A, 0x0000, 0x0000, 0x0002, 
  0x0053, 0xFFFF, 0xFFFF, 0x0005, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x004A, 0x0001, 0x0000, 0x0002, 
  0x0053, 0xFFFF, 0xFFFF, 0x0006, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x004A, 0x0002, 0x0000, 0x0002, 
  0x0053, 0xFFFF, 0xFFFF, 0x0007, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x004A, 0x0003, 0x0000, 0xFFFF, 
  0x0053, 0xFFFF, 0xFFFF, 0x0008, 0xFFFF, 0x0003, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x005A, 0xFFFF, 
  0xFFFF, 0x0001, 0xFFFF, 0x0004, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x0009, 
  0x0048, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0005, 0x005A, 0xFFFF, 0xFFFF, 0x0001, 0x0053, 0xFFFF, 
  0xFFFF, 0x0009, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0006, 0x005A, 0xFFFF, 0xFFFF, 0x0002, 
  0x0053, 0xFFFF, 0xFFFF, 0x0009, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 0xFFFF, 0x0007, 0x005A, 0xFFFF, 
  0xFFFF, 0x0003, 0x0053, 0xFFFF, 0xFFFF, 0x0009, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0x0008, 
  0x005A, 0xFFFF, 0xFFFF, 0x0004, 0x0053, 0xFFFF, 0xFFFF, 0x0009, 0x0048, 0xFFFF, 0xFFFF, 0x0004, 
  0xFFFF, 0x0009, 0x004C, 0xFFFF, 0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0001, 0x004C, 0xFFFF, 
  0xFFFF, 0x0002, 0x004C, 0xFFFF, 0xFFFF, 0x0003, 0x004C, 0xFFFF, 0xFFFF, 0x0004, 0xFFFF, 0xFFFF
};

/*
 * Firmware: counter_em4d1w_firmware
 * Maximum switches: 1 (sensor wire)
 * Led arguments: COIL0, COIL1, COIL2, COIL3
 * Description: 
 *   4 digit electromechanical counter device with 1 sensor wire
 */

const char counter_em4d1w_bytecode[] = R""""(
init, sensor, sensor_release, reset, pt10, pt100, pt1000, pt500, pt5000, \
sub_pulse_10, sub_pulse_100, sub_pulse_1k, sub_pulse_10k, sub_move_start, \
sub_pulse0, sub_pulse1, sub_pulse2, sub_pulse3, sub_pulse, \
sub_pos1, sub_pos2_12, sub_pos2_13, sub_pos2_23, \
sub_pos3_1, sub_pos3_2, sub_pos3_3, sub_reel0
out_digit1, out_digit2, out_digit3
arg_10k, arg_1k, arg_100, arg_10, arg_up, arg_dirty 
reg_state, reg_sensor, reg_10k, reg_1k, reg_100, reg_10, reg_wait, reg_timeout
msg_reset, msg_timeout

init,
P, -1, -1, 1
X,  1,  0, -1
H, -1, -1, arg_dirty
Z, -1, -1, 1;

reset
=,  0, -1, 0
Q, -1, -1, msg_reset
R, -1, -1, reg_timeout
Z, -1, -1, 60
S, -1, -1, sub_move_start
S, -1, -1, sub_pos1
L, -1, -1, arg_dirty
R, -1, -1, reg_state
Z, -1, -1, 0
D, -1, -1, reg_10
D, -1, -1, reg_100
D, -1, -1, reg_1k
D, -1, -1, reg_10k;

sub_move_start
K, sensor, -1, 0
S, -1, -1, sub_pulse0
K, sensor, -1, 0
S, -1, -1, sub_pulse1
K, sensor, -1, 0
A, -1, -1, sub_move_start;

sub_pos1
S, -1, -1, sub_pulse1
K, sensor, 0, 5
S, -1, -1, sub_pulse2
K, sensor, 0, 4
S, -1, -1, sub_pulse3
K, sensor, 0, 3
A, -1, -1, sub_pos1
A, -1, -1, sub_pos2_23
A, -1, -1, sub_pos2_13
A, -1, -1, sub_pos2_12;

sub_pos2_23
S, -1, -1, sub_pulse2
K, sensor, 3, 0 
S, -1, -1, sub_pulse3
K, sensor, 3, 0 
A, -1, -1, sub_pos2_23
S, -1, -1, sub_pulse2 
A, -1, -1, sub_pos3_3
S, -1, -1, sub_pulse3
A, -1, -1, sub_pos3_2;

sub_pos2_13
S, -1, -1, sub_pulse0
K, sensor, 3, 0
S, -1, -1, sub_pulse3
K, sensor, 3, 0
A, -1, -1, sub_pos2_13
S, -1, -1, sub_pulse1
A, -1, -1, sub_pos3_3
S, -1, -1, sub_pulse3
A, -1, -1, sub_pos3_1;

sub_pos2_12
S, -1, -1, sub_pulse0
K, sensor, 3, 0
S, -1, -1, sub_pulse2
K, sensor, 3, 0
A, -1, -1, sub_pos2_12
S, -1, -1, sub_pulse1
A, -1, -1, sub_pos3_2
S, -1, -1, sub_pulse2
A, -1, -1, sub_pos3_1;

sub_pos3_1
K, sensor, 2, 0
S, -1, -1, sub_pulse1
A, -1, -1, sub_pos3_1
S, -1, -1, sub_reel0
S, -1, -1, sub_pulse1;

sub_pos3_2
K, sensor, 2, 0
S, -1, -1, sub_pulse2
A, -1, -1, sub_pos3_2
S, -1, -1, sub_reel0
S, -1, -1, sub_pulse2;

sub_pos3_3
K, sensor, 2, 0
S, -1, -1, sub_pulse3
A, -1, -1, sub_pos3_3
S, -1, -1, sub_reel0
S, -1, -1, sub_pulse3;

sub_reel0
K, sensor, 0, -1
S, -1, -1, sub_pulse0
A, -1, -1, sub_reel0;

pt10
J, arg_up, 0, -1
I, -1, -1, 1
H, -1, -1, arg_dirty
T, -1, -1, out_digit3
S, -1, -1, sub_pulse_10;

pt100
J, arg_up, 0, -1
I, -1, -1, 1
H, -1, -1, arg_dirty
T, -1, -1, out_digit2
S, -1, -1, sub_pulse_100;

pt1000
J, arg_up, 0, -1
I, -1, -1,  1
H, -1, -1, arg_dirty
T, -1, -1, out_digit1
S, -1, -1, sub_pulse_1k;

pt500
J, arg_up, 0, -1
I, -1, -1, 1
H, -1, -1, arg_dirty
T, -1, -1, out_digit2
S, -1, -1, sub_pulse_100
T, -1, -1, out_digit2
S, -1, -1, sub_pulse_100
T, -1, -1, out_digit2
S, -1, -1, sub_pulse_100
T, -1, -1, out_digit2
S, -1, -1, sub_pulse_100
T, -1, -1, out_digit2
S, -1, -1, sub_pulse_100;

pt5000
J, arg_up, 0, -1
I, -1, -1, 1
H, -1, -1, arg_dirty
T, -1, -1, out_digit1
S, -1, -1, sub_pulse_1k
T, -1, -1, out_digit1
S, -1, -1, sub_pulse_1k
T, -1, -1, out_digit1
S, -1, -1, sub_pulse_1k
T, -1, -1, out_digit1
S, -1, -1, sub_pulse_1k
T, -1, -1, out_digit1
S, -1, -1, sub_pulse_1k;

sub_pulse0
H, -1, -1, arg_10k
A, -1, -1, sub_pulse;

sub_pulse1
H, -1, -1, arg_1k
A, -1, -1, sub_pulse;

sub_pulse2
H, -1, -1, arg_100
A, -1, -1, sub_pulse;

sub_pulse3
H, -1, -1, arg_10
A, -1, -1, sub_pulse;

sub_pulse
W, -1, -1, 120
I, -1, -1, -1
>,  0, -1, 0
L, -1, -1, arg_dirty
Q, -1, -1, msg_timeout
R, -1, -1, reg_state
Z, -1, -1, 0
!, -1, -1, -1;

sub_pulse_10
R, -1, -1, reg_10
H, -1, -1, arg_10
=,  9,  3, 0
I, -1, -1, 1
W, -1, -1, 120
J, -1, -1, -1
Z, -1, -1, 0
S, -1, -1, sub_pulse_100;

sub_pulse_100
R, -1, -1, reg_100
H, -1, -1, arg_100
=,  9,  3, 0
I, -1, -1, 1
W, -1, -1, 120
J, -1, -1, -1
Z, -1, -1, 0
S, -1, -1, sub_pulse_1k;

sub_pulse_1k
R, -1, -1, reg_1k
H, -1, -1, arg_1k
=,  9,  3, 0
I, -1, -1, 1
W, -1, -1, 120
J, -1, -1, -1
Z, -1, -1, 0
S, -1, -1, sub_pulse_10k;

sub_pulse_10k
R, -1, -1, reg_10k
H, -1, -1, arg_10k
I, -1, -1, 1
W, -1, -1, 120;

)"""";


const char counter_em4d1w_symbin[] = { 
  "\x78\x56\x34\x12\xd8\x00\x00\x00" "init\0press\0release\0reset\0pt10\0pt100\0pt1000\0pt500\0pt5000\0sub_pulse_10\0sub_pulse_100\0sub_pulse_1k\0sub_pulse_10k\0sub_move_start\0sub_pos1\0sub_pos2_12\0sub_pos2_13\0sub_pos2_23\0sub_pos3_1\0sub_pos3_2\0sub_pos3_3\0sub_reel0\0\0\0\0"
  "\x78\x56\x34\x12\x24\x00\x00\x00" "out_digit1\0out_digit2\0out_digit3\0\0\0\0"
  "\x78\x56\x34\x12\x30\x00\x00\x00" "arg_10k\0arg_1k\0arg_100\0arg_10\0arg_up\0arg_dirty\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "reg_state\0reg_sensor\0reg_10k\0reg_1k\0reg_100\0reg_10\0reg_wait\0\0"
};  

const uint16_t counter_em4d1w_hexbin[] = {
  0x0018, 0x002A, 0x0034, 0x003E, 0x0206, 0x0220, 0x023A, 0x0254, 0x028E, 0x02C8, 0x02EA, 0x030C, 
  0x032E, 0x007C, 0x00A2, 0x0150, 0x0116, 0x00DC, 0x018A, 0x01AC, 0x01CE, 0x01F0, 0xFFFF, 0x0000, 
  0x0050, 0xFFFF, 0xFFFF, 0x0001, 0x0058, 0x0001, 0x0000, 0xFFFF, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 
  0x005A, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0001, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x005A, 0xFFFF, 
  0xFFFF, 0x0001, 0xFFFF, 0x0002, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 
  0xFFFF, 0x0003, 0x0030, 0xFFFF, 0x0000, 0xFFFF, 0x003D, 0x0000, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 
  0xFFFF, 0x000D, 0x0053, 0xFFFF, 0xFFFF, 0x000E, 0x0052, 0xFFFF, 0xFFFF, 0x0000, 0x005A, 0xFFFF, 
  0xFFFF, 0x0000, 0x004C, 0xFFFF, 0xFFFF, 0x0005, 0x0052, 0xFFFF, 0xFFFF, 0x0005, 0x005A, 0xFFFF, 
  0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0004, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 
  0xFFFF, 0x0003, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0002, 0x005A, 0xFFFF, 
  0xFFFF, 0x0000, 0xFFFF, 0x000D, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0000, 0x0000, 0xFFFF, 
  0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0000, 0x0000, 0xFFFF, 
  0x0048, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0000, 0x0000, 0xFFFF, 
  0x0041, 0xFFFF, 0xFFFF, 0x000D, 0xFFFF, 0x000E, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 
  0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0001, 0x0000, 0x0007, 0x0048, 0xFFFF, 
  0xFFFF, 0x0002, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0001, 0x0000, 0x0005, 0x0048, 0xFFFF, 
  0xFFFF, 0x0003, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0001, 0x0000, 0x0003, 0x0041, 0xFFFF, 
  0xFFFF, 0x000E, 0x0041, 0xFFFF, 0xFFFF, 0x0011, 0x0041, 0xFFFF, 0xFFFF, 0x0010, 0x0041, 0xFFFF, 
  0xFFFF, 0x000F, 0xFFFF, 0x0011, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 
  0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0000, 0x0000, 0x0004, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 
  0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x003D, 0x0000, 0x0000, 0x0004, 0x0041, 0xFFFF, 0xFFFF, 0x0011, 
  0x0048, 0xFFFF, 0xFFFF, 0x0002, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0014, 
  0x0048, 0xFFFF, 0xFFFF, 0x0003, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0013, 
  0xFFFF, 0x0010, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0059, 0xFFFF, 
  0xFFFF, 0x0078, 0x003D, 0x0000, 0x0000, 0x0004, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 0x0059, 0xFFFF, 
  0xFFFF, 0x0078, 0x003D, 0x0000, 0x0000, 0x0004, 0x0041, 0xFFFF, 0xFFFF, 0x0010, 0x0048, 0xFFFF, 
  0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0014, 0x0048, 0xFFFF, 
  0xFFFF, 0x0003, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0012, 0xFFFF, 0x000F, 
  0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 
  0x003D, 0x0000, 0x0000, 0x0004, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 
  0x003D, 0x0000, 0x0000, 0x0004, 0x0041, 0xFFFF, 0xFFFF, 0x000F, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 
  0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0013, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 
  0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0012, 0xFFFF, 0x0012, 0x0052, 0xFFFF, 
  0xFFFF, 0x0001, 0x003D, 0x0000, 0x0000, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 
  0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0012, 0x0053, 0xFFFF, 0xFFFF, 0x0015, 0x0048, 0xFFFF, 
  0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0xFFFF, 0x0013, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 
  0x003D, 0x0000, 0x0000, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 
  0x0041, 0xFFFF, 0xFFFF, 0x0013, 0x0053, 0xFFFF, 0xFFFF, 0x0015, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 
  0x0059, 0xFFFF, 0xFFFF, 0x0078, 0xFFFF, 0x0014, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0000, 
  0x0000, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 
  0xFFFF, 0x0014, 0x0053, 0xFFFF, 0xFFFF, 0x0015, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 0x0059, 0xFFFF, 
  0xFFFF, 0x0078, 0xFFFF, 0x0015, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0001, 0x0000, 0xFFFF, 
  0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x0041, 0xFFFF, 0xFFFF, 0x0015, 
  0xFFFF, 0x0004, 0x0030, 0xFFFF, 0x0000, 0xFFFF, 0x004A, 0x0004, 0x0000, 0xFFFF, 0x0049, 0xFFFF, 
  0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0x0053, 0xFFFF, 
  0xFFFF, 0x0009, 0xFFFF, 0x0005, 0x0030, 0xFFFF, 0x0000, 0xFFFF, 0x004A, 0x0004, 0x0000, 0xFFFF, 
  0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 
  0x0053, 0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0x0006, 0x0030, 0xFFFF, 0x0000, 0xFFFF, 0x004A, 0x0004, 
  0x0000, 0xFFFF, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 
  0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000B, 0xFFFF, 0x0007, 0x0030, 0xFFFF, 0x0000, 0xFFFF, 
  0x004A, 0x0004, 0x0000, 0xFFFF, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 
  0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x0053, 0xFFFF, 0xFFFF, 0x000A, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 
  0x0053, 0xFFFF, 0xFFFF, 0x000A, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x0053, 0xFFFF, 0xFFFF, 0x000A, 
  0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x0053, 0xFFFF, 0xFFFF, 0x000A, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 
  0x0053, 0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0x0008, 0x0030, 0xFFFF, 0x0000, 0xFFFF, 0x004A, 0x0004, 
  0x0000, 0xFFFF, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0048, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 
  0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000B, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 
  0xFFFF, 0x000B, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000B, 0x0054, 0xFFFF, 
  0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000B, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 
  0xFFFF, 0x000B, 0xFFFF, 0x0009, 0x0052, 0xFFFF, 0xFFFF, 0x0005, 0x0048, 0xFFFF, 0xFFFF, 0x0003, 
  0x003D, 0x0009, 0x0003, 0x0000, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 
  0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000A, 
  0xFFFF, 0x000A, 0x0052, 0xFFFF, 0xFFFF, 0x0004, 0x0048, 0xFFFF, 0xFFFF, 0x0002, 0x003D, 0x0009, 
  0x0003, 0x0000, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x004A, 0xFFFF, 
  0xFFFF, 0xFFFF, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000B, 0xFFFF, 0x000B, 
  0x0052, 0xFFFF, 0xFFFF, 0x0003, 0x0048, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0009, 0x0003, 0x0000, 
  0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0078, 0x004A, 0xFFFF, 0xFFFF, 0xFFFF, 
  0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x000C, 0xFFFF, 0x000C, 0x0052, 0xFFFF, 
  0xFFFF, 0x0002, 0x0048, 0xFFFF, 0xFFFF, 0x0000, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 
  0xFFFF, 0x0078, 0xFFFF, 0xFFFF
};

};
