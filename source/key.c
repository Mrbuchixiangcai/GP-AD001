//ͷ�ļ�header file//
#include "app_main.h"

//�궨��macro definition//
#define   ADCKEY_NUM  9

//���Ͷ���byte definition//
ATOMIZATION_MODE AtomizationMode1;
//��������variable definition//
uint8_t   gbKeyPress; //��������
uint8_t   gbKeyNone;  //û�а�������
uint8_t   KeyValue;   //����ֵ
uint16_t  AdcValue;   //����ɼ���ADֵ
uint8_t   KeyEvent;   //�����¼�
uint8_t   cntKeyLoop; //��������ѭ�����̰����������������İ���ʱ��
uint8_t   cntKeylong;
uint8_t   KeyCurValueBK;//��ǰ����ֵ����
uint8_t   KeyCurValue; //������ѹ��ǰֵ  current��ǰ��
uint8_t   KeyLastValue;//������ѹ��һ�ε�ֵ
code uint16_t ADKEY_TAB1[ADCKEY_NUM]= //����������ѹֵ���������ֵ
{
	0x0000     ,//power power��������ѹֵ
	0x0519-0x50,//sound sound��������ѹֵ
	0x0651-0x50,//v+//�Ѿ�ʧЧ
	0x08Ae-0x50,//v-
	0x0AE8-0x50,//timer
	0x0C24-0x50,//light
	0x0D9D-0x50,//BT
	0x0E44-0x50,//MIST
	0x0EE5,
};
code uint16_t ADKEY_TAB2[ADCKEY_NUM]= //����������ѹֵ���������ֵ
{
	0x0000+0x50,//power
	0x0519+0x50,//sound
	0x0651+0x50,//v+//�Ѿ�ʧЧ
	0x08AE+0x50,//v-
	0x0AE8+0x50,//timer
	0x0C24+0x50,//light
	0x0D9D+0x50,//BT
	0x0E44+0x50,//MIST
	0x0EE5,
};

//��־δ����flags definetion//
bit Flas_Anhydrous;

