#ifndef	__EEPROM_H__

#define __EEPROM_H__

void int_EEPROM_putc ( uint8_t address , uint8_t data ) ;
uint8_t int_EEPROM_getc ( uint8_t address ) ;

#endif