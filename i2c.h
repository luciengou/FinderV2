//#include "stm8s.h"
#ifndef	_I2C_H
#define	_I2C_H

//I2C Function
//I2C Error Indicator
#define I2C_Error_LED_GPIO_PORT	GPIOD
#define	I2C_Error_LED_GPIO	GPIO_PIN_1
#define I2C_Error_LED_OFF	GPIO_WriteHigh(I2C_Error_LED_GPIO_PORT, I2C_Error_LED_GPIO)
#define I2C_Error_LED_ON	GPIO_WriteLow(I2C_Error_LED_GPIO_PORT, I2C_Error_LED_GPIO)
#define I2C_Time_Out		0xFFFF
#define	I2C_Repeat_Time		0xFF
#define	I2C_By_Function_Base


void I2C_Initial(void);
static void I2C_Clear_ADDRESSSENTMATCHED_Flag(void);
ErrorStatus I2C_ByteWrite(uint8_t I2C_Addrs, uint8_t WriteAddr, uint8_t I2C_Data);
ErrorStatus I2C_ByteRead(uint8_t I2C_Addrs,uint8_t ReadAddr,uint8_t Data_Buffer[],uint8_t Number_Bytes_to_Read);
//void I2C_ByteRead(uint8_t I2C_Addr,uint8_t ReadAddr,uint8_t Data_Buffer[],uint8_t Number_Bytes_to_Read);

#endif