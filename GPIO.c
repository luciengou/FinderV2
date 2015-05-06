#include	"main.h"
#include	"GPIO.h"

void GPIO_Initial(void)
{
	//LATA=0;	//Clear All Output
	// PORT A Assignments
    TRISAbits.TRISA0 = 0;	// RA0 = Breath_LED (Output)
    TRISAbits.TRISA1 = 0;	// RA1
    TRISAbits.TRISA2 = 0;	// RA2 = Buzzer     (Output)
    TRISAbits.TRISA3 = 1;	// RA3 = ServoIn    (Input) MCLR
    TRISAbits.TRISA4 = 0;	// RA4 
    TRISAbits.TRISA5 = 0;	// RA5 

	APFCONbits.T1GSEL = 1 ;	// T1GSEL: Pin Selection bit
							// 0 = T1G function is on RA4
							// 1 = T1G function is on RA3

	LATA=0;					//Clear All Output
	
}
