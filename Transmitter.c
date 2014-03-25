#include <avr/io.h>
#include <stdio.h>
#define F_CPU 12000000UL
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
  
  DDRB |= (1<<DDB0); //led
  SETBIT(PORTB,0); //led off
  
  NRF_Init();
  NRF_Config();
  INT0_init();
  NRF_Start_TX();
  sei();
  
  while(1)
  {
    if(p++%2)
    {
      CLEARBIT(PORTB, 0); // led on
    }
    else
    { 
      SETBIT(PORTB, 0); // led off
    }
    _delay_ms(1000);
    NRF_Transmit((uint8_t *)"Aze",3);
  }
  return 0;
}




ISR(INT0_vect)	
{
uint8_t value;
cli();	
value=NRF_Read_Register_Value(STATUS);
NRF_Reset_IRQ();  
value=NRF_Read_Register_Value(STATUS);

CLEARBIT(PORTB, 0); //led on
_delay_ms(100);
SETBIT(PORTB, 0); //led off
_delay_ms(100);
  
sei();
}


