//头文件header file//
#include "app_main.h"

//宏定义macro definition//

//类型定义byte definition//

//变量定义variable definition//
uint8_t  code Week_Table[]={0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

char  code SPASongs_Num_Table[7][24]= 
{
	"$$${MusicStatus:[07]}\r\n",//没有播放，
	"$$${MusicStatus:[01]}\r\n",//第一首
	"$$${MusicStatus:[02]}\r\n",
	"$$${MusicStatus:[03]}\r\n",
	"$$${MusicStatus:[04]}\r\n",
	"$$${MusicStatus:[05]}\r\n",
	"$$${MusicStatus:[06]}\r\n",
};  

//标志未定义flags definetion//

//函数声明Function declaration//
//void Program1_LaterNum();

//函数定义function definetion//
/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：比较获得的控制指令与预先设定的指令对比是否一致，如果一致就执行相关功能，否则无操作
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
函数原型：
输入参数：
输出参数：
函数功能：把字符转化为代码
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
函数原型：
输入参数：
输出参数：
函数功能：把字符转化为代码
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
函数原型：
输入参数：
输出参数：
函数功能：把要发送的协议转化为字符
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
函数原型：
输入参数：
输出参数：
函数功能：开机要发送的命令和相关操作，这个开机是按键按下开机和接收到“turn on”开机，
          其他app发送颜色并开机时GP389_ON_APP()函数
*******************************************************************/
void GP389_ON(void) //
{
	PlayMode=PLAY_ON;
	PlayMode=((PlayMode_bk==PLAY_MUSIC)?PLAY_MUSIC:PLAY_ON);//如果关机时是music模式，开机回复此模式
	sys_volume=sys_volume_bk;
	color_mode=color_mode_bk;
	//Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);//关机之前如果亮度为0就恢复为1，否则为关机之前亮度//2018.4.9关机之前亮度为0，开机也为0
	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	
	Uart0_SendString_3Step=3;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：app发送颜色、灯光、音乐给机台（此时机台为关机状态），然后开机，除了音量其他不恢复，
          只接受新的功能操作(颜色、灯光、音乐)
*******************************************************************/
void GP389_ON_APP(void) //
{
	PlayMode=PLAY_ON;
	sys_volume=sys_volume_bk;
	//Light_Brightness=((Light_Brightness==0)?1:Light_Brightness);//关机之前如果亮度为0就恢复为1，否则为关机之前亮度//2018.4.9关机之前亮度为0，开机也为0
	ApplicationGP389_ONOFF((PlayMode==PLAY_OFF)?1:0);
	Uart0_SendString_3Step=3;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：按键关机和app控制关机执行的
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
函数原型：
输入参数：
输出参数：
函数功能：闹钟在响应时操作在APP上调了灯光亮度，音量大小，则关闭闹铃并关机，在app上点了特定
          颜色的灯光，或者点了特定音乐，或者点了program,那么闹钟停止，响应所点的操作
*******************************************************************/
//void GP389_OFF_AND_AlarmOFF()//闹钟功能在001和003中屏蔽了
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
函数原型：
输入参数：
输出参数：
函数功能：申请开关机时调用此函数
*******************************************************************/
void ApplicationGP389_ONOFF(uint8_t onoff)
{//$$${GpSta:[00,00,01,01,FFFFFF,03,0A,00,01E;00,01,00,FFFFFF,03,0A,01,FF,17:3B,FF,02]}\r\n"
 //数组中第数据以逗号分隔，数字以 16 进制表示（这里不写"0x"），从左到右依次每个字段（前10个字段）分别表示：
 //》是否关机,00是开机，01是关机
 //》代表第几首歌（01 - 06）
 //》代表开关灯（00关 01开）
 //》代表是否是彩灯模式（00 关 01 开）（注：彩灯循环，为1开启循环）
 //》代表灯的RGB值（000000 - FFFFFF）（彩灯循环这个值为000000）
 //》代表亮度( 00 - 03 )
 //》代表音量值 ( 00 - 0A )
 //》代表暂停pause选择（00表示正常播放,01表示暂停播放）
 //》代表定时器，00(关/不开启定时)，0F(15min)，1E(30min)，3C(60min)   
 //》英文分号“;”隔开这两部分命令
 //》闹钟控制信息：数组中第数据以逗号分隔，数字以 16 进制表示（这里不写"0x"），从左到右依次每个字段（10个字段）分别表示：
 //》代表 Alarm 的 ID 号 （注：现在只有一个闹钟，默认为0）
 //》代表第几首歌（01 - 06）
 //》代表是否是彩灯模式（00 关 01 开）（注：彩灯循环，为1开启循环）
 //》代表灯的RGB值（000000 - FFFFFF）（彩灯循环这个值为000000）
 //》代表亮度( 00 - 03 )
 //》代表音量值 ( 00 - 0a )
 //》代表闹钟总开关（00 关 01: 开）（默认0）
 //》代表周一到周日的开关，每一天用一个 bit 表示
 //  0x80 代表是否重复闹铃
 //  0x01 代表周一启动闹铃，但是只响一次，这一次是七天之内的，
 //  0xFF 代表闹铃全开，并且每周重复，以此类推
 //  BIT            7       6    5    4    3    2    1    0
 //  星期x   repeat  日  六  五  四  三  二  一  （注：repeat为每周都重复闹铃，否则只响一次，这一次
 //  是七天之内的，假如定时13:00，不重复，那这次计时到(7*24*60-1)min之后结束，即七天之后
 //  的12:59分）
 //》代表闹钟时间（17:3B）(都是16进制,:左边是闹钟的时，范围是00-17; :右边是闹钟的分，范围是00-3B)
 //》代表响铃时间（char类型1个字节）00-FF (单位:分钟)
	char i;
//	char code respGP389_AllSta[]={"$$${GpSta:[00,00,00,00,000000,00,00,00,00;00,00,00,000000,00,00,00,00,00:00,00]}\r\n"};
	char code respGP389_AllSta[]={"$$${GpSta:[00,00,00,00,000000,00,00,00,000;00,00,00,000000,]}\r\n"};//闹钟功能在001和003中屏蔽了
	do{WDT_clear();} while(Uart0_EnableSend);
	Count200ms_3Step=0;
	//Uart0_SendString_3Step=0;
	for(i=0;i<UART0_LEN_BUFFER;i++)
		Uart0_TransmitBuffer[i]=0x00;
	//for(i=0;i<82;i++)//闹钟功能在001和003中屏蔽了
	for(i=0;i<41;i++)
		Uart0_TransmitBuffer[i]=respGP389_AllSta[i];
	hex_to_char(&Uart0_TransmitBuffer[11],onoff);
	hex_to_char(&Uart0_TransmitBuffer[14],((PlayMode==PLAY_MUSIC)?spa_name:7));//首先判断是不是音乐模式，如果是，是第几首歌，如果不是就返回暂停，7
	hex_to_char(&Uart0_TransmitBuffer[17],((color_mode==COLOR_OFF)?0:1));//为1时是要关机，所以要和color_mode_bk对比，为0是要开机，已经把color_mode_bk赋值给color_mode
	hex_to_char(&Uart0_TransmitBuffer[20],((color_mode==COLOR_CHANGE)?1:0));
	hex_to_char(&Uart0_TransmitBuffer[23],userR_val);
	hex_to_char(&Uart0_TransmitBuffer[25],userG_val);	
	hex_to_char(&Uart0_TransmitBuffer[27],userB_val);
	hex_to_char(&Uart0_TransmitBuffer[30],Light_Brightness);
	hex_to_char(&Uart0_TransmitBuffer[33],sys_volume);
	hex_to_char(&Uart0_TransmitBuffer[36],enable_SPApause);
	hex_to_char(&Uart0_TransmitBuffer[39],cntTimer);
//	hex_to_char(&Uart0_TransmitBuffer[42],00);//闹钟ID号默认为0   //闹钟功能在001和003中屏蔽了
//	hex_to_char(&Uart0_TransmitBuffer[45],alarm.spa_name);//首先判断是不是音乐模式，如果是，是第几首歌，如果不是就返回暂停，7
//	hex_to_char(&Uart0_TransmitBuffer[48],((alarm.color_mode==COLOR_CHANGE)?1:0));//是否是彩灯模式（循环渐变模式）
//	hex_to_char(&Uart0_TransmitBuffer[51],alarm.userR_val);
//	hex_to_char(&Uart0_TransmitBuffer[53],alarm.userG_val);	
//	hex_to_char(&Uart0_TransmitBuffer[55],alarm.userB_val);
//	hex_to_char(&Uart0_TransmitBuffer[58],alarm.Brightness);//亮度
//	hex_to_char(&Uart0_TransmitBuffer[61],alarm.volume);//音量
//	hex_to_char(&Uart0_TransmitBuffer[64],alarm.Enable); //闹钟总开关
//	hex_to_char(&Uart0_TransmitBuffer[67],alarm.Week);  //闹钟周几响低4位
//	hex_to_char(&Uart0_TransmitBuffer[70],alarm.Hour);  //闹钟定时的时间，小时低四位
//	hex_to_char(&Uart0_TransmitBuffer[73],alarm.Minute);//闹钟定时的时间，分钟低四位
//	hex_to_char(&Uart0_TransmitBuffer[76],alarm.Duration);//闹钟响铃时间，低四位
	Uart0_SendString_3Step=2;
	Uart0_Tx_Pointer=0;
	Uart0_EnableSend=1;
	Uart0_Tx_TimeOut=10;	
	USI0DR=Uart0_TransmitBuffer[Uart0_Tx_Pointer++];
	if(cntFlag_ON_OFF==0)
		cntFlag_ON_OFF=2;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：串口接收到的命令协议和预定的协议对比确定是哪个进而执行操作，否则无操作
*******************************************************************/
void Wifi_CommandDeal(char *Uart0_Receive)
{
	uint8_t i;
	uint8_t temp_time;//临时用的变量，在接受定时时间哪里
	char *WIFI_CMD;
	WIFI_CMD=&Uart0_Receive[3];
//	if(char_compare(WIFI_CMD,"{AlarmSet")==0)//ok  //闹钟功能在001和003中屏蔽了
//	{//$$${AlarmSet:[00,01,00,01,FFFFFF,03,0A,00,00,FF,17:3B,FF,0a/20/00/12/01/01/02]}\r\n
//	 //数组中第数据以逗号分隔，数字以 16 进制表示（这里不写"0x"），从左到右依次每个字段（10个字段）分别表示：
//	 //》代表 Alarm 的 ID 号 （注：现在只有一个闹钟，默认为0）
//	 //》代表第几首歌（01 - 06）
//	 //》代表开关灯（00关 01开）
//	 //》代表是否是彩灯模式（00 关 01 开）（注：彩灯循环，为1开启循环,）
//	 //》代表灯的RGB值（000000 - FFFFFF）（彩灯循环这个值为000000）
//	 //》（去掉）代表灯颜色的下标，与app界面中灯一致，方便灯状态同步云端和app（00-0B）,其中00代表关灯，0A代表彩灯，0B代表自定义颜色
//	 //》代表亮度( 00 - 03 )
//	 //》代表音量值 ( 00 - 0a )
//	 //》代表暂停pause选择（00表示正常播放,01表示暂停播放）
//	 //》(去掉)：代表定时器，00(关/不开启定时)，0F(15min)，1E(30min)，3C(60min)   
//	 //》代表闹钟总开关（00 关 01: 开）（默认0）
//	 //》代表周一到周日的开关，每一天用一个 bit 表示
//	 //  0x80 代表是否重复闹铃
//	 //  0x01 代表周一启动闹铃，但是只响一次，这一次是七天之内的，
//	 //  0xFF 代表闹铃全开，并且每周重复，以此类推
//	 //  BIT      7       6    5    4    3    2    1    0
//	 //  星期x  repeat    日   六   五    四   三   二   一  （注：repeat为每周都重复闹铃，否则只响一次
//	 //》代表闹钟定时时间点（17:3B）(都是16进制,:左边是闹钟的时，范围是00-17; :右边是闹钟的分，范围是00-3B)
//	 //》代表响铃时间（int类型4个字节）00-FF (单位由强大定义)
//	 //》代表当前时间戳，分别按照十六进制数加斜杠表示，格式如下：（时/分/秒/年/月/日/星期）
//	 //  比如 2018 年 1 月 1 日 10 时 32 分 0 秒星期二 那么格式是 0a/20/00/12/01/01/02
//	 //(WIFI_CMD[11]-'0')  //Programma的ID号暂时没有，为以后扩展预备   
//		alarm.spa_name  =(WIFI_CMD[15]-'0');//第几首歌
//		if(alarm.spa_name>=7)
//			alarm.spa_name=0;
//		alarm.color_mode=(WIFI_CMD[18]-'0');//灯光的开关
//		alarm.color_mode=(WIFI_CMD[18]=='1')?((WIFI_CMD[21]=='1')?COLOR_CHANGE:COLOR_USER):COLOR_OFF;
//		alarm.userR_val=char_to_hex(WIFI_CMD[23],WIFI_CMD[24]);//R值  
//		alarm.userG_val=char_to_hex(WIFI_CMD[25],WIFI_CMD[26]);//G值  
//		alarm.userB_val=char_to_hex(WIFI_CMD[27],WIFI_CMD[28]);//B值  
//		alarm.Brightness=(WIFI_CMD[31]-'0');//亮度
//		alarm.volume=char_to_hex(WIFI_CMD[33],WIFI_CMD[34]);//音量
//		alarm.play_pause=WIFI_CMD[37]-'0';//暂停播放
//		alarm.Enable=(WIFI_CMD[40]-'0');//闹钟总开关
//		alarm.Week=char_to_hex(WIFI_CMD[42],WIFI_CMD[43]); 
//		alarm.Hour=char_to_hex(WIFI_CMD[45],WIFI_CMD[46]);   //闹钟响的小时点
//		alarm.Minute=char_to_hex(WIFI_CMD[48],WIFI_CMD[49]); 
//		alarm.Duration=char_to_hex(WIFI_CMD[51],WIFI_CMD[52]);//持续响铃时间，转化为数值
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
//	else if(char_compare(WIFI_CMD,"{AlarmOnOff")==0)  //闹钟功能在001和003中屏蔽了
//	{//$$${AlarmOnOff:[00,00,10/21/33/12/01/1D/01]}\r\n
//	 //》代表 Alarm 的 ID 号 
//	 //》代表闹钟的开关
//	 //》代表当前时间戳，分别按照十六进制数加斜杠表示，格式如下：（时/分/秒/年/月/日/星期）
//	 //	 比如 2018 年 1 月 1 日 10 时 32 分 0 秒星期二 那么格式是  0a/20/00/12/01/01/02
//	 //(WIFI_CMD[11]-'0')  //Programma的ID号暂时没有，为以后扩展预备   
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
//			if(alarm.Runing)//这个一定是开机状态才进去  //闹钟功能在001和003中屏蔽了
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
//			if(alarm.Runing) //闹钟功能在001和003中屏蔽了
//			{//这个一定是开机情况下进入的
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
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
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
//			if(alarm.Runing) //闹钟功能在001和003中屏蔽了
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			enableMute=0;//解mute
			enable_SPApause=0;
			PlayMode=PLAY_MUSIC;
			spa_name=WIFI_CMD[26]-'0';
			if(cur_spa_name==spa_name)
				spa_cmd=SPA_PALY;
			Uart0_SendString_3Step=1;
			Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);//每次更改音乐就发给app同步
			return;
		}
		else if(char_compare(WIFI_CMD, "music play")==0)
		{//$$$music play\r\n
			//Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
			PlayMode=PLAY_MUSIC;
			enableMute=0;     //解除静音
			enable_SPApause=0;//为0表示播放
			if(spa_name==SPA_OFF) //spa_name为枚举变量类型，SPA_ZEN为枚举变量元素
				spa_name=SPA_BROOK;
			else
				spa_cmd=SPA_PALY;			
			/*if(PlayMode==PLAY_BT)
			{
				if(flag_bt_play)
				{
					bt_cmd=BT_PALY;//协议里面是一条命令，所以播放/暂停都要发送这条
				}
			}*/
			return;
		}
		else if(char_compare(WIFI_CMD,"ProPlay\r\n")==0)//接收到就播放最后的接收到的$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n数据
		{//$$${Program
			Uart0Transmit_SendString(Uart0_Receive);
			if(PlayMode==PLAY_OFF)
				GP389_ON_APP();
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
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
			program1.enableMute=enableMute=((enable_SPApause==0)?0:1);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
			spa_cmd=((enable_SPApause==0)?SPA_PALY:SPA_PAUSE);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
			TimerMode=program1.TimerMode;
			return;
		} 
		else if (char_compare(WIFI_CMD, "Turn on Humidifier") == 0)
		{//$$$Turn on/off Humidifier\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if (PlayMode == PLAY_OFF)
				GP389_ON_APP();
			//			if(alarm.Runing) //闹钟功能在001和003中屏蔽了
			//			{
			//				alarm.Runing=0;
			//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
			//			}
			if(AtomizationMode1==ATOMIZATION_OFF) //这一条是怕在雾化器已经开启并且不再最小档位，然后又接收到开机命令被强行更改为最小二设立的
				AtomizationMode1 = ATOMIZATION_THREE;
			return;
		}
		else if (char_compare(WIFI_CMD, "Turn off Humidifier") == 0)
		{//$$$Turn on/off Humidifier\r\n
			Uart0Transmit_SendString(Uart0_Receive);
			if (PlayMode == PLAY_OFF)
				GP389_ON_APP();
			//			if(alarm.Runing) //闹钟功能在001和003中屏蔽了
			//			{
			//				alarm.Runing=0;
			//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
			//			}
			AtomizationMode1 = ATOMIZATION_OFF;
			return;
		}
//		else if(char_compare(WIFI_CMD,"{Program")==0)//关机状态下打开
//		{//$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n
//			Uart0Transmit_SendString(Uart0_Receive);
//			if(PlayMode==PLAY_OFF)
//				GP389_ON_APP();
//			if(alarm.Runing)
//			{
//				alarm.Runing=0;
//				TimerMode=cntTimer=alarm.cntTimer=TIMER_OFF; 
//			}
//			Program1_LaterNum(); //先把低数字赋给高数字，
//			program1.spa_name=spa_name=WIFI_CMD[14]-'0';
//			program1.color_mode=color_mode=((WIFI_CMD[17]=='1')?COLOR_WHITE:COLOR_OFF);//灯光的开关
//			program1.color_mode=color_mode=((WIFI_CMD[17]=='1')?((WIFI_CMD[20]=='1')?COLOR_CHANGE:COLOR_USER):COLOR_OFF);//先判断上一组是否开启灯光，如果开启灯光就再判断是否开启彩灯，如果不开启就是用户值																								   //用户值是下面RGB的值
//			program1.userR_val=userR_val=char_to_hex(WIFI_CMD[22],WIFI_CMD[23]);
//			program1.userG_val=userG_val=char_to_hex(WIFI_CMD[24],WIFI_CMD[25]);
//			program1.userB_val=userB_val=char_to_hex(WIFI_CMD[26],WIFI_CMD[27]);
//			program1.Light_Brightness=Light_Brightness=char_to_bcd(WIFI_CMD[29],WIFI_CMD[30]);
//			program1.sys_volume=sys_volume=char_to_hex(WIFI_CMD[32],WIFI_CMD[33]);//音量
//			program1.PlayMode=PlayMode=PLAY_MUSIC;
//			program1.enable_SPApause=enable_SPApause=WIFI_CMD[36]-'0';
//			program1.enableMute=enableMute=((enable_SPApause==0)?0:1);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
//			spa_cmd=((enable_SPApause==0)?SPA_PALY:SPA_PAUSE);//无论之前播放还是暂停，只要闹钟响应并且闹钟允许播放就播放
//			program1.TimerMode=TimerMode=cntTimer=char_to_hex(WIFI_CMD[38],WIFI_CMD[39]);
//			return;
//		}
		else if(char_compare(WIFI_CMD,"VER0.\r\n")==0) 
		{
			return;
		}
		else if(char_compare(WIFI_CMD,"VER\r\n")==0) 
		{
			Uart0Transmit_SendString(MCU_VER);//每次升级需要手动改写这里
			return;
		}
		else if(char_compare(WIFI_CMD,"GpSta")==0)
		{//最终状态是关机
			Uart0_SendString_3Step=0;
			cntFlag_ON_OFF=0;
			return;//PlayMode=PLAY_OFF;	因为有强制关机，所以可以不要GP389_OFF()/GP389_ON()
		}
//		else if(char_compare(WIFI_CMD,"GpSta:[00]\r\n")==0)
//		{//最终状态是开机，
//			Uart0_SendString_3Step=0;
//			cntFlag_ON_OFF=0;
//			return;//PlayMode=PLAY_OFF;	因为有强制关机，所以可以不要GP389_OFF()/GP389_ON()
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
			Falg_TimeSync_Allow=0;//接收到同步信息就不再发送请求同步，所以置为0
			return;
		}	
		else if(char_compare(WIFI_CMD, "Turn off unit")==0) 
		{//这条协议命令是app直接发送过来的关机命令，不用申请就可以关机，但是收到这条命令要判断
		 //此时不用进行判断是不是在关机状态，因为关机之后需要设定的状态对程序无影响，或者开机之
		 //后会初始化这些状态
		 //$$$Turn off unit\r\n
			Uart0Transmit_SendString(Uart0_Receive);
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
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
		{//$$$previous song\r\n 上一首歌  
			spa_cmd = SPA_PALY;
			if ((spa_name > SPA_OFF) && (spa_name <= SPA_ZEN))
				spa_name--;
			else
				if (spa_name == SPA_OFF)//形成循环
					spa_name = SPA_ZEN;
			Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);//每次更改音乐就发给app同步
			return;
		}
		else if (char_compare(WIFI_CMD, "next song\r\n") == 0)
		{//$$$next song\r\n 下一首歌
			spa_cmd = SPA_PALY;
			if ((spa_name >= SPA_OFF) && (spa_name < SPA_ZEN))
				spa_name++;
			else
				if (spa_name == SPA_ZEN) //形成循环
					spa_name = SPA_OFF;
			Uart0Transmit_SendString(&SPASongs_Num_Table[spa_name][0]);//每次更改音乐就发给app同步
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
//					bt_cmd=BT_PAUSE;//协议里面是一条命令，所以播放/暂停都要发送这条
//				}
//			}
			return;
		}
		else if(char_compare(WIFI_CMD,"{MusicStatus")==0)
		{
			return;
		}
		else if(char_compare(WIFI_CMD, "Adjust Output Volume")==0)
		{//$$$Adjust Output Volumexx\r\n，xx代表01-10
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			sys_volume=char_to_bcd(WIFI_CMD[20],WIFI_CMD[21]);
			enableMute=0;
		}
		else if(char_compare(WIFI_CMD, "volume up")==0) 
		{//$$$volume up\r\n
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			sys_volume=((sys_volume < 10)?((sys_volume<=8)?(sys_volume += 2):(sys_volume += 1)):10);
			enableMute=0;
		}
		else if(char_compare(WIFI_CMD, "volume down")==0) 
		{//volume down\r\n
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
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
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			temp_time=char_to_bcd(WIFI_CMD[10], WIFI_CMD[11]);//只检测前两个数字，再判断
			TimerMode=cntTimer= (temp_time == 90) ? 90 : ((temp_time == 60) ? 60 : ((temp_time == 30) ? 30 : 0));
		}
		else if(char_compare(WIFI_CMD, "Turn off Timer")==0)//ok
		{//$$$Turn off Timer\r\n
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			TimerMode=cntTimer=TIMER_OFF;
		}
		else if(char_compare(WIFI_CMD, "Turn off Lightr")==0)  
		{//$$$Turn off Lightr\r\n
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			color_mode=COLOR_OFF;
		} 
		else if(char_compare(WIFI_CMD,"light up")==0)
		{//$$$light up\r\n
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			if(Light_Brightness<3)
				Light_Brightness++;
		}
		else if(char_compare(WIFI_CMD,"light down")==0)
		{//$$$light down\r\n
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			if(Light_Brightness>0)
				Light_Brightness--;
		}
		else if(char_compare(WIFI_CMD,"Adjust Light Brightness")==0)
		{//$$$Adjust Light Brightness 01\r\n(01-03)
//			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
//			{
//				GP389_OFF_AND_AlarmOFF();
//				return;
//			}
			Light_Brightness=char_to_bcd(WIFI_CMD[24],WIFI_CMD[25]);
		}
		else if (char_compare(WIFI_CMD, "Adjust moist mode") == 0)
		{//$$$Adjust moist mode XX\r\n (01-03)
		 //			if(alarm.Runing)  //闹钟功能在001和003中屏蔽了
		 //			{
		 //				GP389_OFF_AND_AlarmOFF();
		 //				return;
		 //			}
			AtomizationMode1 = char_to_bcd(WIFI_CMD[18], WIFI_CMD[19]);
		}
//		else if(char_compare(WIFI_CMD,"AlaGpSta\r\n")==0) //闹钟功能在001和003中屏蔽了
//		{//W-G:"$$$AlaGpSta\r\n"
//		 //G-W:"$$${AlaGpSta:[00,01,01,00,FFFFFF,03,0A,00,01,FF,17:3B,FF]}\r\n"
//		 //数组中第数据以逗号分隔，数字以 16 进制表示（这里不写"0x"），从左到右依次每个字段（13个字段）分别表示：
//		 //》代表 Alarm 的 ID 号 （注：现在只有一个闹钟，默认为0）
//		 //》代表第几首歌（01 - 06）
//		 //》代表开关灯（00关 01开）
//		 //》代表是否是彩灯模式（00 关 01 开）（注：彩灯循环，为1开启循环）
//		 //》代表灯的RGB值（000000 - FFFFFF）（彩灯循环这个值为000000）
//		 //》代表亮度( 00 - 03 )
//		 //》代表音量值 ( 00 - 0a )
//		 //》代表暂停pause选择（00表示正常播放,01表示暂停播放）
//		 //（去掉）》代表定时器，00(关/不开启定时)，0F(15min)，1E(30min)，3C(60min)   
//		 //》代表闹钟总开关（00 关 01: 开）（默认0）
//		 //》代表周一到周日的开关，每一天用一个 bit 表示
//		 //    0x80 代表是否重复闹铃
//		 //    0x01 代表周一启动闹铃，但是只响一次，这一次是七天之内的，
//		 //    0xFF 代表闹铃全开，并且每周重复，以此类推
//		 //    BIT            7       6    5    4    3    2    1    0
//		 //    星期x   repeat  日  六  五  四  三  二  一  （注：repeat为每周都重复闹铃，否则只响一次，这一次
//		 //  是七天之内的，假如定时13:00，不重复，那这次计时到(7*24*60-1)min之后结束，即七天之后
//		 //  的12:59分）
//		 //》代表闹钟定时时间点（17:3B）(都是16进制,:左边是闹钟的时，范围是00-17; :右边是闹钟的分，范围是00-3B)
//		 //》代表响铃时间（char类型1个字节）00-FF (单位:分钟)
//		 //（去掉）》代表当前时间戳，分别按照十六进制数加斜杠表示，格式如下：（时/分/秒/年/月/日/星期） 
//		 //    比如 2018 年 1 月 1 日 10 时 32 分 0 秒星期二 那么格式是 0a/20/00/12/01/01/02
//			char  code respAlaGpSta[]={"$$${AlaGpSta:[00,00,00,00,000000,00,00,00,00,00,00:00,00]}\r\n"};
//			for(i=0;i<UART0_LEN_BUFFER;i++)
//				Uart0_Receive[i]=0x00;
//			for(i=0;i<62;i++)
//				Uart0_Receive[i]=respAlaGpSta[i];
//			hex_to_char(&Uart0_Receive[14],0);//ID号默认为0
//			hex_to_char(&Uart0_Receive[17],((alarm.spa_name!=0)?alarm.spa_name:7));//首先判断是不是音乐模式，如果是，是第几首歌，如果不是就返回暂停，7
//			hex_to_char(&Uart0_Receive[20],((alarm.color_mode==COLOR_OFF)?0:1));//开关灯
//			hex_to_char(&Uart0_Receive[23],(((alarm.color_mode==COLOR_CHANGE)?1:0)));//是否是彩灯模式（循环渐变模式）
//			hex_to_char(&Uart0_Receive[26],alarm.userR_val);
//			hex_to_char(&Uart0_Receive[28],alarm.userG_val);
//			hex_to_char(&Uart0_Receive[30],alarm.userB_val);
//			hex_to_char(&Uart0_Receive[33],alarm.Brightness);//亮度
//			hex_to_char(&Uart0_Receive[36],alarm.volume);//音量
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
			hex_to_char(&Uart0_Receive[14],1);//在这里是开机
			hex_to_char(&Uart0_Receive[17],((PlayMode==PLAY_MUSIC)?spa_name:7));//首先判断是不是音乐模式，如果是，是第几首歌，如果不是就返回暂停，7
			hex_to_char(&Uart0_Receive[20],(color_mode==COLOR_OFF)?0:1);
			hex_to_char(&Uart0_Receive[23],(color_mode==COLOR_CHANGE)?0:1);//是否是彩灯模式（循环渐变模式）
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
		{//mcu update+0X43A4+0X0044,接收到这个信息开始升级
			Update_ByteSizeSum=(char_to_hex(WIFI_CMD[10],WIFI_CMD[11])<<8)|char_to_hex(WIFI_CMD[12],WIFI_CMD[13]);//多少个字节，地位在前，高位在后
			Update_PacketSum  =(char_to_hex(WIFI_CMD[17],WIFI_CMD[18])<<8)|char_to_hex(WIFI_CMD[19],WIFI_CMD[20]);//多少个包
			update_handle();
		}
		else if(char_compare(WIFI_CMD,"ATWQ\r\n")==0) 
		{//$$$ATWQ\r\n
			Uart0_SendString_3Step=0;
			return;
		}
		else if(char_compare(WIFI_CMD,"VER\r\n")==0) 
		{
			Uart0Transmit_SendString(MCU_VER);//每次升级需要手动改写这里
			return;
		}
		else if(char_compare(WIFI_CMD,"Enter TestMode:")==0) 
		{
			Uart0_SendString_3Step=0;
			cntFlash=0;
			Led_Flash=0;
			setFactory=1;//置一让三个timer灯常亮三秒
			PlayMode=PLAY_ON;
			bt_send_cmd(BT_CLEAR_LIST);//清除蓝牙列表列表
			return;
		}
		Uart0Transmit_SendString(Uart0_Receive);
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
	}
	else //关机状态下接收特定wifi数据
	{
		if(char_compare(WIFI_CMD,"{NorGpSta")==0)
		{//W-G:"$$$NorGpSta\r\n"
		 //G-W:"$$${NorGpSta:[00,00,01,01,FFFFFF,03,0A,00,1E]}\r\n"
			char  code respNorGpSta[]={"$$${NorGpSta:[00,00,00,00,000000,00,00,00,00]}\r\n"};
			for(i=0;i<UART0_LEN_BUFFER;i++)
				Uart0_Receive[i]=0x00;
			for(i=0;i<48;i++)
				Uart0_Receive[i]=respNorGpSta[i];
			hex_to_char(&Uart0_Receive[14],0);//在这里是开机
			hex_to_char(&Uart0_Receive[17],((PlayMode==PLAY_MUSIC)?spa_name:7));//首先判断是不是音乐模式，如果是，是第几首歌，如果不是就返回暂停，7
			hex_to_char(&Uart0_Receive[20],(color_mode==COLOR_OFF)?0:1);
			hex_to_char(&Uart0_Receive[23],(color_mode==COLOR_CHANGE)?0:1);//是否是彩灯模式（循环渐变模式）
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
函数原型：
输入参数：
输出参数：
函数功能：在app上发送“状态“功能给机台，状态功能是同时设置好几个操作（音乐，灯光等），然后一起发过来
          然后机台记录下来第几次发的，把新的一次复制给program1，上一次的program1复制给program2，因为
		  数据和数据类型都一样，所以可以直接复制
*******************************************************************/
//void Program1_LaterNum()
//{
//	program3=program2;//因为数据和数据类型完全一样，所以可以直接赋值
//	program2=program1;
//}




