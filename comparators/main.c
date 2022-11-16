/*	
 *	Hello Comparators
 *
 *	device: ATmega16
 *
 *	https://github.com/torukonopawa
 *
 *	2x LEDs
 *	slowly turning on/off the led
 *	(brightness changes smoothly)
 */

#include<avr/io.h>
#include<util/delay.h>

void init_io(void);
void init_tc0(void);

int main(void) {

	init_io();
	init_tc0();

	PORTB &= ~(1<<PB3); //LED ON
	OCR0 = 0; //level

	while(1)
	{
		if(!(PIND&(1<<PD2)))
		{
			while(OCR0 < 255)
			{
				OCR0 = OCR0 + 1;
				_delay_ms(50);
			}
		}
		else if(!(PIND&(1<<PD3)))
		{
			while(OCR0 > 0)
			{
				OCR0 = OCR0 - 1;
				_delay_ms(50);
			}
		}
	}

	return 0;
}

void init_tc0(void) {
	/*
	 * 1 MHz 8bit
	 * task->	500Hz -> 2ms
	 * 			2 button
	 * 			duty cycle up/down
	 *
	 * 0-255,
	 * f 		T = 1us		tot = 256us		0.256 ms
	 * f/8 		T = 8us		tot = 2048us	2.048 ms
	 * f/64 	T = 64us	tot = 16384us	16.384 ms
	 * f/256 	T = 256us	tot = 65536us	65.536 ms
	 * f/1024	T = 1024us	tot = 262144us	262.144 ms
	 *
	 */

	//TCCR0 |= (1<<CS00);
	TCCR0 |= (1<<CS01);
	//TCCR0 |= (1<<CS02); //set to f/8

	TCCR0 |= (1<<WGM00);
	TCCR0 |= (1<<WGM01); //fast pwm mode
	TCCR0 |= (1<<COM01); //non-inverting mode
}

void init_io(void) {
	DDRD &= ~(1<<PD2); //XXXX XX1X SW
	DDRD &= ~(1<<PD3); //XXXX X1XX SW

	DDRB |= (1<<PB3); //XXXX 0XXX LED

	PORTD |= (1<<PD2); //pull up for SW
	PORTD |= (1<<PD3); //pull up for SW
	PORTB |= (1<<PB3); //LEDS turned off

}