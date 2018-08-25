//======================================================
// Main program routine
// - Device name  : MC96F6432
// - Package type : 44MQFP
//======================================================
// For XDATA variable : V1.041.00 ~
#define		MAIN	1

// Generated    : Sat, Jan 06, 2018 (12:08:12)
#include "app_main.h"
#include "MC96F6432.h"
#include "func_def.h"


/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void main()
{
	while(1)
	{
		cli();          	// disable INT. during peripheral setting
		port_init();    	// initialize ports
		clock_init();   	// initialize operation clock
		ADC_init();     	// initialize A/D convertor
		LCD_init();     	// initialize LCD
		Timer0_init();  	// initialize Timer0
		Timer2_init();  	// initialize Timer2
		Timer3_init();  	// initialize Timer3
		Timer4_init();  	// initialize Timer4
		UART0_init();    	// initialize UART interface
		WT_init();      	// initialize Watch timer
		WDT_init();
		sei();          	// enable INT.	
		app_main();
		BootLoader_handle();
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_USI1_Rx() interrupt 3
{
	uint8_t tmp;
	tmp=USI1DR;	
	USI1ST1&=~0x20;
	Uart1_Rx_TimeOut=10;
	if(Uart1_ReceiveBuffer_A_B)
	{
		if(Uart1_Rx_Pointer<UART1_LEN_BUFFER)	
			Uart1_ReceiveBuffer_A[Uart1_Rx_Pointer++]=tmp;
		if(tmp=='\n')
		{
			Uart1_Rx_Pointer =0;
			Uart1_RX_Finish_A=1;
			Uart1_ReceiveBuffer_A_B=~Uart1_ReceiveBuffer_A_B;
		}
	}
	else
	{
		if(Uart1_Rx_Pointer<UART1_LEN_BUFFER)	
			Uart1_ReceiveBuffer_B[Uart1_Rx_Pointer++]=tmp;
		if(tmp=='\n')
		{
			Uart1_Rx_Pointer =0;
			Uart1_RX_Finish_B=1;
			Uart1_ReceiveBuffer_A_B=~Uart1_ReceiveBuffer_A_B;
		}
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_USI1_Tx() interrupt 4
{
	USI1ST1&=~0x80;
	if(Uart1_EnableSend)
	{		 
		Uart1_Tx_TimeOut=10;
		USI1DR=Uart1_TransmitBuffer[Uart1_Tx_Pointer++];
		if(Uart1_TransmitBuffer[Uart1_Tx_Pointer]=='\0')
			Uart1_EnableSend=0;
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_USI0_Rx() interrupt 9
{
	uint8_t tmp;
	tmp=USI0DR;	
	USI0ST1&=~0x20;
	Uart0_ReceiveBuffer[mUart0_Rx_Pointer]=tmp;
	if(++mUart0_Rx_Pointer>=(UART0_RX_LEN_BUFFER-1))	
		mUart0_Rx_Pointer=0;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_USI0_Tx() interrupt 10
{
	USI0ST1&=~0x80;
	if(Uart0_EnableSend)
	{		 
		Uart0_Tx_TimeOut=20;
		USI0DR=Uart0_TransmitBuffer[Uart0_Tx_Pointer++];
		if(Uart0_TransmitBuffer[Uart0_Tx_Pointer]=='\0')
			Uart0_EnableSend=0;
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_Timer0() interrupt 13
{
	sys_tick(); 
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_Timer2() interrupt 15
{
	// Timer2 interrupt
	// TODO: add your code here
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_Timer3() interrupt 16
{
	led_rgb_drive();
	voice_in_timer();
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_Timer4() interrupt 17
{
	// Timer4 interrupt
	// TODO: add your code here
	if (T4ISR & 0x80) 
		T4ISR &= ~0x80;	// clear IOVR
	if (T4ISR & 0x40) 
		T4ISR &= ~0x40;	// clear IBTM
	if (T4ISR & 0x20) 
		T4ISR &= ~0x20;	// clear ICMA
	if (T4ISR & 0x10) 
		T4ISR &= ~0x10;	// clear ICMB
	if (T4ISR & 0x08) 
		T4ISR &= ~0x08;	// clear ICMC
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void INT_WT() interrupt 20
{
	gbHalfSecond=(!gbHalfSecond);
	if(gbHalfSecond)
	{
	 	if(++gRTC_Sec>59) 
		{
			gRTC_Sec=0;
			/*if(++gRTC_Minute>59)
			{
				gRTC_Minute=0;
				if(++gRTC_Hour>23)
				{	
					gRTC_Hour=0; 
					gRTC_Week<<=1;
					if(gRTC_Week==0x80)
						gRTC_Week=0x01;
				}
			}*/
		}
		if(++gRTC_Minute>59)
		{
			gRTC_Minute=0;
			if(++gRTC_Hour>23)
			{	
				gRTC_Hour=0;
				gRTC_Week<<=1;
				if(gRTC_Week==0x80)
					gRTC_Week=0x01;
			}
		}
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
unsigned int ADC_read()
{
	unsigned int adcVal;
	adcVal = ((ADCDRH << 8) | ADCDRL);	
	return	adcVal;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void ADC_init()
{
	// initialize A/D convertor
	ADCCRL = 0x80;  	// setting
	ADCCRH = 0x07;  	// trigger source, alignment, frequency
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void ADC_start(unsigned char ch)
{
	// start A/D convertor
	ADCCRL  = (ADCCRL & 0xf0) | (ch & 0x0f);	// select channel
	ADCCRL |= 0x40; 	// start ADC
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void LCD_init()
{
	// initialize LCD
	// Frame freq. = 16.000000Hz
	LCDCRH = 0x00;  	// LCD control High
	LCDCRL = 0x00;  	// LCD control Low
	LCDCCR = 0x00;  	// LCD contrast
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void Timer2_init()
{
	// initialize Timer2
	// 16bit PWM, period = 1.000000mS ( 1000.000000Hz )
	//     PWM duty = 50.000000%
	T2CRH = 0x30;   	// PWM setting High, 1 shot mode
						//T2CRH |= 0x30;	// PWM repeat mode
	T2CRL = 0xD1;   	// PWM setting Low
	T2BDRH = 0x01;  	// duty High 占空比
	T2BDRL = 0xF3;  	// duty Low
	T2ADRH = 0x03;  	// period count High  周期
	T2ADRL = 0xE7;  	// period count Low
	IE2 |= 0x08;    	// Enable Timer2 interrupt
	//T2CRH |= 0x80;  	// enable counter

}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void Timer4_init()
{
		// initialize Timer4
	// 10bit PWM, period = 1.000000mS ( 1000.000000Hz )
	//     PWM A duty = 50.000000%
	//     PWM B duty = 50.000000%
	//     PWM C duty = 50.000000%
	T4CR   = 0x24;    	// PWM setting
	T4PCR1 = 0x88;  	// enable PWM
	T4PPRL = 0xE7;  	// period Low 
	T4PPRH = 0x03;  	// period High 
	T4ADRL = 0xF3;  	// duty Low 
	T4ADRH = 0x01;  	// duty High 
	T4BDRL = 0xF3;  	// duty Low
	T4BDRH = 0x01;  	// duty High
	T4CDRL = 0xF3;  	// duty Low
	T4CDRH = 0x01;  	// duty High
	T4PCR2 = 0x20;  	// enable PWM output，只开了PWM4AA，
	//T4PCR2 = 0x10;  	// enable PWM output
	T4PCR3 = 0X00;  	// set PWM output polarity
	T4DLYA = 0x00;  	// TODO: do yourself, PWM dalay AA & AB
	T4DLYB = 0x00;  	// TODO: do yourself, PWM dalay BA & BB
	T4DLYC = 0x00;  	// TODO: do yourself, PWM dalay CA & CB
	T4MSK  = 0x90;   	// TODO: do yourself, interrupt masking
	IE2   |= 0x20;    	// Enable Timer4 interrupt
	T4CR  |= 0x10;   	// clear counter
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void WDT_clear()
{
	WDTCR |= 0x20;  	// Clear Watch-dog timer
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void WDT_init()
{
	// initialize Watch-dog timer
	WDTDR = 0x01;   	// period
	WDTCR = 0xC0;   	// setting
	WDTCR |= 0x02;	// Use WDTRC
	WDT_clear();
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void Timer0_init()
{
	// initialize Timer0
	// 8bit timer, period = 1.002667mS
	T0CR  = 0x88;    	// timer setting
	T0DR  = 0x5D;    	// period count
	IE2  |= 0x02;    	// Enable Timer0 interrupt
	T0CR |= 0x01;   	// clear counter
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void Timer3_init()
{
	// initialize Timer3
	// 8bit timer, period = 0.050000mS
	T3CR  = 0x8A;    	// timer setting
	T3DR  = 0x4A;    	// period count
	IE2  |= 0x10;    	// Enable Timer3 interrupt
	T3CR |= 0x01;   	// clear counter
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void UART0_init()
{
	// initialize UART interface
	// UART0 : ASync. 57692bps N 8 1
	USI0CR2  = 0x02; 	// activate USI0
	USI0CR1  = 0x06; 	// Async/Sync, bit count, parity
	USI0CR2 |= 0xEC;	// interrupt, speed
	USI0CR3  = 0x00; 	// stop bit
	USI0BD   = 0x0C;  	// baud rate
	IE1     |= 0x18;    	// enable UART0 interrupt	
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
bit enUart1=0;
void UART1_init()
{
	// UART1 : ASync. 9615bps N 8 1
	if(!enUart1)
	{
		USI1CR2  = 0x02; 	// activate USI1
		USI1CR1  = 0x06; 	// Async/Sync, bit count, parity
		USI1CR2 |= 0xEC;	// interrupt, speed
		USI1CR3  = 0x00; 	// stop bit
		USI1BD   = 0x4D;  	// baud rate
		IE      |= 0x18;     	// enable UART1 interrupt
	}
	enUart1=1;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void UART1_def_init()
{
	// UART1 : ASync. 9615bps N 8 1
	if(enUart1)
	{
		USI1CR2 = 0x00; 	// activate USI1
		USI1CR1 = 0x00; 	// Async/Sync, bit count, parity
		USI1CR2 = 0x00;	  // interrupt, speed
		USI1CR3 = 0x00; 	// stop bit
		USI1BD  = 0x00;  	// baud rate
		IE     &=~0x18;  // enable UART1 interrupt
	}
	enUart1=0;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void WT_init()
{
	// initialize Watch timer
	WTCR = 0x88;    	// setting
	WTDR = 0x01;    	// set duty
	WTDR = 0x81;    	// clear WT
	IE3 |= 0x04;    	// Enable WT interrupt
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void clock_init()
{
	// external clock
	XTFLSR = 0xA8;  	// MC96F6432A only, others ignore this line
	OSCCR  = 0x0A;   	// Enable int. 1MHz and Ext. OSC
	BITCR  = 0x09;   	// Set waiting time : 16ms@1MHz
	while((BITCR & 0x80) == 0);	// Ext. OSC stabilizing time
	SCCR   = 0x01;    	// Change to Ext. OSC
	OSCCR |= 0x05;  	// Disable Int. OSC

	IE |= 0x80;//总中断打开
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：
*******************************************************************/
void port_init()
{
	// initialize ports
	//   1 : P55      out 
	//   2 : RxD0     in  
	//   3 : TxD0     out 
	//   4 : P42      out 
	//   5 : P43      out 
	//   6 : P37      out 
	//   7 : P36      out 
	//   8 : P35      out 
	//   9 : P34      out 
	//  10 : P33      out 
	//  11 : P32      out 
	//  12 : P31      out 
	//  13 : P30      in  
	//  14 : P27      out 
	//  15 : P26      in  
	//  16 : P25      out 
	//  17 : P24      out 
	//  18 : P23      out 
	//  19 : P22      out 
	//  20 : P21      out 
	//  21 : TxD1     out 
	//  22 : RxD1     in  
	//  23 : P11      out 
	//  24 : P12      in  
	//  25 : P13      out 
	//  26 : P14      out 
	//  27 : P15      out 
	//  28 : P16      in  
	//  29 : P17      out 
	//  30 : AN5      in  
	//  31 : P06      out 
	//  32 : P05      out 
	//  33 : P04      out 
	//  34 : P03      out 
	//  35 : P02      out 
	//  36 : P01      out 
	//  37 : P00      out 
	//  40 : P50      out 
	//  41 : P51      out 
	//  42 : P52      out 
	//  43 : SXIN     in  
	//  44 : SXOUT    out 
	P0IO = 0x7F;    	// direction  为1为输出，P11输入
	P0PU = 0x00;    	// pullup
	P0OD = 0x00;    	// open drain
	P0DB = 0x00;    	// bit7~6(debounce clock), bit5~0=P07~02 debounce
	P0   = 0x00;    	// port initial value

	P1IO = 0x9A;    	// direction
	P1PU = 0x20;    	// pullup
	P1OD = 0x00;    	// open drain
	P15DB = 0x00;   	// debounce : P54, 52, 17, 16, 12, 11
	P1   = 0x00;    	// port initial value

	P2IO = 0xFF;    	// direction
	P2PU = 0x00;    	// pullup
	P2OD = 0x02;    	// open drain
	P2   = 0x00;    	// port initial value

	P3IO = 0xBE;    	// direction
	P3PU = 0x00;    	// pullup
	P3   = 0x00;    	// port initial value

	P4IO = 0xFA;    	// direction P43输出,P42输入
	P4PU = 0x0C;    	// pullup，P43为上拉
	P4OD = 0x00;    	// open drain
	P4   = 0x00;    	// port initial value

	P5IO = 0xFF;    	// direction
	P5PU = 0x00;    	// pullup
	P5   = 0x00;    	// port initial value

	// Set port functions
	P0FSRH = 0x20;  	// P0 selection High 
	P0FSRL = 0x1E;  	// P0 selection Low F0FSRL4/3、2/1为0001_1110,开启PWM4AB、A
	P1FSRH = 0x00;  	// P1 selection High
	P1FSRL = 0x0F;  	// P1 selection Low
	P2FSRH = 0x00;  	// P2 selection High
	P2FSRL = 0x03;  	// P2 selection Low
	P3FSR  = 0x00;   	// P3 selection
	P4FSR  = 0x0A;   	// P4 selection
	P5FSR  = 0x2D;   	// P5 selection
}





