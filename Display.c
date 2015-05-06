#include	"LED.h"
#include	"Timer.h"
#include	"SPI.h"
#include	"LiquidCrystal_SPI.h"
//#include	"ADC.h"
#include	"Display.h"

void Display_Channel_Value(uint8_t Ch_No, uint16_t Ch_Val)
{

	static uint16_t Pre_Ch_Val[4];
	
	if(Pre_Ch_Val[Ch_No]!=Ch_Val)
	{
		switch(Ch_No)
		{
			case 0 :
				LiquidCrystal_SPI_setCursor(4,0);
				LiquidCrystal_SPI_printstr("    ");
				LiquidCrystal_SPI_setCursor(4,0);

				break;
			case 1 :
				LiquidCrystal_SPI_setCursor(12,0);
				LiquidCrystal_SPI_printstr("    ");
				LiquidCrystal_SPI_setCursor(12,0);

				break;
			case 2 :
				LiquidCrystal_SPI_setCursor(4,1);
				LiquidCrystal_SPI_printstr("    ");
				LiquidCrystal_SPI_setCursor(4,1);

				break;
			case 3 :
						
				LiquidCrystal_SPI_setCursor(12,1);
				LiquidCrystal_SPI_printstr("    ");
				LiquidCrystal_SPI_setCursor(12,1);

				break;
		}
		Pre_Ch_Val[Ch_No]=Ch_Val;
		LiquidCrystal_SPI_printnum(Ch_Val);
	}
	
}