//ͷ�ļ�header file//
#include "app_main.h"

//�궨��macro definition//

//���Ͷ���byte definition//

//��������variable definition//
uint8_t  code Week_Table[]={0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

char  code SPASongs_Num_Table[7][24]= 
{
	"$$${MusicStatus:[07]}\r\n",//û�в��ţ�
	"$$${MusicStatus:[01]}\r\n",//��һ��
	"$$${MusicStatus:[02]}\r\n",
	"$$${MusicStatus:[03]}\r\n",
	"$$${MusicStatus:[04]}\r\n",
	"$$${MusicStatus:[05]}\r\n",
	"$$${MusicStatus:[06]}\r\n",
};  

//��־δ����flags definetion//

//��������Function declaration//
//void Program1_LaterNum();

//��������function definetion//
/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ��Ƚϻ�õĿ���ָ����Ԥ���趨��ָ��Ա��Ƿ�һ�£����һ�¾�ִ����ع��ܣ������޲���
*******************************************************************/
uint8_t char_compare( char *source,  char *dest)
{
	while(*dest)
	{	
		if((*source!=*dest)||(*source==0)||(*dest==0))
			return 1;
		source++;
		dest++;
	}
	return 0;
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ����ַ�ת��Ϊ����
*******************************************************************/
char char_to_hex(char indata1,char indata2)
{
	if(indata1>='A')
		indata1=((indata1-'A')+0x0A);
	else
		indata1=(indata1-'0');
	if(indata2>='A')
		indata2=((indata2-'A')+0x0A);
	else
		indata2=(indata2-'0');
	return ((indata1<<4)|indata2);
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ����ַ�ת��Ϊ����
*******************************************************************/
char char_to_bcd(char indata1,char indata2)
{
	if(indata1>='A')
		indata1=((indata1-'A')+0x0A);
	else
		indata1=(indata1-'0');
	if(indata2>='A')
		indata2=((indata2-'A')+0x0A);
	else
		indata2=(indata2-'0');
	return (indata1*10+indata2);
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ���Ҫ���͵�Э��ת��Ϊ�ַ�
*******************************************************************/
void hex_to_char(uint8_t *p,uint8_t indata)
{
	uint8_t tmp;
	tmp=indata>>4;
	if(tmp>=0x0A)
	 *p=((tmp-0x0A)+'A'); 
	else
	 *p=(tmp+'0');
	p++;
	tmp=indata&0x0F;
	if(tmp>=0x0A)
		*p=((tmp-0x0A)+'A'); 
	else
		*p=(tmp+'0');
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�����Ҫ���͵��������ز�������������ǰ������¿����ͽ��յ���turn on��������
          ����app������ɫ������ʱGP389_ON_APP()����
*******************************************************************/
void GP389_ON(void) //
{
	PlayMode=PLAY_ON;
	PlayMode=((PlayMode_bk==PLAY_MUSIC)?PLAY_MUSIC:PLAY_ON);//����ػ�ʱ��musicģʽ�������ظ���ģʽ
	sys_volume=sys_volume_bk;
	color_mode=color_mode_bk;
	//Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);//�ػ�֮ǰ�������Ϊ0�ͻָ�Ϊ1������Ϊ�ػ�֮ǰ����//2018.4.9�ػ�֮ǰ����Ϊ0������ҲΪ0
	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	
	Uart0_SendString_3Step=3;
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�app������ɫ���ƹ⡢���ָ���̨����ʱ��̨Ϊ�ػ�״̬����Ȼ�󿪻������������������ָ���
          ֻ�����µĹ��ܲ���(��ɫ���ƹ⡢����)
*******************************************************************/
void GP389_ON_APP(void) //
{
	PlayMode=PLAY_ON;
	sys_volume=sys_volume_bk;
	//Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);//�ػ�֮ǰ�������Ϊ0�ͻָ�Ϊ1������Ϊ�ػ�֮ǰ����//2018.4.9�ػ�֮ǰ����Ϊ0������ҲΪ0
	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	Uart0_SendString_3Step=3;
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ������ػ���app���ƹػ�ִ�е�
*******************************************************************/
void GP389_OFF(void) //
{
	PlayMode_bk=PlayMode;
	PlayMode=PLAY_OFF;
	sys_volume_bk=sys_volume;
	color_mode_bk=color_mode;
	color_mode=COLOR_OFF;
	TimerMode=cntTimer=TIMER_OFF;//
	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	Uart0_SendString_3Step=3;
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ���������Ӧʱ������APP�ϵ��˵ƹ����ȣ�������С����ر����岢�ػ�����app�ϵ����ض�
          ��ɫ�ĵƹ⣬���ߵ����ض����֣����ߵ���program,��ô����ֹͣ����Ӧ����Ĳ���
*******************************************************************/
//void GP389_OFF_AND_AlarmOFF()//���ӹ�����001��003��������
//{
//	alarm.Runing=0;
//	gbKeyNone=0;
//	GP389_OFF();
//	//color_mode=COLOR_OFF;
//	Uart0_SendString_3Step=5;
//	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);	
//	TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 	
//	return;
//}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ����뿪�ػ�ʱ���ô˺���
*******************************************************************/
void ApplicationGP389_ONOFF(uint8_t onoff)
{//$$${GpSta:[00,00,01,01,FFFFFF,03,0A,00,01E;00,01,00,FFFFFF,03,0A,01,FF,17:3B,FF,02]}\r\n"
 //�����е������Զ��ŷָ��������� 16 ���Ʊ�ʾ�����ﲻд"0x"��������������ÿ���ֶΣ�ǰ10���ֶΣ��ֱ��ʾ��
 //���Ƿ�ػ�,00�ǿ�����01�ǹػ�
 //������ڼ��׸裨01 - 06��
 //�������صƣ�00�� 01����
 //�������Ƿ��ǲʵ�ģʽ��00 �� 01 ������ע���ʵ�ѭ����Ϊ1����ѭ����
 //������Ƶ�RGBֵ��000000 - FFFFFF�����ʵ�ѭ�����ֵΪ000000��
 //����������( 00 - 03 )
 //����������ֵ ( 00 - 0A )
 //��������ͣpauseѡ��00��ʾ��������,01��ʾ��ͣ���ţ�
 //������ʱ����00(��/��������ʱ)��0F(15min)��1E(30min)��3C(60min)   
 //��Ӣ�ķֺš�;������������������
 //�����ӿ�����Ϣ�������е������Զ��ŷָ��������� 16 ���Ʊ�ʾ�����ﲻд"0x"��������������ÿ���ֶΣ�10���ֶΣ��ֱ��ʾ��
 //������ Alarm �� ID �� ��ע������ֻ��һ�����ӣ�Ĭ��Ϊ0��
 //������ڼ��׸裨01 - 06��
 //�������Ƿ��ǲʵ�ģʽ��00 �� 01 ������ע���ʵ�ѭ����Ϊ1����ѭ����
 //������Ƶ�RGBֵ��000000 - FFFFFF�����ʵ�ѭ�����ֵΪ000000��
 //����������( 00 - 03 )
 //����������ֵ ( 00 - 0a )
 //�����������ܿ��أ�00 �� 01: ������Ĭ��0��
 //��������һ�����յĿ��أ�ÿһ����һ�� bit ��ʾ
 //  0x80 �����Ƿ��ظ�����
 //  0x01 ������һ�������壬����ֻ��һ�Σ���һ��������֮�ڵģ�
 //  0xFF ��������ȫ��������ÿ���ظ����Դ�����
 //  BIT            7       6    5    4    3    2    1    0
 //  ����x   repeat  ��  ��  ��  ��  ��  ��  һ  ��ע��repeatΪÿ�ܶ��ظ����壬����ֻ��һ�Σ���һ��
 //  ������֮�ڵģ����綨ʱ13:00�����ظ�������μ�ʱ��(7*24*60-1)min֮�������������֮��
 //  ��12:59�֣�
 //����������ʱ�䣨17:3B��(����16����,:��������ӵ�ʱ����Χ��00-17; :�ұ������ӵķ֣���Χ��00-3B)
 //����������ʱ�䣨char����1���ֽڣ�00-FF (��λ:����)
	char i;
//	char code respGP389_AllSta[]={"$$${GpSta:[00,00,00,00,000000,00,00,00,00;00,00,00,000000,00,00,00,00,00:00,00]}\r\n"};
	char code respGP389_AllSta[]={"$$${GpSta:[00,00,00,00,000000,00,00,00,000;00,00,00,000000,]}\r\n"};//���ӹ�����001��003��������
	do{WDT_clear();} while(Uart0_EnableSend);
	Count200ms_3Step=0;
	//Uart0_SendString_3Step=0;
	for(i=0;i<UART0_LEN_BUFFER;i++)
		Uart0_TransmitBuffer[i]=0x00;
	//for(i=0;i<82;i++)//���ӹ�����001��003��������
	for(i=0;i<41;i++)
		Uart0_TransmitBuffer[i]=respGP389_AllSta[i];
	hex_to_char(&Uart0_TransmitBuffer[11],onoff);
	hex_to_char(&Uart0_TransmitBuffer[14],((PlayMode==PLAY_MUSIC)?spa_name:7));//�����ж��ǲ�������ģʽ������ǣ��ǵڼ��׸裬������Ǿͷ�����ͣ��7
	hex_to_char(&Uart0_TransmitBuffer[17],((color_mode==COLOR_OFF)?0:1));//Ϊ1ʱ��Ҫ�ػ�������Ҫ��color_mode_bk�Աȣ�Ϊ0��Ҫ�������Ѿ���color_mode_bk��ֵ��color_mode
	hex_to_char(&Uart0_TransmitBuffer[20],((color_mode==COLOR_CHANGE)?1:0));
	hex_to_char(&Uart0_TransmitBuffer[23],userR_val);
	hex_to_char(&Uart0_TransmitBuffer[25],userG_val);	
	hex_to_char(&Uart0_TransmitBuffer[27],userB_val);
	hex_to_char(&Uart0_TransmitBuffer[30],Light_Brightness);
	hex_to_char(&Uart0_TransmitBuffer[33],sys_volume);
	hex_to_char(&Uart0_TransmitBuffer[36],enable_SPApause);
	hex_to_char(&Uart0_TransmitBuffer[39],cntTimer);
//	hex_to_char(&Uart0_TransmitBuffer[42],00);//����ID��Ĭ��Ϊ0   //���ӹ�����001��003��������
//	hex_to_char(&Uart0_TransmitBuffer[45],alarm.spa_name);//�����ж��ǲ�������ģʽ������ǣ��ǵڼ��׸裬������Ǿͷ�����ͣ��7
//	hex_to_char(&Uart0_TransmitBuffer[48],((alarm.color_mode==COLOR_CHANGE)?1:0));//�Ƿ��ǲʵ�ģʽ��ѭ������ģʽ��
//	hex_to_char(&Uart0_TransmitBuffer[51],alarm.userR_val);
//	hex_to_char(&Uart0_TransmitBuffer[53],alarm.userG_val);	
//	hex_to_char(&Uart0_TransmitBuffer[55],alarm.userB_val);
//	hex_to_char(&Uart0_TransmitBuffer[58],alarm.Brightness);//����
//	hex_to_char(&Uart0_TransmitBuffer[61],alarm.volume);//����
//	hex_to_char(&Uart0_TransmitBuffer[64],alarm.Enable); //�����ܿ���
//	hex_to_char(&Uart0_TransmitBuffer[67],alarm.Week);  //�����ܼ����4λ
//	hex_to_char(&Uart0_TransmitBuffer[70],alarm.Hour);  //���Ӷ�ʱ��ʱ�䣬Сʱ����λ
//	hex_to_char(&Uart0_TransmitBuffer[73],alarm.Minute);//���Ӷ�ʱ��ʱ�䣬���ӵ���λ
//	hex_to_char(&Uart0_TransmitBuffer[76],alarm.Duration);//��������ʱ�䣬����λ
	Uart0_SendString_3Step=2;
	Uart0_Tx_Pointer=0;
	Uart0_EnableSend=1;
	Uart0_Tx_TimeOut=10;	
	USI0DR=Uart0_TransmitBuffer[Uart0_Tx_Pointer++];
	if(cntFlag_ON_OFF==0)
		cntFlag_ON_OFF=2;
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ����ڽ��յ�������Э���Ԥ����Э��Ա�ȷ�����ĸ�����ִ�в����������޲���
*******************************************************************/
void Wifi_CommandDeal(char *Uart0_Receive)
{
	uint8_t i;
	uint8_t temp_time;//��ʱ�õı������ڽ��ܶ�ʱʱ������
	char *WIFI_CMD;
	WIFI_CMD=&Uart0_Receive[3];
//	if(char_compare(WIFI_CMD,"{AlarmSet")==0)//ok  //���ӹ�����001��003��������
//	{//$$${AlarmSet:[00,01,00,01,FFFFFF,03,0A,00,00,FF,17:3B,FF,0a/20/00/12/01/01/02]}\r\n
//	 //�����е������Զ��ŷָ��������� 16 ���Ʊ�ʾ�����ﲻд"0x"��������������ÿ���ֶΣ�10���ֶΣ��ֱ��ʾ��
//	 //������ Alarm �� ID �� ��ע������ֻ��һ�����ӣ�Ĭ��Ϊ0��
//	 //������ڼ��׸裨01 - 06��
//	 //�������صƣ�00�� 01����
//	 //�������Ƿ��ǲʵ�ģʽ��00 �� 01 ������ע���ʵ�ѭ����Ϊ1����ѭ��,��
//	 //������Ƶ�RGBֵ��000000 - FFFFFF�����ʵ�ѭ�����ֵΪ000000��
//	 //����ȥ�����������ɫ���±꣬��app�����е�һ�£������״̬ͬ���ƶ˺�app��00-0B��,����00����صƣ�0A����ʵƣ�0B�����Զ�����ɫ
//	 //����������( 00 - 03 )
//	 //����������ֵ ( 00 - 0a )
//	 //��������ͣpauseѡ��00��ʾ��������,01��ʾ��ͣ���ţ�
//	 //��(ȥ��)������ʱ����00(��/��������ʱ)��0F(15min)��1E(30min)��3C(60min)   
//	 //�����������ܿ��أ�00 �� 01: ������Ĭ��0��
//	 //��������һ�����յĿ��أ�ÿһ����һ�� bit ��ʾ
//	 //  0x80 �����Ƿ��ظ�����
//	 //  0x01 ������һ�������壬����ֻ��һ�Σ���һ��������֮�ڵģ�
//	 //  0xFF ��������ȫ��������ÿ���ظ����Դ�����
//	 //  BIT      7       6    5    4    3    2    1    0
//	 //  ����x  repeat    ��   ��   ��    ��   ��   ��   һ  ��ע��repeatΪÿ�ܶ��ظ����壬����ֻ��һ��
//	 //���������Ӷ�ʱʱ��㣨17:3B��(����16����,:��������ӵ�ʱ����Χ��00-17; :�ұ������ӵķ֣���Χ��00-3B)
//	 //����������ʱ�䣨int����4���ֽڣ�00-FF (��λ��ǿ����)
//	 //������ǰʱ������ֱ���ʮ����������б�ܱ�ʾ����ʽ���£���ʱ/��/��/��/��/��/���ڣ�
//	 //  ���� 2018 �� 1 �� 1 �� 10 ʱ 32 �� 0 �����ڶ� ��ô��ʽ�� 0a/20/00/12/01/01/02
//	 //(WIFI_CMD[11]-'0')  //Programma��ID����ʱû�У�Ϊ�Ժ���չԤ��   
//		alarm.spa_name  =(WIFI_CMD[15]-'0');//�ڼ��׸�
//		if(alarm.spa_name>=7)
//			alarm.spa_name=0;
//		alarm.color_mode=(WIFI_CMD[18]-'0');//�ƹ�Ŀ���
//		alarm.color_mode=(WIFI_CMD[18]=='1')?((WIFI_CMD[21]=='1')?COLOR_CHANGE:COLOR_USER):COLOR_OFF;
//		alarm.userR_val=char_to_hex(WIFI_CMD[23],WIFI_CMD[24]);//Rֵ  
//		alarm.userG_val=char_to_hex(WIFI_CMD[25],WIFI_CMD[26]);//Gֵ  
//		alarm.userB_val=char_to_hex(WIFI_CMD[27],WIFI_CMD[28]);//Bֵ  
//		alarm.Brightness=(WIFI_CMD[31]-'0');//����
//		alarm.volume=char_to_hex(WIFI_CMD[33],WIFI_CMD[34]);//����
//		alarm.play_pause=WIFI_CMD[37]-'0';//��ͣ����
//		alarm.Enable=(WIFI_CMD[40]-'0');//�����ܿ���
//		alarm.Week=char_to_hex(WIFI_CMD[42],WIFI_CMD[43]); 
//		alarm.Hour=char_to_hex(WIFI_CMD[45],WIFI_CMD[46]);   //�������Сʱ��
//		alarm.Minute=char_to_hex(WIFI_CMD[48],WIFI_CMD[49]); 
//		alarm.Duration=char_to_hex(WIFI_CMD[51],WIFI_CMD[52]);//��������ʱ�䣬ת��Ϊ��ֵ
//		IE3 &= ~0x04;    	// Enable WT interrupt
//		gRTC_Hour  =char_to_hex(WIFI_CMD[54],WIFI_CMD[55]);
//		gRTC_Minute=char_to_hex(WIFI_CMD[57],WIFI_CMD[58]);
//		gRTC_Sec   =char_to_hex(WIFI_CMD[60],WIFI_CMD[61]);
//		gRTC_Week  =WIFI_CMD[73]-'0';
//		gRTC_Week=Week_Table[gRTC_Week];
//		IE3 |= 0x04;    	// Enable WT interrupt
//		if((alarm.Enable)&&(!(alarm.Week&0x7F)))
//		 Flag_alarm_wake_only=1;
//		else
//		 Flag_alarm_wake_only=0;
//	}
//	else if(char_compare(WIFI_CMD,"{AlarmOnOff")==0)  //���ӹ�����001��003��������
//	{//$$${AlarmOnOff:[00,00,10/21/33/12/01/1D/01]}\r\n
//	 //������ Alarm �� ID �� 
//	 //���������ӵĿ���
//	 //������ǰʱ������ֱ���ʮ����������б�ܱ�ʾ����ʽ���£���ʱ/��/��/��/��/��/���ڣ�
//	 //	 ���� 2018 �� 1 �� 1 �� 10 ʱ 32 �� 0 �����ڶ� ��ô��ʽ��  0a/20/00/12/01/01/02
//	 //(WIFI_CMD[11]-'0')  //Programma��ID����ʱû�У�Ϊ�Ժ���չԤ��   
//		alarm.Enable=(WIFI_CMD[17]-'0'); 
//		IE3 &= ~0x04;    	// Enable WT interrupt
//		gRTC_Hour    =char_to_hex(WIFI_CMD[19],WIFI_CMD[20]);
//		gRTC_Minute  =char_to_hex(WIFI_CMD[22],WIFI_CMD[23]);
//		gRTC_Sec     =char_to_hex(WIFI_CMD[25],WIFI_CMD[26]);
//		gRTC_Week    =WIFI_CMD[38]-'0';
//		gRTC_Week=Week_Table[gRTC_Week];
//		IE3 |= 0x04;    	// Enable WT interrupt
//		if((alarm.Enable)&&(!(alarm.Week&0x7F)))
//		 Flag_alarm_wake_only=1;
//		else
//		 Flag_alarm_wake_only=0;
//	}
		if(char_compare(WIFI_CMD, "Turn on unit")==0) 
		{//$$$Turn on unit\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
			{
				GP389_ON();	
				//ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
				return;
			}
		}
		else if(char_compare(WIFI_CMD, "Turn on Lightr")==0)
		{//$$$Turn on Lightr\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();	
//			if(alarm.Runing)//���һ���ǿ���״̬�Ž�ȥ  //���ӹ�����001��003��������
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			color_mode=color_mode_bk;
			if(color_mode==COLOR_OFF)
				color_mode=COLOR_WHITE;
			Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);
			return;
		}
		else if(char_compare(WIFI_CMD, "Change light color")==0)  
		{//$$$Change light color Allplay\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing) //���ӹ�����001��003��������
//			{//���һ���ǿ�������½����
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			color_mode=COLOR_CHANGE;
			play_rgb_index=0;
			curR_val=0;
			curG_val=0;
			curB_val=0;
			Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);
			return;
		} 
		else if(char_compare(WIFI_CMD, "User color")==0) 
		{//$$$User color FFFFFF\r\n(000000-FFFFFF)
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			userR_val=char_to_hex(WIFI_CMD[11],WIFI_CMD[12]);
			userG_val=char_to_hex(WIFI_CMD[13],WIFI_CMD[14]);
			userB_val=char_to_hex(WIFI_CMD[15],WIFI_CMD[16]);
			color_mode=COLOR_USER;
			Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);
			return;
		}
		else if(char_compare(WIFI_CMD, "Select Sound file")==0) 
		{//$$$Select Sound file to play01\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing) //���ӹ�����001��003��������
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			enableMute=0;//��mute
			enable_SPApause=0;
			PlayMode=PLAY_MUSIC;
			spa_name=WIFI_CMD[26]-'0';
			if(cur_spa_name==spa_name)
				spa_cmd=SPA_PALY;
			Uart0_SendString_3Step=1;
			Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);//ÿ�θ������־ͷ���appͬ��
			return;
		}
		else if(char_compare(WIFI_CMD, "music play")==0)
		{//$$$music play\r\n
			//Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			PlayMode=PLAY_MUSIC;
			enableMute=0;     //�������
			enable_SPApause=0;//Ϊ0��ʾ����
			if(spa_name==SPA_OFF) //spa_nameΪö�ٱ������ͣ�SPA_ZENΪö�ٱ���Ԫ��
				spa_name=SPA_BROOK;
			else
				spa_cmd=SPA_PALY;			
			/*if(PlayMode==PLAY_BT)
			{
				if(flag_bt_play)
				{
					bt_cmd=BT_PALY;//Э��������һ��������Բ���/��ͣ��Ҫ��������
				}
			}*/
			return;
		}
		else if(char_compare(WIFI_CMD,"ProPlay\r\n")==0)//���յ��Ͳ������Ľ��յ���$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n����
		{//$$${Program
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			spa_name=program1.spa_name;
			color_mode=program1.color_mode;
			userR_val=program1.userR_val;
			userG_val=program1.userG_val;
			userB_val=program1.userB_val;
			Light_Brightness=program1.Light_Brightness;
			sys_volume=program1.sys_volume;
			PlayMode=program1.PlayMode;
			enable_SPApause=program1.enable_SPApause;
			program1.enableMute=enableMute=((enable_SPApause==0)?0:1);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
			spa_cmd=((enable_SPApause==0)?SPA_PALY:SPA_PAUSE);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
			TimerMode=program1.TimerMode;
			return;
		} 
		else if (char_compare(WIFI_CMD, "Turn on Humidifier") == 0)
		{//$$$Turn on/off Humidifier\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if (PlayMode == PLAY_OFF)
				GP389_ON_APP();
			//			if(alarm.Runing) //���ӹ�����001��003��������
			//			{
			//				alarm.Runing=0;
			//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
			//			}
			if(AtomizationMode1==ATOMIZATION_OFF) //��һ�������������Ѿ��������Ҳ�����С��λ��Ȼ���ֽ��յ��������ǿ�и���Ϊ��С��������
				AtomizationMode1 = ATOMIZATION_THREE;
			return;
		}
		else if (char_compare(WIFI_CMD, "Turn off Humidifier") == 0)
		{//$$$Turn on/off Humidifier\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if (PlayMode == PLAY_OFF)
				GP389_ON_APP();
			//			if(alarm.Runing) //���ӹ�����001��003��������
			//			{
			//				alarm.Runing=0;
			//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
			//			}
			AtomizationMode1 = ATOMIZATION_OFF;
			return;
		}
//		else if(char_compare(WIFI_CMD,"{Program")==0)//�ػ�״̬�´�
//		{//$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n
//			Uart0Transmit_SendString(Uart0_Receive);
//			if(PlayMode==PLAY_OFF)
//				GP389_ON_APP();
//			if(alarm.Runing)
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
//			Program1_LaterNum(); //�Ȱѵ����ָ��������֣�
//			program1.spa_name=spa_name=WIFI_CMD[14]-'0';
//			program1.color_mode=color_mode=((WIFI_CMD[17]=='1')?COLOR_WHITE:COLOR_OFF);//�ƹ�Ŀ���
//			program1.color_mode=color_mode=((WIFI_CMD[17]=='1')?((WIFI_CMD[20]=='1')?COLOR_CHANGE:COLOR_USER):COLOR_OFF);//���ж���һ���Ƿ����ƹ⣬��������ƹ�����ж��Ƿ����ʵƣ���������������û�ֵ																								   //�û�ֵ������RGB��ֵ
//			program1.userR_val=userR_val=char_to_hex(WIFI_CMD[22],WIFI_CMD[23]);
//			program1.userG_val=userG_val=char_to_hex(WIFI_CMD[24],WIFI_CMD[25]);
//			program1.userB_val=userB_val=char_to_hex(WIFI_CMD[26],WIFI_CMD[27]);
//			program1.Light_Brightness=Light_Brightness=char_to_bcd(WIFI_CMD[29],WIFI_CMD[30]);
//			program1.sys_volume=sys_volume=char_to_hex(WIFI_CMD[32],WIFI_CMD[33]);//����
//			program1.PlayMode=PlayMode=PLAY_MUSIC;
//			program1.enable_SPApause=enable_SPApause=WIFI_CMD[36]-'0';
//			program1.enableMute=enableMute=((enable_SPApause==0)?0:1);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
//			spa_cmd=((enable_SPApause==0)?SPA_PALY:SPA_PAUSE);//����֮ǰ���Ż�����ͣ��ֻҪ������Ӧ�������������žͲ���
//			program1.TimerMode=TimerMode=cntTimer=char_to_hex(WIFI_CMD[38],WIFI_CMD[39]);
//			return;
//		}
		else if(char_compare(WIFI_CMD,"VER0.\r\n")==0) 
		{
			return;
		}
		else if(char_compare(WIFI_CMD,"VER\r\n")==0) 
		{
			Uart0Transmit_SendString(MCU_VER);//ÿ��������Ҫ�ֶ���д����
			return;
		}
		else if(char_compare(WIFI_CMD,"GpSta")==0)
		{//����״̬�ǹػ�
			Uart0_SendString_3Step=0;
			cntFlag_ON_OFF=0;
			return;//PlayMode=PLAY_OFF;	��Ϊ��ǿ�ƹػ������Կ��Բ�ҪGP389_OFF()/GP389_ON()
		}
//		else if(char_compare(WIFI_CMD,"GpSta:[00]\r\n")==0)
//		{//����״̬�ǿ�����
//			Uart0_SendString_3Step=0;
//			cntFlag_ON_OFF=0;
//			return;//PlayMode=PLAY_OFF;	��Ϊ��ǿ�ƹػ������Կ��Բ�ҪGP389_OFF()/GP389_ON()
//		}
	
	
	if(PlayMode!=PLAY_OFF)
	{
		if(char_compare(WIFI_CMD,"{TimeSync")==0)
		{//$$${TimeSync:[0A/20/00/12/01/01/02]}\r\n
			IE3 &= ~0x04;    	// Enable WT interrupt
			gRTC_Hour  =char_to_hex(WIFI_CMD[11],WIFI_CMD[12]);
			gRTC_Minute=char_to_hex(WIFI_CMD[14],WIFI_CMD[15]);
			gRTC_Sec   =char_to_hex(WIFI_CMD[17],WIFI_CMD[18]);
			gRTC_Week  =WIFI_CMD[30]-'0';
			gRTC_Week  =Week_Table[gRTC_Week];
			IE3 |= 0x04;    	// Enable WT interrupt
			Falg_TimeSync_Allow=0;//���յ�ͬ����Ϣ�Ͳ��ٷ�������ͬ����������Ϊ0
			return;
		}	
		else if(char_compare(WIFI_CMD, "Turn off unit")==0) 
		{//����Э��������appֱ�ӷ��͹����Ĺػ������������Ϳ��Թػ��������յ���������Ҫ�ж�
		 //��ʱ���ý����ж��ǲ����ڹػ�״̬����Ϊ�ػ�֮����Ҫ�趨��״̬�Գ�����Ӱ�죬���߿���֮
		 //����ʼ����Щ״̬
		 //$$$Turn off unit\r\n
			Uart0Transmit_SendString(Uart0_Receive);
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			
			Uart0_SendString_3Step=3;
			GP389_OFF();
			//ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
			return;
		}
		else if(char_compare(WIFI_CMD, "Turn on BT")==0) 
		{//$$$Turn on BT\r\n
			PlayMode=PLAY_BT;
		}
		else if (char_compare(WIFI_CMD, "previous song\r\n") == 0)
		{//$$$previous song\r\n ��һ�׸�  
			spa_cmd = SPA_PALY;
			if ((spa_name > SPA_OFF) && (spa_name <= SPA_ZEN))
				spa_name--;
			else
				if (spa_name == SPA_OFF)//�γ�ѭ��
					spa_name = SPA_ZEN;
			Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);//ÿ�θ������־ͷ���appͬ��
			return;
		}
		else if (char_compare(WIFI_CMD, "next song\r\n") == 0)
		{//$$$next song\r\n ��һ�׸�
			spa_cmd = SPA_PALY;
			if ((spa_name >= SPA_OFF) && (spa_name < SPA_ZEN))
				spa_name++;
			else
				if (spa_name == SPA_ZEN) //�γ�ѭ��
					spa_name = SPA_OFF;
			Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);//ÿ�θ������־ͷ���appͬ��
			return;
		}
		else if(char_compare(WIFI_CMD, "music pause")==0)
		{//$$$music pause\r\n
			if(PlayMode==PLAY_MUSIC)
			{				    
				enable_SPApause=1;
				spa_cmd=SPA_PAUSE;
			}
//			if(PlayMode==PLAY_BT)
//			{
//				if(flag_bt_play) 
//				{
//					bt_cmd=BT_PAUSE;//Э��������һ��������Բ���/��ͣ��Ҫ��������
//				}
//			}
			return;
		}
		else if(char_compare(WIFI_CMD,"{MusicStatus")==0)
		{
			return;
		}
		else if(char_compare(WIFI_CMD, "Adjust Output Volume")==0)
		{//$$$Adjust Output Volumexx\r\n��xx����01-10
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			sys_volume=char_to_bcd(WIFI_CMD[20],WIFI_CMD[21]);
			enableMute=0;
		}
		else if(char_compare(WIFI_CMD, "volume up")==0) 
		{//$$$volume up\r\n
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			sys_volume=((sys_volume < 10)?((sys_volume<=8)?(sys_volume += 2):(sys_volume += 1)):10);
			enableMute=0;
		}
		else if(char_compare(WIFI_CMD, "volume down")==0) 
		{//volume down\r\n
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			sys_volume=((sys_volume > 1)?((sys_volume>=3)?(sys_volume -= 2):(sys_volume -= 1)):1);
			enableMute=0;
		}
		else if(char_compare(WIFI_CMD, "music mute")==0) 
		{//$$$music mute\r\n
			enableMute=1;
			return;
		}
		else if(char_compare(WIFI_CMD, "music unmute")==0) 
		{//$$$music unmute\r\n
			enableMute=0;
			return;
		}
		else if(char_compare(WIFI_CMD, "Set Timer")==0)//ok
		{//$$$Set Timer XXX\r\n(30/60/90)
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			temp_time=char_to_bcd(WIFI_CMD[10], WIFI_CMD[11]);//ֻ���ǰ�������֣����ж�
			TimerMode=cntTimer= (temp_time == 90) ? 90 : ((temp_time == 60) ? 60 : ((temp_time == 30) ? 30 : 0));
		}
		else if(char_compare(WIFI_CMD, "Turn off Timer")==0)//ok
		{//$$$Turn off Timer\r\n
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			TimerMode=cntTimer=TIMER_OFF;
		}
		else if(char_compare(WIFI_CMD, "Turn off Lightr")==0)  
		{//$$$Turn off Lightr\r\n
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			color_mode=COLOR_OFF;
		} 
		else if(char_compare(WIFI_CMD,"light up")==0)
		{//$$$light up\r\n
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			if(Light_Brightness<3)
				Light_Brightness++;
		}
		else if(char_compare(WIFI_CMD,"light down")==0)
		{//$$$light down\r\n
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			if(Light_Brightness>0)
				Light_Brightness--;
		}
		else if(char_compare(WIFI_CMD,"Adjust Light Brightness")==0)
		{//$$$Adjust Light Brightness 01\r\n(01-03)
//			if(alarm.Runing)  //���ӹ�����001��003��������
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			Light_Brightness=char_to_bcd(WIFI_CMD[24],WIFI_CMD[25]);
		}
		else if (char_compare(WIFI_CMD, "Adjust moist mode") == 0)
		{//$$$Adjust moist mode XX\r\n (01-03)
		 //			if(alarm.Runing)  //���ӹ�����001��003��������
		 //			{
		 //				GP389_OFF_AND_AlarmOFF();
		 //				return;
		 //			}
			AtomizationMode1 = char_to_bcd(WIFI_CMD[18], WIFI_CMD[19]);
		}
//		else if(char_compare(WIFI_CMD,"AlaGpSta\r\n")==0) //���ӹ�����001��003��������
//		{//W-G:"$$$AlaGpSta\r\n"
//		 //G-W:"$$${AlaGpSta:[00,01,01,00,FFFFFF,03,0A,00,01,FF,17:3B,FF]}\r\n"
//		 //�����е������Զ��ŷָ��������� 16 ���Ʊ�ʾ�����ﲻд"0x"��������������ÿ���ֶΣ�13���ֶΣ��ֱ��ʾ��
//		 //������ Alarm �� ID �� ��ע������ֻ��һ�����ӣ�Ĭ��Ϊ0��
//		 //������ڼ��׸裨01 - 06��
//		 //�������صƣ�00�� 01����
//		 //�������Ƿ��ǲʵ�ģʽ��00 �� 01 ������ע���ʵ�ѭ����Ϊ1����ѭ����
//		 //������Ƶ�RGBֵ��000000 - FFFFFF�����ʵ�ѭ�����ֵΪ000000��
//		 //����������( 00 - 03 )
//		 //����������ֵ ( 00 - 0a )
//		 //��������ͣpauseѡ��00��ʾ��������,01��ʾ��ͣ���ţ�
//		 //��ȥ����������ʱ����00(��/��������ʱ)��0F(15min)��1E(30min)��3C(60min)   
//		 //�����������ܿ��أ�00 �� 01: ������Ĭ��0��
//		 //��������һ�����յĿ��أ�ÿһ����һ�� bit ��ʾ
//		 //    0x80 �����Ƿ��ظ�����
//		 //    0x01 ������һ�������壬����ֻ��һ�Σ���һ��������֮�ڵģ�
//		 //    0xFF ��������ȫ��������ÿ���ظ����Դ�����
//		 //    BIT            7       6    5    4    3    2    1    0
//		 //    ����x   repeat  ��  ��  ��  ��  ��  ��  һ  ��ע��repeatΪÿ�ܶ��ظ����壬����ֻ��һ�Σ���һ��
//		 //  ������֮�ڵģ����綨ʱ13:00�����ظ�������μ�ʱ��(7*24*60-1)min֮�������������֮��
//		 //  ��12:59�֣�
//		 //���������Ӷ�ʱʱ��㣨17:3B��(����16����,:��������ӵ�ʱ����Χ��00-17; :�ұ������ӵķ֣���Χ��00-3B)
//		 //����������ʱ�䣨char����1���ֽڣ�00-FF (��λ:����)
//		 //��ȥ����������ǰʱ������ֱ���ʮ����������б�ܱ�ʾ����ʽ���£���ʱ/��/��/��/��/��/���ڣ� 
//		 //    ���� 2018 �� 1 �� 1 �� 10 ʱ 32 �� 0 �����ڶ� ��ô��ʽ�� 0a/20/00/12/01/01/02
//			char  code respAlaGpSta[]={"$$${AlaGpSta:[00,00,00,00,000000,00,00,00,00,00,00:00,00]}\r\n"};
//			for(i=0;i<UART0_LEN_BUFFER;i++)
//				Uart0_Receive[i]=0x00;
//			for(i=0;i<62;i++)
//				Uart0_Receive[i]=respAlaGpSta[i];
//			hex_to_char(&Uart0_Receive[14],0);//ID��Ĭ��Ϊ0
//			hex_to_char(&Uart0_Receive[17],((alarm.spa_name!=0)?alarm.spa_name:7));//�����ж��ǲ�������ģʽ������ǣ��ǵڼ��׸裬������Ǿͷ�����ͣ��7
//			hex_to_char(&Uart0_Receive[20],((alarm.color_mode==COLOR_OFF)?0:1));//���ص�
//			hex_to_char(&Uart0_Receive[23],(((alarm.color_mode==COLOR_CHANGE)?1:0)));//�Ƿ��ǲʵ�ģʽ��ѭ������ģʽ��
//			hex_to_char(&Uart0_Receive[26],alarm.userR_val);
//			hex_to_char(&Uart0_Receive[28],alarm.userG_val);
//			hex_to_char(&Uart0_Receive[30],alarm.userB_val);
//			hex_to_char(&Uart0_Receive[33],alarm.Brightness);//����
//			hex_to_char(&Uart0_Receive[36],alarm.volume);//����
//			hex_to_char(&Uart0_Receive[39],alarm.play_pause);
//			hex_to_char(&Uart0_Receive[42],alarm.Enable);
//			hex_to_char(&Uart0_Receive[45],alarm.Week);
//			hex_to_char(&Uart0_Receive[48],alarm.Hour);
//			hex_to_char(&Uart0_Receive[51],alarm.Minute);
//			hex_to_char(&Uart0_Receive[54],alarm.Duration);
//			Uart0_SendString_3Step=2;
//		}
		else if(char_compare(WIFI_CMD,"NorGpSta\r\n")==0)
		{//W-G:"$$$NorGpSta\r\n"
		 //G-W:"$$${NorGpSta:[00,00,01,01,FFFFFF,03,0A,00,01E,02]}\r\n"
			char  code respNorGpSta[]={"$$${NorGpSta:[00,00,00,00,000000,00,00,00,000,00]}\r\n"};
			for(i=0;i<UART0_LEN_BUFFER;i++)
				Uart0_Receive[i]=0x00;
			for(i=0;i<48;i++)
				Uart0_Receive[i]=respNorGpSta[i];
			hex_to_char(&Uart0_Receive[14],1);//�������ǿ���
			hex_to_char(&Uart0_Receive[17],((PlayMode==PLAY_MUSIC)?spa_name:7));//�����ж��ǲ�������ģʽ������ǣ��ǵڼ��׸裬������Ǿͷ�����ͣ��7
			hex_to_char(&Uart0_Receive[20],(color_mode==COLOR_OFF)?0:1);
			hex_to_char(&Uart0_Receive[23],(color_mode==COLOR_CHANGE)?0:1);//�Ƿ��ǲʵ�ģʽ��ѭ������ģʽ��
			hex_to_char(&Uart0_Receive[26],userR_val);
			hex_to_char(&Uart0_Receive[28],userG_val);
			hex_to_char(&Uart0_Receive[30],userB_val);
			hex_to_char(&Uart0_Receive[33],Light_Brightness);
			hex_to_char(&Uart0_Receive[36],sys_volume);
			hex_to_char(&Uart0_Receive[39],enable_SPApause);
			hex_to_char(&Uart0_Receive[42],cntTimer);
			hex_to_char(&Uart0_Receive[46],AtomizationMode1);
			Uart0_SendString_3Step=2;
		}
		else if(char_compare(WIFI_CMD," update")==0) 
		{//mcu update+0X43A4+0X0044,���յ������Ϣ��ʼ����
			Update_ByteSizeSum=(char_to_hex(WIFI_CMD[10],WIFI_CMD[11])<<8)|char_to_hex(WIFI_CMD[12],WIFI_CMD[13]);//���ٸ��ֽڣ���λ��ǰ����λ�ں�
			Update_PacketSum  =(char_to_hex(WIFI_CMD[17],WIFI_CMD[18])<<8)|char_to_hex(WIFI_CMD[19],WIFI_CMD[20]);//���ٸ���
			update_handle();
		}
		else if(char_compare(WIFI_CMD,"ATWQ\r\n")==0) 
		{//$$$ATWQ\r\n
			Uart0_SendString_3Step=0;
			return;
		}
		else if(char_compare(WIFI_CMD,"VER\r\n")==0) 
		{
			Uart0Transmit_SendString(MCU_VER);//ÿ��������Ҫ�ֶ���д����
			return;
		}
		else if(char_compare(WIFI_CMD,"Enter TestMode:")==0) 
		{
			Uart0_SendString_3Step=0;
			cntFlash=0;
			Led_Flash=0;
			setFactory=1;//��һ������timer�Ƴ�������
			PlayMode=PLAY_ON;
			bt_send_cmd(BT_CLEAR_LIST);//��������б��б�
			return;
		}
		Uart0Transmit_SendString(Uart0_Receive);
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
	}
	else //�ػ�״̬�½����ض�wifi����
	{
		if(char_compare(WIFI_CMD,"{NorGpSta")==0)
		{//W-G:"$$$NorGpSta\r\n"
		 //G-W:"$$${NorGpSta:[00,00,01,01,FFFFFF,03,0A,00,1E]}\r\n"
			char  code respNorGpSta[]={"$$${NorGpSta:[00,00,00,00,000000,00,00,00,00]}\r\n"};
			for(i=0;i<UART0_LEN_BUFFER;i++)
				Uart0_Receive[i]=0x00;
			for(i=0;i<48;i++)
				Uart0_Receive[i]=respNorGpSta[i];
			hex_to_char(&Uart0_Receive[14],0);//�������ǿ���
			hex_to_char(&Uart0_Receive[17],((PlayMode==PLAY_MUSIC)?spa_name:7));//�����ж��ǲ�������ģʽ������ǣ��ǵڼ��׸裬������Ǿͷ�����ͣ��7
			hex_to_char(&Uart0_Receive[20],(color_mode==COLOR_OFF)?0:1);
			hex_to_char(&Uart0_Receive[23],(color_mode==COLOR_CHANGE)?0:1);//�Ƿ��ǲʵ�ģʽ��ѭ������ģʽ��
			hex_to_char(&Uart0_Receive[26],userR_val);
			hex_to_char(&Uart0_Receive[28],userG_val);
			hex_to_char(&Uart0_Receive[30],userB_val);
			hex_to_char(&Uart0_Receive[33],Light_Brightness);
			hex_to_char(&Uart0_Receive[36],sys_volume);
			hex_to_char(&Uart0_Receive[39],enable_SPApause);
			hex_to_char(&Uart0_Receive[42],cntTimer);
			Uart0_SendString_3Step=2;
		}
		Uart0Transmit_SendString(Uart0_Receive);
	}
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ���app�Ϸ��͡�״̬�����ܸ���̨��״̬������ͬʱ���úü������������֣��ƹ�ȣ���Ȼ��һ�𷢹���
          Ȼ���̨��¼�����ڼ��η��ģ����µ�һ�θ��Ƹ�program1����һ�ε�program1���Ƹ�program2����Ϊ
		  ���ݺ��������Ͷ�һ�������Կ���ֱ�Ӹ���
*******************************************************************/
//void Program1_LaterNum()
//{
//	program3=program2;//��Ϊ���ݺ�����������ȫһ�������Կ���ֱ�Ӹ�ֵ
//	program2=program1;
//}




