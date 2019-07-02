# Automaton Pinball

Pinball programming framework being built on a 1973 Williams OXO pinball machine.

- Modular cat5 cable connected architecture with just a single Teensy 3.2 mcu
- Three dimensional switch matrix with 5 x 8 x 8 = 320 switches
- Outputs controlled by addressable led strips (sk6812/ws2812b)
- Solenoids driven by optical mosfet switches
- Galvanic isolation between logical (5V) and power circuits (24V+)
- Event driven Automaton-framework based code
