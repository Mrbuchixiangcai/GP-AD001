#ifndef __RGB_H__
#define __RGB_H__

//宏定义macro definition//

//类型定义byte definition//
typedef enum
{
	COLOR_OFF    =0X00,	
	COLOR_WHITE       ,
	COLOR_RED         ,
	COLOR_ORANGE      ,
	COLOR_YELLOW      ,
	COLOR_GREEN       ,	
	COLOR_CYAN        ,
	COLOR_BLUE        ,
	COLOR_PURPLE      ,
	COLOR_CHANGE      ,
	COLOR_USER        ,
}COLOR_MODE;

//变量定义variable definition//

//标志位定义flags definetion//

//函数定义function definetion//
void led_rgb_drive(void);

//外部调用_宏定义macro definition//

//外部调用_类型定义byte definition//
extern COLOR_MODE color_mode;
extern COLOR_MODE alarm_color_mode;//闹钟专用
//外部调用_变量定义variable definition//
extern uint8_t R_val;
extern uint8_t G_val;
extern uint8_t B_val;
extern uint8_t  curR_val;
extern uint8_t  curG_val;
extern uint8_t  curB_val;
extern uint8_t  sumRGB_val;
extern uint8_t  userR_val;
extern uint8_t  userG_val;
extern uint8_t  userB_val;
extern uint8_t  Light_Brightness;
extern uint8_t  play_rgb_index;
extern uint8_t  play_rgb_speed;	
extern uint16_t cnt_rgb_pause;

//外部调用_标志位定义flags definetion//
extern uint8_t  tickRGB;

//外部调用_函数定义function definetion//
extern void RGB_Handle(void);

#endif





