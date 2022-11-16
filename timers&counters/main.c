/*	
 *	Hello Timers&Counters
 *
 *	device: ATmega16
 *
 *	https://github.com/torukonopawa
 *
 *	2x LEDs
 *	1Mhz clock frequency, 8bit timer with f/1024 prescaler:
 *	-> 0.001024*256 seconds ~= 0.262 seconds
 *	to get 1 seconds -> every 4 overflow interrupt
 */
 
#include<avr/io.h>
#include<avr/interrupt.h>

void init_io(void);
void init_tc0(void);

volatile int interruptCounter = 0; //volatile since used inside an interrupt

ISR(TIMER0_OVF_vect) {
	interruptCounter++;
	if(interruptCounter == 4)
	{
		PORTB ^= (1<<PB3); //turn on/off
		interruptCounter = 0;
	}
}

int main(void) {
	init_io();
	init_tc0();

	PORTB &= ~(1<<PB0); //first led turned on

	while(1)
	{}

	return 0;
}

void init_tc0(void) {
	TCCR0 |= (1<<CS00);
	//TCCR0 |= (1<<CS01);
	TCCR0 |= (1<<CS02); //set to f/1024

	TIMSK |= (1<<TOIE0);

	sei();
}
void init_io(void) {
	DDRB |= (1<<PB0); //XXXX X0XX LED
	DDRB |= (1<<PB3); //XXXX 0XXX LED

	PORTB |= (1<<PB0 | 1<<PB3); //LEDS turned off
}
