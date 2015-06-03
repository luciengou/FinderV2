#include	"main.h"
#include	"Timer.h"
#include	"INT.h"
#include	"GPIO.h"
#include	"ADC.h"
#include	"OSC.h"
#include	"Loop.h"
#include	"PWM.h"
#include	"FVR.h"
//#include	"Serial595.h"
#include	"Buzzer.h"
#include	"TMR1GATE.h"
#include	"EEPROM.h"

void setup(void)
{
	//OSC Initial
	OSC_Initial ( HF_4M ) ;

	//GPIO Initial
	GPIO_Initial ( ) ;

	//Set PWM Pin
	PWM_Initial ( ) ;		// For Breath LED
	PWM_OFF;
	//Timer Initial
	Timer_Initial ( ) ;

	//ADC Initial
	ADC_Initial ( FVR_CH );	// ADC Select

	//Interrupt Initial
	INT_Initial ( ) ;		// Interrupt Init

	//Timer1 Gate Initial
	TMR1_Gate_Initial ( TMR1_Gate_Source_Timer1_Gate_Pin ) ;

	//INTCONbits.PEIE = 0 ;		//Enable peripheral interrupt
	//INTCONbits.INTE = 1 ;		//Disable External interrupt
	INTCONbits.GIE  = 1 ;		//Enable global interrupt

	//Read Finder_Mode from EEPROM , or Initial EEPROM

	uint8_t	c = 0 ;

	c= int_EEPROM_getc( EEPROM_Address );

	switch ( c )
	{
		case 0 :
			Finder_Mode = HIGH_LOW ;
			
			Beep(Buzzer_Freqeuncy,500);
			
			uint8_t i=3;
						
			while (--i)
			{
				delay_ms(100);
				Beep(Buzzer_Freqeuncy,50);				
			}

			break ;

		case 1 :
			Finder_Mode = MOVING_STABLE ;
			
			uint8_t i=3;
						
			while (--i)
			{
				delay_ms(100);
				Beep(Buzzer_Freqeuncy,50);
			}

			break ;
		
		default :
			
			int_EEPROM_putc ( EEPROM_Address , 0 );
	}

}
