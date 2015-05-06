#include	"main.h"
#include	"Timer.h"
#include	"Buzzer.h"

void Beep( uint16_t Buzzer_Freq , uint16_t beep_time_ms )
{
	//uint16_t	us , beep_timer ;
	static uint16_t	us , beep_timer ;

	INTCONbits.T0IE	= 0 ;
#ifdef Buzzer_In_Hardware

	static uint16_t Reload_16 ;
	static uint8_t R1H , R1L ;

	//uint16_t	Reload_16 ;
	//uint8_t 	R1H , R1L ;

	us = 1000000 / ( Buzzer_Freq * 2 ) ;
	Reload_16 = 65536UL - us + Buzzer_Freq_Offset ;	//us < 65536us
	
	// 8-bit reload values (High & Low)

	R1H= (uint8_t)(Reload_16 >> 8);
	R1L= (uint8_t)(Reload_16 & 0x00FF);

	beep_timer = beep_time_offset + ( beep_time_ms * 5 ) ;

	while( beep_timer-- )
	{
		TMR1H= R1H ;
		TMR1L= R1L ;
		
		Timer1_ON ;	//Timer1 Start
		
		Buzzer ^= 1 ;
	
		while( !PIR1bits.TMR1IF ) ;
		PIR1bits.TMR1IF = 0 ;

		Timer1_OFF ;	//Timer1 Stop

		NOP();	
	}
#else
	us = 1000000 / ( Buzzer_Freq * 2 ) ;
	beep_timer = beep_time_offset + ( beep_time_ms * 5 ) ;

	while( beep_timer-- )
	{
		Buzzer ^= 1 ;
		__delay_us ( 180 ) ;	// 370/2 = 185
	}
#endif
	Buzzer = 0 ;

	INTCONbits.T0IE	= 1 ;
}

void Buzzer_Go ( Buzzer_Type B_Type )
{
	switch ( B_Type )
	{
		case Initial_Bell:
			Beep(Buzzer_Freqeuncy,500);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;
		
		case Servo_High_Bell :
			Beep(Buzzer_Freqeuncy,500);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,100);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;
		
		case Servo_Mid_Bell:
			Beep(Buzzer_Freqeuncy,100);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;

		//case Servo_Low_Bell:
		//	Beep(Buzzer_Freqeuncy,100);
		//	break;
		
		case Servo_NO_SIGNAL_Bell:
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Servo_Stable_2MIN_Bell:
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Servo_Stable_3MIN_Bell:
			Beep(Buzzer_Freqeuncy,250);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Warning_10min_Bell:
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Warning_20min_Bell:
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Warning_30min_Bell:
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Warning_45min_Bell:
			Beep(Buzzer_Freqeuncy,50);
			delay_ms(100);
			Beep(Buzzer_Freqeuncy,50);
			break;

		case Warning_Low_Voltage_Bell:
			Beep(Buzzer_Freqeuncy,50);
			break;
	}
}