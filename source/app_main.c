//头文件header file//
#include "app_main.h"

/**********
//这里写重要的事或者说明
//每次升级需要手动改写APP_MAIN.h模块的#define MCU_VER     "$$$VER0.x\r\n" 的版本号
//每次升级需要手动改写APP_MAIN.h模块的#define Key_MCU_VER "$$$Enter TestMode:0.x\r\n" 的版本号
//每次大的升级，比如0.xx升级到1.xx时修改wifi.c的“else if(char_compare(WIFI_CMD,"VER0.\r\n")==0)”中的VER0.
***********/

//宏定义macro definition//

//类型定义byte definition//
//ALRAM_TypeDef alarm;D:\ProgramFiles
PLAY_MODE PlayMode;
TIMER_MODE TimerMode;
PROGRAM_TypeDef program1; //在接收到“program Play（暂定）”时，播放最后一次接收到的“$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n”数据
PROGRAM_TypeDef program2;
PROGRAM_TypeDef program3;

//变量定义variable definition//
uint8_t idata cntAppTick;
uint8_t idata gRTC_Sec; // RTC数据
uint8_t idata gRTC_Sec_bk;
uint8_t idata gRTC_Hour;
uint8_t idata gRTC_Hour_bk;
uint8_t xdata gRTC_Hour_bk_24; //计数24小时
uint8_t idata gRTC_Minute;
uint8_t idata gRTC_Minute_bk;
uint8_t idata gRTC_Week; //周几
uint8_t idata cntTimer;
uint8_t idata sys_volume;
uint8_t idata sys_volume_bk; //在不断电情况下开关机保存音量值   //
uint8_t idata color_mode_bk; //在不断电情况下开关机保存颜色模式 //
uint8_t idata PlayMode_bk; //在不断电情况下开关机保存开机状态模式 //
uint8_t xdata timeSync_PowerON_Cnt; //上电10s请求同步时间
// uint8_t idata
// data_bk[64];//设置了看门狗，在遇到意外重启之后会从0x0000启动，之前的数据都会清除
//所以用这个数组在一些数据变动之后5s内	进行备份，如果数据不变动就不备份
// uint8_t
// data_bk_Cnt;//设置了看门狗，在遇到意外重启之后会从0x0000启动，之前的数据都会清除
//所以用这个数组在一些数据变动之后5s内	进行备份，如果数据不变动就不备份
//此变量是进行计数5s的
uint8_t cntFlag_ON_OFF = 0;

//标志位定义flags definetion//
bit AppTick0;
bit AppTick1;
bit AppTick2;
bit AppTick3;
bit AppTick4;
bit AppTick5;
bit AppTick1ms;
bit gbHalfSecond;
bit enableMute; //为1就整体静音，为0就解除静音
bit enableMute_bk;
bit setReset;
bit setFactory;
bit Flag_Demo_Mode;
bit Flag_TestMode;
uint8_t Falg_TimeSync_Allow; //时间允许标志为，如果为一就一直两分钟请求同步，
bit Flag_data_bk; //设置了看门狗，在遇到意外重启之后会从0x0000启动，之前的数据都会清除
                  //所以用这个数组在一些数据变动之后5s内
                  //进行备份，如果数据不变动就不备份 变动时把此标志位置一
bit Flag_alarm_say_to_IOT; //闹钟响应时应为要连续发送两条语句各3次，所以第二条要放在uart里面
bit Flag_time_sync; //时间请求同步时应为要发送语句3次，此时有可能和其他的要送发送的
                    //冲突，所以第二条要放在uart里面
bit Flag_alarm_wake_only;

uint8_t cntFlash;
uint8_t Led_Flash;
// extern bit       Flag_data_bk;

