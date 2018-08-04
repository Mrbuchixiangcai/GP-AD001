#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "app_main.h"

//�궨��macro definition//
#define MCU_VER "$$$VER0.45\r\n"
#define Key_MCU_VER "$$$Enter TestMode:0.45\r\n"
#define LED_BLUE(a)         \
        if (a)              \
            P3 |= 0x20;     \
        else                \
            P3 &= ~0x20;

#define LED_GREEN(a)        \
        if (a)              \
            P3 |= 0x10;     \
        else                \
            P3 &= ~0x10;

#define LED_RED(a)          \
        if (a)              \
            P3 |= 0x08;     \
        else                \
            P3 &= ~0x08;

#define LED_15MIN(a)        \
        if (a)              \
            P0 |= 0x10;     \
        else                \
            P0 &= ~0x10

#define LED_30MIN(a)        \
        if (a)              \
            P0 |= 0x20;     \
        else                \
            P0 &= ~0x20

#define LED_60MIN(a)        \
        if (a)              \
            P0 |= 0x40;     \
        else                \
            P0 &= ~0x40

#define VOIC_DATA(a)        \
        if (a)              \
            P1 |= 0x80;     \
        else                \
            P1 &= ~0x80

#define VOIC_BUSY() (P1 & 0x40)
#define CHECK_DC5V() (P3 & 0x01)
#define BT_LED_DET() (P1 & 0x02)
#define BT_MUTE_DET() (P1 & 0x04)
#define EN_MUTE()           \
        do {                \
            P2 &= ~0x10;    \
        } while (0)
#define DE_MUTE()           \
        do {                \
            P2 |= 0x10;     \
        } while (0)
#define SET_AUCH()          \
        do {                \
            P2 |= 0x02;     \
        } while (0)
#define CLR_AUCH()          \
        do {                \
            P2 &= ~0x02;    \
        } while (0)

#define SET_BT_LED()        \
        do {                \
            P1 |= 0x10;     \
        } while (0)
#define CLR_BT_LED()        \ 
        do {                \
            P1 &= ~0x10;    \
        } while (0)

#define SET_BT_POWER()      \
        do {                \
            P2 |= 0x40;     \
        } while (0)
#define CLR_BT_POWER()      \
        do {                \
            P2 &= ~0x40;    \
        } while (0)
#define SET_VOICE_POWER()   \
        do {                \
            P3 |= 0x02;     \
        } while (0)
#define CLR_VOICE_POWER()   \
        do {                \
            P3 &= ~0x02;    \
        } while (0)

//���Ͷ���byte definition//
typedef enum { PLAY_OFF = 0, PLAY_ON, PLAY_BT, PLAY_MUSIC } PLAY_MODE;
typedef enum {
  TIMER_OFF = 0,
  TIMER_15MIN = 15,
  TIMER_30MIN = 30,
  TIMER_45MIN = 45,
  TIMER_60MIN = 60,
} TIMER_MODE;

typedef struct {
  uint8_t Enable; //�����ܿ���
  uint8_t Week;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t userR_val;
  uint8_t userG_val;
  uint8_t userB_val;
  uint8_t spa_name;
  uint8_t volume;
  uint8_t play_pause;
  uint8_t color_mode;
  uint8_t Brightness;
  uint8_t Duration;
  uint8_t Runing;
  uint8_t cntTimer;
} ALRAM_TypeDef;

typedef struct {
  uint8_t spa_name;
  uint8_t color_mode;
  uint8_t userR_val;
  uint8_t userG_val;
  uint8_t userB_val;
  uint8_t Light_Brightness;
  uint8_t sys_volume;
  uint8_t PlayMode;
  uint8_t enable_SPApause;
  uint8_t enableMute;
  uint8_t TimerMode;
} PROGRAM_TypeDef;

//��������variable definition//

//��־λ����flags definetion//

//��������function definetion//

//�ⲿ����_�궨��macro definition//

//�ⲿ����_���Ͷ���byte definition//
extern TIMER_MODE TimerMode;
extern PLAY_MODE PlayMode;
extern ALRAM_TypeDef alarm;
extern PROGRAM_TypeDef program1; //�ڽ��յ���program Play���ݶ�����ʱ���������һ�ν��յ��ġ�$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n������
extern PROGRAM_TypeDef program2;
extern PROGRAM_TypeDef program3;
//�ⲿ����_��������variable definition//
extern uint8_t idata cntAppTick;
extern uint8_t idata gRTC_Sec; // RTC����
extern uint8_t idata gRTC_Sec_bk;
extern uint8_t idata gRTC_Hour;
extern uint8_t idata gRTC_Hour_bk;
extern uint8_t xdata gRTC_Hour_bk_24; //����24Сʱ
extern uint8_t idata gRTC_Minute;
extern uint8_t idata gRTC_Minute_bk;
extern uint8_t idata gRTC_Week; //�ܼ�
extern uint8_t idata cntTimer;
extern uint8_t idata sys_volume;
extern uint8_t idata sys_volume_bk; //�ڲ��ϵ�����¿��ػ���������ֵ   //
extern uint8_t idata color_mode_bk; //�ڲ��ϵ�����¿��ػ�������ɫģʽ //
extern uint8_t idata PlayMode_bk; //�ڲ��ϵ�����¿��ػ����濪��״̬ģʽ //
extern uint8_t xdata timeSync_PowerON_Cnt; //�ϵ�10s����ͬ��ʱ��
// uint8_t idata
// data_bk[64];//�����˿��Ź�����������������֮����0x0000������֮ǰ�����ݶ������
//���������������һЩ���ݱ䶯֮��5s��	���б��ݣ�������ݲ��䶯�Ͳ�����
// uint8_t
// data_bk_Cnt;//�����˿��Ź�����������������֮����0x0000������֮ǰ�����ݶ������
//���������������һЩ���ݱ䶯֮��5s��	���б��ݣ�������ݲ��䶯�Ͳ�����
//�˱����ǽ��м���5s��
extern uint8_t cntFlag_ON_OFF;

//��־λ����flags definetion//
extern bit AppTick0;
extern bit AppTick1;
extern bit AppTick2;
extern bit AppTick3;
extern bit AppTick4;
extern bit AppTick5;
extern bit AppTick1ms;
extern bit gbHalfSecond;
extern bit enableMute; //Ϊ1�����徲����Ϊ0�ͽ������
extern bit enableMute_bk;
extern bit setReset;
extern bit setFactory;
extern bit Flag_Demo_Mode;
extern bit Flag_TestMode;
extern uint8_t Falg_TimeSync_Allow; //ʱ�������־Ϊ�����Ϊһ��һֱ����������ͬ����
extern bit Flag_data_bk; //�����˿��Ź�����������������֮����0x0000������֮ǰ�����ݶ������
                  //���������������һЩ���ݱ䶯֮��5s��
                  //���б��ݣ�������ݲ��䶯�Ͳ����� �䶯ʱ�Ѵ˱�־λ��һ
extern bit Flag_alarm_say_to_IOT; //������ӦʱӦΪҪ����������������3�Σ����Եڶ���Ҫ����uart����
extern bit Flag_time_sync; //ʱ������ͬ��ʱӦΪҪ�������3�Σ���ʱ�п��ܺ�������Ҫ�ͷ��͵�
                    //��ͻ�����Եڶ���Ҫ����uart����
extern bit Flag_alarm_wake_only;

extern uint8_t cntFlash;
extern uint8_t Led_Flash;

//�ⲿ����_��������function definetion//
extern void sys_tick(void);
extern void Compare_1MinutePorc(void);
extern void app_main(void);

#endif








