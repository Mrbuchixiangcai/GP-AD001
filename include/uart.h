#ifndef __UART_H__
#define __UART_H__

#define UART0_LEN_BUFFER    100
#define UART0_RX_LEN_BUFFER 301   

extern uint8_t  idata  Uart0_EnableSend;
extern uint8_t  idata  Rx_Pointer;
extern uint8_t  idata  Uart0_Tx_Pointer;
extern uint16_t xdata  mUart0_Rx_Pointer;
extern uint16_t xdata  sUart0_Rx_Pointer;
extern uint8_t  idata  Uart0_Tx_TimeOut;
extern uint8_t  idata  Uart0_Rx_TimeOut;
extern uint8_t  idata  WIFI_CMD[UART0_LEN_BUFFER];
extern uint8_t  xdata  Uart0_TransmitBuffer[UART0_LEN_BUFFER];
extern uint8_t  xdata  Uart0_ReceiveBuffer[UART0_RX_LEN_BUFFER];

extern void Uart0Transmit_SendString(char *str); 
extern void Uart0Transmit_SendString_3Step(void); 
extern void Uart0_Receive_Parse(void);
extern uint8_t  idata  Count200ms_3Step;
extern uint8_t  idata  Uart0_SendString_3Step;

#endif





