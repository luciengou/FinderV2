#ifndef __main_H_

#define	__main_H_

#ifdef __PICC__
#include	<htc.h> 
#else
#include	<xc.h> 
#endif
#include	<stdint.h>

#define _XTAL_FREQ 4000000

//#define MAIN_POWER_5V
//#define Disable_45min_Warning	//Disable 45min Continue Warning
//#define Enable_60min_Warning





#define Battery_Low_Voltage	4300	// Battery Low Detect at 4.3v

#define Move_Stable_Timer_2min	120
#define Move_Stable_Timer_3min	180

#define EEPROM_Address	0xFF

#define	Breath_High	0xFF
#define	Breath_Low	0x00

#define Finder_On_Times		3
#define Finder_High			1750	
#define Finder_Mid			1500
#define Finder_Low			1250	
#define Speed_Factor		2

#define SERVO_CHECK_TIMEOUT	2500
#define Finder_Mode_Change_Timer_Offset 3

#define Mid_Offset		20
#define Stable_Offset	10
#define STABLE_IN_2MIN	1200
#define STABLE_IN_3MIN  1800

#define PWM_Speed		8
#define PWM_OFF_Delay	0x90

/*!< Signed integer types  */
typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;

#define	ON		1
#define OFF		0
#define	YES		1
#define NO		0
#define	HIGH	1
#define LOW 	0

#define Buzzer		LATAbits.LATA5
#define Breath_LED	LATAbits.LATA2
#define ServoIn		PORTAbits.RA3
#define Delay_Test1	LATAbits.LATA4
#define Delay_Test2	LATAbits.LATA5

void main(void);
void setup(void);

#endif