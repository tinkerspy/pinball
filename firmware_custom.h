#pragma once

/* Custom firmware for the Atm_device programmable pinball device class
 *  
 *  tictactoe
 *  
 *  playfield.device( <switch-id> ); // Instantiates object
 *  playfield.device( <switch-id>, <led-id>, <firmware> ); // Initializes object
 *  
 */

namespace custom_firmware {

const char game_bytecode[] = R""""(
init, press, release, sub_wait_players, sub_wait_reset, sub_loop, \
sub_ball_loop, sub_player_loop, sub_core, sub_wait_playing, sub_wait_collecting
out_init, out_enable, out_counter_reset, out_players_zero, out_ball_zero,\
out_player_zero, out_ball_adv, out_player_adv, out_3bonus, out_collect,\
out_kickoff, out_over, out_players_adv
arg_enabled, arg_collecting, arg_again, arg_counter0, arg_counter1,\
arg_counter2, arg_counter3, arg_counter4, arg_counter5
reg_state, reg_max_players, reg_no_of_players, reg_player, reg_no_of_balls,\
reg_ball, reg_ball_cntr

press
R, -1, -1, reg_ball_cntr
>,  0, -1, 0
R, -1, -1, reg_no_of_players
=,  6,  0, 2
R, -1, -1, reg_max_players
=,  6, -1, 0
R, -1, -1, reg_no_of_players
=,  5,  0, 2
R, -1, -1, reg_max_players
=,  5, -1, 0
R, -1, -1, reg_no_of_players
=,  4,  0, 2
R, -1, -1, reg_max_players
=,  4, -1, 0
R, -1, -1, reg_no_of_players
=,  3,  0, 2
R, -1, -1, reg_max_players
=,  3, -1, 0
R, -1, -1, reg_no_of_players
=,  2,  0, 2
R, -1, -1, reg_max_players
=,  2, -1, 0
R, -1, -1, reg_no_of_players
=,  1,  0, 2
R, -1, -1, reg_max_players
=,  1, -1, 0
R, -1, -1, reg_no_of_players
T, -1, -1, out_players_adv
I, -1, -1, 1

init
0, -1,  0, -1
P, -1, -1, 1
T, -1, -1, out_init
Y, -1, -1, 100
R, -1, -1, 0
D, -1, -1, reg_no_of_balls
R, -1, -1, 1
D, -1, -1, reg_max_players
R, -1, -1, 0
Z, -1, -1, 0
A, -1, -1, sub_loop

sub_loop
T, -1, -1, out_over
R, -1, -1, reg_ball_cntr
Z, -1, -1, 0
R, -1, -1, reg_no_of_players
Z, -1, -1, 0
S, -1, -1, sub_wait_players
Y, -1, -1, 100
T, -1, -1, out_init
T, -1, -1, out_players_zero
T, -1, -1, out_ball_zero
T, -1, -1, out_counter_reset
S, -1, -1, sub_wait_reset
R, -1, -1, reg_no_of_balls
D, -1, -1, reg_ball
T, -1, -1, out_player_zero
S, -1, -1, sub_ball_loop
A, -1, -1, sub_loop

sub_ball_loop
R, -1, -1, reg_no_of_players
D, -1, -1, reg_player
T, -1, -1, out_player_zero
Y, -1, -1, 10
S, -1, -1, sub_player_loop
Y, -1, -1, 10
R, -1, -1, reg_ball
I, -1, -1, -1
=,  0, -1, 0
T, -1, -1, out_ball_adv
A, -1, -1, sub_ball_loop

sub_player_loop
S, -1, -1, sub_core
J, arg_again, -2, 0
R, -1, -1, reg_player
I, -1, -1, -1
=,  0, -1, 0
T, -1, -1, out_player_adv
A, -1, -1, sub_player_loop

sub_core
Y, -1, -1, 500
R, -1, -1, reg_ball
T,  1,  out_3bonus, -1
T, -1, -1, out_init
Y, -1, -1, 100
T, -1, -1, out_kickoff
Y, -1, -1, 1000
T, -1, -1, out_enable
Y, -1, -1, 100
S, -1, -1, sub_wait_playing
Y, -1, -1, 100
T, -1, -1, out_collect
Y, -1, -1, 100
S, -1, -1, sub_wait_collecting
Y, -1, -1, 100
R, -1, -1, reg_ball_cntr
I, -1, -1, 1
=,  1,  0, -1
R, -1, -1, reg_no_of_players
D, -1, -1, reg_player

sub_wait_players
R, -1, -1, reg_no_of_players
Y, -1, -1, 10
A,  0, sub_wait_players, -1

sub_wait_reset
Y, -1, -1, 100
J, arg_counter0, -2, 0
J, arg_counter1, -3, 0
J, arg_counter2, -4, 0
J, arg_counter3, -5, 0
J, arg_counter4, -6, 0
J, arg_counter5, -7, 0

sub_wait_playing
Y, -1, -1, 100
J, arg_enabled, -2, -1

sub_wait_collecting
Y, -1, -1, 100
J, arg_collecting, -2, -1
)"""";

const char game_symbin[] = { 
  "\x78\x56\x34\x12\x8c\x00\x00\x00" "init\0press\0release\0sub_wait_players\0sub_wait_reset\0sub_loop\0sub_ball_loop\0sub_player_loop\0sub_core\0sub_wait_playing\0sub_wait_collecting\0\0\0\0\0"
  "\x78\x56\x34\x12\xb0\x00\x00\x00" "out_init\0out_enable\0out_counter_reset\0out_players_zero\0out_ball_zero\0out_player_zero\0out_ball_adv\0out_player_adv\0out_3bonus\0out_collect\0out_kickoff\0out_over\0out_players_adv\0\0\0\0"
  "\x78\x56\x34\x12\x74\x00\x00\x00" "arg_enabled\0arg_collecting\0arg_again\0arg_counter0\0arg_counter1\0arg_counter2\0arg_counter3\0arg_counter4\0arg_counter5\0\0"
  "\x00\x00\x00\x00\x00\x00\x00\x00" "reg_state\0reg_max_players\0reg_no_of_players\0reg_player\0reg_no_of_balls\0reg_ball\0reg_ball_cntr\0\0"
};

const uint16_t game_hexbin[] = { 
  0x0083, 0x000D, 0x0000, 0x0195, 0x01A3, 0x00B1, 0x00F7, 0x0125, 0x0143, 0x01C1, 0x01CB, 0xFFFF, 
  0x0001, 0x0052, 0xFFFF, 0xFFFF, 0x0006, 0x003E, 0x0000, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 
  0x0002, 0x003D, 0x0006, 0x0000, 0x0002, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0006, 0xFFFF, 
  0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0002, 0x003D, 0x0005, 0x0000, 0x0002, 0x0052, 0xFFFF, 0xFFFF, 
  0x0001, 0x003D, 0x0005, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0002, 0x003D, 0x0004, 0x0000, 
  0x0002, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0004, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 
  0x0002, 0x003D, 0x0003, 0x0000, 0x0002, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0003, 0xFFFF, 
  0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0002, 0x003D, 0x0002, 0x0000, 0x0002, 0x0052, 0xFFFF, 0xFFFF, 
  0x0001, 0x003D, 0x0002, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0002, 0x003D, 0x0001, 0x0000, 
  0x0002, 0x0052, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0001, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 
  0x0002, 0x0054, 0xFFFF, 0xFFFF, 0x000C, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0xFFFF, 0x0000, 0x0030, 
  0xFFFF, 0x0000, 0xFFFF, 0x0050, 0xFFFF, 0xFFFF, 0x0001, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0059, 
  0xFFFF, 0xFFFF, 0x0064, 0x0052, 0xFFFF, 0xFFFF, 0x0000, 0x0044, 0xFFFF, 0xFFFF, 0x0004, 0x0052, 
  0xFFFF, 0xFFFF, 0x0001, 0x0044, 0xFFFF, 0xFFFF, 0x0001, 0x0052, 0xFFFF, 0xFFFF, 0x0000, 0x005A, 
  0xFFFF, 0xFFFF, 0x0000, 0x0041, 0xFFFF, 0xFFFF, 0x0005, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 
  0x000B, 0x0052, 0xFFFF, 0xFFFF, 0x0006, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0052, 0xFFFF, 0xFFFF, 
  0x0002, 0x005A, 0xFFFF, 0xFFFF, 0x0000, 0x0053, 0xFFFF, 0xFFFF, 0x0003, 0x0059, 0xFFFF, 0xFFFF, 
  0x0064, 0x0054, 0xFFFF, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 
  0x0004, 0x0054, 0xFFFF, 0xFFFF, 0x0002, 0x0053, 0xFFFF, 0xFFFF, 0x0004, 0x0052, 0xFFFF, 0xFFFF, 
  0x0004, 0x0044, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0xFFFF, 0xFFFF, 0x0005, 0x0053, 0xFFFF, 0xFFFF, 
  0x0006, 0x0041, 0xFFFF, 0xFFFF, 0x0005, 0xFFFF, 0x0006, 0x0052, 0xFFFF, 0xFFFF, 0x0002, 0x0044, 
  0xFFFF, 0xFFFF, 0x0003, 0x0054, 0xFFFF, 0xFFFF, 0x0005, 0x0059, 0xFFFF, 0xFFFF, 0x000A, 0x0053, 
  0xFFFF, 0xFFFF, 0x0007, 0x0059, 0xFFFF, 0xFFFF, 0x000A, 0x0052, 0xFFFF, 0xFFFF, 0x0005, 0x0049, 
  0xFFFF, 0xFFFF, 0xFFFF, 0x003D, 0x0000, 0xFFFF, 0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0006, 0x0041, 
  0xFFFF, 0xFFFF, 0x0006, 0xFFFF, 0x0007, 0x0053, 0xFFFF, 0xFFFF, 0x0008, 0x004A, 0x0002, 0xFFFE, 
  0x0000, 0x0052, 0xFFFF, 0xFFFF, 0x0003, 0x0049, 0xFFFF, 0xFFFF, 0xFFFF, 0x003D, 0x0000, 0xFFFF, 
  0x0000, 0x0054, 0xFFFF, 0xFFFF, 0x0007, 0x0041, 0xFFFF, 0xFFFF, 0x0007, 0xFFFF, 0x0008, 0x0059, 
  0xFFFF, 0xFFFF, 0x01F4, 0x0052, 0xFFFF, 0xFFFF, 0x0005, 0x0054, 0x0001, 0x0008, 0xFFFF, 0x0054, 
  0xFFFF, 0xFFFF, 0x0000, 0x0059, 0xFFFF, 0xFFFF, 0x0064, 0x0054, 0xFFFF, 0xFFFF, 0x000A, 0x0059, 
  0xFFFF, 0xFFFF, 0x03E8, 0x0054, 0xFFFF, 0xFFFF, 0x0001, 0x0059, 0xFFFF, 0xFFFF, 0x0064, 0x0053, 
  0xFFFF, 0xFFFF, 0x0009, 0x0059, 0xFFFF, 0xFFFF, 0x0064, 0x0054, 0xFFFF, 0xFFFF, 0x0009, 0x0059, 
  0xFFFF, 0xFFFF, 0x0064, 0x0053, 0xFFFF, 0xFFFF, 0x000A, 0x0059, 0xFFFF, 0xFFFF, 0x0064, 0x0052, 
  0xFFFF, 0xFFFF, 0x0006, 0x0049, 0xFFFF, 0xFFFF, 0x0001, 0x003D, 0x0001, 0x0000, 0xFFFF, 0x0052, 
  0xFFFF, 0xFFFF, 0x0002, 0x0044, 0xFFFF, 0xFFFF, 0x0003, 0xFFFF, 0x0003, 0x0052, 0xFFFF, 0xFFFF, 
  0x0002, 0x0059, 0xFFFF, 0xFFFF, 0x000A, 0x0041, 0x0000, 0x0003, 0xFFFF, 0xFFFF, 0x0004, 0x0059, 
  0xFFFF, 0xFFFF, 0x0064, 0x004A, 0x0003, 0xFFFE, 0x0000, 0x004A, 0x0004, 0xFFFD, 0x0000, 0x004A, 
  0x0005, 0xFFFC, 0x0000, 0x004A, 0x0006, 0xFFFB, 0x0000, 0x004A, 0x0007, 0xFFFA, 0x0000, 0x004A, 
  0x0008, 0xFFF9, 0x0000, 0xFFFF, 0x0009, 0x0059, 0xFFFF, 0xFFFF, 0x0064, 0x004A, 0x0000, 0xFFFE, 
  0xFFFF, 0xFFFF, 0x000A, 0x0059, 0xFFFF, 0xFFFF, 0x0064, 0x004A, 0x0001, 0xFFFE, 0xFFFF, 0xFFFF, 
  0xFFFF
};


enum { IN_GAME_INIT, IN_GAME_PRESS, IN_GAME_RELEASE, SUB_GAME_WAIT_PLAYERS, SUB_GAME_WAIT_RESET, SUB_GAME_LOOP,
         SUB_GAME_BALL_LOOP, SUB_GAME_PLAYER_LOOP, SUB_GAME_CORE, SUB_GAME_WAIT_PLAYING, SUB_GAME_WAIT_COLLECTING }; 
enum { OUT_GAME_INIT, OUT_GAME_ENABLE, OUT_GAME_COUNTER_RESET, OUT_GAME_PLAYERS_ZERO, OUT_GAME_BALL_ZERO, OUT_GAME_PLAYER_ZERO, 
        OUT_GAME_BALL_ADV, OUT_GAME_PLAYER_ADV, OUT_GAME_3BONUS, OUT_GAME_COLLECT, OUT_GAME_KICKOFF, 
        OUT_GAME_OVER, OUT_GAME_PLAYERS_ADV }; 
enum { ARG_GAME_ENABLED, ARG_GAME_COLLECTING, ARG_GAME_AGAIN, ARG_GAME_COUNTER0, ARG_GAME_COUNTER1, 
        ARG_GAME_COUNTER2, ARG_GAME_COUNTER3, ARG_GAME_COUNTER4, ARG_GAME_COUNTER5 };
enum { REG_GAME_STATE, REG_GAME_MAX_PLAYERS, REG_GAME_NO_OF_PLAYERS, REG_GAME_PLAYER, REG_GAME_NO_OF_BALLS, 
         REG_GAME_BALL, REG_GAME_BALL_CNTR };


enum { IN_ANI_INIT, IN_ANI_CYCLE };
enum { ARG_LED0, ARG_LED1, ARG_LED2 }; 

int16_t animation_firmware[] = {
  IN_ANI_INIT, 
  IN_ANI_CYCLE,
  -1,

  IN_ANI_INIT,
  'P', -1, -1, 1,  // Persistent
  'H', -1, -1, ARG_LED0,
  'L', -1, -1, ARG_LED1,
  'L', -1, -1, ARG_LED2,
  -1,

  IN_ANI_CYCLE,
  'L', -1, -1, ARG_LED0, 
  'H', -1, -1, ARG_LED1, 
  'Y', -1, -1, 0,
  'L', -1, -1, ARG_LED1, 
  'H', -1, -1, ARG_LED2, 
  'Y', -1, -1, 0,
  'L', -1, -1, ARG_LED2, 
  'H', -1, -1, ARG_LED0, 
  'Y', -1, -1, 0,
  'L', -1, -1, ARG_LED0, 
  'H', -1, -1, ARG_LED1, 
  'Y', -1, -1, 0,
  'L', -1, -1, ARG_LED1, 
  'H', -1, -1, ARG_LED2, 
  'Y', -1, -1, 0,
  'L', -1, -1, ARG_LED2, 
  'H', -1, -1, ARG_LED0, 
  'Y', -1, -1, 0,
  'A', -1, -1, IN_ANI_CYCLE,
  -1,

  -1,
};


// Williams OXO tic-tac-toe game

enum { // Inputs & Subs
  IN_OXO_INIT,
  IN_OXO_1O, IN_OXO_1X, IN_OXO_2O, IN_OXO_2X, IN_OXO_3O, IN_OXO_3X,             
  IN_OXO_4, IN_OXO_5, IN_OXO_6, IN_OXO_7, IN_OXO_8, IN_OXO_9, 
  IN_OXO_TOGGLE, IN_OXO_COLLECT, IN_OXO_SINGLE, IN_OXO_TRIPLE,
  SUB_OXO_COLLECT, 
  SUB_OXO_MATCH_123, SUB_OXO_MATCH_456, SUB_OXO_MATCH_789,
  SUB_OXO_MATCH_147, SUB_OXO_MATCH_258, SUB_OXO_MATCH_369,
  SUB_OXO_MATCH_159, SUB_OXO_MATCH_357, 
  SUB_OXO_MATCH_ALL,
  SUB_OXO_CLEAR,
};

enum { OUT_OXO_SCORE, OUT_OXO_WIN_ROW, OUT_OXO_WIN_ALL, OUT_OXO_COLLECT }; // Outputs

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
  ARG_OXO_X, ARG_OXO_O, ARG_OXO_TRIPLE, ARG_OXO_COLLECT,
};

