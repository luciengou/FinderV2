#ifndef __main_H_

#define	__main_H_

#ifdef __PICC__
#include	<htc.h> 
#else
#include	<xc.h> 
#endif
#include	<stdint.h>

#define _XTAL_FREQ 4000000

// Oscillator / resonator frequency (in Hz) e.g. (11059200UL)
//#define OSC_FREQ (4000000UL)

// Number of oscillations per instruction (12, etc)
// 12 - Original 8051 / 8052 and numerous modern versions
//  6 - Various Infineon and Philips devices, etc.
//  4 - Dallas 320, 520 etc.
//  1 - Dallas 420, etc.
//#define OSC_PER_INST (4)

//#define MAIN_POWER_5V
#define MAIN_POWER_2S

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
#define Ext_Vin 	LATAbits.LATA4	;Ext_Vin

//#define Power_Pin	0
//#define Power_Pin	AN0_CH

void main(void);
void setup(void);

#endif