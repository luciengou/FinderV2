#include	"main.h"
#include	"Timer.h"

static void TIM0_Init( uint8_t Tick )
{
	//Timer 0 initial
	OPTION_REG &= 0xF8 ;		// Reset Prescaler
	OPTION_REGbits.TMR0CS = 0 ;	//0 = Internal instruction cycle clock (FOSC/4)
	
	if( Tick == 0)
	{
		OPTION_REGbits.PSA = 1 ;	// 0 = Prescaler is not assigned to the Timer0 module
									// Tick = Fosc/4 , 1 : 1
	}
	else
	{	
		OPTION_REGbits.PSA = 0 ;	// 0 = Prescaler is assigned to the Timer0 module
		OPTION_REG |= Tick - 1 ;		// Set TMR0 Prescaler
	}
}
static void TIM1_Init(uint8_t Tick)
{
	//Timer 1 initial
	T1CON = 0 ;		//Reset Timer 1
	T1CON |=Tick << 4 ;
	
	T1GCONbits.TMR1GE = 0 ;

	Timer1_OFF ;
	
	PIR1bits.TMR1IF = 0 ;
	
}

void Timer_Initial(void)
{
  	TIM0_Init(Timer0_4096us);	
  	TIM1_Init(Delay_Base_1us);
}
