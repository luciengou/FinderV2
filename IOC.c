#include	"led.h"

void IOC_init(void)
{
	IOCAP3=0;		//Interrupt-on-Change enabled on the pin for a positive going edge
	IOCAN3=1;		//Interrupt-on-Change enabled on the pin for a negative going edge.
	IOCIE=0;		//Disable Interrupt On Change interrupt
}