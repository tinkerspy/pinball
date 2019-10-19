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

enum { IN_GAME_INIT, IN_GAME_PRESS, IN_GAME_RELEASE, SUB_GAME_WAIT_PLAYERS, SUB_GAME_WAIT_RESET, 
         SUB_GAME_BALL_LOOP, SUB_GAME_PLAYER_LOOP, SUB_GAME_CORE, SUB_GAME_WAIT_PLAYING, SUB_GAME_WAIT_COLLECTING }; 
enum { OUT_GAME_INIT, OUT_GAME_ENABLE, OUT_GAME_COUNTER_RESET, OUT_GAME_PLAYERS_ZERO, OUT_GAME_BALL_ZERO, OUT_GAME_PLAYER_ZERO, 
        OUT_GAME_BALL_ADV, OUT_GAME_PLAYER_ADV, OUT_GAME_3BONUS, OUT_GAME_COLLECT, OUT_GAME_KICKOFF, OUT_GAME_PLAYERS_FIX, OUT_GAME_OVER };
enum { ARG_GAME_ENABLED, ARG_GAME_COUNTER0, ARG_GAME_COUNTER1, ARG_GAME_COUNTER2, ARG_GAME_COUNTER3, ARG_GAME_COLLECTING, ARG_GAME_AGAIN };
enum { REG_GAME_STATE, REG_GAME_NOPLAYERS, REG_GAME_NOBALLS, REG_GAME_BALL, REG_GAME_PLAYER };

int16_t game_firmware[] = {
  IN_GAME_INIT, 
  IN_GAME_PRESS, 
  IN_GAME_RELEASE, 
  SUB_GAME_WAIT_PLAYERS, 
  SUB_GAME_WAIT_RESET, 
  SUB_GAME_BALL_LOOP, 
  SUB_GAME_PLAYER_LOOP, 
  SUB_GAME_CORE,
  SUB_GAME_WAIT_PLAYING, 
  SUB_GAME_WAIT_COLLECTING,
  -1,
          
  IN_GAME_PRESS,
  'R', -1, -1,  REG_GAME_NOPLAYERS,
  'I', -1, -1,  1,
  //'U', -1, -1, -1,
  -1,
  
  IN_GAME_INIT,
  'R', -1, -1, REG_GAME_NOBALLS,
  'Z', -1, -1, 3,
  'T', -1, -1, OUT_GAME_OVER,      // $
  'S', -1, -1, SUB_GAME_WAIT_PLAYERS,
  'T', -1, -1, OUT_GAME_INIT,
  'T', -1, -1, OUT_GAME_PLAYERS_ZERO,
  'T', -1, -1, OUT_GAME_BALL_ZERO,
  'T', -1, -1, OUT_GAME_COUNTER_RESET,
  'S', -1, -1, SUB_GAME_WAIT_RESET,
  'R', -1, -1, REG_GAME_NOBALLS,   
  'D', -1, -1, REG_GAME_BALL,      // ball = noballs
  'S', -1, -1, SUB_GAME_BALL_LOOP,
  'A', -1, -1, IN_GAME_INIT,
  -1,
  
  SUB_GAME_BALL_LOOP,
  'R', -1, -1, REG_GAME_NOPLAYERS, // $$
  'D', -1, -1, REG_GAME_PLAYER,    // player = noplayers
  'S', -1, -1, SUB_GAME_PLAYER_LOOP,
  'T', -1, -1, OUT_GAME_BALL_ADV,
  'R', -1, -1, REG_GAME_BALL,
  'I', -1, -1, -1,                 // ball--
  'A',  0, -1, SUB_GAME_BALL_LOOP,
  -1,
  
  SUB_GAME_PLAYER_LOOP,
  'S', -1, -1, SUB_GAME_CORE,
  'J', ARG_GAME_AGAIN, -2, 0,
  'T', -1, -1, OUT_GAME_PLAYER_ADV,
  'R', -1, -1, REG_GAME_PLAYER,
  'I', -1, -1, -1,                 // player--
  'A',  0, -1, SUB_GAME_PLAYER_LOOP, // } while ( player > 0 );
  -1,
  
  SUB_GAME_CORE,
  'Y', -1, -1, 500,               
  'R', -1, -1, REG_GAME_BALL,
  'T',  1,  OUT_GAME_3BONUS, -1,
  'T', -1, -1, OUT_GAME_INIT,
  'Y', -1, -1, 100,               
  'T', -1, -1, OUT_GAME_ENABLE,
  'Y', -1, -1, 100,               
  'T', -1, -1, OUT_GAME_KICKOFF,
  'Y', -1, -1, 100,               
  '!', -1, -1, -1, // FIXME EXIT!!!!
  'S', -1, -1, SUB_GAME_WAIT_PLAYING,
  'Y', -1, -1, 100,               
  'T', -1, -1, OUT_GAME_COLLECT,
  'Y', -1, -1, 100,               
  'S', -1, -1, SUB_GAME_WAIT_COLLECTING,
  'Y', -1, -1, 100,               
  'T', -1, -1, OUT_GAME_PLAYERS_FIX,
  -1,
  
  SUB_GAME_WAIT_PLAYERS,
  'R', -1, -1, REG_GAME_NOPLAYERS,
  'Y', -1, -1, 10,                 // Wait (busy) until game_players > 0
  'A',  0, SUB_GAME_WAIT_PLAYERS, -1,
  -1,
  
  SUB_GAME_WAIT_RESET,
  'Y', -1, -1, 100,                // Wait (busy) until counter reset finishes
  'J', ARG_GAME_COUNTER0, -2, 0,
  'J', ARG_GAME_COUNTER1, -3, 0,
  'J', ARG_GAME_COUNTER2, -4, 0,
  'J', ARG_GAME_COUNTER2, -5, 0,
  -1,
  
  SUB_GAME_WAIT_PLAYING,
  'Y', -1, -1, 100,               
  'J', ARG_GAME_ENABLED, -2, -1,
  -1,
  
  SUB_GAME_WAIT_COLLECTING,
  'Y', -1, -1, 100,               
  'J', ARG_GAME_COLLECTING, -2, -1,
  -1,

-1,
};

/*
        
100 trigger GAME_OVER
102 while ( !switch( FRONTBTN ) 
105   wait 0
110 wend
120 trigger INIT
130 trigger RESET
140 trigger PLAYERS_ZERO
150 while ( led( COUNTER0 ) or led( COUNTER1 ) or led( COUNTER2 ) or led( COUNTER3 )
155   wait 0
160 wend
170 for BALL = 0 to NUMBER_OF_BALLS -1 
170   for PLAYER = 0 to NUMBER_OF_PLAYERS -1 ` Dit moet anders 
180     delay 500
190     trigger INIT
200     trigger PLAYERUP_ADVANCE
210     trigger BALLUP_ADVANCE
220     if ( BALL = NUMBER_OF_BALLS -1 ) then trigger TRIPLE_BONUS
230     trigger FEEDER
240     delay 500
245     trigger PLAYFIELD_ENABLE
247     delay 100
250     while ( led( LED_EXTRA ) ) 
260       wait 0
270     wend
280     trigger BONUS_COLLECT
285     delay 100
290     while ( led( VLED_COLLECTING ) ) 
300       wait 0
310     wend
320     delay 500
330     trigger PLAYERS_FIX
340     if ( led( again) ) then GOTO 180
350   next
360 next
380 goto 100
*/



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
