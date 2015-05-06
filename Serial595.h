#ifndef	_Serial595_H
#define	_Serial595_H

//#define	SPI_Mode	//Using Hardware SPI Interface

#define Digit_Num 4				//Numbers of Digit
const static uint8_t SEG_TABLE[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

#ifndef	SPI_Mode

#define LED_CLK		LATAbits.LATA0
#define LED_EN		LATAbits.LATA4
#define LED_SI		LATAbits.LATA1

#define Serial595_SI LED_SI
#define Serial595_CLK LED_CLK
#define Serial595_EN LED_EN

#endif

#ifdef	SPI_Mode

#define LED_EN		P4_bit.P40
//#define LED_EN		P1_bit.P14

#define Serial595_EN LED_EN

#endif

void Serial595_Initial(void);
void Serial595(uint16_t LEDIO_Work);

#endif