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
  	I2C->SR1; //�Ѧ�P251 ŪSR1 (�ҩ��i�H���n)
	I2C->SR3; //�M�ZŪSR3 �M��ADDR�]���_�w���I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)�^
}

ErrorStatus I2C_ByteWrite(uint8_t I2C_Addrs, uint8_t WriteAddr, uint8_t I2C_Data)
{
	I2C_GenerateSTART(ENABLE);

	I2C_Time_Out_Counter=I2C_Time_Out;
#ifdef	I2C_By_Function_Base
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))	//EV5
#else
	while(!(I2C->SR1 & 0x01))  //����START�o�e�� EV5
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
	while(!(I2C->SR1 & 0x02))  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
		if(!(I2C_Time_Out_Counter--))		//Timeout Detect
		{
			//UART1_Printf("Byte Write: EV6 Error!! \n");
			return ERROR;
		}

	//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	I2C_Clear_ADDRESSSENTMATCHED_Flag();

#else
	while(!(I2C->SR1 & 0x02))		//���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
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
								//EV8_2 TxE=1 �ABTF=1�A���Ͱ������ɥѵw��M���C
	while(!(I2C->SR1 & 0x84))	//�˴�SR1 TXE1 BTF��m�]�u����stm8����ack,TxE�~�|�m1�A���o�y�۷�_�P�_����ack�S��?�^
								//�b�o�e�a�}�M�M��ADDR ���Z�AI2C���f�i�J�D�]�Ʊ����Ҧ��C�H�U��stm8s������u��UP252�]��97�D�]�Ʊ����Ҧ������ǦC�ϡ^
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
								//EV8_2 TxE=1 �ABTF=1�A���Ͱ������ɥѵw��M���C
	while(!(I2C->SR1 & 0x84))	//�˴�SR1 TXE1 BTF��m�]�u����stm8����ack,TxE�~�|�m1�A���o�y�۷�_�P�_����ack�S��?�^
								//�b�o�e�a�}�M�M��ADDR ���Z�AI2C���f�i�J�D�]�Ʊ����Ҧ��C�H�U��stm8s������u��UP252�]��97�D�]�Ʊ����Ҧ������ǦC�ϡ^
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
				while(I2C->SR3 & 0x02)						//�����`�u�Ŷ� �˴�i2c-SR3 busy��
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
				while(!(I2C->SR1 & 0x01))  //����START�o�e�� EV5
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
				while(!(I2C->SR1 & 0x02))  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 2: EV6 Error!! \n");
						i=7;
						break;
				  	}

				//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
				I2C_Clear_ADDRESSSENTMATCHED_Flag();
#else
				while(!(I2C->SR1 & 0x02))  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
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
				//EV8_2 TxE=1 �ABTF=1�A���Ͱ������ɥѵw��M���C
  				while(!(I2C->SR1 & 0x84))	//�˴�SR1 TXE1 BTF��m�]�u����stm8����ack,TxE�~�|�m1�A���o�y�۷�_�P�_����ack�S��?�^
    										//�b�o�e�a�}�M�M��ADDR ���Z�AI2C���f�i�J�D�]�Ʊ����Ҧ��C�H�U��stm8s������u��UP252�]��97�D�]�Ʊ����Ҧ������ǦC�ϡ^
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
				while(!(I2C->SR1 & 0x01))  //����START�o�e�� EV5
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
				while(!(I2C->SR1 & 0x02))  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
					if(!(I2C_Time_Out_Counter--))		//Timeout Detect
				  	{
						//UART1_Printf("Byte Read case 5: EV6 Error!! \n");
						i=7;
						break;
				  	}

				//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
				I2C_Clear_ADDRESSSENTMATCHED_Flag();
