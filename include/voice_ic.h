#ifndef __VOICE_IC_H__
#define __VOICE_IC_H__

//宏定义macro definition//
#define uint8_t  unsigned char 
#define uint16_t unsigned int 	

//类型定义byte definition//
//typedef enum
//{
//	SPA_OFF        =0X00,
//    SPA_BROOK      =0X01, //小溪
//    SPA_CAMPFIRE   =0X02, //篝火
//    SPA_CANONInD   =0X03, //卡农D大调
//    SPA_FAN        =0X04, //风
//    SPA_OUTDOOR    =0X05, //野外
//    SPA_ZEN        =0X06, //禅
//	SPA_VOL0       =0X07, //音量0
//    SPA_VOL15      =0X16, //音量15
//	SPA_PAUSE      =0X17, //暂停
//    SPA_PALY       =0X18, //播放
//}SPA_NAME;
typedef enum
{
	SPA_OFF        =0X00,
	SPA_SONG1      =0X01,
	SPA_SONG2      =0X02,
	SPA_SONG3      =0X03,
	SPA_SONG4      =0X04,
	SPA_SONG5      =0X05,
	SPA_SONG6      =0X06,
	SPA_SONG7      =0X07,
	SPA_SONG8      =0X08,
	SPA_SONG9      =0X09,
	SPA_SONG10     =0X0A,
	SPA_SONG11     =0X0B,
	SPA_SONG12     =0X0C,
	SPA_PAUSE      =0XF0, //停止码
	
	SPA_VOL0       =0X07, //音量0
    SPA_VOL15      =0X16, //音量15
	//SPA_PAUSE      =0X17, //暂停
    SPA_PALY       =0X18, //播放
}SPA_NAME;

typedef enum
{
    VOICE_STEP_START =0,
    VOICE_STEP_INITI1  ,
    VOICE_STEP_INITI2  ,
    VOICE_STEP_INITI3  ,
    VOICE_STEP_INITI4  ,
}VOICE_STEP;


extern VOICE_STEP  Voice_Step;
extern SPA_NAME spa_name;
extern uint8_t  idata voice_send_step; //语音发送步骤
extern uint8_t  idata voice_send_data; //语音发送数据
extern uint16_t idata voice_50us_cnt; //50us计时
extern uint16_t idata voice_50us_cnt_set; //50us计时设计
extern uint8_t  idata spa_cmd; //spa命令
extern uint8_t  idata spa_volume; //音量
extern uint8_t  idata cur_spa_name;
extern uint8_t  idata cntSPAOn;

//外部调用_标志位定义flags definetion//
extern bit           gbVoice_Work;
extern bit           flag_voice_send; //语音发送标志
extern bit           enableMute; //voice语音播放时的暂停播放标志位，通过判断这个标志去执行命令
extern bit		     enableMute_bk;
extern bit           enable_SPApause; //0表示播放

//外部调用_函数定义function definetion//
extern void set_voice_vol(uint8_t vol); 
extern void voice_send(uint8_t key);
extern void voice_in_timer(void);
extern void DealWith_Voice(void); 

#endif




