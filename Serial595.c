#include	"main.h"
#include	"Serial595.h"
//#include	"SPI.h"

#ifndef SPI_Mode

void Serial595_Initial(void)
{
	//Serial595 Initial
	Serial595_SI=0;
	Serial595_EN=0;
	Serial595_CLK=0;
}

void Serial595(unsigned int LEDIO_Work)
{
	unsigned char	i=0, j=0, Temp_Value[Digit_Num];

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
		while(i<8)
		{
			LED_CLK=LOW;

			if(Temp_Value[j] & 0x80) LED_SI=HIGH;
			else	LED_SI=LOW;

			Temp_Value[j]<<=1;

			LED_CLK=HIGH;
			i++;

		}
		i=0;
		j++;
	}

	LED_EN=HIGH;
	LED_EN=LOW;
}
#endif

#ifdef	SPI_Mode
void Serial595_Initial(void)
{
	//Serial595 Initial
	uint8_t	j=0;
	
	while(j<Digit_Num)
	{
		SPI_Write_Byte(0x00);
		
		j++;
	}

	LED_EN=HIGH;
	LED_EN=LOW;

}

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
		SPI_Write_Byte(Temp_Value[j]);
		
		j++;
	}

	LED_EN=HIGH;
	LED_EN=LOW;
}

#endif