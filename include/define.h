#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "app_main.h"

//宏定义macro definition//
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

//类型定义byte definition//
typedef enum { PLAY_OFF = 0, PLAY_ON, PLAY_BT, PLAY_MUSIC } PLAY_MODE;
typedef enum {
  TIMER_OFF = 0,
  TIMER_15MIN = 15,
  TIMER_30MIN = 30,
  TIMER_45MIN = 45,
  TIMER_60MIN = 60,
} TIMER_MODE;

typedef struct {
  uint8_t Enable; //闹钟总开关
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

//变量定义variable definition//

//标志位定义flags definetion//

//函数定义function definetion//

//外部调用_宏定义macro definition//

//外部调用_类型定义byte definition//
extern TIMER_MODE TimerMode;
extern PLAY_MODE PlayMode;
extern ALRAM_TypeDef alarm;
extern PROGRAM_TypeDef program1; //在接收到“program Play（暂定）”时，播放最后一次接收到的“$$${Program:[00,01,01,00,FFFFFF,03,0A,00,1E]}\r\n”数据
extern PROGRAM_TypeDef program2;
extern PROGRAM_TypeDef program3;
//外部调用_变量定义variable definition//
extern uint8_t idata cntAppTick;
extern uint8_t idata gRTC_Sec; // RTC数据
extern uint8_t idata gRTC_Sec_bk;
extern uint8_t idata gRTC_Hour;
extern uint8_t idata gRTC_Hour_bk;
extern uint8_t xdata gRTC_Hour_bk_24; //计数24小时
extern uint8_t idata gRTC_Minute;
extern uint8_t idata gRTC_Minute_bk;
extern uint8_t idata gRTC_Week; //周几
extern uint8_t idata cntTimer;
extern uint8_t idata sys_volume;
extern uint8_t idata sys_volume_bk; //在不断电情况下开关机保存音量值   //
extern uint8_t idata color_mode_bk; //在不断电情况下开关机保存颜色模式 //
extern uint8_t idata PlayMode_bk; //在不断电情况下开关机保存开机状态模式 //
extern uint8_t xdata timeSync_PowerON_Cnt; //上电10s请求同步时间
// uint8_t idata
// data_bk[64];//设置了看门狗，在遇到意外重启之后会从0x0000启动，之前的数据都会清除
//所以用这个数组在一些数据变动之后5s内	进行备份，如果数据不变动就不备份
// uint8_t
// data_bk_Cnt;//设置了看门狗，在遇到意外重启之后会从0x0000启动，之前的数据都会清除
//所以用这个数组在一些数据变动之后5s内	进行备份，如果数据不变动就不备份
//此变量是进行计数5s的
extern uint8_t cntFlag_ON_OFF;

//标志位定义flags definetion//
extern bit AppTick0;
extern bit AppTick1;
extern bit AppTick2;
extern bit AppTick3;
extern bit AppTick4;
extern bit AppTick5;
extern bit AppTick1ms;
extern bit gbHalfSecond;
extern bit enableMute; //为1就整体静音，为0就解除静音
extern bit enableMute_bk;
extern bit setReset;
extern bit setFactory;
extern bit Flag_Demo_Mode;
extern bit Flag_TestMode;
extern uint8_t Falg_TimeSync_Allow; //时间允许标志为，如果为一就一直两分钟请求同步，
extern bit Flag_data_bk; //设置了看门狗，在遇到意外重启之后会从0x0000启动，之前的数据都会清除
                  //所以用这个数组在一些数据变动之后5s内
                  //进行备份，如果数据不变动就不备份 变动时把此标志位置一
extern bit Flag_alarm_say_to_IOT; //闹钟响应时应为要连续发送两条语句各3次，所以第二条要放在uart里面
extern bit Flag_time_sync; //时间请求同步时应为要发送语句3次，此时有可能和其他的要送发送的
                    //冲突，所以第二条要放在uart里面
extern bit Flag_alarm_wake_only;

extern uint8_t cntFlash;
extern uint8_t Led_Flash;

//外部调用_函数定义function definetion//
extern void sys_tick(void);
extern void Compare_1MinutePorc(void);
extern void app_main(void);

#endif