#else
				while(!(I2C->SR1 & 0x02))	//���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
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
	while(!(I2C->SR1 & 0x02));  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
	I2C_Clear_ADDRESSSENTMATCHED_Flag();

	I2C_SendData((uint8_t) (ReadAddr));	//Send LSB Address

	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));		//EV8

	I2C_GenerateSTART(ENABLE);

	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));			//EV5

	I2C_Send7bitAddress(I2C_Addrs, I2C_DIRECTION_RX);

	//while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//EV6
	//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	while(!(I2C->SR1 & 0x02));  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1
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

  while(I2C->SR3 & 0x02);  //�����`�u�Ŷ� �˴�i2c-SR3 busy��

  //�H�U��stm8s������u��UP251�]��96�D�]�Ƶo�e�Ҧ��o�e�ǦC�ϡ^

  //S �_�l����
  I2C->CR2 |= 0x01;  //���Ͱ_�l��            CR2 start��

  //EV5�GSB=1�ASR1 �M�Z�N�a�}�g�JDR�H�s���N�M���ӼЧӡC
  while(!(I2C->SR1 & 0x01));  //����START�o�e�� E5

  //ADDRESS (�o�e�Ҧ�)
  I2C->DR = I2C_Addrs & ~0x01;  //����a�}(�̦Z�@��O0,��ܵo�e)


  while(!(I2C->SR1 & 0x02));  //���S7�쾹��a�}�o�e���}�B����ack,ADDR�m1

  //EV6:ADDR �b�n��Ū��SR1�Z�A��SR3�H�s��Ū�ާ@ �N�M�����
  I2C->SR1; //��P251 ŪSR1 (�����ҩ��i�H���n)
  I2C->SR3; //�M�ZŪSR3 �M  ADDR�]���_�w���I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)�^


  //DATA �o�e�H�s���a�}
  I2C->DR = ReadAddr;

  //EV8_2 TxE=1 �ABTF=1�A���Ͱ������ɥѵw��M���C
  while(!(I2C->SR1 & 0x84));  //�˴�SR1 TXE1 BTF��m�]�u����stm8����ack,TxE�~�|�m1�A���o�y�۷�_�P�_����ack�S��?�^

  //�b�o�e�a�}�M�M��ADDR ���Z�AI2C���f�i�J�D�]�Ʊ����Ҧ��C�H�U��stm8s������u��UP252�]��97�D�]�Ʊ����Ҧ������ǦC�ϡ^

  //S ���`�_�l����
  I2C->CR2 |= 0x01;  //���ͭ��`�_�l��

  //EV5�GSB=1�AŪSR1 �M�Z�N�a�}�g�JDR�H�s���N�M���ӼЧӡC
  while(!(I2C->SR1 & 0x01));  //����START?�e��

  //ADDRESS (����)
  I2C->DR = I2C_Addrs | 0x01;  //�̦Z�@��O1,��ܱ����A�o�e���Z�۰ʵo�eack(���e�OCR2 ack��ϯ�)


  //EV6:ADDR �b�n��Ū��SR1�Z�A��SR3�H�s��Ū�ާ@ �N�M�����
  while(!(I2C->SR1 & 0x02));  //���S7�쾹��a�}?�e���}�B����ack,ADDR�m1

  I2C->SR1; //��P251 ŪSR1 (�����ҩ��i�H���n)
  I2C->SR3; //�M�ZŪSR3 �M  ADDR�]���_�w���I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)�^


  //�`��Ū�����u
  while(Number_Bytes_to_Read)
  {
      //EV7_1 �GRxNE=1 �AŪDR�H�s���M���ӼЧӡC�]�mACK=0�MSTOP �ШD�C(�b�����̦Z�@�Ӧr�`�e)
	if(Number_Bytes_to_Read == 1) //�����ҩ��b�̦Z�@�Ӧr�`�e�Z���@��
	{
    	I2C->CR2 &= ~0x04; //ack�ϯ�
		I2C->CR2 |= 0x02;  //�����?��stop
    }

      //����EV7 RxNE=1�]����@�Ӧr�`�ZRxNE�m1�^ �A�P�_DR�H�s�������u
    if(I2C->SR1 & 0x40)
    {
              *Data_Buffer=I2C->DR;	//�b�����Ҧ��U�A���짹��r�`�Z,�۰ʵo�eack(���e�OCR2 ack��ϯ�A���ݭn�M��CR2 ack��m1)
              						//�b�C������r�`�Z�[I2C_AcknowledgeConfig(I2C_ACK_CURR)�L����N�q�A
              Data_Buffer++;
              Number_Bytes_to_Read--;
    }

  }

  I2C->CR2 |= 0x04;//���U�@�`���}�l �]�m ack�ϯ�A�W�� EV7_1�]�mack=0�o�estop�Z;�ݭn��ʳ]�mack=1�ϯ�A���n�b�������u���e

  //���O�I���O�I�ܦh�Ҥl���S���[�W�o�y�I�I ���L�L�S���[�`��! �p�G�L�b�`���@���N�|�X�{���D�CCR2 ack����N�O�ϯ઺�N��I�I�ܦh�H���z�Ѧ��ݭn��ʳ]�m�I
  //���O�n�����I2C �d�Ӧh�I�I �Q��M�աI�S���J�Ӭݩx�誺���ɡI

}
*/
