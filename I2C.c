#include	"led.h"
#include	"I2C.h"

static uint16_t I2C_Time_Out_Counter;

void I2C_Initial(void)
{
	//I2C Initial
	I2C_DeInit();
	I2C_Cmd(ENABLE);
	I2C_Init(100000, I2C_Addr, I2C_DUTYCYCLE_2, I2C_ACK_CURR,
		I2C_ADDMODE_7BIT, 16);
}

static void I2C_Clear_ADDRESSSENTMATCHED_Flag(void)
{
  	I2C->SR1; //參考P251 讀SR1 (證明可以不要)
	I2C->SR3; //然后讀SR3 清除ADDR（等于庫函數I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)）
}

ErrorStatus I2C_ByteWrite(uint8_t I2C_Addrs, uint8_t WriteAddr, uint8_t I2C_Data)
{
	I2C_GenerateSTART(ENABLE);

	I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))	//EV5
#else
	while(!(I2C->SR1 & 0x01))  //等待START發送完 EV5
#endif
		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
		{
			//UART1_Printf("Byte Write: EV5 Error!! \n");
			return ERROR;
		}

	I2C_Send7bitAddress(I2C_Addrs, I2C_DIRECTION_TX);

	I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base

	//while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	//EV6
	while(!(I2C->SR1 & 0x02))  //等特7位器件地址?送完并且收到ack,ADDR置1
		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
		{
			//UART1_Printf("Byte Write: EV6 Error!! \n");
			return ERROR;
		}

	//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	I2C_Clear_ADDRESSSENTMATCHED_Flag();

#else
	while(!(I2C->SR1 & 0x02))		//等特7位器件地址?送完并且收到ack,ADDR置1
		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
		{
			//UART1_Printf("Byte Write: EV6 Error!! \n");
			return ERROR;
		}

	I2C_Clear_ADDRESSSENTMATCHED_Flag();
#endif
	I2C_SendData((uint8_t) (WriteAddr));	//Send LSB Address

	I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))	//EV8
#else
								//EV8_2 TxE=1 ，BTF=1，產生停止條件時由硬件清除。
	while(!(I2C->SR1 & 0x84))	//檢測SR1 TXE1 BTF位置（只有當stm8收到ack,TxE才會置1，其實這句相當于判斷收到ack沒有?）
								//在發送地址和清除ADDR 之后，I2C接口進入主設備接收模式。以下見stm8s中文數据手冊P252（圖97主設備接收模式接收序列圖）
#endif
		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
		{
			//UART1_Printf("Byte Write: EV8 Error!! \n");
			return ERROR;
		}

	I2C_SendData(I2C_Data);

	I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))	//EV8_2
#else
								//EV8_2 TxE=1 ，BTF=1，產生停止條件時由硬件清除。
	while(!(I2C->SR1 & 0x84))	//檢測SR1 TXE1 BTF位置（只有當stm8收到ack,TxE才會置1，其實這句相當于判斷收到ack沒有?）
								//在發送地址和清除ADDR 之后，I2C接口進入主設備接收模式。以下見stm8s中文數据手冊P252（圖97主設備接收模式接收序列圖）
#endif
		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
		{
			//UART1_Printf("Byte Write: EV8 Error!! \n");
			return ERROR;
		}

	I2C_GenerateSTOP(ENABLE);

	return	SUCCESS;
}

ErrorStatus I2C_ByteRead(uint8_t I2C_Addrs,uint8_t ReadAddr,uint8_t Data_Buffer[],uint8_t Number_Bytes_to_Read)
{
	uint8_t i=0;
	ErrorStatus	status=ERROR;

	while(i<=7)
	{
		switch(i)
		{
			case 0:
			  	i++;		//Pass
				I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
				while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))	//Line busy
#else
				while(I2C->SR3 & 0x02)						//等待總線空閒 檢測i2c-SR3 busy位
#endif
					if(!(I2C_Time_Out_Counter--))			//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 0: Line Busy!! \n");
						i=7;
						break;
					}

				break;

			case 1:
				i++;		//Pass
				I2C_GenerateSTART(ENABLE);

				I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
				while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))	//EV5
#else
				while(!(I2C->SR1 & 0x01))  //等待START發送完 EV5
#endif
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 1: EV5 Error!! \n");
						i=7;
						break;
					}

	  			break;

			case 2:
				i++;		//Pass
				I2C_Send7bitAddress(I2C_Addrs, I2C_DIRECTION_TX);

				I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
				//while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	//EV6
				while(!(I2C->SR1 & 0x02))  //等特7位器件地址?送完并且收到ack,ADDR置1
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 2: EV6 Error!! \n");
						i=7;
						break;
				  	}

				//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
				I2C_Clear_ADDRESSSENTMATCHED_Flag();
