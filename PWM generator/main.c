#include<avr/io.h>
#include<util/delay.h>
//#include<avr/interrupt.h>

#define	_MAX16bit	65536 //0xFFFF

#define	period_20ms	(_MAX16bit-20000) //0xb1df
#define duty_MIN	(_20ms+1000)
#define duty_MAX	(_20ms+2500)

#define add_p25ms	250
#define add_p5ms	500
#define add_1ms		1000

void init_io(void);
void init_tc1(void);

int main(void) {

	init_io();
	init_tc1();
	
	//TCNT1H |= 0xFF; //TCNT1L |= 0xFF;
	TCNT1 = period_20ms; //period is set to 20ms
	OCR1A = duty_MIN; //duty cycle 1ms/20ms

	while(1)
	{
		if(!(PIND & (1<<PD2)))
		{
			while(OCR1A < duty_MAX)
			{
				OCR1A = OCR1A + add_p25ms;
				_delay_ms(50);
			}
		}
		else if(!(PIND & (1<<PD3)))
		{
			while(OCR1A > duty_MIN)
			{
				OCR1A = OCR1A - add_p25ms;
				_delay_ms(50);
			}
		}
	}

	return 0;
}

void init_tc1(void) {
	/*
	 * 1 MHz 16bit
	 * task-> 20ms (50Hz)
	 *
	 * 0-65536,
	 * f 		T = 1us		tot = 65536us	65.536 ms
	 * f/8 		T = 8us		tot =  ms
	 * f/64 	T = 64us	tot =  ms
	 * f/256 	T = 256us	tot =  ms
	 * f/1024	T = 1024us	tot =  ms
	 */

	TCCR1B |= (1<<CS10);
	//TCCR1B |= (1<<CS11);
	//TCCR1B |= (1<<CS12); //set to f

	TCCR1A |= (1<<COM1A1);
	//TCCR1A |= (1<<COM1A0); //non-inverting mode

	// WGM	13	12	11	10
	//		1	1	1	0 //fast pwm mode (top: ICR1)
	//TCCR1A |= (1<<WGM10);
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<WGM13);

	//top: 0xFFFF
	ICR1 = 0xFFFF;
}

void init_io(void) {
	DDRD &= ~(1<<PD2); //XXXX XX1X SW
	DDRD &= ~(1<<PD3); //XXXX X1XX SW

	DDRD |= (1<<PD5); //XXXX 0XXX LED *PD5 OC1A pin

	PORTD |= (1<<PD2); //pull up for SW
	PORTD |= (1<<PD3); //pull up for SW
	PORTB |= (1<<PD5); //LEDS turned off
}


