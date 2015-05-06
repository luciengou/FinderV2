#include	"LED.h"
#include	"ISPIAP.h"

uint8_t PSFR_Read( uint8_t OP_Mode )
{
	IFMT = OP_Mode ;
	SCMD = 0x46 ;
	SCMD = 0xB9 ;				//ISP Enable
		
	return IFD	;				//IFD : ISP/IAP Flash Data Register
}

void PSFR_Write( uint8_t OP_Mode , uint8_t PSFR_Data)
{
	IFD	= PSFR_Data ;
	IFMT = OP_Mode ;
	SCMD = 0x46 ;
	SCMD = 0xB9 ;				//ISP Enable

}