#ifndef _KEY_H_
#define _KEY_H_

//�궨��macro definition//
#define	 K_D  		(1<<5) //��������û�б���������Ϊ�����ǲɼ���ѹ��ʽ���ڲɼ���ѹʱ��ȷ���˿�
#define	 K_U  		(2<<5) //�̰�
#define	 K_H  		(3<<5) //�������� hold
#define	 K_R  		(4<<5) //����
#define	 K_L  		(5<<5) //����
#define	 K_LU  		(6<<5) //������
#define	 KD(x)  	(x|K_D)
#define	 KU(x)  	(x|K_U)
#define	 KH(x)  	(x|K_H)
#define	 KR(x)  	(x|K_R)
#define	 KL(x)  	(x|K_L)
#define	 KLU(x)  	(x|K_LU)
#define  cKEY_CLICK     5   //����ʱ�䳣��
#define  cKEY_HOLD      50  //��סʱ�䳣��
#define  cKEY_RATE_VOL  20  //����ʱ�䳣��
#define  KeyVolAdd()  (P1&0x20)  //P1.5 �����Ӽ�

//���Ͷ���byte definition//
enum{
    T_NC    ,
    T_POWER ,
    T_MUSIC , //SOUND��
    T_VOLINC, //volume increasing������
    T_VOLDEC, //volume decline������
    T_TIMER ,	
    T_LIGHT ,
    T_BT    ,
    T_DEFAULT,//ͬʱ��������+��������
	T_CLEAR ,
	T_TESTMODE,//ͬ�°��¡�����+���͡�SOUND�����������ģʽ
};
extern uint8_t	gbKeyNone;
extern uint8_t  gbKeyPress;
extern uint8_t  KeyValue;
extern void  KeyScan(void);
extern void  KeyComMsg(void);

#endif





