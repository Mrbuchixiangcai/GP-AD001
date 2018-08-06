//头文件header file//
#include "app_main.h"

//宏定义macro definition//

//类型定义byte definition//
SPA_NAME      spa_name; //枚举变量类型
VOICE_STEP    Voice_Step; //语音步骤枚举变量类型


//变量定义variable definition//
uint8_t idata voice_send_step; //语音发送步骤
uint8_t idata voice_send_data; //语音发送数据
uint8_t idata voice_50us_cnt; //50us计时
uint8_t idata voice_50us_cnt_set; //50us计时设计
uint8_t idata spa_cmd; //spa命令
uint8_t idata spa_volume; //音量
uint8_t idata cur_spa_name;
uint8_t idata cntSPAOn;
code uint8_t voice_vol_tab[]=
{
	0x00,0x01,0x02,0x03,0x05,0x07,
	0x09,0x0A,0x0C,0x0E,0x0F,
};

//标志未定义flags definetion//
bit	  gbVoice_Work;
bit   flag_voice_send; //语音发送标志
bit   enable_SPApause;//为1为暂停


/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void set_voice_vol(uint8_t vol) //设置语音音量 
{
	voice_send(SPA_VOL15-voice_vol_tab[vol]);
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void voice_send(uint8_t key) //语音发送
{
	if(!flag_voice_send)
	{
		flag_voice_send = 1;
		voice_send_data = key-1;
		voice_send_step = 0;
		voice_50us_cnt =  0;
		VOIC_DATA(0); //端口点评置位/复位宏定义
		voice_50us_cnt_set =200;//change1
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void voice_in_timer(void)
{
	if(flag_voice_send)
	{
		if(++voice_50us_cnt >= voice_50us_cnt_set)
		{
			voice_50us_cnt = 0;
			switch(voice_send_step) //协议
			{
				case 0:		
          			VOIC_DATA(1); 
					voice_50us_cnt_set = 160;
					break;					
				case 1:
					VOIC_DATA(0); 
					voice_50us_cnt_set = 20;			
					break;
				case 2:
				case 4:
				case 6:
				case 8:
				case 10:
				case 12:
				case 14:
				case 16:
					VOIC_DATA(1);
					if(voice_send_data & 0x80)
						voice_50us_cnt_set = 30;
					else
						voice_50us_cnt_set = 10;
					break;
				case 2+1:
				case 4+1:
				case 6+1:
				case 8+1:
				case 10+1:
				case 12+1:
				case 14+1:
				case 16+1:
					VOIC_DATA(0); ; 
					if(voice_send_data & 0x80)
						voice_50us_cnt_set = 10;
					else
						voice_50us_cnt_set =30;
					voice_send_data=voice_send_data<<1;
					break;
				case 17+1:
						voice_50us_cnt_set = 200;
					break;
				default:
					VOIC_DATA(0);
					flag_voice_send = 0;
					break;
			}
			voice_send_step++;
		}	
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void DealWith_Voice(void) //被主函数调用
{
	if(PlayMode == PLAY_MUSIC)
	{
		if(gbVoice_Work==0)
		{ //类似于初始化，用于刷新数据
			cntSPAOn=0;
			cur_spa_name=~spa_name;
			spa_volume  =~sys_volume; 
			Voice_Step=VOICE_STEP_START;
		}
		gbVoice_Work=1;	
		switch(Voice_Step)
		{
			case VOICE_STEP_START:
				EN_MUTE();
				if(++cntSPAOn>25)
				{
				  CLR_AUCH();
					SET_BT_POWER(); //置位PA11，对应蓝牙芯片的vbat脚
					cntSPAOn=0;
					Voice_Step++;
				}
				break;
			case VOICE_STEP_INITI1:
        UART1_init(); 
				SET_VOICE_POWER();
				if(++cntSPAOn>50)
				{
					cntSPAOn=0;
					Voice_Step++;
				}
				break;
			case VOICE_STEP_INITI2:
				if(++cntSPAOn>200)
				{
					Voice_Step++;
					cntSPAOn=0;
				}
				break;
			case VOICE_STEP_INITI3:
				Voice_Step++;
				break;
			default: 
				if(spa_cmd)
				{ 
					if(!flag_voice_send) 
					{
						if(spa_cmd==SPA_PAUSE)
							Uart0Transmit_SendString(&SPASongs_Num_Table[0][0]);
						if(spa_cmd==SPA_PALY)
							Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);
						voice_send(spa_cmd);
						spa_cmd=0;
					}
				} 
				else if(spa_name!=cur_spa_name) 
				{
					if(!flag_voice_send) 
					{
						cur_spa_name=spa_name;
						if((spa_name==SPA_NONE)||(spa_name==SPA_PAUSE))
						{
							enable_SPApause=1;
							cur_spa_name=SPA_PAUSE;
							Uart0Transmit_SendString(&SPASongs_Num_Table[0][0]);
						}
					  else if((spa_name<=SPA_WHITENOISE))
						{
							Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);
						}
						voice_send(cur_spa_name);	
						cur_spa_name=spa_name;
					}
				}
				else if(sys_volume!=spa_volume)
				{
					if(!flag_voice_send)
					{
						spa_volume=sys_volume;
            			set_voice_vol(sys_volume);
					}
				}
				else if((enableMute!=enableMute_bk)&&(!Uart0_SendString_3Step))
				{
					enableMute_bk=enableMute;
					if(enableMute)
					{
					 	Uart0Transmit_SendString(&SPASongs_Num_Table[0][0]);
					}
					else
					{
					 	Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);
					}
				}
				else
				{
				 if((enable_SPApause) || (enableMute) || (sys_volume==0))
					EN_MUTE();
				 else 
					DE_MUTE();
			  }
				break;
		}
	}	
	else
	{
		if(gbVoice_Work!=0)
		{
			Voice_Step=VOICE_STEP_START; //为下一次发送做准备
		}
		gbVoice_Work=0;
		switch(Voice_Step)
		{
			case VOICE_STEP_START:
				if(!flag_bt_conn)
					EN_MUTE();
				Voice_Step++;
				break;
			case VOICE_STEP_INITI1:
				if(++cntSPAOn>20)
				{
					cntSPAOn=0;
				  SET_VOICE_POWER();
					Voice_Step++;
				}
				break;
			case VOICE_STEP_INITI2:
				if(!Uart0_SendString_3Step)
				{
					Uart0Transmit_SendString(&SPASongs_Num_Table[0][0]);
					Voice_Step++;
				}
				break;
			case VOICE_STEP_INITI3:
				if(!flag_bt_conn)
					Voice_Step++;
				break;
			default: 
				break;
		}
	}
}




