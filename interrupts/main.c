/*	
 *	Hello Interrupts
 *
 *	device: ATmega16
 *
 *	https://github.com/torukonopawa
 *
 *	1x button, 2x LEDs
 *	INT0 interrupt is used to turn on/off a LED
 *	while another LED is turning on/off with 5 sec delay
 */

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

void init_io(void);
void init_int0(void);

ISR(INT0_vect) {
	PORTB ^= (1<<PB3);
}

int main(void) {
	//b1 sw, b2 b3 leds
	init_io();
	init_int0();

	while(1)
	{
		PORTB ^= (1<<PB2);
		_delay_ms(5000);
	}

	return 0;
}

void init_int0(void) {
	MCUCR |= (1<<ISC01);
	GICR |= (1<<INT0);
	sei();
}

void init_io(void) {
	DDRD &= ~(1<<PD2); //XXXX XX1X SW
	DDRB |= (1<<PB2); //XXXX X0XX LED
	DDRB |= (1<<PB3); //XXXX 0XXX LED

	PORTD |= (1<<PD2); //pull up for SW
	PORTB |= (1<<PB2 | 1<<PB3); //LEDS turned off
}

