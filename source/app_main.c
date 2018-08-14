//头文件header file//
#include "app_main.h"

/**********
//这里写重要的事或者说明
//每次升级需要手动改写APP_MAIN.h模块的#define MCU_VER     "$$$VER0.x\r\n" 的版本号
//每次升级需要手动改写APP_MAIN.h模块的#define Key_MCU_VER "$$$Enter TestMode:0.x\r\n" 的版本号
//每次大的升级，比如0.xx升级到1.xx时修改wifi.c的“else if(char_compare(WIFI_CMD,"VER0.\r\n")==0)”中的VER0.
***********/

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：LED灯的闪烁，恢复出厂设置和清楚wifi列表的LED相关操作
*******************************************************************/
void setLed_Handle(void)//LED灯的相关处理
{
	if(setReset)//清除wifi列表闪烁三秒
	{
		if(++cntFlash>=12)//500ms
		{
		 cntFlash=0;
		 if(++Led_Flash>24)//3s 快闪，4次/s
		  setReset=0;
		}
		LED_15MIN(Led_Flash%2);//Led_Flash为1(因为上面先加不会为0)时余数为1亮，为2时灭
		LED_30MIN(Led_Flash%2);
		LED_60MIN(Led_Flash%2);
	}
	else if(setFactory)//恢复出厂设置常亮三秒
	{
//		uint8_t i;
//		uint8_t	code *EEPAddress_data_bk;
//		EEPAddress_data_bk=0x0400+64;
		if(++cntFlash>=50)//500ms
		{
			cntFlash=0;
			if(++Led_Flash>6)//3s
			{
			 setFactory=0;
//			for(i=0;i<64;i++)//
//			{//恢复出厂设置时把EEPROM(data_bk[i])的数据也清掉
//				eeprom_page_write_data(EEPAddress_data_bk,0);;
//			}
			 ((void (code *) (void)) 0x0000)();
			}

		}
		LED_15MIN(1);
		LED_30MIN(1);
		LED_60MIN(1);
	}
	else if(Flag_TestMode)//进入测试模式TIMER60LED灯闪烁2s提示
	{
		if(++cntFlash>=12)//500ms
		{
		 cntFlash=0;
		 if(++Led_Flash>16)//2s 快闪，4次/s
		  Flag_TestMode=0;
		}
		//LED_15MIN(Led_Flash%2);//Led_Flash为1(因为上面先加不会为0)时余数为1亮，为2时灭
		//LED_30MIN(Led_Flash%2);
		LED_60MIN(Led_Flash%2);
	}
	else if((PlayMode<=PLAY_ON)&&(color_mode==COLOR_OFF)) 
	{
		LED_15MIN(0);
		LED_30MIN(0);
		LED_60MIN(0);
		TimerMode=cntTimer=TIMER_OFF;
	}	
	else if(TimerMode==TIMER_15MIN)
	{
	  	LED_15MIN(1);
		LED_30MIN(0);
		LED_60MIN(0);
	}
	else if(TimerMode==TIMER_30MIN)
	{
		LED_15MIN(0);
		LED_30MIN(1);
		LED_60MIN(0);
	}	
	else if(TimerMode==TIMER_60MIN)
	{
		LED_15MIN(0);
		LED_30MIN(0);
		LED_60MIN(1);
	}
	else 
	{
		LED_15MIN(0);
		LED_30MIN(0);
		LED_60MIN(0);
	}	
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：初始化端口和变量的操作
*******************************************************************/
void PowerON_Reset(void)
{
	//uint8_t i;
	//uint8_t	code *EEPAddress_data_bk;
	sys_volume=2; //默认音量是第5级
	Light_Brightness=1;
	PlayMode=PLAY_ON; //初始化上电为开机
	enableMute=1;//整体静音与其他参数逻辑与，为1静音，为0解除静音 
	EN_MUTE();
	Uart0_SendString_3Step=3;
  	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	Falg_TimeSync_Allow=1;
//	EEPAddress_data_bk=0x0400+64;
//	for(i=0;i<64;i++)//
//	{//读数据出来赋给data_bk
//		data_bk[i]=eeprom_byte_read_data(EEPAddress_data_bk+i);
//	}
//	if(data_bk[0]==0xAA)
//	{//如果data_bk[0]为0，说明为第一次出厂第一次上电或者是恢复出厂设置
//	 //把读出来的数据赋给相应的数据，对应在写EEPROM时的顺序
//		//PlayMode = ((data_bk[1]<=3) && (data_bk[1]>=0))?data_bk[1]:0;
//		PlayMode = ((data_bk[1]<=3)?data_bk[1]:0;
//		spa_name = ((data_bk[2]<=0x18) && (data_bk[2]>=0x00))?data_bk[2]:0;
//		spa_volume = ((data_bk[3]<=10) && (data_bk[3]>=0))?data_bk[3]:0;
//		enable_SPApause = ((data_bk[4]==0) && (data_bk[4]==1))?data_bk[4]:0;
//			spa_cmd = (enable_SPApause==0)?SPA_PALY:SPA_PAUSE;
//		enableMute = ((data_bk[5]==0) && (data_bk[5]==1))?data_bk[5]:0;
//		color_mode = ((data_bk[6]<=10) && (data_bk[6]>=0))?data_bk[6]:0;
//		userR_val = ((data_bk[7]<=255) && (data_bk[7]>=0))?data_bk[7]:0;
//		userG_val = ((data_bk[8]<=255) && (data_bk[8]>=0))?data_bk[8]:0;
//		userB_val = ((data_bk[9]<=255) && (data_bk[9]>=0))?data_bk[9]:0;
//		Light_Brightness = ((data_bk[10]<=3) && (data_bk[10]>=0))?data_bk[10]:0;
//		TimerMode = ((data_bk[11]==0) || (data_bk[11]==15) || (data_bk[11]==30) || (data_bk[11]==60))?data_bk[11]:0;
//		cntTimer = ((data_bk[12]<=60) && (data_bk[12]>=0))?data_bk[12]:0;
//		alarm.cntTimer = ((data_bk[13]<=60) && (data_bk[13]>=0))?data_bk[13]:0;
//		alarm.spa_name = ((data_bk[14]<=0x18) && (data_bk[14]>=0x00))?data_bk[14]:0;
//		alarm.volume = ((data_bk[15]<=10) && (data_bk[15]>=0))?data_bk[15]:0;
//		alarm.color_mode = ((data_bk[16]<=10) && (data_bk[16]>=0))?data_bk[16]:0;
//		alarm.userR_val = ((data_bk[17]<=255) && (data_bk[17]>=0))?data_bk[17]:0;
//		alarm.userG_val = ((data_bk[18]<=255) && (data_bk[18]>=0))?data_bk[18]:0;
//		alarm.userB_val = ((data_bk[19]<=255) && (data_bk[19]>=0))?data_bk[19]:0;
//		alarm.Brightness = ((data_bk[20]<=3) && (data_bk[20]>=0))?data_bk[20]:0;
//		alarm.Enable = ((data_bk[21]==1) || (data_bk[21]==0))?data_bk[21]:0;
//		alarm.Week = data_bk[22];
//		alarm.Hour = data_bk[23];
//		alarm.Minute = data_bk[24];
//		alarm.Duration = data_bk[25];
//		alarm.play_pause = data_bk[26];
//		program1.spa_name = ((data_bk[27]<=0x18) && (data_bk[27]>=0x00))?data_bk[27]:0;
//		program1.color_mode = ((data_bk[28]<=10) && (data_bk[28]>=0))?data_bk[28]:0;
//		program1.userR_val = ((data_bk[29]<=255) && (data_bk[29]>=0))?data_bk[29]:0;
//		program1.userG_val = ((data_bk[30]<=255) && (data_bk[30]>=0))?data_bk[30]:0;
//		program1.userB_val = ((data_bk[31]<=255) && (data_bk[31]>=0))?data_bk[31]:0;
//		program1.Light_Brightness = ((data_bk[32]<=3) && (data_bk[32]>=0))?data_bk[32]:0;
//		program1.sys_volume = ((data_bk[33]<=10) && (data_bk[33]>=0))?data_bk[33]:0;
//		program1.PlayMode = ((data_bk[34]<=3) && (data_bk[34]>=0))?data_bk[34]:0;
//		program1.enable_SPApause = ((data_bk[35]==0) && (data_bk[35]==1))?data_bk[35]:0;
//	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：主函数，被Main()调用，每个AppTickx执行一次是10ms
*******************************************************************/
void app_main(void)
{
  	PowerON_Reset();
//		if((PlayMode==PLAY_ON) || (PlayMode==PLAY_BT)) //开机进入
//		{
//			enableMute=0;
//			enable_SPApause=0;
//			PlayMode=PLAY_MUSIC;
//			if(spa_name==SPA_NONE) //spa_name为枚举变量类型，SPA_WHITENOISE为枚举变量元素
//			{
//				spa_name=SPA_BRAHM_LULLABY;
//			}
//		}
	while (1)
	{
		
		if (AppTick1ms) //3ms
		{
			AppTick1ms = 0;
		}
		if (AppTick0)
		{
			AppTick0 = 0;
		}
		if (AppTick1)
		{
			AppTick1 = 0;
			PWM_DutyCycle_Change();
		}
		if (AppTick2)
		{
			AppTick2 = 0;
			
		}
		if (AppTick3)
		{
			AppTick3 = 0;
			
		}
		if (AppTick4)
		{
			AppTick4 = 0;
			
		}
		WDT_clear();
	}
//	while(1)
//	{ 
//    	Uart0_Receive_Parse();		
//		if(AppTick1ms) //3ms
//		{
//			AppTick1ms=0;
//		}
//		if(AppTick0)
//		{
//			AppTick0=0;
//			KeyScan();
//			KeyComMsg();
//		}
//		if(AppTick1)
//		{
//			AppTick1=0;
//			BlueMode_Handle();
//		}
//		if(AppTick2)
//		{
//			AppTick2=0;
//			DealWith_Voice();
//		}
//		if(AppTick3)
//		{
//			AppTick3=0;
//			RGB_Handle();
//		}
//		if(AppTick4)
//		{
//			AppTick4=0;
//			setLed_Handle();
//			Uart0Transmit_SendString_3Step(); 
//			Compare_1MinutePorc(); //10ms被调用一次
//		}
//		 WDT_clear();
//	}
}