#else
				while(!(I2C->SR1 & 0x02))  //等特7位器件地址?送完并且收到ack,ADDR置1
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 2: EV6 Error!! \n");
						i=7;
						break;
					}

				I2C_Clear_ADDRESSSENTMATCHED_Flag();
#endif
	  			break;

			case 3:
				i++;
				I2C_SendData((uint8_t) (ReadAddr));	//Send Read Data Address

				I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
				while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))	//EV8
#else
				//EV8_2 TxE=1 ，BTF=1，產生停止條件時由硬件清除。
  				while(!(I2C->SR1 & 0x84))	//檢測SR1 TXE1 BTF位置（只有當stm8收到ack,TxE才會置1，其實這句相當于判斷收到ack沒有?）
    										//在發送地址和清除ADDR 之后，I2C接口進入主設備接收模式。以下見stm8s中文數据手冊P252（圖97主設備接收模式接收序列圖）
#endif
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 3: EV8 Error!! \n");
						i=7;
						break;
				  	}

	  			break;

			case 4:
				i++;
				I2C_GenerateSTART(ENABLE);

				I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
				while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))	//EV5
#else
				while(!(I2C->SR1 & 0x01))  //等待START發送完 EV5
#endif
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 4: EV5 Error!! \n");
						i=7;
						break;
				  	}

	  			break;

			case 5:
				i++;
				I2C_Send7bitAddress(I2C_Addrs, I2C_DIRECTION_RX);

				I2C_Time_Out_Counter=I2C_Time_Out;

#ifdef	I2C_By_Function_Base
				//while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	//EV6
				while(!(I2C->SR1 & 0x02))  //等特7位器件地址?送完并且收到ack,ADDR置1
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 5: EV6 Error!! \n");
						i=7;
						break;
				  	}

				//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
				I2C_Clear_ADDRESSSENTMATCHED_Flag();
#else
				while(!(I2C->SR1 & 0x02))	//等特7位器件地址?送完并且收到ack,ADDR置1
			  		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 5: EV6 Error!! \n");
						i=7;
						break;
				  	}

				I2C_Clear_ADDRESSSENTMATCHED_Flag();
#endif
	  			break;

			case 6:
				i=0xFF;	//I2C no error
				status=SUCCESS;

				I2C_Time_Out_Counter=I2C_Time_Out;

				while(Number_Bytes_to_Read>0)
				{
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
					{
						//UART1_Printf("Byte Read case 6: Data Reading Error!! \n");
						i=7;
						break;
					}

					if(Number_Bytes_to_Read==1)
					{
		  				I2C_AcknowledgeConfig(I2C_ACK_NONE);
						I2C_GenerateSTOP(ENABLE);
					}

					if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
					{
		 				*Data_Buffer=I2C_ReceiveData();
						Data_Buffer++;
						Number_Bytes_to_Read--;
					}
				}

				if(i==0xFF) I2C_AcknowledgeConfig(I2C_ACK_CURR);

				break;

			case 7:
				i=0xFD;	//Time Out Error
				status=ERROR;
				break;
		}
	}

	return status;
}

