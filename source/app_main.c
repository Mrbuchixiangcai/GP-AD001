//ͷ�ļ�header file//
#include "app_main.h"

/**********
//����д��Ҫ���»���˵��
//ÿ��������Ҫ�ֶ���дAPP_MAIN.hģ���#define MCU_VER     "$$$VER0.x\r\n" �İ汾��
//ÿ��������Ҫ�ֶ���дAPP_MAIN.hģ���#define Key_MCU_VER "$$$Enter TestMode:0.x\r\n" �İ汾��
//ÿ�δ������������0.xx������1.xxʱ�޸�wifi.c�ġ�else if(char_compare(WIFI_CMD,"VER0.\r\n")==0)���е�VER0.
***********/

//�궨��macro definition//

//���Ͷ���byte definition//
//ALRAM_TypeDef alarm;D:\ProgramFiles
PLAY_MODE PlayMode;
TIMER_MODE TimerMode;
PROGRAM_TypeDef program1; //�ڽ��յ���program Play���ݶ�����ʱ���������һ�ν��յ��ġ�$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n������
PROGRAM_TypeDef program2;
PROGRAM_TypeDef program3;

//��������variable definition//
uint8_t idata cntAppTick;
uint8_t idata gRTC_Sec; // RTC����
uint8_t idata gRTC_Sec_bk;
uint8_t idata gRTC_Hour;
uint8_t idata gRTC_Hour_bk;
uint8_t xdata gRTC_Hour_bk_24; //����24Сʱ
uint8_t idata gRTC_Minute;
uint8_t idata gRTC_Minute_bk;
uint8_t idata gRTC_Week; //�ܼ�
uint8_t idata cntTimer;
uint8_t idata sys_volume;
uint8_t idata sys_volume_bk; //�ڲ��ϵ�����¿��ػ���������ֵ   //
uint8_t idata color_mode_bk; //�ڲ��ϵ�����¿��ػ�������ɫģʽ //
uint8_t idata PlayMode_bk; //�ڲ��ϵ�����¿��ػ����濪��״̬ģʽ //
uint8_t xdata timeSync_PowerON_Cnt; //�ϵ�10s����ͬ��ʱ��
// uint8_t idata
// data_bk[64];//�����˿��Ź�����������������֮����0x0000������֮ǰ�����ݶ������
//���������������һЩ���ݱ䶯֮��5s��	���б��ݣ�������ݲ��䶯�Ͳ�����
// uint8_t
// data_bk_Cnt;//�����˿��Ź�����������������֮����0x0000������֮ǰ�����ݶ������
//���������������һЩ���ݱ䶯֮��5s��	���б��ݣ�������ݲ��䶯�Ͳ�����
//�˱����ǽ��м���5s��
uint8_t cntFlag_ON_OFF = 0;

//��־λ����flags definetion//
bit AppTick0;
bit AppTick1;
bit AppTick2;
bit AppTick3;
bit AppTick4;
bit AppTick5;
bit AppTick1ms;
bit gbHalfSecond;
bit enableMute; //Ϊ1�����徲����Ϊ0�ͽ������
bit enableMute_bk;
bit setReset;
bit setFactory;
bit Flag_Demo_Mode;
bit Flag_TestMode;
uint8_t Falg_TimeSync_Allow; //ʱ�������־Ϊ�����Ϊһ��һֱ����������ͬ����
bit Flag_data_bk; //�����˿��Ź�����������������֮����0x0000������֮ǰ�����ݶ������
                  //���������������һЩ���ݱ䶯֮��5s��
                  //���б��ݣ�������ݲ��䶯�Ͳ����� �䶯ʱ�Ѵ˱�־λ��һ
bit Flag_alarm_say_to_IOT; //������ӦʱӦΪҪ����������������3�Σ����Եڶ���Ҫ����uart����
bit Flag_time_sync; //ʱ������ͬ��ʱӦΪҪ�������3�Σ���ʱ�п��ܺ�������Ҫ�ͷ��͵�
                    //��ͻ�����Եڶ���Ҫ����uart����
bit Flag_alarm_wake_only;

uint8_t cntFlash;
uint8_t Led_Flash;
// extern bit       Flag_data_bk;

