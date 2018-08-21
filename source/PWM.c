#include "app_main.h"

//宏定义macro definition//

//类型定义byte definition//

//变量定义variable definition//

//标志未定义flags definetion//

//函数声明Function declaration//

//函数定义function definetion//
/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：开启PWM
*******************************************************************/
void PWM_Open()//开启PWM
{
	T4PCR2 |= 0x08; //允许输出PWM
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：关闭PWM
*******************************************************************/
void PWM_Shutdown()
{
	T4PCR2 &= ~0x08; //禁止输出PWM
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：PWM占空比改变,因为是高电平启动(禁止时低电平，要这个低电平)，所以占空比要反着
*******************************************************************/
void PWM_DutyCycle_Change()
{
	T4ADRL = PWM_REVERSE - 0xE0;  	// duty Low   占空比：
	T4ADRH = PWM_REVERSE - 0x06;  	// duty High
	MOTOR(1);
}

//void PWM_DutyCycle_Change()
//{
//	if ((AtomizationMode1 == ATOMIZATION_OFF) || (Flas_Anhydrous))
//	{
//		AtomizationMode1 == ATOMIZATION_OFF
//		T4ADRL = PWM_REVERSE - 0x00;// duty Low   占空比：
//		T4ADRH = PWM_REVERSE - 0x00;// duty High
//		T4PCR2 &= ~0x08;			//禁止输出PWM
//		return;
//	}
//	else if (AtomizationMode1 == ATOMIZATION_ONE)
//	{
//		T4ADRL = PWM_REVERSE - 0xFF;// duty Low   占空比：100%
//		T4ADRH = PWM_REVERSE - 0xFF;// duty High
//	}
//	else if (AtomizationMode1 == ATOMIZATION_TWO)
//	{
//		T4ADRL = PWM_REVERSE - 0xED;// duty Low   占空比：75%
//		T4ADRH = PWM_REVERSE - 0x02;// duty High
//	}
//	else if (AtomizationMode1 == ATOMIZATION_THREE)
//	{
//		T4ADRL = PWM_REVERSE - 0xF3;// duty Low   占空比：50%
//		T4ADRH = PWM_REVERSE - 0x01;// duty High
//	}
//	MOTOR(AtomizationMode1);//风扇停或者开看Atomization的枚举值
//	
//}