//函数定义function definetion//
/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：滴答时钟，循环一次10ms
*******************************************************************/
void sys_tick(void)
{
	AppTick1ms = 1;
	if (cntAppTick == 0)
		AppTick0 = 1;
	if (cntAppTick == 1)
		AppTick1 = 1;
	if (cntAppTick == 2)
		AppTick2 = 1;
	if (cntAppTick == 3)
		AppTick3 = 1;
	if (cntAppTick == 4)
		AppTick4 = 1;
	if (cntAppTick == 5)
		AppTick5 = 1;
	if (cntAppTick == 6)
	{
		if (--Uart0_Tx_TimeOut == 0)
			Uart0_EnableSend = 0;
		if (--Uart0_Rx_TimeOut == 0)
			Rx_Pointer = 0;
		if (--Uart1_Tx_TimeOut == 0)
			Uart1_EnableSend = 0;
		if (--Uart1_Rx_TimeOut == 0)
			Uart1_Rx_Pointer = 0;
	}
	if (++cntAppTick >= 10)
		cntAppTick = 0;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：时间比较函数，每秒、每分钟、每小时都可以做判断
*******************************************************************/
void Compare_1MinutePorc(void)
{
	//uint8_t	code *EEPAddress_data_bk;
	if (gRTC_Sec != gRTC_Sec_bk)
	{
		gRTC_Sec_bk = gRTC_Sec;
		//Uart0Transmit_SendString(MCU_VER);
//		if (alarm.Runing)//闹钟功能在001和003中屏蔽了
//		{
//			if (++cntSetVolume>10)
//			{//当闹钟响应时，初始音量为1，每10s加一级
//				cntSetVolume = 0;
//				if (sys_volume<alarm.volume)
//					sys_volume++;
//			}
//		}
		if (Falg_TimeSync_Allow == 1)
		{//如果Falg_TimeSync_Allow为1就开始计数10，10s到了就发送请求同步信息
		 //然后如果没有收到回复的时间信息就每隔两分钟发一次
			timeSync_PowerON_Cnt++;
			if (timeSync_PowerON_Cnt >= 10)
			{
				timeSync_PowerON_Cnt = 0;
				Uart0_SendString_3Step = 3;
				Uart0Transmit_SendString("$$$Time sync\r\n");//开机10s到了就发送请求同步时间指令
				Falg_TimeSync_Allow = 2;
			}
		}
		//刷EEPROM，每次数据变动都在5s之内刷新一次
		//		if(Flag_data_bk)
		//		{
		//			if(++data_bk_Cnt>5)
		//			{
		//				data_bk_Cnt=0;
		//				Flag_data_bk=0;
		//				EEPAddress_data_bk=0x0400+64;
		//				data_bk[0]  = 0xAA;
		//				data_bk[1]  = PlayMode;
		//				data_bk[2]  = spa_name;
		//				data_bk[3]  = spa_volume;
		//				data_bk[4]  = enable_SPApause;
		//				data_bk[5]  = enableMute;
		//				data_bk[6]  = color_mode;
		//				data_bk[7]  = userR_val;
		//				data_bk[8]  = userG_val;
		//				data_bk[9]  = userB_val;
		//				data_bk[10] = Light_Brightness;
		//				data_bk[11] = TimerMode;
		//				data_bk[12] = cntTimer;
		//				data_bk[13] = alarm.cntTimer;
		//				data_bk[14] = alarm.spa_name;
		//				data_bk[15] = alarm.volume;
		//				data_bk[16] = alarm.color_mode;
		//				data_bk[17] = alarm.userR_val;
		//				data_bk[18] = alarm.userG_val;
		//				data_bk[19] = alarm.userB_val;
		//				data_bk[20] = alarm.Brightness;
		//				data_bk[21] = alarm.Enable;
		//				data_bk[22] = alarm.Week;
		//				data_bk[23] = alarm.Hour;
		//				data_bk[24] = alarm.Minute;
		//				data_bk[25] = alarm.Duration;
		//				data_bk[26] = alarm.play_pause;
		//				data_bk[27] = program1.spa_name;
		//				data_bk[28] = program1.color_mode;
		//				data_bk[29] = program1.userR_val;
		//				data_bk[30] = program1.userG_val;
		//				data_bk[31] = program1.userB_val;
		//				data_bk[32] = program1.Light_Brightness;
		//				data_bk[33] = program1.sys_volume;
		//				data_bk[34] = program1.PlayMode;
		//				data_bk[35] = program1.enable_SPApause;
		//				eeprom_page_write_data(EEPAddress_data_bk,&data_bk);
		//			}
		//		}
		//		else
		//		{
		//			data_bk_Cnt=0;
		//		}


		if (gRTC_Minute != gRTC_Minute_bk)
		{
			gRTC_Minute_bk = gRTC_Minute;
			if ((cntTimer) && (--cntTimer == 0))
			{//先判断cntTimer是否大于0，然后再判断--cntTimer是否为0，这样如果cntTimer为1，那"--cntTime"为0
				GP389_OFF();
				//color_mode=COLOR_OFF;
				//TimerMode=cntTimer=TIMER_OFF;// 
//				alarm.Runing = 0;//闹钟功能在001和003中屏蔽了
				Uart0_SendString_3Step = 3;    // 
				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
			}
//			if ((alarm.cntTimer) && (--alarm.cntTimer == 0))//闹钟功能在001和003中屏蔽了
//			{//先判断alarm.cntTimer是否大于0，然后再判断--alarm.cntTimer是否为0，这样如果alarm.cntTimer为1，那"--alarm.cntTimer"为0
//				GP389_OFF();
//				//color_mode=COLOR_OFF;
//				//TimerMode=cntTimer=TIMER_OFF;// 
//				alarm.Runing = 0;
//				Uart0_SendString_3Step = 5;    // 
//				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
//			}
//			if ((alarm.Enable == 1) && (gRTC_Hour == alarm.Hour) && (gRTC_Minute == alarm.Minute))//闹钟功能在001和003中屏蔽了
//			{
//				if ((gRTC_Week & alarm.Week) || (Flag_alarm_wake_only))
//				{
//					if (!(alarm.Week & 0x80))//是否每周重复
//						alarm.Week &= ~gRTC_Week;
//					Flag_alarm_wake_only = 0;
//					sys_volume = 1;
//					enableMute = 0;
//					cntSetVolume = 0;
//					alarm.Runing = 1;
//					PlayMode = PLAY_MUSIC;
//					userR_val = alarm.userR_val;
//					userG_val = alarm.userG_val;
//					userB_val = alarm.userB_val;
//					spa_name = alarm.spa_name;
//					enable_SPApause = alarm.play_pause;
//					enableMute = ((enable_SPApause == 0) ? 0 : 1);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
//					spa_cmd = ((enable_SPApause == 0) ? SPA_PALY : SPA_PAUSE);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
//					color_mode = alarm.color_mode;
//					Light_Brightness = alarm.Brightness;
//					TimerMode = cntTimer = alarm.cntTimer = alarm.Duration;
//					ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
//					Uart0_SendString_3Step = 3;
//					Flag_alarm_say_to_IOT = 1;
//				}
//			}
			if (Falg_TimeSync_Allow == 2)
			{//同步时间为
				timeSync_PowerON_Cnt++;
				if (timeSync_PowerON_Cnt >= 2)
				{
					timeSync_PowerON_Cnt = 0;
					Flag_time_sync = 1;
				}
			}

			if (gRTC_Hour != gRTC_Hour_bk)
			{
				gRTC_Hour_bk = gRTC_Hour;
				gRTC_Hour_bk_24++;
				if (gRTC_Hour_bk_24 >= 24)
				{
					gRTC_Hour_bk_24 = 0;
					Falg_TimeSync_Allow = 2;
					Flag_time_sync = 1;
				}
			}
		}
	}
}

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
		LED_30MIN(Led_Flash%2);//Led_Flash为1(因为上面先加不会为0)时余数为1亮，为2时灭
		LED_60MIN(Led_Flash%2);
		LED_90MIN(Led_Flash%2);
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
		LED_30MIN(1);
		LED_60MIN(1);
		LED_90MIN(1);
	}
	else if(Flag_TestMode)//进入测试模式TIMER60LED灯闪烁2s提示
	{
		if(++cntFlash>=12)//500ms
		{
		 cntFlash=0;
		 if(++Led_Flash>16)//2s 快闪，4次/s
		  Flag_TestMode=0;
		}
		//LED_30MIN(Led_Flash%2);//Led_Flash为1(因为上面先加不会为0)时余数为1亮，为2时灭
		//LED_60MIN(Led_Flash%2);
		LED_90MIN(Led_Flash%2);
	}
	else if((PlayMode<=PLAY_ON)&&(color_mode==COLOR_OFF)) 
	{
		LED_30MIN(0);
		LED_60MIN(0);
		LED_90MIN(0);
		TimerMode=cntTimer=TIMER_OFF;
	}	
	else if(TimerMode==TIMER_30MIN)
	{
	  	LED_30MIN(1);
		LED_60MIN(0);
		LED_90MIN(0);
	}
	else if(TimerMode==TIMER_60MIN)
	{
		LED_30MIN(0);
		LED_60MIN(1);
		LED_90MIN(0);
	}	
	else if(TimerMode==TIMER_90MIN)
	{
		LED_30MIN(0);
		LED_60MIN(0);
		LED_90MIN(1);
	}
	else 
	{
		LED_30MIN(0);
		LED_60MIN(0);
		LED_90MIN(0);
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
//			if(spa_name==SPA_OFF) //spa_name为枚举变量类型，SPA_ZEN为枚举变量元素
//			{
//				spa_name=SPA_BROOK;
//			}
//		}
//	while (1)
//	{
//		
//		if (AppTick1ms) //3ms
//		{
//			AppTick1ms = 0;
//		}
//		if (AppTick0)
//		{
//			AppTick0 = 0;
//		}
//		if (AppTick1)
//		{
//			AppTick1 = 0;
//			PWM_DutyCycle_Change();
//		}
//		if (AppTick2)
//		{
//			AppTick2 = 0;
//			
//		}
//		if (AppTick3)
//		{
//			AppTick3 = 0;
//			
//		}
//		if (AppTick4)
//		{
//			AppTick4 = 0;
//			
//		}
//		WDT_clear();
//	}
	while(1)
	{ 
    	Uart0_Receive_Parse();		
		if(AppTick1ms) //3ms
		{
			AppTick1ms=0;
		}
		if(AppTick0)
		{
			AppTick0=0;
			KeyScan();
			KeyComMsg();
		}
		if(AppTick1)
		{
			AppTick1=0;
			PWM_DutyCycle_Change();
			BlueMode_Handle();
		}
		if(AppTick2)
		{
			AppTick2=0;
			DealWith_Voice();
		}
		if(AppTick3)
		{
			AppTick3=0;
			RGB_Handle();
		}
		if(AppTick4)
		{
			AppTick4=0;
			setLed_Handle();
			Uart0Transmit_SendString_3Step(); 
			Compare_1MinutePorc(); //10ms被调用一次
		}
		 WDT_clear();
	}
}




