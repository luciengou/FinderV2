#include	"LED.h"
#include	"I2C.h"
#include	"MCP79411.h"

static uint8_t hex_bcd(unsigned char temp)
{
	return ((temp/10)*16)+(temp%10);
}

static uint8_t bcd_hex(unsigned char temp)
{
	return ((temp>>4)*10)+(temp&0x0f);
}

void MCP79411_Initial(void)
{
	uint8_t	Temp=0;
	uint8_t	Time_Out_Counter;

	GPIO_Init(I2C_Error_LED_GPIO_PORT, I2C_Error_LED_GPIO, GPIO_MODE_OUT_PP_LOW_SLOW);

	I2C_Error_LED_OFF;
	Time_Out_Counter=I2C_Repeat_Time;
	while(I2C_ByteRead(MCP79411_I2C_Addr, 0x00, &Temp, 1)==ERROR)
		if(!(Time_Out_Counter--))		//Timeout Detect
	  	{
			I2C_Error_LED_ON;
			break;
		}

	if(!(Temp & 0x80))
	{
		I2C_Error_LED_OFF;
		Time_Out_Counter=I2C_Repeat_Time;
		while(I2C_ByteWrite(MCP79411_I2C_Addr, 0x00, 0x80)==ERROR)	//RTC Initial
			if(!(Time_Out_Counter--))		//Timeout Detect
	  		{
				I2C_Error_LED_ON;
				break;
			}
	}

	I2C_Error_LED_OFF;
	Time_Out_Counter=I2C_Repeat_Time;
	while(I2C_ByteRead(MCP79411_I2C_Addr, 0x03, &Temp, 1)==ERROR)
		if(!(Time_Out_Counter--))		//Timeout Detect
	  	{
			I2C_Error_LED_ON;
			break;
		}

	if(!(Temp & 0x08))
	{
		I2C_Error_LED_OFF;
		Time_Out_Counter=I2C_Repeat_Time;
		while(I2C_ByteWrite(MCP79411_I2C_Addr, 0x03, 0x08)==ERROR)	//RTC Initial RTC 03h VBATEN
			if(!(Time_Out_Counter--))		//Timeout Detect
		  	{
				I2C_Error_LED_ON;
				break;
			}
	}

}

void MCP79411_Write_Time(uint8_t MCP79411_Sec_Value, uint8_t MCP79411_Min_Value)
{
	uint8_t temp;
	uint8_t Time_Out_Counter;

	temp=hex_bcd(MCP79411_Sec_Value);

	I2C_Error_LED_OFF;
	Time_Out_Counter=I2C_Repeat_Time;
	while(I2C_ByteWrite(MCP79411_I2C_Addr, 0x00, temp | 0x80)==ERROR)	//RTC Initial
		if(!(Time_Out_Counter--))		//Timeout Detect
		{
			I2C_Error_LED_ON;
			break;
		}

	temp=hex_bcd(MCP79411_Min_Value);

	I2C_Error_LED_OFF;
	Time_Out_Counter=I2C_Repeat_Time;
	while(I2C_ByteWrite(MCP79411_I2C_Addr, 0x01, temp)==ERROR)	//RTC Initial
		if(!(Time_Out_Counter--))		//Timeout Detect
  		{
			I2C_Error_LED_ON;
			break;
		}

}