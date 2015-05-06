#ifndef	__PWM_H_

#define __PWM_H_

// PWM mode:
// If CCP1M<3:2> = 00, 01, 10:
// xx = P1A assigned as Capture/Compare input; P1B, P1C, P1D assigned as port pins(1)

// If CCP1M<3:2> = 11:
// 00 = Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
// 01 = Full-Bridge output forward; P1D modulated; P1A active; P1B, P1C inactive(1)
// 10 = Half-Bridge output; P1A, P1B modulated with dead-band control; P1C, P1D assigned as port pins
// 11 = Full-Bridge output reverse; P1B modulated; P1C active; P1A, P1D inactive(1)
    
//PWM Output Mode
#define PWM_Single_Output_Mode				0x00
#define PWM_Full_Bridge_Output_Mode			0x01
#define PWM_Half_Bridge_Output_Mode			0x02
#define PWM_Full_Bridge_Reverse_Output_Mode	0x03

#define PWM_OFF		CCP1CONbits.CCP1M = 0x00
#define PWM_ON		CCP1CONbits.CCP1M = 0x0C
#define PWM_PortA	CCP1CONbits.P1M   = PWM_Single_Output_Mode
#define PWM_PortAB	CCP1CONbits.P1M   = PWM_Half_Bridge_Output_Mode
#define PWM_PortB	CCP1CONbits.P1M   = PWM_Full_Bridge_Reverse_Output_Mode

void PWM_Initial(void);

#endif