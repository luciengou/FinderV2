#include	"main.h"
#include	"INT.h"
#include	"ADC.h"
#include	"loop.h"

void INT_Initial(void)
{
	INTCON = 0 ;
	INTCONbits.TMR0IE = 1 ;		//TMR0 Interrunpt Enable

	PIE1 = 0 ;
}

static void interrupt isr(void)
{
	static uint8_t	Timer_Count=0;
	
	if( INTCONbits.TMR0IF ) {		// Timer0 Overflow Interrupt
		
		++Timer_Count ;
		
		if( Timer_Count >= 0x18 )	//Timer0 base on 4096us, 100ms/4096us = 0x18
		{
			loop() ;
			Timer_Count = 0;
		}
				
		//for Breath LED
		if(!(Timer_Count&PWM_Speed))
			if(status==0){

				if(CCPR1L==0xFF) PWM_OFF_Count++;
				else CCPR1L++;

				if(PWM_OFF_Count>PWM_OFF_Delay)
				{
					status=1;
					PWM_OFF_Count=0;
				}
			}else if(status==1){
	
				if(CCPR1L==0)	PWM_OFF_Count++;
				else CCPR1L--;

				if(PWM_OFF_Count>PWM_OFF_Delay)
				{
					status=0;
					PWM_OFF_Count=0;
				}
			}
				
		INTCONbits.TMR0IF = 0;				// clear the interrupt flag
	}
}