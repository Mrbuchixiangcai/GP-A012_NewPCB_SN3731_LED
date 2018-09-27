#include "app_main.h"

uint8_t  enUart_Send =0;
uint8_t  mPtr_Receive=0;
uint8_t  sPtr_Receive=0;
uint8_t  Uart1_RX_Timeout;
uint8_t  BT_ReceiveBuffer[RECEIVELEN];
uint8_t  Uart1_ReceiveBuffer[RECEIVELEN];
uint8_t  Uart1_TransmitBuffer[RECEIVELEN];



HAL_StatusTypeDef User_UART_Receive_IT(UART_HandleTypeDef *huart)
{
  uint16_t uhMask = huart->Mask;
	Uart1_ReceiveBuffer[mPtr_Receive]=(uint8_t)(huart->Instance->RDR & (uint8_t)uhMask);
	if(++mPtr_Receive>=(RECEIVELEN))
		mPtr_Receive=0;
	Uart1_RX_Timeout=10;
  return HAL_OK;
}
void Usart1SendData(uint8_t *pdata) 
{
  uint8_t size = 0;
  HAL_UART_DMAStop(&huart1);	
  while ((*pdata !='\n')&&(*pdata !='\0'))  
  {           
    Uart1_TransmitBuffer[size] = *pdata;
    size++;
    pdata++;
  }
  HAL_UART_Transmit_IT(&huart1, Uart1_TransmitBuffer, size);
}

//void hex_to_char(uint8_t *p,uint8_t indata)
//{
//	uint8_t tmp;
//	tmp=indata>>4;
//	if(tmp>=0x0A)
//	 *p=((tmp-0x0A)+'A'); 
//	else
//	 *p=(tmp+'0');
//	p++;
//	tmp=indata&0x0F;
//	if(tmp>=0x0A)
//		*p=((tmp-0x0A)+'A'); 
//	else
//		*p=(tmp+'0');
//}
//char char_to_bcd(char indata1,char indata2)
//{
//	if(indata1>='A')
//		indata1=((indata1-'A')+0x0A);
//	else
//		indata1=(indata1-'0');
//	if(indata2>='A')
//		indata2=((indata2-'A')+0x0A);
//	else
//		indata2=(indata2-'0');
//	return (indata1*10+indata2);
//}


void BlueMode_Receive(void)
{
 static uint8_t ptrReceive=0;	
 while(mPtr_Receive!=sPtr_Receive)
 {
  Uart1_RX_Timeout=10;
  BT_ReceiveBuffer[ptrReceive++]=Uart1_ReceiveBuffer[sPtr_Receive];
  Uart1_ReceiveBuffer[sPtr_Receive]=0x00;
  if(++sPtr_Receive>=(RECEIVELEN))
   sPtr_Receive=0;
 }	
 if(Uart1_RX_Timeout != 0)
 {
  if(--Uart1_RX_Timeout==0)
  {
   if((BT_ReceiveBuffer[0]=='B')&&(BT_ReceiveBuffer[1]=='0')&&(BT_ReceiveBuffer[2]=='0')&&(BT_ReceiveBuffer[3]=='5'))
   {//按键传过来B005x //enUart_Send=0;
	if(BT_ReceiveBuffer[4]=='0')
	{
	 FireMode=MODE0_OFF_FIRE;
	 if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))==1)
	  PlayMode=PLAY_BT;
	 else
	  PlayMode=PLAY_OFF;
	}
	else if(BT_ReceiveBuffer[4]=='1')
	 FireMode=MODE1_SMALL_FIRE1;
	else if(BT_ReceiveBuffer[4]=='2')
	 FireMode=MODE3_BIG_FIRE1;
	else if(BT_ReceiveBuffer[4]=='3')
	 FireMode=FOLLOW_WIND;
	else if(BT_ReceiveBuffer[4]=='4')
	 FireMode=MODE4_PULSATE_MUSIC;
	else if(BT_ReceiveBuffer[4]=='5')
	 FireMode=MUSIC_PULSE_FLASH;
	else if(BT_ReceiveBuffer[4]=='6')
	 FireMode=MUSIC_PULSE_FLASH_2;
	return;
   }
   ptrReceive=0;
   memset(BT_ReceiveBuffer,0,sizeof(BT_ReceiveBuffer));
  }
 }
 else ptrReceive=0;
   
}
void BlueMode_Handle(void)
{
	BlueMode_Receive();
}	






