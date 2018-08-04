#ifndef __BT_H__
#define __BT_H__

//�궨��macro definition//
#define UART1_LEN_BUFFER 8

//���Ͷ���byte definition//

enum
{
	BT_NONE=0,
	BT_PAUSE,
	BT_PALY,
	BT_PREV,
	BT_NEXT,
	BT_PARIR,
	BT_PARIR_EXT,
	BT_LINK_BACK,
	BT_POWER_DOWN,
	BT_DISCONN,
	BT_CLEAR_LIST,
	BT_VOL,
};
typedef enum
 {
  BT_STEP_START =0,
  BT_STEP_INITI1  ,
  BT_STEP_INITI2  ,
  BT_STEP_INITI3  ,
  BT_STEP_INITI4  ,
  BT_STEP_INITI5  ,
  BT_STEP_INITI6  ,
  BT_STEP_INITI7  ,
  BT_STEP_INITI8  ,
  BT_STEP_INITI9  ,
  BT_STEP_INITI10  ,
  BT_STEP_INITI11  ,
  BT_STEP_INITI12  ,
 }BT_STEP;

 //��������variable definition//

//��־λ����flags definetion//

//��������function definetion//

//�ⲿ����_�궨��macro definition//

//�ⲿ����_���Ͷ���byte definition//
extern BT_STEP  BT_Step; 

 //�ⲿ����_��������variable definition//
extern uint8_t  idata bt_cmd;       
extern uint8_t  idata cntMuteBT;   
extern uint8_t  idata bt_volume;    
extern uint8_t  idata Uart1_Rx_Pointer;
extern uint8_t  idata Uart1_Tx_Pointer;
extern uint8_t  idata Uart1_EnableSend; 
extern uint8_t  idata Uart1_Tx_TimeOut;
extern uint8_t  idata Uart1_Rx_TimeOut; 
extern uint8_t  xdata Uart1_TransmitBuffer[UART1_LEN_BUFFER]; 
extern uint8_t  xdata Uart1_ReceiveBuffer_A[UART1_LEN_BUFFER]; 
extern uint8_t  xdata Uart1_ReceiveBuffer_B[UART1_LEN_BUFFER]; 

 //�ⲿ����_��־λ����flags definetion//
extern bit BT_Work; 
extern bit flag_bt_conn;  
extern bit flag_bt_play; 
extern bit flag_bt_pause; 
extern bit Uart1_RX_Finish_A;   
extern bit Uart1_RX_Finish_B;  
extern bit Uart1_ReceiveBuffer_A_B;

//�ⲿ����_��������function definetion//
extern void bt_send_cmd(uint8_t cmd);
extern void BlueMode_Receive(void);  
extern void BlueMode_Handle(void);

#endif




