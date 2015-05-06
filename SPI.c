#include	"LED.h"
#include	"SPI.h"

void SPI_Initial(void)
{
	//SPCON:SPI Control Register
	SPCON_bit.CPHA = 0 ;
	//Bit 2: CPHA, SPI clock phase select
	//0: Data is driven when /SS pin is low (SSIG=0) and changes on the trailing edge of SPICLK. Data is sampled on the leading edge of SPICLK.
	//1: Data is driven on the leading edge of SPICLK, and is sampled on the trailing edge.
	//(Note: If SSIG=1, CPHA must not be 1, otherwise the operation is not defined.)		
	
	SPCON_bit.CPOL = 0 ;
	//Bit 3: CPOL, SPI clock polarity select
	//0: SPICLK is low when Idle. The leading edge of SPICLK is the rising edge and the trailing edge is the falling edge.
	//1: SPICLK is high when Idle. The leading edge of SPICLK is the falling edge and the trailing edge is the rising edge.

	SPCON_bit.DORD = 0 ;
	//Bit 5: DORD, SPI data order.
	//0: The MSB of the data byte is transmitted first.
	//1: The LSB of the data byte is transmitted first.		
		
	SPCON_bit.SSIG = 1 ;
	//Bit 7: SSIG, nSS is ignored.
	//0: The nSS pin decides whether the device is a master or slave.
	//1: MSTR decides whether the device is a master or slave.
	
	SPCON_bit.SPEN=1;
	//Bit 6: SPEN, SPI enable.
	//0: The SPI interface is disabled and all SPI pins will be general-purpose I/O ports.
	//1: The SPI is enabled.
	
	SPCON_bit.MSTR=1;
	//Bit 4: MSTR, Master/Slave mode select
	//0: Selects slave SPI mode.
	//1: Selects master SPI mode.
	
	SPCON_bit.SPR0=0;
	SPCON_bit.SPR1=0;
	//Bit 1~0: SPR1-SPR0 , SPI clock rate select (in master mode)
	//SPR1,SPR0
	//00: SYSCLK/4
	//01: SYSCLK/16
	//10: SYSCLK/64
	//11: SYSCLK/128
	
	SPI_nCS=0;
}

void SPI_Write_Byte(uint8_t Send_Data)
{
	SPDAT=Send_Data;
	
	while( !SPSTAT_bit.SPIF );	
		//Bit 7: SPIF, SPI transfer completion flag
		//0: The SPIF is cleared in software by writing ¡¥1¡¦ to this bit.
		//1: When a serial transfer finishes, the SPIF bit is set and an interrupt is generated 
		//if SPI interrupt is enabled. If nSS pin is driven low when SPI is in master mode with SSIG=0, 
		//SPIF will also be set to signal the ¡§mode change¡¨.
	SPSTAT_bit.SPIF=1;

}

uint8_t SPI_Read(void)
{
	SPDAT = 0xFF; //trigger SPI read
	
	while( !SPSTAT_bit.SPIF );	
	SPSTAT_bit.SPIF=1;
	
	//while(!SPSTAT & 0x80); //wait transfer finishes
	//SPSTAT &= ~0x80; //clear SPI interrupt flag
	
	return SPDAT;
}