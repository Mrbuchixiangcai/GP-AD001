//ͷ�ļ�header file//
#include "app_main.h"

//�궨��macro definition//

//���Ͷ���byte definition//
ALRAM_TypeDef alarm;
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
	static uint8_t cntSetVolume;
	//uint8_t	code *EEPAddress_data_bk;
	if (gRTC_Sec != gRTC_Sec_bk)
	{
		gRTC_Sec_bk = gRTC_Sec;
		//Uart0Transmit_SendString(MCU_VER);
		if (alarm.Runing)
		{
			if (++cntSetVolume>10)
			{//��������Ӧʱ����ʼ����Ϊ1��ÿ10s��һ��
				cntSetVolume = 0;
				if (sys_volume<alarm.volume)
					sys_volume++;
			}
		}
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
				alarm.Runing = 0;
				Uart0_SendString_3Step = 3;    // 
				ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
			}
			if ((alarm.cntTimer) && (--alarm.cntTimer == 0))
			{//���ж�alarm.cntTimer�Ƿ����0��Ȼ�����ж�--alarm.cntTimer�Ƿ�Ϊ0���������alarm.cntTimerΪ1����"--alarm.cntTimer"Ϊ0
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
					if (!(alarm.Week & 0x80))//�Ƿ�ÿ���ظ�
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
					enableMute = ((enable_SPApause == 0) ? 0 : 1);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
					spa_cmd = ((enable_SPApause == 0) ? SPA_PALY : SPA_PAUSE);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
					color_mode = alarm.color_mode;
					Light_Brightness = alarm.Brightness;
					TimerMode = cntTimer = alarm.cntTimer = alarm.Duration;
					ApplicationGP389_ONOFF((PlayMode == PLAY_OFF) ? 1 : 0);
					Uart0_SendString_3Step = 3;
					Flag_alarm_say_to_IOT = 1;
				}
			}
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





