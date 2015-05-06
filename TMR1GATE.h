#ifndef	__TMR1GATE_H_

#define __TMR1GATE_H_
//00 = Timer1 Gate pin
//01 = Timer0 overflow output
//10 = Comparator 1 optionally synchronized output (SYNCC1OUT)
//11 = Comparator 2 optionally synchronized output (SYNCC2OUT)

#define TMR1_Gate_Source_Timer1_Gate_Pin	0x00
#define TMR1_Gate_Source_Timer0_Overflow	0x01
#define TMR1_Gate_Source_Comparator1		0x02
#define TMR1_Gate_Source_Comparator2		0x03

#define Timer1_Gate_GO	( T1GCONbits.T1GGO_nDONE = 1 )

void TMR1_Gate_Initial ( uint8_t TMR1_Gate_Source ) ;

#endif