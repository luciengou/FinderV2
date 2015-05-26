#include	"main.h"
#include	"Loop.h"
#include	"Timer.h"
#include	"ADC.h"
//#include	"Serial595.h"
#include	"Buzzer.h"
#include	"TMR1GATE.h"
#include	"EEPROM.h"
#include	"PWM.h"

void loop(void)
{
	uint16_t VDD_Result ;
	//uint32_t VDD_Result ;
#ifdef MAIN_POWER_2S	
	uint16_t ADC_FVR ;
#endif
	uint8_t VR , VL ;	

	static uint16_t Servo_Width ;
	static uint16_t Servo_Check_Timer ;

	static uint8_t Timer_Min = 0 , Timer_Sec = 0 , Timer_100ms = 0;

	static uint8_t	Servo_Stable_Flag = 0 ;
	static uint16_t Pre_Servo_Width , STABLE_TIMER_SEC = 0;
	
	if ( ++ Timer_100ms >= 10 )
	{
		Timer_100ms = 0;			// 98304 * 10 us

		if ( Servo_Stable_Flag ) STABLE_TIMER_SEC ++ ;	// For Servo Stable Timer

		if ( ++ Timer_Sec >= 60 )	// 983040 * 61 = 59.965440 s
		{
				Timer_Sec = 0 ;
				++ Timer_Min ;

				if ( Timer_Min > 120 ) Timer_Min = 120 ;	// Timer Limit to 120 minutes
		}
	}
	
	switch ( Work_State_G )
	{
		case SERVO_CHECK:

			T1GCONbits.TMR1GE = 1 ;	// Timer1 Gate Enable bit

			TMR1H = 0 ;		// Clear Timer1 
			TMR1L = 0 ;

			Timer1_ON ;
			Timer1_Gate_GO ;

			Servo_Check_Timer = SERVO_CHECK_TIMEOUT ;
			while ( -- Servo_Check_Timer  && T1GCONbits.T1GGO_nDONE ) ;

			VR = TMR1H ;
			VL = TMR1L ;

			Timer1_OFF ;
			T1GCONbits.TMR1GE = 0 ;	// Timer1 Gate Disable
#ifdef MAIN_POWER_5V			
			// Servo Signal Stop Check
			if ( Servo_Check_Timer == 0 ){

				Buzzer_Go ( Servo_NO_SIGNAL_Bell ) ;	//Servo Signal Stop Warning

				PWM_OFF;
				Breath_LED ^= 1 ;

				Work_State_G = SERVO_CH                                                                                          ECK ;	//Next Work
				break;
			}
#endif
			Servo_Width = ( uint16_t ) ( VR <<8 | VL ) ;

			static uint8_t Finder_Mode_Change_Counter = 0 , Finder_Mode_Change_Timer = 0 ;

			if ( Timer_Min < 1 && Timer_Sec < 10 )
			{

				Breath_LED ^= 1 ;
				// Switch High Low High Low in one second , then Change Finder Mode
				if ( Servo_Width > Finder_Mid && Finder_Mode_Change_Counter == 0 )
				{
					Finder_Mode_Change_Counter ++ ;
					Finder_Mode_Change_Timer = Timer_Sec ;
				}
				else if (
						 	( Servo_Width < Finder_Low  && Finder_Mode_Change_Counter == 1 ) |
							( Servo_Width > Finder_Mid  && Finder_Mode_Change_Counter == 2 ) |
							( Servo_Width < Finder_Low	&& Finder_Mode_Change_Counter == 3 ) 

						) Finder_Mode_Change_Counter ++ ;
				else if ( Servo_Width < Finder_Low	&& Finder_Mode_Change_Counter == 4 )					
				{
					Finder_Mode_Change_Counter = 0 ;

					if ( Finder_Mode == HIGH_LOW )
					{
						uint8_t i=3;
						
						while (--i)
						{
							delay_ms(100);
							Beep(Buzzer_Freqeuncy,50);
							
						}
						Finder_Mode = MOVING_STABLE ;
						int_EEPROM_putc ( EEPROM_Address , 1);
					}
					else
					{
						Beep(Buzzer_Freqeuncy,500);
						
						uint8_t i=3;
						
						while (--i)
						{
							delay_ms(100);
							Beep(Buzzer_Freqeuncy,50);
						}

						Finder_Mode = HIGH_LOW ;
						int_EEPROM_putc ( EEPROM_Address , 0 );
					}
				}

				if ( ( Finder_Mode_Change_Timer + Finder_Mode_Change_Timer_Offset ) < Timer_Sec )
				{
					Finder_Mode_Change_Counter = 0 ;
				}
				
			}
			else
				PWM_ON;		// After 10 sec from power on , Breath LED Continue

			// classify = (input > 0) ? "positive" : "negative";
			Work_State_G = ( Finder_Mode == HIGH_LOW ) ? SERVO_HIGH_MID_LOW_CHECK : SERVO_MOVE_STABLE ;
	
			break;
			
		case SERVO_HIGH_MID_LOW_CHECK:

			if ( Servo_Width > Finder_High ) Buzzer_Go ( Servo_High_Bell ) ;
			else
			//if ( Servo_Width < Finder_Low ) Buzzer_Go ( Servo_Low_Bell ) ;
			//else
			if ( Servo_Width > (Finder_Mid - Mid_Offset) && 
			     Servo_Width < (Finder_Mid + Mid_Offset)
			   )  Buzzer_Go ( Servo_Mid_Bell ) ;
			
			Work_State_G = MAIN_POWER_CHECK ;	//Next Work

			break;
			
		case SERVO_MOVE_STABLE:

			if ( Servo_Width < ( Pre_Servo_Width + Stable_Offset ) &&
			     Servo_Width > ( Pre_Servo_Width - Stable_Offset )  )
			{
				Servo_Stable_Flag = 1;
			
				if ( STABLE_TIMER_SEC == Move_Stable_Timer_2min ){ 
					Buzzer_Go ( Servo_Stable_2MIN_Bell );		// 2min warning
				}
				
				if ( STABLE_TIMER_SEC >  Move_Stable_Timer_3min ){
					Buzzer_Go ( Servo_Stable_3MIN_Bell );		// 3min warning
				}				
			}
			else
			{
				Pre_Servo_Width = Servo_Width ;
				Servo_Stable_Flag = 0;
				STABLE_TIMER_SEC = 0 ;
			}

			Work_State_G = MAIN_POWER_CHECK ;	//Next Work

			break;

		case MAIN_POWER_CHECK:

#ifdef MAIN_POWER_2S	

			//----- Ext Vin Reading ----- 
			//ADCON0 |= Ext_Vin << 2 ;	// Select to Power_Pin Read 2S Lipo Voltage

			ADC_Convertion_ON ;		// Start ADC Convertion
									// ADC Result in ADCRESH ADCRESL
			while( ADCON0bits.GO_nDONE ) ;

			ADC_Stack_Input () ;

			//----- Ext Vin END ----
			
			//VDD_Result =( VDD_Result *1024 ) / ADC_FVR ;
			VDD_Result = Get_ADC_Average_Value() * 4 ;
			//VDD_Result=ADC_FVR;
			//VDD_Result = Get_ADC_Average_Value();
			
			if ( (VDD_Result > 3072 ) && (VDD_Result < 4096 ) ) Buzzer_Go( Warning_30min_Bell ) ;	//Three Time
			if ( (VDD_Result > 1024 ) && (VDD_Result < 3072 ) ) Buzzer_Go( Warning_20min_Bell ) ;	//Two Time
			if ( (VDD_Result >    0 ) && (VDD_Result < 1024 ) ) Buzzer_Go( Warning_10min_Bell ) ;	//One Time
			
			/*
			if ( (VDD_Result >600) && (VDD_Result < 1024) ) Buzzer_Go( Warning_10min_Bell ) ;	//One Time
			if ( VDD_Result >300) && (VDD_Result <  600) ) Buzzer_Go( Warning_20min_Bell ) ;	//One Time
			if ( (VDD_Result >0  ) && (VDD_Result <  300) ) Buzzer_Go( Warning_30min_Bell ) ;	//One Time
			*/
			/*
			if ( (VDD_Result >4) && (VDD_Result <  5) ) Buzzer_Go( Warning_10min_Bell ) ;	//One Time
			if ( (VDD_Result >2) && (VDD_Result <  4) ) Buzzer_Go( Warning_20min_Bell ) ;	//One Time
			if ( (VDD_Result >0  ) && (VDD_Result <  2) ) Buzzer_Go( Warning_30min_Bell ) ;	//One Time
			*/
			
/*
			switch ( VDD_Result )
			{
				case 700 ... 1023 :
					Buzzer_Go( Warning_10min_Bell ) ;	//One Time
					break ;
					
				case 400 ... 699 :
					Buzzer_Go( Warning_20min_Bell ) ;	//Two Times
					break ;
					
				case 1 ... 399 :
					Buzzer_Go( Warning_30min_Bell ) ;	//Three Times
					break ;
			}
*/
			// For Debug			
			//Serial595( VDD_Result );

			//if ( VDD_Result < Battery_Low_Voltage ) Buzzer_Go ( Warning_Low_Voltage_Bell );
#endif

#ifdef MAIN_POWER_5V			
			ADC_Convertion_ON ;		// Start ADC Convertion
									// ADC Result in ADCRESH ADCRESL
			while( ADCON0bits.GO_nDONE ) ;
			
			ADC_Stack_Input () ;
			
			VDD_Result=1048576/Get_ADC_Average_Value();

			// For Debug			
			//Serial595( VDD_Result );

			if ( VDD_Result < Battery_Low_Voltage ) Buzzer_Go ( Warning_Low_Voltage_Bell );
#endif
			Work_State_G = TOTAL_TIME_CHECK ;	//Next Work

			break;
		
		case TOTAL_TIME_CHECK:
			
			if ( Timer_Min == 10 && Timer_Sec == 0 ) Buzzer_Go ( Warning_10min_Bell );
			
			if ( Timer_Min == 20 && Timer_Sec == 0 ) Buzzer_Go ( Warning_20min_Bell );

			if ( Timer_Min == 30 && Timer_Sec == 0 ) Buzzer_Go ( Warning_30min_Bell );
			
			if ( Timer_Min >= 45 )
			{
				Buzzer_Go ( Warning_45min_Bell );
				Timer_Min = 46 ;	// Warning Forever after 46min
			}
			
			Work_State_G = SERVO_CHECK ;	//Next Work

			break;
	}
	
}