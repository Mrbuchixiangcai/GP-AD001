#ifndef __BOOT_H__
#define __BOOT_H__

#define PAGE_SIZE            64
#define BOOT_ADDRESS_START   0x0100     
#define BOOT_ADDRESS_END     0x03FF  
#define APP_ADDRESS_START    0x0500     
#define APP_ADDRESS_END      0x3FFF   
extern void BootLoader_handle(void);

#endif




