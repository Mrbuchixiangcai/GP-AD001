//头文件header file//
#include "app_main.h"

//宏定义macro definition//

//类型定义byte definition//
BT_STEP  BT_Step; 

//变量定义variable definition//
uint8_t  idata bt_cmd;       
uint8_t  idata cntMuteBT;   
uint8_t  idata bt_volume;    
uint8_t  idata Uart1_Rx_Pointer;
uint8_t  idata Uart1_Tx_Pointer;
uint8_t  idata Uart1_EnableSend; 
uint8_t  idata Uart1_Tx_TimeOut;
uint8_t  idata Uart1_Rx_TimeOut; 
uint8_t  xdata Uart1_TransmitBuffer[UART1_LEN_BUFFER];
uint8_t  xdata Uart1_ReceiveBuffer_A[UART1_LEN_BUFFER]; 
uint8_t  xdata Uart1_ReceiveBuffer_B[UART1_LEN_BUFFER]; 

bit BT_Work; 
bit flag_bt_conn;  //蓝牙连接标志位
bit flag_bt_play;  //蓝牙播放
bit flag_bt_pause; //暂停
bit Uart1_RX_Finish_A;   
bit Uart1_RX_Finish_B;  
bit Uart1_ReceiveBuffer_A_B;

void Uart1Transmit_SendString(char *str) 
{
	uint8_t i;
  	while(Uart1_EnableSend);
	for(i=0;i<UART1_LEN_BUFFER;i++)		
	 	Uart1_TransmitBuffer[i]=0;
	for(i=0;*str!='\0';i++) 
	{ 
		Uart1_TransmitBuffer[i]=*str; 
		str++;
  	}
	Uart1_Tx_Pointer=0;
	Uart1_EnableSend=1;
	USI1DR=Uart1_TransmitBuffer[Uart1_Tx_Pointer++];
}
char  code BT_Command_tab[][8]= //用于改变歌曲时发送给wifi的第几首歌
{
	"     \r\n",//NONE
	"AT+MT\r\n",//BT_PAUSE
	"AT+MA\r\n",//BT_PALY
	"AT+ME\r\n",//BT_PREV
	"AT+MD\r\n",//BT_NEXT
	"AT+CA\r\n",//BT_PARIR
	"AT+CB\r\n",//BT_PARIR_EXT
	"AT+CC\r\n",//BT_LINK_BACK
	"AT+CP\r\n",//BT_POWER_DOWN
	"AT+CD\r\n",//BT_DISCONN
	"AT+CZ\r\n",//BT_CLEAR_LIST
	"AT+H0\r\n",
	"AT+H1\r\n",
	"AT+H2\r\n",
	"AT+H3\r\n",
	"AT+H4\r\n",
	"AT+H5\r\n",
	"AT+H6\r\n",
	"AT+H7\r\n",
	"AT+H8\r\n",
	"AT+H9\r\n",
	"AT+HA\r\n",
	"AT+HB\r\n",
	"AT+HC\r\n",
	"AT+HD\r\n",	
	"AT+HE\r\n",
	"AT+HF\r\n",	
};

void bt_send_cmd(uint8_t cmd) 
{	
  	uint8_t code bt_vol_send_tab[]={0,1,2,3,6,7,9,10,12,14,15};
	if(cmd<BT_VOL)
		Uart1Transmit_SendString(&BT_Command_tab[cmd][0]);
	else
		Uart1Transmit_SendString(&BT_Command_tab[BT_VOL+bt_vol_send_tab[sys_volume]][0]);
} 

