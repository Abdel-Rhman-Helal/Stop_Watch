# Stop_Watch
it's a simple Mini Project Using Atmega32 to count time and overflow when reach to 100 Hours

Using ATmega32 Microcontroller with frequency 1Mhz.

Timer1 in ATmega32 with CTC mode to count the Stop Watch time.

six Common Anode 7-segments.

six 7-segments in the project using the multiplexed technique.

one 7447 decoder for all 7-segments.

## there is a 3 Push button
1. reset:
External Interrupt INT0 with falling edge. Connect a push button with the 
internal pull-up resistor. If a falling edge detected the Stop Watch time should be
reset.

2. paused:
External Interrupt INT1 with raising edge. Connect a push button with the 
external pull-down resistor. If a raising edge detected the Stop Watch time should be

3. resumed:
External Interrupt INT2 with falling edge. Connect a push button with the 
internal pull-up resistor. If a falling edge detected the Stop Watch time should be
resumed
