#include	"main.h"
#include	"TMR1GATE.h"

void TMR1_Gate_Initial ( uint8_t TMR1_Gate_Source )
{
	T1GCON = 0 ;
	T1GCONbits.TMR1GE = 0 ;	// Timer1 Gate Enable bit
							// 1 = Timer1 counting is controlled by the Timer1 gate function
							// 0 = Timer1 counts regardless of Timer1 gate function
	T1GCONbits.T1GPOL = 1 ;	// Timer1 Gate Polarity bit
							// 1 = Timer1 gate is active-high (Timer1 counts when gate is high)
							// 0 = Timer1 gate is active-low (Timer1 counts when gate is low)
	//T1GCONbits.T1GTM  = 1 ;	// Timer1 Gate Toggle Mode bit
							// 1 = Timer1 Gate Toggle mode is enabled
							// 0 = Timer1 Gate Toggle mode is disabled and toggle flip flop is cleared
							// Timer1 gate flip-flop toggles on every rising edge.
	T1GCONbits.T1GSPM = 1 ;	// Timer1 Gate Single-Pulse Mode bit
							// 1 = Timer1 gate Single-Pulse mode is enabled and is controlling Timer1 gate
							// 0 = Timer1 gate Single-Pulse mode is disabled
	//T1GCONbits.T1GGO_nDONE = 1 ;	// Timer1 Gate Single-Pulse Acquisition Status bit
									// 1 = Timer1 gate single-pulse acquisition is ready, waiting for an edge
									// 0 = Timer1 gate single-pulse acquisition has completed or has not been started
	//T1GCONbits.T1GVAL = 1 ;	// Timer1 Gate Current State bit
							// Indicates the current state of the Timer1 gate that could be provided to TMR1H:TMR1L.
	//T1GCON &= 0xFC ;
	T1GCON |= TMR1_Gate_Source ;
	
	//T1GCONbits.T1GSS1 = 1 ;	// Timer1 Gate Source Select bits
	//T1GCONbits.T1GSS0 = 1 ;	//
	
}