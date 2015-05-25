#ifndef	_ADC_H
#define	_ADC_H

#define AN0_CH	0x00
#define AN1_CH	0x01
#define AN2_CH	0x02
#define AN3_CH	0x03
#define AN4_CH	0x04
#define TEMP_CH	0x1D 
#define DAC_CH	0x1E
#define FVR_CH	0x1F

//ADCS<2:0>: A/D Conversion Clock Select bits
//000 = FOSC/2 Quickly Convertion
//001 = FOSC/8
//010 = FOSC/32
//011 = FRC (clock supplied from a dedicated RC oscillator)
//100 = FOSC/4 for 4MHZ Fosc in 1.0us Convering Times
//101 = FOSC/16
//110 = FOSC/64 Slowly Convertion
//111 = FRC (clock supplied from a dedicated RC oscillator)	

#define ADC_CLK_2	0x00
#define ADC_CLK_8	0x01
#define ADC_CLK_32	0x02
#define ADC_CLK_FRC	0x03
#define ADC_CLK_4	0x04
#define ADC_CLK_16	0x05
#define ADC_CLK_64	0x06
#define ADC_CLK_FRC2 0x07

#define ADC_Stack_Deep	2

#define ADC_Convertion_ON	ADCON0bits.GO_nDONE=1;	//Start ADC Convertion
#define ADC_Convertion_OFF	ADCON0bits.GO_nDONE=0;	//Stop ADC Convertion

static uint16_t ADC_ADCRES[ADC_Stack_Deep];

void ADC_Initial(uint8_t AD_Ch) ;
void ADC_Stack_Input(void);
uint16_t Get_ADC_Average_Value( void );



#endif