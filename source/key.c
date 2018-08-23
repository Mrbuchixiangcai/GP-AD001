//头文件header file//
#include "app_main.h"

//宏定义macro definition//
#define   ADCKEY_NUM  9

//类型定义byte definition//
ATOMIZATION_MODE AtomizationMode1;
//变量定义variable definition//
uint8_t   gbKeyPress; //按键按下
uint8_t   gbKeyNone;  //没有按键按下
uint8_t   KeyValue;   //按键值
uint16_t  AdcValue;   //最初采集的AD值
uint8_t   KeyEvent;   //按键事件
uint8_t   cntKeyLoop; //计数按键循环，短按，长按，超长按的按下时间
uint8_t   cntKeylong;
uint8_t   KeyCurValueBK;//当前按键值备份
uint8_t   KeyCurValue; //按键电压当前值  current当前的
uint8_t   KeyLastValue;//按键电压上一次的值
code uint16_t ADKEY_TAB1[ADCKEY_NUM]= //按键正常电压值与允许误差值
{
	0x0000     ,//power power键正常电压值
	0x0519-0x50,//sound sound键正常电压值
	0x0651-0x50,//v+//已经失效
	0x08Ae-0x50,//v-
	0x0AE8-0x50,//timer
	0x0C24-0x50,//light
	0x0D9D-0x50,//BT
	0x0E44-0x50,//MIST
	0x0EE5,
};
code uint16_t ADKEY_TAB2[ADCKEY_NUM]= //按键正常电压值与允许误差值
{
	0x0000+0x50,//power
	0x0519+0x50,//sound
	0x0651+0x50,//v+//已经失效
	0x08AE+0x50,//v-
	0x0AE8+0x50,//timer
	0x0C24+0x50,//light
	0x0D9D+0x50,//BT
	0x0E44+0x50,//MIST
	0x0EE5,
};

//标志未定义flags definetion//
bit Flas_Anhydrous;

