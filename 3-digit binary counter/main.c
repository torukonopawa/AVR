/*	
 *	3 digit binary counter
 *
 *	device: ATmega16
 *
 *	https://github.com/torukonopawa
 *
 *	1x button, 3x LEDs
 *	binary number displayed with 3 LEDs, incremented ith button
 *
 */

#include<avr/io.h>
#include<util/delay.h>

void init_io(void);

int main(void) {

	init_io();
	int counter = 0;

	while(1) {

		if(!(PINB & (1<<PB1))) counter++; //PORTB ^= (1<<PB2);

		switch(counter) {
		case 0:
			// - - -
			PORTB |= (1<<PB2 | 1<<PB3 | 1<<PB4);
			break;
		case 1:
			// - - 1
			PORTB |= (1<<PB3 | 1<<PB4);
			PORTB &=~ (1<<PB2);
			break;
		case 2:
			// - 1 -
			PORTB |= (1<<PB2 | 1<<PB4);
			PORTB &=~ (1<<PB3);
			break;
		case 3:
			// - 1 1
			PORTB |= (1<<PB4);
			PORTB &=~ (1<<PB2 | 1<<PB3);
			break;
		case 4:
			// 1 - -
			PORTB |= (1<<PB2 | 1<<PB3);
			PORTB &=~ (1<<PB4);
			break;
		case 5:
			// 1 - 1
			PORTB |= (1<<PB3);
			PORTB &=~ (1<<PB2 | 1<<PB4);
			break;
		case 6:
			// 1 1 -
			PORTB |= (1<<PB2);
			PORTB &=~ (1<<PB3 | 1<<PB4);
			break;
		case 7:
			// 1 1 1
			PORTB &=~ (1<<PB2 | 1<<PB3 | 1<<PB4);
			break;
		default:
            // if counter > 7 (111)
			counter = 0;
		}

		_delay_ms(500);
		 //PORTB |= (1<<PB2);
	}

	return 0;
}

void init_io(void){
	DDRB &= ~(1<<PB1);//SW1 input
	// ???? ??0?
	PORTB |= (1<<PB1);//pull-up
	// ???? ??1?

	DDRB |= (1<<PB2 | 1<<PB3 | 1<<PB4);//LED1,2,3 out
	// ???1 11??
	PORTB |= (1<<PB2 | 1<<PB3 | 1<<PB4);//LED1,2,3 off
	// ???1 11??
}

