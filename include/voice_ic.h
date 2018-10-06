#ifndef __VOICE_IC_H__
#define __VOICE_IC_H__

//�궨��macro definition//
#define uint8_t  unsigned char 
#define uint16_t unsigned int 	

//���Ͷ���byte definition//
//typedef enum
//{
//	SPA_OFF        =0X00,
//    SPA_BROOK      =0X01, //СϪ
//    SPA_CAMPFIRE   =0X02, //����
//    SPA_CANONInD   =0X03, //��ũD���
//    SPA_FAN        =0X04, //��
//    SPA_OUTDOOR    =0X05, //Ұ��
//    SPA_ZEN        =0X06, //��
//	SPA_VOL0       =0X07, //����0
//    SPA_VOL15      =0X16, //����15
//	SPA_PAUSE      =0X17, //��ͣ
//    SPA_PALY       =0X18, //����
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
	SPA_PAUSE      =0XF0, //ֹͣ��
	
	SPA_VOL0       =0X07, //����0
    SPA_VOL15      =0X16, //����15
	//SPA_PAUSE      =0X17, //��ͣ
    SPA_PALY       =0X18, //����
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
extern uint8_t  idata voice_send_step; //�������Ͳ���
extern uint8_t  idata voice_send_data; //������������
extern uint16_t idata voice_50us_cnt; //50us��ʱ
extern uint16_t idata voice_50us_cnt_set; //50us��ʱ���
extern uint8_t  idata spa_cmd; //spa����
extern uint8_t  idata spa_volume; //����
extern uint8_t  idata cur_spa_name;
extern uint8_t  idata cntSPAOn;

//�ⲿ����_��־λ����flags definetion//
extern bit           gbVoice_Work;
extern bit           flag_voice_send; //�������ͱ�־
extern bit           enableMute; //voice��������ʱ����ͣ���ű�־λ��ͨ���ж������־ȥִ������
extern bit		     enableMute_bk;
extern bit           enable_SPApause; //0��ʾ����

//�ⲿ����_��������function definetion//
extern void set_voice_vol(uint8_t vol); 
extern void voice_send(uint8_t key);
extern void voice_in_timer(void);
extern void DealWith_Voice(void); 

#endif




