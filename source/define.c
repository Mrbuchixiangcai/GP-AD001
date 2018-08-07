//头文件header file//
#include "app_main.h"

//宏定义macro definition//

//类型定义byte definition//
ALRAM_TypeDef alarm;
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
	static uint8_t cntSetVolume;
	//uint8_t	code *EEPAddress_data_bk;
	if (gRTC_Sec != gRTC_Sec_bk)
	{
		gRTC_Sec_bk = gRTC_Sec;
		//Uart0Transmit_SendString(MCU_VER);
		if (alarm.Runing)
		{
			if (++cntSetVolume>10)
			{//当闹钟响应时，初始音量为1，每10s加一级
				cntSetVolume = 0;
				if (sys_volume<alarm.volume)
					sys_volume++;
			}
		}
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
				alarm.Runing = 0;
				Uart0_SendString_3Step = 3;    // 
				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
			}
			if ((alarm.cntTimer) && (--alarm.cntTimer == 0))
			{//先判断alarm.cntTimer是否大于0，然后再判断--alarm.cntTimer是否为0，这样如果alarm.cntTimer为1，那"--alarm.cntTimer"为0
				GP389_OFF();
				//color_mode=COLOR_OFF;
				//TimerMode=cntTimer=TIMER_OFF;// 
				alarm.Runing = 0;
				Uart0_SendString_3Step = 5;    // 
				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
			}
			if ((alarm.Enable == 1) && (gRTC_Hour == alarm.Hour) && (gRTC_Minute == alarm.Minute))
			{
				if ((gRTC_Week & alarm.Week) || (Flag_alarm_wake_only))
				{
					if (!(alarm.Week & 0x80))//是否每周重复
						alarm.Week &= ~gRTC_Week;
					Flag_alarm_wake_only = 0;
					sys_volume = 1;
					enableMute = 0;
					cntSetVolume = 0;
					alarm.Runing = 1;
					PlayMode = PLAY_MUSIC;
					userR_val = alarm.userR_val;
					userG_val = alarm.userG_val;
					userB_val = alarm.userB_val;
					spa_name = alarm.spa_name;
					enable_SPApause = alarm.play_pause;
					enableMute = ((enable_SPApause == 0) ? 0 : 1);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
					spa_cmd = ((enable_SPApause == 0) ? SPA_PALY : SPA_PAUSE);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
					color_mode = alarm.color_mode;
					Light_Brightness = alarm.Brightness;
					TimerMode = cntTimer = alarm.cntTimer = alarm.Duration;
					ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
					Uart0_SendString_3Step = 3;
					Flag_alarm_say_to_IOT = 1;
				}
			}
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





