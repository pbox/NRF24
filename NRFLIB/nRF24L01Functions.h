#include "nRF24L01.h"
#include "nRF24L01Config.h"

// easier config macros ////////////////////////////////////////////////////////
#define CONCAT(a,b) a ## b

#define SPI_DDR2(a) CONCAT(DDR,a)
#define SPI_DDR SPI_DDR2(SPI_PORT_NAME)
#define SPI_PORT2(a) CONCAT(PORT,a)
#define SPI_PORT SPI_PORT2(SPI_PORT_NAME)

#define CE_DDR2(a) CONCAT(DDR,a)
#define CE_DDR CE_DDR2(CE_PORT_NAME)
#define CE_PORT2(a) CONCAT(PORT,a)
#define CE_PORT CE_PORT2(CE_PORT_NAME)

#define IRQ_DDR2(a) CONCAT(DDR,a)
#define IRQ_DDR IRQ_DDR2(IRQ_PORT_NAME)
#define IRQ_PORT2(a) CONCAT(PORT,a)
#define IRQ_PORT IRQ_PORT2(IRQ_PORT_NAME)

// bitwise operations macros ///////////////////////////////////////////////////

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT)) 
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT)) 

// SPI functions ///////////////////////////////////////////////////////////////

void Init_SPI(void)
{
  CE_DDR |= (1<<CE_PIN);
  SPI_DDR |= (1<<MOSI_PIN) | (1<<CSN_PIN) | (1<<SCK_PIN);
  SETBIT(SPI_PORT,CSN_PIN);
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); // fsk/16
}

uint8_t Send_SPI(uint8_t data)
{
  SPDR=data;
  while (!(SPSR &(1<<SPIF)));  //wait
  return SPDR;
}

// NRF functions ///////////////////////////////////////////////////////////////
void NRF_Init(void) 
{
  Init_SPI();
  CLEARBIT(CE_PORT, CE_PIN);
  SETBIT(SPI_PORT, CSN_PIN);	
}


void NRF_Write_Register_Buffer(uint8_t reg, const uint8_t* buffer, uint8_t size)
{
  CLEARBIT(SPI_PORT, CSN_PIN);	
  Send_SPI( W_REGISTER | ( REGISTER_MASK & reg ) ); // 0b001x xxxx
  while (size--)
  Send_SPI(*buffer++);
  SETBIT(SPI_PORT, CSN_PIN);
}

void NRF_Write_Register_Value(uint8_t reg, uint8_t value)
{
  CLEARBIT(SPI_PORT, CSN_PIN);	
  Send_SPI( W_REGISTER | ( REGISTER_MASK & reg ) ); // 0b001x xxxx
  Send_SPI(value);
  SETBIT(SPI_PORT, CSN_PIN);
}

uint8_t NRF_Read_Register_Value(uint8_t reg)
{
  uint8_t temp;	
  CLEARBIT(SPI_PORT, CSN_PIN);	
  Send_SPI( R_REGISTER | ( REGISTER_MASK & reg ) ); // 0b000x xxxx
  temp = Send_SPI(NOP);
  SETBIT(SPI_PORT, CSN_PIN);
  return temp;
}

void NRF_Reset_IRQ(void)
{
  NRF_Write_Register_Value(STATUS,RESET_IRQ); 
}

void NRF_Flush_RX(void)
{
  CLEARBIT(SPI_PORT, CSN_PIN);
  Send_SPI(FLUSH_RX);
  SETBIT(SPI_PORT, CSN_PIN);
}


void NRF_Flush_TX(void)
{
  CLEARBIT(SPI_PORT, CSN_PIN);
  Send_SPI(FLUSH_TX);
  SETBIT(SPI_PORT, CSN_PIN);
}

void NRF_Config(void) 
{
  NRF_Write_Register_Value(EN_AA,EN_AA_VAL);
  NRF_Write_Register_Value(SETUP_RETR,SETUP_RETR_VAL);
  NRF_Write_Register_Value(EN_RXADDR,EN_RXADDR_VAL); 
  NRF_Write_Register_Value(SETUP_AW,ADDRESS_LENGTH - 2);
  NRF_Write_Register_Value(RF_CH,RF_CH_VAL);
  NRF_Write_Register_Value(RF_SETUP,RF_SETUP_VAL);
  NRF_Write_Register_Buffer(RX_ADDR_P0,(uint8_t*)RX_ADDR_P0_VAL,ADDRESS_LENGTH);
  NRF_Write_Register_Buffer(TX_ADDR,(uint8_t*)TX_ADDR_VAL,ADDRESS_LENGTH);
  NRF_Write_Register_Value(RX_PW_P0,RX_PW_P0_VAL); 
  NRF_Reset_IRQ(); // just in case
  NRF_Flush_RX();
  NRF_Flush_TX();
}


void NRF_Start_RX(void)
{
  NRF_Write_Register_Value(CONFIG,CONFIG_VAL| 0x01);
  _delay_us(5); // tpd2stby
  SETBIT(CE_PORT, CE_PIN);
  _delay_us(150); // tstby2a, Thce and Tpece2csn
}

void NRF_Start_TX(void)
{
  NRF_Write_Register_Value(CONFIG,CONFIG_VAL);
  _delay_us(5); // tpd2stby and tstby2a
}

void NRF_Transmit(uint8_t *buffer, uint8_t size)
{
uint8_t actual_size, nops;
uint8_t *p;
p=&buffer[0];
actual_size=size>RX_PW_P0_VAL?RX_PW_P0_VAL:size;
nops=RX_PW_P0_VAL-actual_size;

NRF_Flush_TX();

CLEARBIT(SPI_PORT, CSN_PIN);
Send_SPI( W_TX_PAYLOAD );

while (actual_size-- )
  Send_SPI(*p++); 
while (nops-- )
  Send_SPI(NOP);   
  
  
SETBIT(SPI_PORT, CSN_PIN);
SETBIT(CE_PORT, CE_PIN);
_delay_us(15); // Thce and Tpece2csn
CLEARBIT(CE_PORT, CE_PIN);
}


