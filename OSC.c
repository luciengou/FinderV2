#include "main.h"
#include "OSC.h"
#include "Timer.h"

void OSC_Initial( uint8_t Fre )
{
    // set up oscillator control register
    OSCCONbits.SPLLEN=0;    // PLL is disabled
    OSCCONbits.IRCF=Fre;   //set OSCCON IRCF bits to select OSC frequency=4Mhz
    OSCCONbits.SCS=0x02;    //set the SCS bits to select internal oscillator block

}