const char tictactoe_symbols[] = 
  "INIT, OXO_1O, OXO_1X, OXO_2O, OXO_2X, OXO_3O, OXO_3X, OXO_4, OXO_5, OXO_6, OXO_7, OXO_8, OXO_9," 
    "TOGGLE, COLLECT, SINGLE, TRIPLE SUB_COLLECT, SUB_MATCH_123, SUB_MATCH_456, SUB_MATCH_789,"
    "SUB_MATCH_147, SUB_MATCH_258, SUB_MATCH_369, SUB_MATCH_159, SUB_MATCH_357, SUB_MATCH_ALL, SUB_CLEAR\n"
  "OUT_SCORE, OUT_WIN_ROW, OUT_WIN_ALL, OUT_COLLECT\n"
  "ARG_1A, ARG_1B, ARG_1C, ARG_2A, ARG_2B, ARG_2C, ARG_3A, ARG_3B, ARG_3C, ARG_4A, ARG_4B, ARG_4C,"
    "ARG_5A, ARG_5B, ARG_5C, ARG_6A, ARG_6B, ARG_6C, ARG_7A, ARG_7B, ARG_7C, ARG_8A, ARG_8B, ARG_8C,"
    "ARG_9A, ARG_9B, ARG_9C, ARG_X, ARG_O, ARG_TRIPLE, ARG_COLLECT\n";