/*
void I2C_ByteRead(uint8_t I2C_Addrs,uint8_t ReadAddr,uint8_t Data_Buffer[],uint8_t Number_Bytes_to_Read)
{
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));				//Line busy

	I2C_GenerateSTART(ENABLE);

	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));			//EV5

	I2C_Send7bitAddress(I2C_Addrs, I2C_DIRECTION_TX);

	//while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//EV6
	//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	while(!(I2C->SR1 & 0x02));  //等特7位器件地址?送完并且收到ack,ADDR置1
	I2C_Clear_ADDRESSSENTMATCHED_Flag();

	I2C_SendData((uint8_t) (ReadAddr));	//Send LSB Address

	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));		//EV8

	I2C_GenerateSTART(ENABLE);

	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));			//EV5

	I2C_Send7bitAddress(I2C_Addrs, I2C_DIRECTION_RX);

	//while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//EV6
	//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	while(!(I2C->SR1 & 0x02));  //等特7位器件地址?送完并且收到ack,ADDR置1
	I2C_Clear_ADDRESSSENTMATCHED_Flag();

	while(Number_Bytes_to_Read>0)
	{
		if(Number_Bytes_to_Read==1)
		{
		  	I2C_AcknowledgeConfig(I2C_ACK_NONE);
			I2C_GenerateSTOP(ENABLE);
		}

		if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
		 	*Data_Buffer=I2C_ReceiveData();
			Data_Buffer++;
			Number_Bytes_to_Read--;
		}
	}
	I2C_AcknowledgeConfig(I2C_ACK_CURR);

}
*/
/*
void I2C_ByteRead(uint8_t I2C_Addrs,uint8_t ReadAddr,uint8_t Data_Buffer[],uint8_t Number_Bytes_to_Read)
{

  while(I2C->SR3 & 0x02);  //等待總線空閒 檢測i2c-SR3 busy位

  //以下見stm8s中文數据手冊P251（圖96主設備發送模式發送序列圖）

  //S 起始條件
  I2C->CR2 |= 0x01;  //產生起始位            CR2 start位

  //EV5：SB=1，SR1 然后將地址寫入DR寄存器將清除該標志。
  while(!(I2C->SR1 & 0x01));  //等待START發送完 E5

  //ADDRESS (發送模式)
  I2C->DR = I2C_Addrs & ~0x01;  //器件地址(最后一位是0,表示發送)


  while(!(I2C->SR1 & 0x02));  //等特7位器件地址發送完并且收到ack,ADDR置1

  //EV6:ADDR 在軟件讀取SR1后，對SR3寄存器讀操作 將清除改位
  I2C->SR1; //見P251 讀SR1 (實驗證明可以不要)
  I2C->SR3; //然后讀SR3 清  ADDR（等于庫函數I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)）


  //DATA 發送寄存器地址
  I2C->DR = ReadAddr;

  //EV8_2 TxE=1 ，BTF=1，產生停止條件時由硬件清除。
  while(!(I2C->SR1 & 0x84));  //檢測SR1 TXE1 BTF位置（只有當stm8收到ack,TxE才會置1，其實這句相當于判斷收到ack沒有?）

  //在發送地址和清除ADDR 之后，I2C接口進入主設備接收模式。以下見stm8s中文數据手冊P252（圖97主設備接收模式接收序列圖）

  //S 重复起始條件
  I2C->CR2 |= 0x01;  //產生重复起始位

  //EV5：SB=1，讀SR1 然后將地址寫入DR寄存器將清除該標志。
  while(!(I2C->SR1 & 0x01));  //等待START?送完

  //ADDRESS (接收)
  I2C->DR = I2C_Addrs | 0x01;  //最后一位是1,表示接收，發送完后自動發送ack(提前是CR2 ack位使能)


  //EV6:ADDR 在軟件讀取SR1后，對SR3寄存器讀操作 將清除改位
  while(!(I2C->SR1 & 0x02));  //等特7位器件地址?送完并且收到ack,ADDR置1

  I2C->SR1; //見P251 讀SR1 (實驗證明可以不要)
  I2C->SR3; //然后讀SR3 清  ADDR（等于庫函數I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)）


  //循環讀取數据
  while(Number_Bytes_to_Read)
  {
      //EV7_1 ：RxNE=1 ，讀DR寄存器清除該標志。設置ACK=0和STOP 請求。(在接收最后一個字節前)
	if(Number_Bytes_to_Read == 1) //實驗證明在最后一個字節前后都一樣
	{
    	I2C->CR2 &= ~0x04; //ack使能
		I2C->CR2 |= 0x02;  //停止位?生stop
    }

      //測試EV7 RxNE=1（收到一個字節后RxNE置1） ，判斷DR寄存器有數据
    if(I2C->SR1 & 0x40)
    {
              *Data_Buffer=I2C->DR;	//在接收模式下，收到完整字節后,自動發送ack(提前是CR2 ack位使能，不需要專門CR2 ack位置1)
              						//在每次收到字節后加I2C_AcknowledgeConfig(I2C_ACK_CURR)無任何意義，
              Data_Buffer++;
              Number_Bytes_to_Read--;
    }

  }

  I2C->CR2 |= 0x04;//為下一循環開始 設置 ack使能，上面 EV7_1設置ack=0發送stop后;需要手動設置ack=1使能，必要在接收數据之前

  //切記！切記！很多例子都沒有加上這句！！ 不過他沒有加循環! 如果他在循環一次就會出現問題。CR2 ack位其實就是使能的意思！！很多人都理解成需要手動設置！
  //都是軟件模擬I2C 搞太多！！ 想當然啦！沒有仔細看官方的文檔！

}
*/
