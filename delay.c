#include	"main.h"
#include	"INT.h"
#include	"Timer.h"

void delay_25ms(void)
{
#ifndef	Hardware_Delay
	__delay_ms( 25 );
#else
	delay_ms( 25 );
#endif
}
void delay_1s(void)
{
#ifndef	Hardware_Delay
	__delay_ms(1000);
#else
	delay_ms ( 1000 ) ;
#endif
}
void delay_500ms(void)
{
#ifndef	Hardware_Delay
	__delay_ms(500);
#else
	//delay_ms ( 500 ) ;
	uint8_t i = 10 ;

	while ( i-- )
	{
		TMR1H = 0x3C ;	// 65536-50000=0x3CB0
		TMR1L = 0xB0 ;	// 0xFC18
	
		Timer1_ON ;		//Timer1 Start

		while( !PIR1bits.TMR1IF ) ;
		PIR1bits.TMR1IF = 0 ;

		Timer1_OFF ;	//Timer1 Stop
	}
	
#endif
}
void delay_ms(uint16_t ms)
{
	while(ms--){
#ifndef	Hardware_Delay
		__delay_ms(1);
#else
		delay_1ms();
#endif
	}
}
void delay_1ms(void)
{
#ifdef	Hardware_Delay
	TMR1H = 0xFC ;	// 65536-1000=0xFC18
	TMR1L = 0x18 ;	// 0xFC18
	
	Timer1_ON ;		//Timer1 Start

	while( !PIR1bits.TMR1IF ) ;
	PIR1bits.TMR1IF = 0 ;

	Timer1_OFF ;	//Timer1 Stop
#else
	__delay_ms(1);
#endif
}

void delay_1us(void)
{
	__delay_us(1);
}

void delay_us(uint16_t us)
{
	while ( us-- )
		__delay_us(1);
}	
