#ifndef	_Uart_H
#define	_Uart_H

#define Uart1_Next_Line		Uart1_Send_Return()
//#define Uart1_SendChar(x)	UART1_SendData8(x)

void Uart1_Initial(const uint16_t BAUD_RATE);
void Uart1_Printf(uint8_t *message);
void Uart1_SendChar(uint8_t c);
void Uart1_SendInt(uint32_t temp);
void Uart1_Send_Return(void);
uint8_t Uart1_RecieveChar(void);

#endif