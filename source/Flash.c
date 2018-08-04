#include "app_main.h"
#include <intrins.h>

unsigned char eeprom_write_enable[4];

void eeprom_init(void)
{
	FIDR   = 0;
	FMCR   = 0;  
	FSADRH = 0;
	FSADRM = (unsigned char)(EEP_ADDRESS_START >> 8);	    
	FSADRL = (unsigned char)EEP_ADDRESS_START;	 
}
void Program_Reset(void)
{
	((void (code *) (void)) 0x0000)();
}
void eeprom_wr_flag_endable(unsigned char enable)
{
	unsigned char i = 0;
	unsigned char ret = 0xA2;   
	for(i=0;i<4;i++)
	{
	if(enable)  
		eeprom_write_enable[i]=ret;
	else                        
		eeprom_write_enable[i]= 0 ;
	ret++;
	}        
}
void EpromResetCMD(void)
{
	FMCR = 0x01;
	_nop_();  
	_nop_();
  	_nop_();  
	_nop_();
}
void EpromWriteCMD(unsigned char code *ptr_addr)
{
	unsigned int tmp = (unsigned char code *)ptr_addr;	
	FSADRH = 0;
	FSADRM = (unsigned char)(tmp>>8);	    
	FSADRL = (unsigned char)tmp;	        
	FIDR   = 0xA5;                                            	                    
	if((eeprom_write_enable[0]==0xA2) && (eeprom_write_enable[1]==0xA3) && (eeprom_write_enable[2]==0xA4) && (eeprom_write_enable[3]==0xA5))
	{
		FMCR = 0x03;                                             
		_nop_(); 
		_nop_();
		_nop_();  
		_nop_();
	}
	else
	{
		Program_Reset();                                         
	}
	eeprom_init();           	
}
void EpromEraseCMD(unsigned char code *ptr_addr)
{
	unsigned char i;
	unsigned int  tmp = (unsigned char code *)ptr_addr;	
	unsigned char xdata *ImageDat = 0x8000;	                                                                                                                        
	EpromResetCMD();                                           
	for(i=0; i<FLASH_PAGE_SIZE; i++)          	
		ImageDat[i]=0;                                             
	FSADRH = 0;                                               
	FSADRM = (unsigned char)(tmp>>8);	
	FSADRL = (unsigned char)tmp;	    
	FIDR   = 0xA5;                                             
	if((eeprom_write_enable[0]==0xA2) && (eeprom_write_enable[1]==0xA3) && (eeprom_write_enable[2]==0xA4) && (eeprom_write_enable[3]==0xA5))
	{
		FMCR = 0x02;                                             
		_nop_(); 
		_nop_();
		_nop_();  
		_nop_();
	}
	else
	{
		Program_Reset();                                         
	}
	eeprom_init();                                               
}
unsigned char eeprom_page_write_data(unsigned char code *eep_address,unsigned char *dat)
{
	unsigned char i;
	unsigned char j=0;
	unsigned char xdata *ImageDat = 0x8000;	      
	unsigned char code *ROMadr = (unsigned char code *)eep_address;   	                                        
	cli();    
	eeprom_wr_flag_endable(1);  
	EpromEraseCMD(ROMadr);                                    
	EpromResetCMD();                                                                                                
	for(i=0;i<FLASH_PAGE_SIZE;i++)          	
		ImageDat[i] = dat[j++];                                  
	EpromWriteCMD(ROMadr);                                                                                                                         
	eeprom_wr_flag_endable(0);   
	ROMadr+=FLASH_PAGE_SIZE;
	sei();    
  	return 0;
}
unsigned char  eeprom_byte_read_data(unsigned char code *eep_address)
{  
	unsigned char code *p = (unsigned char code *)eep_address;    
	return (*p);														 
}





