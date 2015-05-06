#ifndef	__OSC_H__

#define __OSC_H__

//IRCF<3:0>: Internal Oscillator Frequency Select bits
//000x = 31 kHz LF
//0010 = 31.25 kHz MF
//0011 = 31.25 kHz HF(1)
//0100 = 62.5 kHz MF
//0101 = 125 kHz MF
//0110 = 250 kHz MF
//0111 = 500 kHz MF (default upon Reset)
//1000 = 125 kHz HF(1)
//1001 = 250 kHz HF(1)
//1010 = 500 kHz HF(1)
//1011 = 1MHz HF
//1100 = 2MHz HF
//1101 = 4MHz HF
//1110 = 8 MHz or 32 MHz HF(see Section 5.2.2.1 ¡§HFINTOSC¡¨)
//1111 = 16 MHz HF

#define LF_31K00	0x00	
#define MF_31K25	0x02	
#define HF_31K25	0x03	
#define MF_62K50	0x04	
#define MF_125K 	0x05	
#define MF_250K 	0x06	
#define MF_500K 	0x07	
#define HF_125K 	0x08	
#define HF_250K 	0x09	
#define HF_500K 	0x0A	
#define HF_1M	  	0x0B	
#define HF_2M	  	0x0C	
#define HF_4M	  	0x0D	
#define HF_8M	  	0x0E	
#define HF_16M	  	0x0F	
#define HF_32M	  	0x0E	

void OSC_Initial( uint8_t Fre );

#endif