#include	"main.h"
#include	"FVR.h"

void FVR_Initial(void)
{

	FVRCON &= 0xFC ;			// Reset ADC FVR Ref Selection
	FVRCON |= ADFVR_REF_1024 | 0x80 ;	// Set FVR Ref 1.024
								
	//FVRCONbits.CDAFVR0=0;		//CDAFVR<1:0>: Comparator and DAC Fixed Voltage Reference Selection bits
	//FVRCONbits.CDAFVR1=0;		//00 = Comparator, DAC and CPS module Fixed Voltage Reference Peripheral output is off
								//01 = Comparator, DAC and CPS module Fixed Voltage Reference Peripheral output is 1x (1.024V)
								//10 = Comparator, DAC and CPS module Fixed Voltage Reference Peripheral output is 2x (2.048V)(2)
								//11 = Comparator, DAC and CPS module Fixed Voltage Reference Peripheral output is 4x (4.096V)(2)
								
	//FVRCONbits.TSRNG=0;			//TSRNG: Temperature Indicator Range Selection bit
								//0 = VOUT = VDD - 2VT (Low Range)
								//1 = VOUT = VDD - 4VT (HIgh Range)
	
	//FVRCONbits.TSEN=0;			//TSEN: Temperature Indicator Enable bit
								//0 = Temperature Indicator is disabled
								//1 = Temperature Indicator is enabled

	//FVRCONbits.FVRRDY=0;		//FVRRDY: Fixed Voltage Reference Ready Flag bit(1)
								//0 = Fixed Voltage Reference output is not ready or not enabled
								//1 = Fixed Voltage Reference output is ready for use
								
	//FVRCONbits.FVREN=1;			//FVR

	//if(FVRCONbits.FVREN)			//if FVR Enable then Wait FVR Ready
		//while(!FVRCONbits.FVRRDY);

}