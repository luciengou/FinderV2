#ifndef	_MCP79411_H
#define	_MCP79411_H

#define I2C_Error_LED_GPIO_PORT	GPIOD
#define	I2C_Error_LED_GPIO	GPIO_PIN_1
#define I2C_Error_LED_OFF	GPIO_WriteHigh(I2C_Error_LED_GPIO_PORT, I2C_Error_LED_GPIO)
#define I2C_Error_LED_ON	GPIO_WriteLow(I2C_Error_LED_GPIO_PORT, I2C_Error_LED_GPIO)
#define	I2C_Repeat_Time		0xFF
#define MCP79411_I2C_Addr	0xDE
#define I2C_Addr		MCP79411_I2C_Addr

static uint8_t hex_bcd(unsigned char temp);
static uint8_t bcd_hex(unsigned char temp);
void MCP79411_Initial(void);
void MCP79411_Write_Time(uint8_t MCP79411_Sec_Value, uint8_t MCP79411_Min_Value);
#endif