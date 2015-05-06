#include	"led.h"
#include	"UART.h"

void Uart1_Initial(const uint16_t BAUD_RATE)
{
	PCON0 &= 0x7F;   // Set SMOD bit to 0 (don't double baud rates)

   	//  Receiver enabled.
   	//  8-bit data, 1 start bit, 1 stop bit, 
   	//  Variable baud rate (asynchronous)
   	//  Receive flag will only be set if a valid stop bit is received
   	//  Set TI (transmit buffer is empty)
	
   	SCON0 = 0x72;

   	TMOD |= 0x20;   // T1 in mode 2, 8-bit auto reload

   	TH1 = (256 - (uint8_t)((((uint32_t)OSC_FREQ / 100) * 3125) 
            / ((uint32_t) BAUD_RATE * OSC_PER_INST * 1000)));

   	TL1 = TH1;  
   	TCON_bit.TR1 = 1;  // Run the timer
   	SCON0_bit.TI0 = 1;   // Send first character (dummy)

}
						   
//
//  Send a Char to the debug port (UART1).
//
	
void Uart1_SendChar(uint8_t c)
{

	while(!SCON0_bit.TI0);		//Transmit data register empty
  					//0: Data is not transferred to the shift register
					//1: Data is transferred to the shift register
	SCON0_bit.TI0=0;
	SBUF0=c;


}

//
//  Send Numbers to the debug port (UART1).
//

void Uart1_SendInt(uint32_t temp)
{
	uint8_t x=0,temp_char[10];

	while(temp>0){
		temp_char[x]=temp % 10+0x30;
		temp /=10;
		x++;
	}

	while(x>0){
	  	x--;

		Uart1_SendChar(temp_char[x]);

	}

}

//
//  Send a message to the debug port (UART1).
//

void Uart1_Printf(uint8_t *message)
{
    uint8_t *ch = message;
    while (*ch)
    {
		Uart1_SendChar((uint8_t) *ch);

        ch++;                               //  Grab the next character.
    }
}

//
//  Send a Return to the debug port (UART1).
//

void Uart1_Send_Return(void)
{
	Uart1_SendChar('\r');
}

//
//  Send a Return to the debug port (UART1).
//

uint8_t Uart1_RecieveChar(void)
{
	while(!SCON0_bit.RI0);
	SCON0_bit.RI0=0;	
	return SBUF0;
}