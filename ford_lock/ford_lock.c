/*
 * ford_lock.c
 *
 * Created: 20.06.2017 9:37:56
 *  Author: OFPR
 */ 
#define F_CPU 500000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#define LOCK PORTB=0x04
#define UNLOCK PORTB=0x08

void setup(void)
{
	cli();
	DDRB = 0x0C;
	DDRD = 0x20;
	PORTD = 0x2C; 
	MCUCR = 1<<ISC00|1<<ISC11;//1<<ISC00|1<<ISC01; 
	GIMSK = 1<<INT0|1<<INT1;//vnewnee prerivanie
	asm("WDR");
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = (1<<WDE) | (1<<WDP3) | (1<<WDP0);
	sei();
}

int main(void)
{
	setup();
    while(1)
    {
		PORTB &= 0x00;
		asm("WDR");
		_delay_ms(100);
    }
}

ISR(INT0_vect)
{
	cli();
	_delay_ms(50);
	//LOCK;
	if ((PIND & (1 << PD2)) == 0)
	{
		LOCK;
	} 
	else
	{
		UNLOCK;
	}
	_delay_ms(800);

	//EIFR &= ~((1<<INTF0)|(1<<INTF1));
	EIFR = 0xFF;
	PORTB &= 0x00;
	sei();
}
ISR(INT1_vect)
{
	cli();
	_delay_ms(50);
	if (((PIND & (1 << PD3)) == 0)&&((PIND & (1 << PD2)) == 0))
	{
		UNLOCK;
	}
	_delay_ms(800);

	//EIFR &= ~((1<<INTF0)|(1<<INTF1));
	EIFR = 0xFF;
	PORTB &= 0x00;
	sei();
}