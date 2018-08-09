//ͷ�ļ�header file//
#include "app_main.h"

//�궨��macro definition//

//���Ͷ���byte definition//

//��������variable definition//
uint8_t  idata  Rx_Pointer;
uint8_t  idata  Uart0_EnableSend;
uint8_t  idata  Uart0_Tx_Pointer;
uint16_t xdata  mUart0_Rx_Pointer;
uint16_t xdata  sUart0_Rx_Pointer;
uint8_t  idata  Uart0_Tx_TimeOut;
uint8_t  idata  Uart0_Rx_TimeOut;

uint8_t  xdata  WIFI_ReceiveBuffer[UART0_LEN_BUFFER];
uint8_t  xdata  Uart0_TransmitBuffer[UART0_LEN_BUFFER];
uint8_t  xdata  Uart0_ReceiveBuffer[UART0_RX_LEN_BUFFER];
uint8_t  idata  Count200ms_3Step;
uint8_t  idata  Uart0_SendString_3Step;

//��־δ����flags definetion//

//��������Function declaration//

//��������function definetion//
/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�
*******************************************************************/
void Uart0Transmit_SendString(char *str) 
{
	uint8_t i;
 do{WDT_clear();} while(Uart0_EnableSend);
	Count200ms_3Step=0;
	//Uart0_SendString_3Step=0;
	for(i=0;i<UART0_LEN_BUFFER;i++)		
	 	Uart0_TransmitBuffer[i]=0;
	for(i=0;*str!='\0';i++) 
	{ 
		Uart0_TransmitBuffer[i]=*str; 
		str++;
  	}
	Uart0_Tx_Pointer=0;
	Uart0_EnableSend=1;
	Uart0_Tx_TimeOut=10;	
	USI0DR=Uart0_TransmitBuffer[Uart0_Tx_Pointer++];
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�
*******************************************************************/
void Uart0Transmit_SendString_3Step(void) 
{
	if(Uart0_SendString_3Step)
	{
		if(Count200ms_3Step>=20)	
		{		
			do{WDT_clear();} while(Uart0_EnableSend);
			Uart0_SendString_3Step--;
			Count200ms_3Step=0;
			Uart0_Tx_Pointer=0;
			Uart0_EnableSend=1;
			Uart0_Tx_TimeOut=10;
			USI0DR=Uart0_TransmitBuffer[Uart0_Tx_Pointer++];
		}
		else 
			Count200ms_3Step++;
	}
	else
	{
//		if(Flag_alarm_say_to_IOT)D:\ProgramFiles
//		{
//			Flag_alarm_say_to_IOT=0;
//			Uart0_SendString_3Step=3;
//			Uart0Transmit_SendString("$$$Alarm up\r\n");//������Ӧ��֪ͨIoT���Ա�app�������ƻ�̨
//		}
//		else if(Flag_time_sync)
		if(Flag_time_sync)
		{
			Flag_time_sync=0;
			Uart0_SendString_3Step=3;
			Uart0Transmit_SendString("$$$Time sync\r\n");//����У׼ʱ�䣬һ��Сʱ����һ�Σ���һ��Ҫ������
		}
		else if(cntFlag_ON_OFF)
		{
			ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);//����У׼ʱ�䣬һ��Сʱ����һ�Σ���һ��Ҫ������
			cntFlag_ON_OFF--;
			Uart0_SendString_3Step=2;
		}		
	}
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�
*******************************************************************/
void Uart0_Receive_Parse(void)
{
	uint8_t i;
	while(mUart0_Rx_Pointer!=sUart0_Rx_Pointer)
	{
		WDT_clear();
		Uart0_Rx_TimeOut=10;	
		WIFI_ReceiveBuffer[Rx_Pointer]=Uart0_ReceiveBuffer[sUart0_Rx_Pointer];
		Uart0_ReceiveBuffer[sUart0_Rx_Pointer]=0x00;
		if(++sUart0_Rx_Pointer>=(UART0_RX_LEN_BUFFER-1))	
			sUart0_Rx_Pointer=0;
		if(WIFI_ReceiveBuffer[Rx_Pointer++]=='\n')
		{
			Wifi_CommandDeal(WIFI_ReceiveBuffer);
			for(i=0;i<UART0_LEN_BUFFER;i++)
				WIFI_ReceiveBuffer[i]=0x00;
			Rx_Pointer=0x00;
			//Flag_data_bk=1;
		}
	}	
}






