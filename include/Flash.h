#ifndef _FLASH_HEADER_
#define _FLASH_HEADER_

#define EEP_ADDRESS_START       0x4000     
#define EEP_ADDRESS_END         0x7FFF    
#define EEP_ADDRESS_MSG_START   0x0400    
#define EEP_ADDRESS_MSG_END     0x04FF   

//#define EEP_ADDRESS_BOOT_START   BOOT_ADDRESS_START    
//#define EEP_ADDRESS_BOOT_END     BOOT_ADDRESS_END   

#define FLASH_PAGE_SIZE     64
extern void eeprom_init(void);
extern unsigned char eeprom_page_write_data(unsigned char code *eep_address,unsigned char *dat);
extern unsigned char  eeprom_byte_read_data(unsigned char code *eep_address);

#endif





