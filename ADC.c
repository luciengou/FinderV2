#include "main.h"
#include "ADC.h"
#include "FVR.h"
#include "Timer.h"

void ADC_Initial(uint8_t AD_Ch)
{
#ifdef MAIN_POWER_5V
		// FVR Initial
		if ( AD_Ch == FVR_CH ) FVR_Initial();
#endif

#ifdef MAIN_POWER_2S
		FVR_Initial();

		ANSELA = 0x10 ;			// Set RA4 Analog Input
								//1 = Analog input. Pin is assigned as analog input(1). Digital Input buffer disabled.
								//0 = Digital I/O. Pin is assigned to port or Digital special function
#endif
		// Analoge I/O Setting
#ifdef MAIN_POWER_5V
		ANSELA = 0x00 ;			//1 = Analog input. Pin is assigned as analog input(1). Digital Input buffer disabled.
								//0 = Digital I/O. Pin is assigned to port or Digital special function
#endif
		ADCON0 &= 0x03 ;	// Reset ADC Channel Selection
							// GO_nDONE=0, ADON=0, ADFM=0

		ADCON0 |= AD_Ch << 2 ;	// Select ADC Channel

		ADCON1 &= 0x8F ;		// Reset ADC Conversion Clock Selection	
		ADCON1 |= ADC_CLK_16 << 4 ;	// Set ADC Conversion Clock Selection (Fosc/64 slowest)

    	DACCON0bits.DACEN=0;    // turn DAC off

		PIR1bits.ADIF=0;					//Clear ADC interrupt flag
		ADCON0bits.ADON=1;	//ADC ON

#ifdef MAIN_POWER_2S

		FVRCON |= ADFVR_REF_4096 ;	// Set ADC VRef 4.096
		ADCON1 |= ADPREF_FVR ;		//VREF+ is connected to internal Fixed Voltage Reference (FVR) module

		uint8_t i = ADC_Stack_Deep ;

		ADCON0 |= AD_Ch << 2 ;	// Select ADC Channel

		while ( --i )
		{
			ADC_Convertion_ON ;	// Start ADC Convertion
								// ADC Result in ADCRESH ADCRESL
			while( ADCON0bits.GO_nDONE ) ;

			ADC_Stack_Input () ;
		}
#endif

#ifdef MAIN_POWER_5V

		uint8_t i = ADC_Stack_Deep ;

		while ( --i )
		{
			ADC_Convertion_ON ;	// Start ADC Convertion
								// ADC Result in ADCRESH ADCRESL
			while( ADCON0bits.GO_nDONE ) ;

			ADC_Stack_Input () ;
		}
#endif
}

static uint16_t ADC_Value_Total;

void ADC_Stack_Input(void)
{
	static uint8_t ADC_Stack_Counter;

	ADC_Value_Total-=ADC_ADCRES[ADC_Stack_Counter];
	
	ADC_ADCRES[ADC_Stack_Counter]=ADRESH<<2 | ADRESL>>6;
	
	ADC_Value_Total+=ADC_ADCRES[ADC_Stack_Counter];
	
	ADC_Stack_Counter++;
	
	if(ADC_Stack_Counter>=ADC_Stack_Deep) ADC_Stack_Counter=0;
	
	return;
}

uint16_t Get_ADC_Average_Value( void )
{
	return ( ADC_Value_Total / ADC_Stack_Deep );
}

