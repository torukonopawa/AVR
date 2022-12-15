/*
 * tempSoftwarePwm.c
 *
 * Created: 12/14/2022 2:33:25 PM
 * Author : MH
 */ 
#define F_CPU 8000000UL
// - PWM GENERATORS -
#define pwm_ddr_1	DDRB
#define pwm_port_1	PORTB
#define pwm_pin_1	PB0

#define pwm_ddr_2	DDRB
#define pwm_port_2	PORTB
#define pwm_pin_2	PB1

// - CONTROL BUTTONS -
#define duty_up_ddr_1		DDRD
#define duty_up_port_1		PORTD
#define duty_up_PINx_1		PIND
#define duty_up_pin_1		PD7

#define duty_down_ddr_1		DDRD	
#define duty_down_port_1	PORTD
#define duty_down_PINx_1	PIND
#define duty_down_pin_1		PD6

#define duty_up_ddr_2		DDRD
#define duty_up_port_2		PORTD
#define duty_up_PINx_2		PIND
#define duty_up_pin_2		PD5

#define duty_down_ddr_2		DDRD
#define duty_down_port_2	PORTD
#define duty_down_PINx_2	PIND
#define duty_down_pin_2		PD4

// - CONSTANTS -
#define resolution 8

//	f/8 =>	256*8us = 2048us
#define timer0_clock (1<<CS01)|(1<<CS00) //((1<<CS02)|(1<<CS01)|(1<<CS00))

//tc0 => 0-255 to be safe, 5-250
#define duty_min_1	5
#define duty_max_1	250

#define duty_min_2	5
#define duty_max_2	250

#define duty_step	5

// - - - END - - -

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t timer_period;
volatile uint16_t counter;
volatile uint8_t duty_1;
volatile uint8_t duty_2;
volatile uint8_t period;


void init_io();
void init_tc0();

ISR(TIMER0_OVF_vect) {
	// - - - 1.Solution - - -
	//every	1.250ms
	counter++;
	
	if(counter < duty_1)
	{
		pwm_port_1 |= (1<<pwm_pin_1);
	}
	else if(counter >= duty_1 && counter < period)
	{
		pwm_port_1 &= ~(1<<pwm_pin_1);
	}
	else
	{
		counter = 0;
	}
	
	TCNT0 = 0xFF - timer_period;
	//TCNT0 = 255 - 156;
	
	// - - - 2. Solution - - -
	//every 2.048ms
	/*
	counter++;
	
	if(counter < duty_1)
	{
		pwm_port_1 |= (1<<pwm_pin_1);
	}
	else if(counter >= duty_1 && counter < period)
	{
		pwm_port_1 &= ~(1<<pwm_pin_1);
	}
	else
	{
		counter = 0;
	}
	*/
	// - - - END - - -
}

int main(void) {
    init_io();
	init_tc0();
	
	// - - - 1. Solution - - -
	
	//	minimum 30.7 Hz
	//	maximum 7812.5 Hz
	uint8_t fPWM = 50;
	float tInterval = (1 / (2 * fPWM * resolution)); //sec
	//tInterval = 0.00125s, 1.25ms, 1250us
	//	f/8 =>	256*8us = 2048us
	//		1step	8us(f/{8})
	//		?step	1250us(tInterval)
	//timer_period = (tInterval / 8);
	timer_period = (tInterval / 8) * 1000000;
	TCNT0 = 0xFF - timer_period;
	//duty_1 = timer_period * 0.25;
	// PROBLEM HERE CANNOT CAST READ 16 FROM PERIODDDDDDDDD
	duty_1 = 15;
	period = (float) 1 / (fPWM * tInterval);//((1000/fPWM)/(tInterval/1000)); //for 50Hz -> 16
	duty_2 = timer_period * 0.25;
	//timer_period range: [timer_period_min,timer_period_max]
	
	// - - - END - - -
	
	// - - - 2. Solution - - -
	//2nd solution is better with f/1 hz
	//		f/1 => 20ms/0.256ms = 78.125<-period
	//		f/8 => 20ms/2.048ms = 9.765<-period
	// - - - f/1 - - -
	/*
	period = 78;
	duty = 40;
	*/
	// - - - end - - -
	// - - - f/8 - - -
	/*
	period = 10;
	duty_1 = 4;
	duty_2 = 2;
	*/
	// - - - end - - -
	// - - - END - - -
	
	sei();
    while (1) 
    {
		//pwm_port_1 ^= (1<<pwm_pin_1);
		//_delay_ms(500);
		pwm_port_2 ^= (1<<pwm_pin_2);
		_delay_ms(500);
		/*
		if((duty_up_PINx_1 & (1<<duty_up_pin_1)))
		{
			_delay_ms(5);
			if((duty_up_PINx_1 & (1<<duty_up_pin_1)))
			{
				if(duty_1 < (duty_max_1 - duty_step - 1))
					duty_1 = duty_1 + duty_step;
			}
		}
		if((duty_down_PINx_1 & (1<<duty_down_pin_1)))
		{
			_delay_ms(5);
			if((duty_down_PINx_1 & (1<<duty_down_pin_1)))
			{
				if(duty_1 > (duty_min_1 + duty_step + 1))
					duty_1 = duty_1 - duty_step;
			}
		}
		if((duty_up_PINx_2 & (1<<duty_up_pin_2)))
		{
			_delay_ms(5);
			if((duty_up_PINx_2 & (1<<duty_up_pin_2)))
			{
				if(duty_2 < (duty_max_2 - duty_step - 1))
					duty_2 = duty_2 + duty_step;
			}
		}
		if((duty_down_PINx_2 & (1<<duty_down_pin_2)))
		{
			_delay_ms(5);
			if((duty_down_PINx_2 & (1<<duty_down_pin_2)))
			{
				if(duty_2 > (duty_min_2 + duty_step + 1))
					duty_2 = duty_2 - duty_step;
			}
		}
		*/
    }
}

void init_io() {
	//out
	pwm_ddr_1 |= (1<<pwm_pin_1);
	pwm_port_1 |= (1<<pwm_pin_1); //1 -> led off
	
	pwm_ddr_2 |= (1<<pwm_pin_2);
	pwm_port_2 |= (1<<pwm_pin_2);
	
	//in
	duty_up_ddr_1 &= ~(1<<duty_up_pin_1);
	duty_up_port_1 |= (1<<duty_up_pin_1); //1 -> pull-up
	
	duty_down_ddr_1 &= ~(1<<duty_down_pin_1);
	duty_down_port_1 |= (1<<duty_down_pin_1);
	
	duty_up_ddr_2 &= ~(1<<duty_up_pin_2);
	duty_up_port_2 |= (1<<duty_up_pin_2); //1 -> pull-up
	
	duty_down_ddr_2 &= ~(1<<duty_down_pin_2);
	duty_down_port_2 |= (1<<duty_down_pin_2);
}

void init_tc0() {
	TCCR0B |= timer0_clock; //f/1
	
	TIMSK0 |= (1<<TOIE0); //OVF_int enable
}