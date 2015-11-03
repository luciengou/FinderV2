#ifndef	__Buzzer_H_

#define	__Buzzer_H_

#define Buzzer_Freqeuncy 2700
#define Buzzer_Freq_Offset 18
#define beep_time_offset 40

#define Buzzer_In_Hardware

typedef enum {

	Initial_Bell ,
	Servo_High_Bell ,
	Servo_Mid_Bell ,
	//Servo_Low_Bell ,
	Servo_NO_SIGNAL_Bell ,
	Servo_Stable_2MIN_Bell ,
	Servo_Stable_3MIN_Bell ,
	Warning_10min_Bell ,
	Warning_20min_Bell ,
	Warning_30min_Bell ,
	Warning_45min_Bell ,
#ifdef Enable_60min_Warning
	Warning_40min_Bell ,
	Warning_50min_Bell ,
	Warning_60min_Bell ,
#endif
	Warning_Low_Voltage_Bell

} Buzzer_Type;

void Beep ( uint16_t Buzzer_Freq , uint16_t beep_time_ms ) ;
void Buzzer_Go ( Buzzer_Type B_Type ) ;
#endif