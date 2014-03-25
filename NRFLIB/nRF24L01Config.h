#define SPI_PORT_NAME B
#define MISO_PIN 4
#define MOSI_PIN 3
#define SCK_PIN  5
#define CSN_PIN  2

#define CE_PORT_NAME  B
#define CE_PIN    1


#define EN_AA_VAL      0b00000000 // no auto ack
#define SETUP_RETR_VAL 0b11110000 // 4000us + disabled
#define EN_RXADDR_VAL  0b00000001 // 1 pipe
#define RF_CH_VAL              57 // channel (1-127) 
#define RF_SETUP_VAL   0b00000111 // 1mbps, 0db
#define ADDRESS_LENGTH          5 // 3,4 or 5 are the only legal values
#define RX_ADDR_P0_VAL    "Rndom" // 5 byte address 
#define TX_ADDR_VAL       "Rndom" // 5 byte address 
#define RX_PW_P0_VAL            3 // payload size. 3 bytes 
#define CONFIG_VAL     0b00001110 // not masking interrupts, 2 bytes crc, power up

#define RESET_IRQ      0b01110000 
