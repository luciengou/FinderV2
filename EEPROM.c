#include	"main.h"
#include	"EEPROM.h"
//This function Writes data to given address in internal EEPROM of PIC MCU 

void int_EEPROM_putc(uint8_t address, uint8_t data)
{
    EEADR  = address;		// Data Memory Address to write
    EEDATL = data;			// Data Memory Value to write

	EECON1bits.CFGS = 0; 	// Deselect Configuration space
    EECON1bits.EEPGD= 0; 	// Point to DATA memory
    EECON1bits.WREN = 1; 	// Enable writes

	INTCONbits.GIE=0;		// Disable INTs
  
    EECON2=0x55;        // Required sequence for write to internal EEPROM
    EECON2=0xaa;        // Required sequence for write to internal EEPROM

    EECON1bits.WR=1;    // begin write to internal EEPROM
	INTCONbits.GIE=1;		// Enable Interrupts
    EECON1bits.WREN = 0; 	// Disable writes

	asm("NOP");

    while (EECON1bits.WR==1)//Wait till write operation complete
    {
		asm("NOP");
    }

}

// This function reads data from address given in internal EEPROM of PIC 
uint8_t int_EEPROM_getc(uint8_t address)
{
    EEADRL=address;
	EECON1bits.CFGS = 0; // Deselect Config space
    EECON1bits.EEPGD= 0; // Point to DATA memory
    EECON1bits.RD   = 1; // EEPROM Read

    return EEDATL;       // return data
}