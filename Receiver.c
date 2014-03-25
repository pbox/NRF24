#include <avr/io.h>
#include <stdio.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../NRFLIB/nRF24L01Functions.h"


void INT0_init(void)	
{
DDRD &= ~(1<<DDD2);
EICRA |=  (1<<ISC01);
EICRA  &=  ~(1<<ISC00);
EIMSK |=  (1<<INT0);
} 


int main(void)
{
uint8_t p=0;
uint8_t value;
DDRB |= (1<<DDB0); //led
SETBIT(PORTB,0);

NRF_Init();
NRF_Config();
INT0_init();

NRF_Start_RX();

sei();

while(1)
{

if(p++%2)
{
CLEARBIT(PORTB, 0);
}
else
{ 
SETBIT(PORTB, 0);
}
_delay_ms(1000);


	
value=NRF_Read_Register_Value(STATUS);

}
	return 0;
}




ISR(INT0_vect)	
{
cli();	
CLEARBIT(PORTB, 0); //led off
_delay_ms(100);
SETBIT(PORTB, 0); //led on
_delay_ms(100);
CLEARBIT(PORTB, 0); //led off
_delay_ms(100);
SETBIT(PORTB, 0); //led on
_delay_ms(100);
CLEARBIT(PORTB, 0); //led off
_delay_ms(100);
SETBIT(PORTB, 0); //led on
_delay_ms(100);
CLEARBIT(PORTB, 0); //led off
_delay_ms(100);
SETBIT(PORTB, 0); //led on
_delay_ms(100);
CLEARBIT(PORTB, 0); //led off
_delay_ms(100);
SETBIT(PORTB, 0); //led on
_delay_ms(100);
CLEARBIT(PORTB, 0); //led off
_delay_ms(100);
SETBIT(PORTB, 0); //led on
_delay_ms(100);    
sei();
}


