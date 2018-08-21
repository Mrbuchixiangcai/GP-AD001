#ifndef __PWM_H__  //PWM:PWM产生的雾化
#define __PWM_H__

//头文件header file//
#include "app_main.h"

//宏定义macro definition//
#define MOTOR(a)            \
        if (a)              \
            P4 |= 0x08;     \
        else                \
            P4 &= ~0x08;
//类型定义byte definition//

//变量定义variable definition//

//标志位定义flags definetion//

//函数声明Function declaration//

//函数定义function definetion//

//外部调用_宏定义macro definition//

//外部调用_类型定义byte definition//

//外部调用_变量定义variable definition//

//外部调用_标志位定义flags definetion//

//外部调用_函数定义function definetion//
void PWM_Open();
void PWM_Shutdown();
void PWM_DutyCycle_Change();


#endif



