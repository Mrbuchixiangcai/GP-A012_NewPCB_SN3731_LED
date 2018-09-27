#ifndef  __KEY_H__
#define  __KEY_H__

//ͷ�ļ�header file//
#include "app_main.h"

//�궨��macro definition//
#define  K_D     (1<<5)//��һλ�����ƶ�5λ
#define  K_U     (2<<5)
#define  K_H     (3<<5)
#define  K_R     (4<<5)
#define  K_L     (5<<5)
#define  K_LU    (6<<5)
#define  KD(x)   (x|K_D)
#define  KU(x)   (x|K_U)
#define  KH(x)   (x|K_H)
#define  KR(x)   (x|K_R)
#define  KL(x)   (x|K_L)
#define  KLU(x)  (x|K_LU)

#define  cKEY_CLICK    5  //����ʱ�䳣��
#define  cKEY_HOLD     50 //��סʱ�䳣��
#define  cKEY_RATE_VOL 20 //����ʱ�䳣��

#define  KEY_LIGHT()   HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)
#define  BT_POWER()    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define  POWER_LED(a) if (a)												   \
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);   \
					  else													   \
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)

//���Ͷ���byte definition//
enum
{
	T_NC     =0,
	T_BT_POWER ,
	T_LIGHT    ,
};


extern void    KeyScan(void);
extern void    KeyComMsg(void);

#endif









