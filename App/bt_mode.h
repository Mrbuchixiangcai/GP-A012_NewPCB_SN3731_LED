#ifndef  __BT_MODE_H__
#define  __BT_MODE_H__

#include "app_main.h"
#define  RECEIVELEN  30
extern uint8_t  enUart_Send ;
extern uint8_t  mPtr_Receive;
extern uint8_t  sPtr_Receive;
extern uint8_t  Uart1_RX_Timeout;
extern uint8_t  BT_ReceiveBuffer[RECEIVELEN];
extern uint8_t  Uart1_ReceiveBuffer[RECEIVELEN];
extern uint8_t  Uart1_TransmitBuffer[RECEIVELEN];
//extern uint8_t  BT_TransmitBuffer[RECEIVELEN];

extern void Usart1SendData(uint8_t *pdata); 
extern void BlueMode_Handle(void);

#endif








