#ifndef __UPDATE_H__
#define __UPDATE_H__

#define   Update_LEN_BUFFER UART0_RX_LEN_BUFFER 	
extern uint8_t   idata  update_step;
extern uint16_t  idata  Update_Rx_Pointer;
extern uint16_t  idata  Update_PacketSum;
extern uint16_t  idata  Update_ByteSizeSum;
extern uint16_t  idata  Update_PacketCurrent;
extern uint16_t  idata  Update_ByteSizeCurrent;
extern uint8_t   xdata  *Update_ReceiveBuffer;
extern void update_handle(void);

#endif




