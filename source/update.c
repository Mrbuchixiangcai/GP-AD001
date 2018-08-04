//===============================================================
//  	File name : update.c ?PR?UPDATE_HANDLE?UPDATE(0x4000)
//=============================================================== 
#include "app_main.h"

uint8_t   idata  update_step;
uint16_t  idata  Update_Rx_Pointer;
uint16_t  idata  Update_PacketSum;
uint16_t  idata  Update_ByteSizeSum;
uint16_t  idata  Update_PacketCurrent;
uint16_t  idata  Update_ByteSizeCurrent;
uint8_t   xdata  *Update_ReceiveBuffer;

void putChar(char ch)
{
	USI0ST1&=~0xC0;		
	USI0DR=ch;  
	while(!(USI0ST1 & 0xC0));
}
void putString(char *str) 
{
	USI0ST1&=~0xC0;		
	USI0DR=*str++;  
	while(*str!='\0') 
	{ 
		if((USI0ST1 & 0xC0))
		{
			USI0ST1&=~0xC0;		 
			USI0DR=*str++; 
		}
	}
}

void update_handle(void)
{
	uint16_t i;
	uint8_t  Head;	//包头部
	uint16_t Packet_P;
	uint16_t Packet_N;
	uint16_t sumPacket_CRC;
	uint16_t curPacket_CRC;
	uint8_t  TimeOut_L;		
	uint8_t  TimeOut_M;	
	uint8_t  TimeOut_H;	
	uint8_t	 code *curEEPAddress;
	uint8_t	 code *Boot_ROMadr;
	WDTDR = 0x00;   	// period
	WDTCR = 0x00;   	// setting	
	IE3 &=~0x04;   // dsable WT interrupt
	IE  &=~0x18;   // dsable UART1 interrupt
	IE1 &=~0x18;   // dsable UART0 interrupt
	IE2 &=~0x10;   // dsable Timer3 interrupt
	update_step=0;
	Update_Rx_Pointer=0;	
	Update_PacketCurrent=0;	
	TimeOut_L=0;		
	TimeOut_M=0;	
	TimeOut_H=0;	
	Update_ReceiveBuffer=Uart0_ReceiveBuffer;
	for(i=0;i<Update_LEN_BUFFER;i++)
		Update_ReceiveBuffer[i]=0x00;
	LED_RED(1); //端口置位宏定义
	LED_GREEN(1);
	LED_BLUE(1);
	putChar('C');
	while(1)	
	{
		switch(update_step)
		{
			case 0:
				WDT_clear();
				if(USI0ST1 & 0x20)
				{
					Update_ReceiveBuffer[Update_Rx_Pointer++]=USI0DR;
					TimeOut_L=0;		
					TimeOut_M=0;	
					TimeOut_H=0;	
				}
				if(Update_ReceiveBuffer[0]==0x6E)
				{
					if(Update_PacketSum) 
					{
						putString("update fail");
						update_step=0;
						return;
					}
					else
					{
						putString("update ok");
						update_step=6;
						break;
					}
				}
				if(Update_Rx_Pointer==5+256+2)//263
				{
					Update_Rx_Pointer=0;
					update_step++;
				}
				break;
			case 1:
				WDT_clear();
				Update_Rx_Pointer=0;
				Head=Update_ReceiveBuffer[0];
				Packet_P=(Update_ReceiveBuffer[1]<<8)|Update_ReceiveBuffer[2];
				Packet_N=(Update_ReceiveBuffer[3]<<8)|Update_ReceiveBuffer[4];
				if((Head!=0x02)||(Packet_P!=(~Packet_N)))
				{
					putChar(0x15); 
					update_step=0;
					break;
				}
				if(((Packet_P-Update_PacketCurrent)==1)||((Packet_P-Update_PacketCurrent)==0))
				{
					Update_PacketCurrent=Packet_P;
					curEEPAddress=EEP_ADDRESS_START+(Update_PacketCurrent*256);
					if(curEEPAddress>EEP_ADDRESS_END)
					{
						return;	
					}
				}
				else
				{
					putChar(0x15); 
					update_step=0;
					break;
				}
				update_step++;
				break;
			case 2:
				WDT_clear();
				sumPacket_CRC=0;
				curPacket_CRC=0;
				for(i=0;i<256;i++)
					sumPacket_CRC+=Update_ReceiveBuffer[i+5];
				curPacket_CRC =Update_ReceiveBuffer[256+5]<<8;
				curPacket_CRC|=Update_ReceiveBuffer[256+5+1];
				if(sumPacket_CRC!=curPacket_CRC)
				{
					putChar(0x15); 
					update_step=0;
					break;
				}
				update_step++;
				break;
			case 3:
				WDT_clear();
				eeprom_page_write_data(curEEPAddress,&Update_ReceiveBuffer[5]);		
				eeprom_page_write_data(curEEPAddress+64,&Update_ReceiveBuffer[5+64]);		
				eeprom_page_write_data(curEEPAddress+64+64,&Update_ReceiveBuffer[5+64+64]);		
				eeprom_page_write_data(curEEPAddress+64+64+64,&Update_ReceiveBuffer[5+64+64+64]);	
				Update_PacketSum-=1;
				update_step++;
				break;
			case 4:
				WDT_clear();
				for(i=0;i<256;i++)
				{
					if(Update_ReceiveBuffer[i+5]!=eeprom_byte_read_data(curEEPAddress+i))
					{
						putChar(0x15); 
						update_step=0;
						break;
					}
				}
				update_step++;
				break;
			case 5:
				WDT_clear();
				for(i=0;i<Update_LEN_BUFFER;i++)
					{Update_ReceiveBuffer[i]=0x00;WDT_clear();}
				putChar(0x96); 
				update_step=0;
				break;
			case 6:		
//				Boot_ROMadr=BOOT_ADDRESS_START;
//				curEEPAddress=EEP_ADDRESS_START+BOOT_ADDRESS_START;
//				while(Boot_ROMadr<BOOT_ADDRESS_END)
//				{
//					for(i=0;i<64;i++)
//					{Update_ReceiveBuffer[i]=eeprom_byte_read_data(curEEPAddress+i);WDT_clear();}
//					eeprom_page_write_data(Boot_ROMadr,Update_ReceiveBuffer);	//??????????????	
//					curEEPAddress+=64;	
//					Boot_ROMadr  +=64;				 
//				}
			  WDT_clear();
				update_step++;
				break;
			case 7:
				WDT_clear();
				for(i=0;i<FLASH_PAGE_SIZE;i++)
					Update_ReceiveBuffer[i]=0xAA;
				eeprom_page_write_data(EEP_ADDRESS_MSG_START,Update_ReceiveBuffer);	
        WDT_clear();			
				((void (code *)(void))0x0000)();
				return;
				break;
			default:
				WDT_clear();
				update_step=0;
				break;
  		}
		WDT_clear();
		if(++TimeOut_L==0)
		{
			if(++TimeOut_M==0)
			{
				if(++TimeOut_H>=2)
				 ((void (code *)(void))0x0000)();
			}
		}
 	}
}





