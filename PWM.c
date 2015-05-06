#include	"main.h"
#include	"PWM.h"

void PWM_Initial(void)			//PWM1 Output RA0 ,PWM2 Output RA1 Breath LED
{

	//PWM Pin Setup 

	APFCONbits.CCP1SEL=0;
							//APFCON Setting :ALTERNATE PIN FUNCTION CONTROL REGISTER
							//0:P1A=RA2 for PWM
							//1:   =RA5
	APFCONbits.P1BSEL=0;
							//0:P1B=RA0 for PWM
							//1:   =RA4 

	OPTION_REGbits.nWPUEN=0;	//Function_Select Pull High
								//WPUEN: Weak Pull-up Enable bit
								//1 = All weak pull-ups are disabled (except MCLR, if it is enabled)
								//0 = Weak pull-ups are enabled by individual WPUx latch values
	WPUAbits.WPUA3=0;			//RA3 MCLR Weak Pull-Up
	//WPUAbits.WPUA4=0;			//RA4 Function_Select Button Weak Pull-Up

    //******************************************************************************************
    // PWM Period = (1/Fosc) * 4 * (TMR2 Prescaler)* (PR2+1)
    //******************************************************************************************
    // Here are sample PWM periods for different TMR2 Prescalar values for Fosc=16Mhz and PR2=255
    //******************************************************************************************
    // TMR2 Prescalar=1: PWM Period = (1/16000000)*4*1*256 = 64 us or 15.63 khz
    // TMR2 Prescalar=4: PWM Period = (1/16000000)*4*4*256 = 256 us or 3.91 khz
    // TMR2 Prescalar=16: PWM Period = (1/16000000)*4*16*256= 1.024 ms or .976 khz
    // TMR2 Prescalar=64: PWM Period = (1/16000000)*4*64*256= 4.096 ms or .244 khz
    //
    // For this example we will choose the PWM period of 64us (15.63 kHz) so most people
    // will not be able to hear it.

    // ***** Setup PWM output ******************************************************

	//TRISAbits.TRISA5 = 1;	// disable pwm pin output for the moment

    CCP1CONbits.CCP1M=0x0C;	// select PWM mode for CCP module
							// PWM mode:
							// 1100(0x0C) = PWM mode: P1A, P1C active-high; P1B, P1D active-high
							// 1101(0x0D) = PWM mode: P1A, P1C active-high; P1B, P1D active-low
							// 1110(0x0E) = PWM mode: P1A, P1C active-low; P1B, P1D active-high
							// 1111(0x0F) = PWM mode: P1A, P1C active-low; P1B, P1D active-low

    CCP1CONbits.P1M=PWM_Single_Output_Mode;	// select single output on CCP1 pin (RA0)
							// PWM mode:
							// If CCP1M<3:2> = 00, 01, 10:
							// xx = P1A assigned as Capture/Compare input; P1B, P1C, P1D assigned as port pins(1)
							// If CCP1M<3:2> = 11:
							// 00 = Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
							// 01 = Full-Bridge output forward; P1D modulated; P1A active; P1B, P1C inactive(1)
							// 10 = Half-Bridge output; P1A, P1B modulated with dead-band control; P1C, P1D assigned as port pins
							// 11 = Full-Bridge output reverse; P1B modulated; P1C active; P1A, P1D inactive(1)
    
    PR2 = 0xFF;				// set PWM period as 255 per our example above

    CCPR1L =  0x00;         // clear high 8 bits of PWM duty cycle
    CCP1CONbits.DC1B=0x00;	// clear low  2 bits of PWM Duty cycle

                            // Note: PWM uses TMR2 so we need to configure it
    PIR1bits.TMR2IF=0;		// clear TMR2 interrupt flag

    T2CONbits.T2CKPS=0x03;  // select TMR2 prescalar as divide by 1 as per our example above
							// Timer2 Clock Prescale Select bits
							// 00 = Prescaler is 1
							// 01 = Prescaler is 4
							// 10 = Prescaler is 16
							// 11 = Prescaler is 64

    T2CONbits.TMR2ON=1;		// turn TMR2 on

}
