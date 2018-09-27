#ifndef  __APP_MAIN_H__
#define  __APP_MAIN_H__

//头文件header file//
#include "main.h"
#include "stm32f0xx_hal.h"
#include <string.h>
#include "h_adc.h"
#include "h_led.h"
#include "bt_mode.h"
#include "rgb.h"
#include "IO_IIC.h"
#include "SN3731.h"// 
#include "Key.h"
#include "big_fire.h"
#include "small_fire.h"
#include "music_fire.h "
#include "s_small_fire.h"
#include "s_big_fire.h"
#include "s_follow_wind_fire.h "
typedef unsigned         char uint8_t;
typedef unsigned short   int  uint16_t;
typedef unsigned         int  uint32_t;

#define uchar uint8_t
#define uint  uint16_t
#define ulong uint32_t

#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define FRAME_LENGH		96
#define VAL1			0x0A  //DEC:10
#define VAL2			0x1E  //DEC:30
#define VAL3			0x32  //DEC:50
#define VAL4			0x50  //DEC:80
#define VAL5			0x6E  //DEC:110
#define VAL6			0x96  //DEC:150

extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;
extern UART_HandleTypeDef huart1;


typedef enum
{
	MODE0_OFF_FIRE   =0,
	MODE1_SMALL_FIRE1  ,
//	H_SMALL_FIRE1  ,
	MODE3_BIG_FIRE1    ,
//	H_BIG_FIRE1    ,
	FOLLOW_WIND  	   ,
	MODE4_PULSATE_MUSIC,
	MUSIC_PULSE_FLASH  ,
	MUSIC_PULSE_FLASH_2,
}FIRE_SIZE;
#define FIRE_SIZE_MAX  MUSIC_PULSE_FLASH_2

typedef enum
{
	PLAY_OFF =0,
	PLAY_ON	 =1,//蓝牙关
	PLAY_BT  =2,//蓝牙开
}PLAY_MODE;

#define BTPower() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

extern uint8_t   cntAppTick;
extern uint8_t   AppTick1ms;
extern uint8_t   AppTick0;
extern uint8_t   AppTick1;
extern uint8_t   AppTick2;
extern uint8_t   AppTick3;
extern uint8_t   AppTick4;
extern uint8_t   AppTick5;
extern uint8_t   brightness1;
extern uint8_t   electricityBrightness;
extern PLAY_MODE PlayMode; 
extern FIRE_SIZE FireMode;


extern void app_main(void);

#endif







