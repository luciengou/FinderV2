#ifndef	__TIMER_H_

#define	__TIMER_H_
	/*
	Timer 0
	OSC=4Mhz Fosc/4=1us
	Tick PS2 PS1 PS0 PSA  Prescaler	Timer  
	0x00  0   0   0   1     1 : 1	1*256us = 256us
	0x01  0   0   0   0     1 : 2	2*256us = 512us
	0x02  0   0   1   0     1 : 4	4*256us = 1024us
	0x03  0   1   0   0     1 : 8	8*256us = 2048us
	0x04  0   1   1   0     1 : 16	16*256us = 4096us
	0x05  1   0   0   0     1 : 32	32*256us = 8192us
	0x06  1   0   1   0     1 : 64	64*256us = 16384us
	0x07  1   1   0   0     1 : 128	128*256us = 32768us
	0x08  1   1   1   0     1 : 256	256*256us = 65536us
	*/
#define	Timer0_256us		0x00
#define	Timer0_512us		0x01
#define	Timer0_1024us		0x02
#define	Timer0_2048us		0x03
#define	Timer0_4096us		0x04
#define	Timer0_8192us		0x05
#define	Timer0_16384us		0x06
#define	Timer0_32768us		0x07
#define	Timer0_65536us		0x08

#define Timer1_ON	(T1CONbits.TMR1ON = 1)
#define Timer1_OFF	(T1CONbits.TMR1ON = 0)
#define Timer2_ON	(T2CONbits.TMR2ON = 1)
#define Timer2_OFF	(T2CONbits.TMR2ON = 0)
	/*
	Timer 1
	OSC=4Mhz Fosc/4=1us
	Tick T1PS1 T1PS0  Prescaler	Timer
	 0     0     0      1 : 1	1us
	 1     0     1      1 : 2	2us
	 2     1     0      1 : 4	4us
	 3     1     1      1 : 8	8us
	*/

#define Delay_Base_1us	0x00
#define Delay_Base_2us	0x01
#define Delay_Base_4us	0x02
#define Delay_Base_8us	0x03

//#define Hardware_Delay

void delay_ms(uint16_t ms);
void delay_1ms(void);
void delay_25ms(void);
void delay_500ms(void);
void delay_1s(void);
void delay_1us(void);
void delay_us(uint16_t us);

void Timer_Initial(void);
#endif