//��������function definetion//
/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ����ADֵ�����ж����ĸ�����������ǵ����İ�����ADֵ��������Ҳ���Լ��
*******************************************************************/
uint8_t GetKeyValue(void) 
{ 
	uint8_t  i;
	uint8_t  KeyNum=0;
	AdcValue=ADC_read();
	if(AdcValue<0x0fa0) //�ɼ��������ⰴ����ѹֵҪС��4000mv(0x0fa0)
	{
		for(i=0;i<ADCKEY_NUM;i++)
		{
			if((AdcValue>=ADKEY_TAB1[i])&&(AdcValue<=ADKEY_TAB2[i])) //�жϲɼ����İ�����ѹֵΪ��һ������
			{
				KeyNum=i+1; //ȷ��Ϊ�Ǹ���������Ϊi��ʼΪ0�����Լ�1��
				break;
			}
		}
	}
  	ADC_start(5);
	if ((KeyNum == T_VOLDEC) && (!KeyVolAdd())) //ͬʱ���¡�����-���͡�����+��10s���������豸
		KeyNum = T_DEFAULT;
	else if ((KeyNum == T_POWER) && (!KeyVolAdd())) //ͬʱ���¡�����-���͡�����+��10s���������豸
		KeyNum = T_CLEAR;
	else if ((KeyNum == T_MUSIC) && (!KeyVolAdd())) //ͬʱ���¡����ּ����͡�����+��10s�������ģʽ
		KeyNum = T_TESTMODE;
	else if (!KeyVolAdd())
		KeyNum = T_VOLINC;

	if (OLP())//�����ˮ��������,Ϊ1��������ˮΪ�ͣ�Ϊ0
		Flas_Anhydrous = 1;
	else
		Flas_Anhydrous = 0;
	return KeyNum; //���ذ���
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�ȥ�������̰��ж�
*******************************************************************/
void KeyEventPorc(uint8_t KeyTmp) //������ʱ���յ���GetKeyValue()�ķ���ֵ  һ��̶���������
{
	gbKeyPress=0;
	if(KeyCurValue!=KeyTmp) //���if�����ȥ�����ᱻִ�����Σ������ν���ʱֱ������
	{//KeyTmpΪ��ʱֵ��
		if(KeyCurValueBK!=KeyTmp)
			KeyCurValueBK=KeyTmp;
		else //�ڶ��ν���ʱִ����һ��
			KeyCurValue=KeyTmp; //
		return;
	}
	if(KeyCurValue==KeyLastValue)
	{
		if((KeyLastValue==0) || (cntKeyLoop==0))
			return;
		if(--cntKeyLoop==0) //����ڵڶ��ν�KeyEventPorc()��������һ�ν���if(KeyCurValue==KeyLastValue)��֮ǰ��ʱ�����渳ֵΪ5
		{ //cntKeyLoopΪ5����50msʱ��
			KeyValue=KeyLastValue; //����֮���ȷ���İ�����ֵ��KeyValue
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
����ԭ�ͣ�
���������
���������
�������ܣ�������������
*******************************************************************/
void KeyScan(void) 
{
	KeyEventPorc(GetKeyValue());
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ������Ĵ������������̰��������ݴ�������ʵ�֣�����������ʱ�������ⰴ���ر����Ӳ��ػ�
*******************************************************************/
void KeyComMsg(void) 
{
	if(gbKeyPress)
	{ 
//		if(alarm.Runing)//���ӹ�����001��003��������
//		{
//			alarm.Runing=0;
//			gbKeyNone=1;//�޸�����ģʽʱĳЩ�����ر����Ӳ��ػ�ʱ�ز��˵����⣬Ϊ1�õ���İ�����Ч
//			//color_mode=COLOR_OFF;
//			Uart0_SendString_3Step=5;
//			GP389_OFF();
//			//ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);	
//			TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 	
//			return;
//		}
		//Flag_data_bk=1;//���ݸı�ʱ����
	   	switch(KeyValue)
		{
			case KU(T_POWER):
			{ }
			case KLU(T_POWER): //value65 ÿ������7��ģʽ 
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
			case KR(T_POWER): //value97 ÿ������7��ģʽ��
			{
				if(PlayMode!=PLAY_OFF)
				{
					if(cntKeylong==25)//����5s����wifi���
					{
						Uart0_SendString_3Step=5;
						Uart0Transmit_SendString("$$$ATWQ\r\n"); 
					}
					break;
				}
			}
			if(PlayMode!=PLAY_OFF)//�ڿ���״̬�²ſ��Բ�����������
			{
				case KU(T_MUSIC): //value66
				{
					if((PlayMode==PLAY_ON) || (PlayMode==PLAY_BT)) //��������
					{
						enableMute=0;
						enable_SPApause=0;
						PlayMode=PLAY_MUSIC;
						if(spa_name==SPA_OFF) //spa_nameΪö�ٱ������ͣ�SPA_ZENΪö�ٱ���Ԫ��
						{
							spa_name=SPA_BROOK;
						}
					}
					else if(PlayMode==PLAY_MUSIC) //�ڶ��ν���
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
				case KU(T_VOLINC): //value67 û��break���Լ���ִ����һ�����
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
				case KU(T_TIMER): //value37 ����ִ��
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
						color_mode=COLOR_WHITE; //ʵ��ѭ��
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
				case KR(T_DEFAULT): //��ϼ�������10s�������Ӻ����������������ָ���������
				{
					if(cntKeylong==50)
					{					 
						cntFlash=0;
						Led_Flash=0;
						setFactory=1;//��һ������timer�Ƴ�������
						PlayMode=PLAY_ON;
						
						Uart0_SendString_3Step=5;
						Uart0Transmit_SendString("$$$Factory setting\r\n");
						bt_send_cmd(BT_CLEAR_LIST);//��������б��б�
					}
					break;
				}
				case KR(T_CLEAR):  //��ϼ�������10s��power�������ӡ����������wifi�б�
				{
					if(cntKeylong==50)
					{					 
						setReset=1;	//��һ������timer����˸����
						cntFlash=0;
						Led_Flash=0;
						Uart0_SendString_3Step=5;
						Uart0Transmit_SendString("$$$Wifi reset\r\n");
					}
					break;
				}
				case KR(T_TESTMODE):  //��ϼ�������10s��SOUND���͡������ӡ��������������ģʽ
				{
					if(cntKeylong==25)
					{//����10�������ģʽ��Ȼ��ͨ��wifiģ�鷢������
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





