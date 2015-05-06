#ifndef	_ISPIAP_H_

#define _ISPIAP_H_

#define	IFMT_Standby		0
#define IFMT_AP_Read		1
#define IFMT_AP_Prog		2
#define IFMT_AP_PGER		3
#define IFMT_IAPLB_Write	4
#define IFMT_IAPLB_Read		5
#define IFMT_AUXRA_Write	6
#define IFMT_AUXRA_Read		7
#define IFMT_AUXRB_Write	8
#define IFMT_AUXRB_Read		9

/*
IFMT_Mode
	0 0 0 0 0 Standby
	0 0 0 0 1 AP-memory read
	0 0 0 1 0 AP-memory program
	0 0 0 1 1 AP-memory page erase
	0 0 1 0 0 IAPLB write (protected SFR)
	0 0 1 0 1 IAPLB read (protected SFR)
	0 0 1 1 0 AUXRA write (protected SFR)
	0 0 1 1 1 AUXRA read (protected SFR)
	1 0 0 0 0 AUXRB write (protected SFR)
	1 0 0 0 1 AUXRB read (protected SFR)
*/

uint8_t PSFR_Read ( uint8_t OP_Mode ) ;
void PSFR_Write( uint8_t OP_Mode , uint8_t PSFR_Data ) ;

#endif