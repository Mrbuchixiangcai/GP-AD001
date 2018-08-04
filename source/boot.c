#include "app_main.h"

void BootLoader_handle(void)
{
	unsigned char i;
	unsigned int  addr; 
	unsigned char xdata *dat=Uart0_ReceiveBuffer;
	unsigned char xdata *ImageDat=0x8000;	      
	unsigned char code  *APP_ROMadr;   
	unsigned char code  *EEP_ROMadr; 
  
	for(i=0;i<PAGE_SIZE;i++)	
		{dat[i]=0x00;WDTCR |= 0x20;}
	for(EEP_ROMadr=EEP_ADDRESS_MSG_START,i=0;i<PAGE_SIZE;i++)
	{
		dat[i]=*EEP_ROMadr;
		EEP_ROMadr++;
	}
	if((dat[0]==0xAA) && (dat[2]==0xAA) && (dat[4]==0xAA) && (dat[6]==0xAA))
	{
		cli();   		
		APP_ROMadr=0x0000;
		EEP_ROMadr=EEP_ADDRESS_START;
		while(1)	
		{
			WDTCR |= 0x20;
			addr = (unsigned char code *)APP_ROMadr;	
			for(i=0;i<PAGE_SIZE;i++)
			{
				if(((APP_ROMadr>=(0x0000)) && (APP_ROMadr<=(0x0400)))||((APP_ROMadr>=(APP_ADDRESS_START)) && (APP_ROMadr<=(APP_ADDRESS_END))))	
				{
					dat[i]=*EEP_ROMadr;
					EEP_ROMadr++;
					APP_ROMadr++;
				}
				else
					dat[i]=0;
			}
			FMCR   = 1;
			_nop_();  
			_nop_();
			_nop_();  
			_nop_(); 
			for(i=0;i<PAGE_SIZE;i++)
			{ImageDat[i]=0;WDTCR |= 0x20;}                                           	                                          
			FSADRH = 0;                                               
			FSADRM = (unsigned char)(addr>>8);	
			FSADRL = (unsigned char)addr;	    
			FIDR   = 0xA5;  
			_nop_(); 
			_nop_();
			_nop_();  
			_nop_();
			FMCR   = 0x02;                                             
			_nop_(); 
			_nop_();
			_nop_();  
			_nop_();
			FIDR   = 0;
			FMCR   = 0;  
			FSADRH = 0;
			FSADRM = (unsigned char)(APP_ADDRESS_START >> 8);	    
			FSADRL = (unsigned char)APP_ADDRESS_START;	 
			FMCR =1;//Reset
			_nop_();  
			_nop_();
			_nop_();  
			_nop_();   		
			for(i=0; i<PAGE_SIZE;i++){ImageDat[i]=dat[i];}    
			FSADRH = 0;                                               
			FSADRM = (unsigned char)(addr>>8);	
			FSADRL = (unsigned char)addr;	  		
			FIDR   = 0xA5;   
			_nop_();  
			_nop_();
			_nop_();
			_nop_(); 
			FMCR   = 0x03;                                       
			_nop_();  
			_nop_();
			_nop_();
			_nop_(); 
			FIDR   = 0;
			FMCR   = 0;  
			FSADRH = 0;
			FSADRM = (unsigned char)(APP_ADDRESS_START >> 8);	    
			FSADRL = (unsigned char)APP_ADDRESS_START;	 
			if((APP_ROMadr>=(EEP_ADDRESS_MSG_START))&&(APP_ROMadr<=(EEP_ADDRESS_MSG_END+1)))
			{
			 WDTCR |= 0x20;
			 ((void (code *) (void))0x0000)(); 
			}
			if(APP_ROMadr==(0x0100))
			{
		   APP_ROMadr=0x0500;
	   	 EEP_ROMadr=EEP_ADDRESS_START+0x0500;
			}
		  if(APP_ROMadr>=(APP_ADDRESS_END))
			{
				APP_ROMadr=EEP_ADDRESS_MSG_START;	
			}
		}
 	}
 	((void (code *) (void))0x0500)();
	sei();   
}
