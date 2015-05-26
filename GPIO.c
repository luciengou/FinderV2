#include	"main.h"
#include	"GPIO.h"

void GPIO_Initial(void)
{
	//LATA=0;	//Clear All Output
	// PORT A Assignments
    TRISAbits.TRISA0 = 0;	// RA0 = ISP_DAT
    TRISAbits.TRISA1 = 0;	// RA1 = ISP_CLK
    TRISAbits.TRISA2 = 0;	// RA2 = Breath_LED (Output))
    TRISAbits.TRISA3 = 1;	// RA3 = ServoIn    (Input) MCLR
    TRISAbits.TRISA4 = 1;	// RA4 = EXT_VIN	(Input)
    TRISAbits.TRISA5 = 0;	// RA5 = Buzzer     (Output)

	ANSELA = 0x10 ;			// Set RA4 Analog Input

	APFCONbits.T1GSEL = 1 ;	// T1GSEL: Pin Selection bit
							// 0 = T1G function is on RA4
							// 1 = T1G function is on RA3

	LATA=0;					//Clear All Output
	
}
