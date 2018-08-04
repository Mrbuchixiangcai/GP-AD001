#ifndef __WIFI_H__
#define __WIFI_H__

#include "app_main.h"

extern char  code SPASongs_Num_Table[7][24];
extern void Uart0Transmit_SendString(char *str);
extern void ApplicationGP389_ONOFF(uint8_t onoff);
extern void Wifi_CommandDeal(char *Uart0_Receive);
extern void GP389_ON(void);
extern void GP389_OFF(void);
extern void GP389_OFF_AND_AlarmOFF();

#endif