//��������function definetion//
/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ��δ�ʱ�ӣ�ѭ��һ��10ms
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
����ԭ�ͣ�
���������
���������
�������ܣ�ʱ��ȽϺ�����ÿ�롢ÿ���ӡ�ÿСʱ���������ж�
*******************************************************************/
void Compare_1MinutePorc(void)
{
	//uint8_t	code *EEPAddress_data_bk;
	if (gRTC_Sec != gRTC_Sec_bk)
	{
		gRTC_Sec_bk = gRTC_Sec;
		//Uart0Transmit_SendString(MCU_VER);
//		if (alarm.Runing)//���ӹ�����001��003��������
//		{
//			if (++cntSetVolume>10)
//			{//��������Ӧʱ����ʼ����Ϊ1��ÿ10s��һ��
//				cntSetVolume = 0;
//				if (sys_volume<alarm.volume)
//					sys_volume++;
//			}
//		}
		if (Falg_TimeSync_Allow == 1)
		{//���Falg_TimeSync_AllowΪ1�Ϳ�ʼ����10��10s���˾ͷ�������ͬ����Ϣ
		 //Ȼ�����û���յ��ظ���ʱ����Ϣ��ÿ�������ӷ�һ��
			timeSync_PowerON_Cnt++;
			if (timeSync_PowerON_Cnt >= 10)
			{
				timeSync_PowerON_Cnt = 0;
				Uart0_SendString_3Step = 3;
				Uart0Transmit_SendString("$$$Time sync\r\n");//����10s���˾ͷ�������ͬ��ʱ��ָ��
				Falg_TimeSync_Allow = 2;
			}
		}
		//ˢEEPROM��ÿ�����ݱ䶯����5s֮��ˢ��һ��
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
			{//���ж�cntTimer�Ƿ����0��Ȼ�����ж�--cntTimer�Ƿ�Ϊ0���������cntTimerΪ1����"--cntTime"Ϊ0
				GP389_OFF();
				//color_mode=COLOR_OFF;
				//TimerMode=cntTimer=TIMER_OFF;// 
//				alarm.Runing = 0;//���ӹ�����001��003��������
				Uart0_SendString_3Step = 3;    // 
				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
			}
//			if ((alarm.cntTimer) && (--alarm.cntTimer == 0))//���ӹ�����001��003��������
//			{//���ж�alarm.cntTimer�Ƿ����0��Ȼ�����ж�--alarm.cntTimer�Ƿ�Ϊ0���������alarm.cntTimerΪ1����"--alarm.cntTimer"Ϊ0
//				GP389_OFF();
//				//color_mode=COLOR_OFF;
//				//TimerMode=cntTimer=TIMER_OFF;// 
//				alarm.Runing = 0;
//				Uart0_SendString_3Step = 5;    // 
//				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
//			}
//			if ((alarm.Enable == 1) && (gRTC_Hour == alarm.Hour) && (gRTC_Minute == alarm.Minute))//���ӹ�����001��003��������
//			{
//				if ((gRTC_Week & alarm.Week) || (Flag_alarm_wake_only))
//				{
//					if (!(alarm.Week & 0x80))//�Ƿ�ÿ���ظ�
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
//					enableMute = ((enable_SPApause == 0) ? 0 : 1);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
//					spa_cmd = ((enable_SPApause == 0) ? SPA_PALY : SPA_PAUSE);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
//					color_mode = alarm.color_mode;
//					Light_Brightness = alarm.Brightness;
//					TimerMode = cntTimer = alarm.cntTimer = alarm.Duration;
//					ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
//					Uart0_SendString_3Step = 3;
//					Flag_alarm_say_to_IOT = 1;
//				}
//			}
			if (Falg_TimeSync_Allow == 2)
			{//ͬ��ʱ��Ϊ
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
����ԭ�ͣ�
���������
���������
�������ܣ�LED�Ƶ���˸���ָ��������ú����wifi�б��LED��ز���
*******************************************************************/
void setLed_Handle(void)//LED�Ƶ���ش���
{
	if(setReset)//���wifi�б���˸����
	{
		if(++cntFlash>=12)//500ms
		{
		 cntFlash=0;
		 if(++Led_Flash>24)//3s ������4��/s
		  setReset=0;
		}
		LED_30MIN(Led_Flash%2);//Led_FlashΪ1(��Ϊ�����ȼӲ���Ϊ0)ʱ����Ϊ1����Ϊ2ʱ��
		LED_60MIN(Led_Flash%2);
		LED_90MIN(Led_Flash%2);
	}
	else if(setFactory)//�ָ��������ó�������
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
//			{//�ָ���������ʱ��EEPROM(data_bk[i])������Ҳ���
//				eeprom_page_write_data(EEPAddress_data_bk,0);;
//			}
			 ((void (code *) (void)) 0x0000)();
			}

		}
		LED_30MIN(1);
		LED_60MIN(1);
		LED_90MIN(1);
	}
	else if(Flag_TestMode)//�������ģʽTIMER60LED����˸2s��ʾ
	{
		if(++cntFlash>=12)//500ms
		{
		 cntFlash=0;
		 if(++Led_Flash>16)//2s ������4��/s
		  Flag_TestMode=0;
		}
		//LED_30MIN(Led_Flash%2);//Led_FlashΪ1(��Ϊ�����ȼӲ���Ϊ0)ʱ����Ϊ1����Ϊ2ʱ��
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
����ԭ�ͣ�
���������
���������
�������ܣ���ʼ���˿ںͱ����Ĳ���
*******************************************************************/
void PowerON_Reset(void)
{
	//uint8_t i;
	//uint8_t	code *EEPAddress_data_bk;
	sys_volume=2; //Ĭ�������ǵ�5��
	Light_Brightness=1;  
	PlayMode=PLAY_ON; //��ʼ���ϵ�Ϊ����
	enableMute=1;//���徲�������������߼��룬Ϊ1������Ϊ0������� 
	EN_MUTE();
	Uart0_SendString_3Step=3;
  	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	Falg_TimeSync_Allow=1;
//	EEPAddress_data_bk=0x0400+64;
//	for(i=0;i<64;i++)//
//	{//�����ݳ�������data_bk
//		data_bk[i]=eeprom_byte_read_data(EEPAddress_data_bk+i);
//	}
//	if(data_bk[0]==0xAA)
//	{//���data_bk[0]Ϊ0��˵��Ϊ��һ�γ�����һ���ϵ�����ǻָ���������
//	 //�Ѷ����������ݸ�����Ӧ�����ݣ���Ӧ��дEEPROMʱ��˳��
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
����ԭ�ͣ�
���������
���������
�������ܣ�����������Main()���ã�ÿ��AppTickxִ��һ����10ms
*******************************************************************/
void app_main(void)
{
  	PowerON_Reset();
//		if((PlayMode==PLAY_ON) || (PlayMode==PLAY_BT)) //��������
//		{
//			enableMute=0;
//			enable_SPApause=0;
//			PlayMode=PLAY_MUSIC;
//			if(spa_name==SPA_OFF) //spa_nameΪö�ٱ������ͣ�SPA_ZENΪö�ٱ���Ԫ��
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
			Compare_1MinutePorc(); //10ms������һ��
		}
		 WDT_clear();
	}
}




