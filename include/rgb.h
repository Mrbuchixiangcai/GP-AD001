#ifndef __RGB_H__
#define __RGB_H__

//�궨��macro definition//

//���Ͷ���byte definition//
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

//��������variable definition//

//��־λ����flags definetion//

//��������function definetion//
void led_rgb_drive(void);

//�ⲿ����_�궨��macro definition//

//�ⲿ����_���Ͷ���byte definition//
extern COLOR_MODE color_mode;
extern COLOR_MODE alarm_color_mode;//����ר��
//�ⲿ����_��������variable definition//
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

//�ⲿ����_��־λ����flags definetion//
extern uint8_t  tickRGB;

//�ⲿ����_��������function definetion//
extern void RGB_Handle(void);

#endif