void BlueMode_Receive(void) 
{
	uint8_t i;
	char *BT_CMD;
	if((Uart1_RX_Finish_A) || (Uart1_RX_Finish_B))
	{
		if(Uart1_RX_Finish_A)
		{
			Uart1_RX_Finish_A=0;
			BT_CMD=Uart1_ReceiveBuffer_A; 
		}
		else
		{
			Uart1_RX_Finish_B=0;
			BT_CMD=Uart1_ReceiveBuffer_B; 
		}
		if(BT_CMD[0]=='I') 
		{
			if(BT_CMD[1]=='I')
				flag_bt_conn = 1; 
			if(BT_CMD[1]=='A')
				flag_bt_conn = 0; 
			if(BT_CMD[1]=='X')
				flag_bt_conn = 0; 
		}
		else if(BT_CMD[0]=='M')
		{
			if(BT_CMD[1]=='A')
				flag_bt_play=0; 
			if(BT_CMD[1]=='B')
			{
				flag_bt_play=1; 
				bt_volume=~sys_volume;
			}
		}
		else if(BT_CMD[0]=='O') 
		{
			if((BT_CMD[1]=='F'))//if(BT_CMD[1]=='N')
			{
				if(PlayMode==PLAY_BT) 
				{}//GP389_OFF();
			}
		}
		else if(BT_CMD[0]=='H')	
		{
			if(PlayMode==PLAY_BT)
			{
				uint8_t code bt_vol_resp_tab[]={0,1,2,3,4,4,4,5,6,6, 7, 7, 8, 8, 9,10,10};
				if(BT_CMD[1]>'9')
					sys_volume= (BT_CMD[1]-'A')+10;
				else
					sys_volume= BT_CMD[1]-'0';
				if(sys_volume>15)
					sys_volume=15;
				sys_volume=bt_vol_resp_tab[sys_volume];
			}
		}			
		for(i=0;i<UART1_LEN_BUFFER;i++)		
			BT_CMD[i]=0;
	}
}
void BlueMode_Handle(void) //接收到的数据信息/状态进行处理
{
    BlueMode_Receive();  
	if(PlayMode == PLAY_BT)
	{
		if(BT_Work == 0) //初始化蓝牙
		{
			cntMuteBT=0;		
			flag_bt_conn=0;
			flag_bt_play=0;
			bt_volume=~sys_volume; //这个可能为bt_volume=~sys_volume
			BT_Step=BT_STEP_START;
		}
		BT_Work=1;	
		switch(BT_Step)
		{
			case BT_STEP_START:
				EN_MUTE(); //复位PC13端口
				SET_AUCH(); //(maybe)空操作
				BT_Step++;
				break;
			case BT_STEP_INITI1:
        UART1_def_init(); 
        CLR_BT_POWER(); //复位PA11 连接蓝牙芯片vbat端口
			  BT_Step++;
				break;
			case BT_STEP_INITI2:
			     BT_Step++;
				break;
			case BT_STEP_INITI3:
				SET_BT_POWER(); //置位PA11，对应蓝牙芯片的vbat脚
			  BT_Step++;
				break;
			case BT_STEP_INITI4:
				UART1_init(); 
				BT_Step++;
				break;
			default: 
				if(sys_volume!=bt_volume) //如果蓝牙音量信息和系统音量信息不相等，把系统的发给蓝牙
				{
					bt_volume=sys_volume;
					bt_send_cmd(BT_VOL); //串口发送音量信息到app端
				}
				else if(bt_cmd)
				{
					bt_send_cmd(bt_cmd); 
					bt_cmd=BT_NONE; //清零bt_cmd
				}
				if(BT_MUTE_DET()) //读P12端口电平，如果
				{
					if(cntMuteBT>5)
					{
						DE_MUTE(); //置位PC13 不静音
					}
					else
						++cntMuteBT;
				}
				else
				{
					EN_MUTE();	//复位PC13，静音可能	
				}					
				break;
		}
	}	
	else
	{
		if(BT_Work==1)
		{
			cntMuteBT=0;	
			BT_Step=BT_STEP_START;
		}
		BT_Work=0;
		switch(BT_Step)
		{
			case BT_STEP_START:
				EN_MUTE();
				BT_Step++;
				break;
			case BT_STEP_INITI1:
				BT_Step++;
				break;
			case BT_STEP_INITI2:
				BT_Step++;	
				break;
			case BT_STEP_INITI3:
				SET_BT_POWER();
				BT_Step++;
				break;
			case BT_STEP_INITI4:
				BT_Step++;
        CLR_AUCH();
				break;
			default: 
				break;
		}
	}
}





