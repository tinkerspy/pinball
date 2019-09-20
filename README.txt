
Atm_led_scheduler
on( n )
off( n )
set( n, c )
toggle( n, v )
toggle( n )

Atm_element
  Atm_element& onInit( [] );
  Atm_element& onInput( {state,} [] );
  Atm_element& onKick( {state,} [] );
  Atm_element& onLight( {state,} [] );
  Atm_element& on( void );
  Atm_element& off( void );
  Atm_element& kick( void );
  Atm_element& release( void );
  Atm_element& input( void );
  Atm_element& init( void );
  Atm_element& disable( void );
  Atm_element& enable( void );
  Atm_element& toggle( void );
  Atm_element& autoLite( int v = 1 );
  int state() {returns true when led is lit, if there's no led returns the switch pressed state - flipper/score_digit}

  EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_KICK, EVT_RELEASE, EVT_INPUT, EVT_INIT, EVT_DISABLE, EVT_ENABLE  
  