int16_t tictactoe_firmware[] = {
  IN_OXO_INIT,
  IN_OXO_1O, IN_OXO_1X, IN_OXO_2O, IN_OXO_2X, IN_OXO_3O, IN_OXO_3X,             
  IN_OXO_4, IN_OXO_5, IN_OXO_6, IN_OXO_7, IN_OXO_8, IN_OXO_9, 
  IN_OXO_TOGGLE, IN_OXO_COLLECT, IN_OXO_SINGLE, IN_OXO_TRIPLE,
  SUB_OXO_COLLECT, 
  SUB_OXO_MATCH_123, SUB_OXO_MATCH_456, SUB_OXO_MATCH_789,
  SUB_OXO_MATCH_147, SUB_OXO_MATCH_258, SUB_OXO_MATCH_369,
  SUB_OXO_MATCH_159, SUB_OXO_MATCH_357, 
  SUB_OXO_MATCH_ALL,
  SUB_OXO_CLEAR,
  -1,

  IN_OXO_INIT,
  'P', -1, -1, 1,
  'L', -1, -1, ARG_OXO_COLLECT,
  'S', -1, -1, SUB_OXO_CLEAR,
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

  IN_OXO_SINGLE,
  'L', -1, -1, ARG_OXO_TRIPLE,
  -1,

  IN_OXO_TRIPLE,
  'H', -1, -1, ARG_OXO_TRIPLE,
  -1,

  IN_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_COLLECT,
  'S', -1, -1, SUB_OXO_COLLECT,    
  'J', ARG_OXO_TRIPLE, 0, 4,
  'Y', -1, -1, 1000,
  'S', -1, -1, SUB_OXO_COLLECT,    
  'Y', -1, -1, 1000,
  'S', -1, -1, SUB_OXO_COLLECT,    
  'Z', -1, -1, 0,
  'L', -1, -1, ARG_OXO_COLLECT,
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
  'J', ARG_OXO_2B, 0, -1,
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
  'Z', -1, -1, 0,
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
    
  SUB_OXO_CLEAR,
  'L', -1, -1, ARG_OXO_1A,
  'L', -1, -1, ARG_OXO_1B,
  'L', -1, -1, ARG_OXO_1C,
  'L', -1, -1, ARG_OXO_2A,
  'L', -1, -1, ARG_OXO_2B,
  'L', -1, -1, ARG_OXO_2C,
  'L', -1, -1, ARG_OXO_3A,
  'L', -1, -1, ARG_OXO_3B,
  'L', -1, -1, ARG_OXO_3C,
  'L', -1, -1, ARG_OXO_4A,
  'L', -1, -1, ARG_OXO_4B,
  'L', -1, -1, ARG_OXO_4C,
  'L', -1, -1, ARG_OXO_5A,
  'L', -1, -1, ARG_OXO_5B,
  'L', -1, -1, ARG_OXO_5C,
  'L', -1, -1, ARG_OXO_6A,
  'L', -1, -1, ARG_OXO_6B,
  'L', -1, -1, ARG_OXO_6C,
  'L', -1, -1, ARG_OXO_7A,
  'L', -1, -1, ARG_OXO_7B,
  'L', -1, -1, ARG_OXO_7C,
  'L', -1, -1, ARG_OXO_8A,
  'L', -1, -1, ARG_OXO_8B,
  'L', -1, -1, ARG_OXO_8C,
  'L', -1, -1, ARG_OXO_9A,
  'L', -1, -1, ARG_OXO_9B,
  'L', -1, -1, ARG_OXO_9C,
  'L', -1, -1, ARG_OXO_X,
  'H', -1, -1, ARG_OXO_O,  
  'L', -1, -1, ARG_OXO_TRIPLE,  
  -1,
    
  SUB_OXO_COLLECT,
  'J', ARG_OXO_1A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_1B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_1B,
  'J', ARG_OXO_1B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_1A,
  'H', -1, -1, ARG_OXO_1C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_1A,
  'L', -1, -1, ARG_OXO_1C,
  'J', ARG_OXO_2A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_2B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_2B,
  'J', ARG_OXO_2B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_2A,
  'H', -1, -1, ARG_OXO_2C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_2A,
  'L', -1, -1, ARG_OXO_2C,
  'J', ARG_OXO_3A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_3B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_3B,
  'J', ARG_OXO_3B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_3A,
  'H', -1, -1, ARG_OXO_3C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_3A,
  'L', -1, -1, ARG_OXO_3C,
  'J', ARG_OXO_4A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_4B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_4B,
  'J', ARG_OXO_4B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_4A,
  'H', -1, -1, ARG_OXO_4C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_4A,
  'L', -1, -1, ARG_OXO_4C,
  'J', ARG_OXO_5A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_5B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_5B,
  'J', ARG_OXO_5B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_5A,
  'H', -1, -1, ARG_OXO_5C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_5A,
  'L', -1, -1, ARG_OXO_5C,
  'J', ARG_OXO_6A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_6B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_6B,
  'J', ARG_OXO_6B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_6A,
  'H', -1, -1, ARG_OXO_6C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_6A,
  'L', -1, -1, ARG_OXO_6C,
  'J', ARG_OXO_7A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_7B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_7B,
  'J', ARG_OXO_7B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_7A,
  'H', -1, -1, ARG_OXO_7C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_7A,
  'L', -1, -1, ARG_OXO_7C,
  'J', ARG_OXO_8A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_8B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_8B,
  'J', ARG_OXO_8B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_8A,
  'H', -1, -1, ARG_OXO_8C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_8A,
  'L', -1, -1, ARG_OXO_8C,
  'J', ARG_OXO_9A, 0, 4,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_9B,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_9B,
  'J', ARG_OXO_9B, 0, 6,
  'T', -1, -1, OUT_OXO_COLLECT,
  'H', -1, -1, ARG_OXO_9A,
  'H', -1, -1, ARG_OXO_9C,
  'Y', -1, -1, 500,
  'L', -1, -1, ARG_OXO_9A,
  'L', -1, -1, ARG_OXO_9C,
  -1,  

  -1,
};      

};
