#include "app_main.h"

//�궨��macro definition//

//���Ͷ���byte definition//

//��������variable definition//

//��־δ����flags definetion//

//��������Function declaration//

//��������function definetion//
/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�����PWM
*******************************************************************/
void PWM_Open()//����PWM
{
	T4PCR2 |= 0x08; //�������PWM
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ��ر�PWM
*******************************************************************/
void PWM_Shutdown()
{
	T4PCR2 &= ~0x08; //��ֹ���PWM
}

/*******************************************************************
����ԭ�ͣ�
���������
���������
�������ܣ�PWMռ�ձȸı�,��Ϊ�Ǹߵ�ƽ����(��ֹʱ�͵�ƽ��Ҫ����͵�ƽ)������ռ�ձ�Ҫ����
*******************************************************************/
void PWM_DutyCycle_Change()
{
	T4ADRL = PWM_REVERSE - 0xE0;  	// duty Low   ռ�ձȣ�
	T4ADRH = PWM_REVERSE - 0x06;  	// duty High
	MOTOR(1);
}

//void PWM_DutyCycle_Change()
//{
//	if ((AtomizationMode1 == ATOMIZATION_OFF) || (Flas_Anhydrous))
//	{
//		AtomizationMode1 == ATOMIZATION_OFF
//		T4ADRL = PWM_REVERSE - 0x00;// duty Low   ռ�ձȣ�
//		T4ADRH = PWM_REVERSE - 0x00;// duty High
//		T4PCR2 &= ~0x08;			//��ֹ���PWM
//		return;
//	}
//	else if (AtomizationMode1 == ATOMIZATION_ONE)
//	{
//		T4ADRL = PWM_REVERSE - 0xFF;// duty Low   ռ�ձȣ�100%
//		T4ADRH = PWM_REVERSE - 0xFF;// duty High
//	}
//	else if (AtomizationMode1 == ATOMIZATION_TWO)
//	{
//		T4ADRL = PWM_REVERSE - 0xED;// duty Low   ռ�ձȣ�75%
//		T4ADRH = PWM_REVERSE - 0x02;// duty High
//	}
//	else if (AtomizationMode1 == ATOMIZATION_THREE)
//	{
//		T4ADRL = PWM_REVERSE - 0xF3;// duty Low   ռ�ձȣ�50%
//		T4ADRH = PWM_REVERSE - 0x01;// duty High
//	}
//	MOTOR(AtomizationMode1);//����ͣ���߿���Atomization��ö��ֵ
//	
//}



