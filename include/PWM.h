#ifndef __PWM_H__  //PWM:PWM��������
#define __PWM_H__

//ͷ�ļ�header file//
#include "app_main.h"

//�궨��macro definition//
#define MOTOR(a)            \
        if (a)              \
            P4 |= 0x08;     \
        else                \
            P4 &= ~0x08;
//���Ͷ���byte definition//

//��������variable definition//

//��־λ����flags definetion//

//��������Function declaration//

//��������function definetion//

//�ⲿ����_�궨��macro definition//

//�ⲿ����_���Ͷ���byte definition//

//�ⲿ����_��������variable definition//

//�ⲿ����_��־λ����flags definetion//

//�ⲿ����_��������function definetion//
void PWM_Open();
void PWM_Shutdown();
void PWM_DutyCycle_Change();


#endif