//函数定义function definetion//
/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：获得AD值，并判断是哪个按键，如果是单独的按键和AD值按键复合也可以检测
*******************************************************************/
uint8_t GetKeyValue(void) 
{ 
	uint8_t  i;
	uint8_t  KeyNum=0;
	AdcValue=ADC_read();
	if(AdcValue<0x0fa0) //采集到的任意按键电压值要小于4000mv(0x0fa0)
	{
		for(i=0;i<ADCKEY_NUM;i++)
		{
			if((AdcValue>=ADKEY_TAB1[i])&&(AdcValue<=ADKEY_TAB2[i])) //判断采集到的按键电压值为哪一个按键
			{
				KeyNum=i+1; //确定为那个按键，因为i初始为0，所以加1，
				break;
			}
		}
	}
  	ADC_start(5);
	if ((KeyNum == T_VOLDEC) && (!KeyVolAdd())) //同时按下“音量-”和“音量+”10s重置整个设备
		KeyNum = T_DEFAULT;
	else if ((KeyNum == T_POWER) && (!KeyVolAdd())) //同时按下“音量-”和“音量+”10s重置整个设备
		KeyNum = T_CLEAR;
	else if ((KeyNum == T_MUSIC) && (!KeyVolAdd())) //同时按下“音乐键”和“音量+”10s进入测试模式
		KeyNum = T_TESTMODE;
	else if (!KeyVolAdd())
		KeyNum = T_VOLINC;

	if (OLP())//如果无水就拉高了,为1，正常有水为低，为0
		Flas_Anhydrous = 1;
	else
		Flas_Anhydrous = 0;
	return KeyNum; //返回按键
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：去抖，长短按判断
*******************************************************************/
void KeyEventPorc(uint8_t KeyTmp) //被调用时接收的是GetKeyValue()的返回值  一般固定，不更改
{
	gbKeyPress=0;
	if(KeyCurValue!=KeyTmp) //这个if语句是去抖，会被执行两次，第三次进来时直接跳过
	{//KeyTmp为临时值，
		if(KeyCurValueBK!=KeyTmp)
			KeyCurValueBK=KeyTmp;
		else //第二次进来时执行这一条
			KeyCurValue=KeyTmp; //
		return;
	}
	if(KeyCurValue==KeyLastValue)
	{
		if((KeyLastValue==0) || (cntKeyLoop==0))
			return;
		if(--cntKeyLoop==0) //这个在第二次进KeyEventPorc()函数（第一次进“if(KeyCurValue==KeyLastValue)”之前）时被下面赋值为5
		{ //cntKeyLoop为5就是50ms时间
			KeyValue=KeyLastValue; //防抖之后把确定的按键赋值给KeyValue
			if(gbKeyNone==0)
				gbKeyPress=1;
			switch(KeyEvent)
			{
				case 0:
					KeyEvent=1;
					KeyValue |= K_D;
					cntKeyLoop=cKEY_HOLD;
					break;
				case 1:
					KeyEvent=2;
					KeyValue |= K_H;
					cntKeyLoop=cKEY_RATE_VOL;
					break;
				case 2:
				case 3:
					KeyEvent=3;
					KeyValue |= K_R;
					cntKeyLoop=cKEY_RATE_VOL;
					if(cntKeylong<250)
						cntKeylong++;
					break;
			}
		}
	}
	else
	{
		if(KeyLastValue)
		{
			KeyValue=KeyLastValue;
			if(gbKeyNone==0)
				gbKeyPress=1;
			switch(KeyEvent)
			{
				case 1:KeyValue |= K_U; break;
				case 2:KeyValue |= K_LU;break;
				case 3:KeyValue |= K_LU;break;
			}
			KeyEvent=0;
		}
		else
		{
			gbKeyNone=0;
			cntKeylong=0;
		}
		cntKeyLoop=cKEY_CLICK;
		KeyLastValue=KeyCurValue;
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：被主函数调用
*******************************************************************/
void KeyScan(void) 
{
	KeyEventPorc(GetKeyValue());
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：按键的处理，包括长、短按键，数据处理及功能实现，还有闹钟响时按下任意按键关闭闹钟并关机
*******************************************************************/
void KeyComMsg(void) 
{
	if(gbKeyPress)
	{ 
//		if(alarm.Runing)//闹钟功能在001和003中屏蔽了
//		{
//			alarm.Runing=0;
//			gbKeyNone=1;//修复闹钟模式时某些按键关闭闹钟并关机时关不了的问题，为1让弹起的按键无效
//			//color_mode=COLOR_OFF;
//			Uart0_SendString_3Step=5;
//			GP389_OFF();
//			//ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);	
//			TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 	
//			return;
//		}
		//Flag_data_bk=1;//数据改变时备份
	   	switch(KeyValue)
		{
			case KU(T_POWER):
			{ }
			case KLU(T_POWER): //value65 每个按键7个模式 
			{
				if(cntKeylong>=25)
					break;
				Uart0_SendString_3Step=5;
				if(PlayMode!=PLAY_OFF)
				{
					GP389_OFF(); //
				}
				else
					GP389_ON(); //
				//ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
				break;
			}
			case KR(T_POWER): //value97 每个按键7个模式，
			{
				if(PlayMode!=PLAY_OFF)
				{
					if(cntKeylong==25)//长按5s进入wifi配对
					{
						Uart0_SendString_3Step=5;
						Uart0Transmit_SendString("$$$ATWQ\r\n"); 
					}
					break;
				}
			}
			if(PlayMode!=PLAY_OFF)//在开机状态下才可以操作其他按键
			{
				case KU(T_MUSIC): //value66
				{
					if((PlayMode==PLAY_ON) || (PlayMode==PLAY_BT)) //开机进入
					{
						enableMute=0;
						enable_SPApause=0;
						PlayMode=PLAY_MUSIC;
						if(spa_name==SPA_OFF) //spa_name为枚举变量类型，SPA_ZEN为枚举变量元素
						{
							spa_name=SPA_BROOK;
						}
					}
					else if(PlayMode==PLAY_MUSIC) //第二次进入
					{
						enableMute=0;
						if(enable_SPApause)
						{
							enable_SPApause=0;
							spa_cmd=SPA_PALY;
							break;
						}
						enable_SPApause=0;
						if(++spa_name>SPA_ZEN)
						{
							spa_name= SPA_OFF;
	//						if(color_mode==COLOR_OFF)
	//							TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 	
	//					    PlayMode=PLAY_ON;
	//						spa_name=SPA_OFF;
	//						enable_SPApause=1;
						}
					}
					break;
				}
				case KH(T_MUSIC): //value98
				{
					if(PlayMode == PLAY_MUSIC)
					{
						enable_SPApause=~enable_SPApause;
						if(!enable_SPApause)
						{	
							enableMute=0;
							spa_cmd=SPA_PALY;	
						}
						else
						{	
							spa_cmd=SPA_PAUSE;
						}
					}
					break;
				}
				case KU(T_VOLINC): //value67 没有break所以继续执行下一条语句
				case KR(T_VOLINC): //value131
				{
					if(PlayMode>PLAY_ON) 
					{
						if(sys_volume<10)
							sys_volume++;
						enableMute=0;//
					}
					break;
				}
				case KU(T_VOLDEC): //value68
				case KR(T_VOLDEC): //value132
				{
					if(PlayMode>PLAY_ON) 
					{
						if(sys_volume >1) //
							sys_volume--;
						enableMute=0;//
					}
				  break;
				} 
				case KU(T_TIMER): //value37 弹起执行
				{
					if((PlayMode>PLAY_ON)||(color_mode!=COLOR_OFF)) 
					{
						if(TimerMode==TIMER_OFF) 
							TimerMode=cntTimer=TIMER_30MIN;
						else if(TimerMode==TIMER_30MIN)
							TimerMode=cntTimer=TIMER_60MIN; 
						else if(TimerMode==TIMER_60MIN)
							TimerMode=cntTimer=TIMER_90MIN;
						else 
							TimerMode=cntTimer=TIMER_OFF;
					}
					break;
				}
				case KU(T_LIGHT): //value70
				{
					if(color_mode==COLOR_USER) 
						color_mode=COLOR_WHITE; //实现循环
					else if(Light_Brightness==0)
						Light_Brightness=1;
					else if(++color_mode>COLOR_CHANGE)
						color_mode=COLOR_WHITE;
					if(color_mode==COLOR_CHANGE)
					{
						play_rgb_index=0;
						curR_val=0;
						curG_val=0;
						curB_val=0;
					}
					break;
				}
				case KH(T_LIGHT):
				{
					if(color_mode!=COLOR_OFF)
						if(++Light_Brightness>3) 
							Light_Brightness=0;
					break;
				}
				case KU(T_BT): 
				{
					if(PlayMode!=PLAY_BT)
						PlayMode=PLAY_BT;
					break;
				}
				case KH(T_BT):
				{
					if(PlayMode==PLAY_BT)
						bt_cmd=BT_PARIR; 
					break;
				}
				case KU (T_MIST):
				case KLU(T_MIST):
				{
					if (AtomizationMode1 == ATOMIZATION_OFF)
						AtomizationMode1 = ATOMIZATION_THREE;
					else if (AtomizationMode1 == ATOMIZATION_THREE)
						AtomizationMode1 = ATOMIZATION_TWO;
					else if (AtomizationMode1 == ATOMIZATION_TWO)
						AtomizationMode1 = ATOMIZATION_ONE;
					else if (AtomizationMode1 == ATOMIZATION_ONE)
						AtomizationMode1 = ATOMIZATION_OFF;
					break;
				}
				case KR(T_DEFAULT): //组合键，长按10s“音量加和音量减”两个键恢复出厂设置
				{
					if(cntKeylong==50)
					{					 
						cntFlash=0;
						Led_Flash=0;
						setFactory=1;//置一让三个timer灯常亮三秒
						PlayMode=PLAY_ON;
						
						Uart0_SendString_3Step=5;
						Uart0Transmit_SendString("$$$Factory setting\r\n");
						bt_send_cmd(BT_CLEAR_LIST);//清除蓝牙列表列表
					}
					break;
				}
				case KR(T_CLEAR):  //组合键，长按10s“power和音量加”两个键清除wifi列表
				{
					if(cntKeylong==50)
					{					 
						setReset=1;	//置一让三个timer灯闪烁三秒
						cntFlash=0;
						Led_Flash=0;
						Uart0_SendString_3Step=5;
						Uart0Transmit_SendString("$$$Wifi reset\r\n");
					}
					break;
				}
				case KR(T_TESTMODE):  //组合键，长按10s“SOUND”和“音量加”两个键进入测试模式
				{
					if(cntKeylong==25)
					{//长按10进入测试模式，然后通过wifi模块发送命令
						Flag_TestMode=1;
						cntFlash=0;
						Led_Flash=0;
						Uart0_SendString_3Step=3;
						Uart0Transmit_SendString(Key_MCU_VER);
					}
					break;
				}
				default:
					break;
			}
		}
	}
}





