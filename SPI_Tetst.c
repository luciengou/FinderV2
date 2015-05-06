#include "ioMG82FEL564.h"

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

//#define	nCS	 P4_bit.P40

//----- SPCTL -----//
#define	SSIG				   0x80
#define	SPEN				   0x40
#define	DORD				   0x20
#define	MSTR				   0x10
#define	CPOL				   0x08
#define	CPHA				   0x04
#define	SPR1				   0x02
#define	SPR0				   0x01
/*
//----- SPSTAT -----//
#define	SPIF				   0x80
#define	THRF				   0x20
#define	SPIBSY				   0x10
*/
//#define	nCS					P1_bit.P14
#define	nCS					P4_bit.P40

void Initial_SPI(void)
{
	//EIE1_bit.ESPI=1;
	SPCON=0;
	SPCON |= (SSIG | SPEN | MSTR ); // enable SPI and Master mode
	P1M0 = 0x00;
	P1M1 = 0xA0;
	
	P4M0 =0x00;
	P4M1 =0x4D;  //0,2,3,6 01001101
}

void SPI_Write(unsigned char arg)
{
	SPDAT = arg; //write arg
	
	while(!SPSTAT_bit.SPIF); //wait transfer finishes
	SPSTAT_bit.SPIF=1; //clear SPI interrupt flag
	
}

unsigned char SPI_Read(void)
{
	SPDAT = 0xFF; //trigger SPI read
	
	while(!SPSTAT_bit.SPIF); //wait transfer finishes
	SPSTAT_bit.SPIF=1; //clear SPI interrupt flag

	return SPDAT;
}


#define Digit_Num 4				//Numbers of Digit
const static uint8_t SEG_TABLE[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Serial595(uint16_t LEDIO_Work)
{
	uint8_t	i=0, j=0, Temp_Value[Digit_Num];

	while(i<Digit_Num)
	{
		Temp_Value[Digit_Num-i-1] = SEG_TABLE[LEDIO_Work % 10];

		LEDIO_Work/=10;
		i++;
	}

	if(Temp_Value[0]==SEG_TABLE[0])	Temp_Value[0]=0;
	if(Temp_Value[0]==0 && Temp_Value[1]==SEG_TABLE[0])	Temp_Value[1]=0;
	if(Temp_Value[0]==0 && Temp_Value[1]==0 && Temp_Value[2]==SEG_TABLE[0])	Temp_Value[2]=0;

	i=0;

	while(j<Digit_Num)
	{
		SPI_Write(Temp_Value[j]);

		j++;
	}

	nCS=1;
	nCS=0;
}

#define	LED_1	P4_bit.P42
#define	LED_2	P4_bit.P43
#define	LED_3	P4_bit.P46
// Delay
#define NOP()         asm("nop")            //nop

void main(void)
{
	uint16_t	i=0;
	uint32_t	delay;
	Initial_SPI();
	IE_bit.EA=1;
	
	while (1)
	{
		delay=0x1FFFF;
		while(delay--) NOP();
				
		LED_1^=1;
		Serial595(i++);
	}
	
}