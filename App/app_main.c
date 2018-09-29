/*******************/
/*头文件header file*/
/*******************/
#include "app_main.h"

/************************/
/*宏定义macro definition*/
/************************/

/*************************/
/*类型定义byte definition*/
/*************************/
PLAY_MODE PlayMode;
FIRE_SIZE FireMode;

/****************************/
/*标志位定义flags definetion*/
/****************************/
uint8_t   AppTick1ms;
uint8_t   AppTick0;
uint8_t   AppTick1;
uint8_t   AppTick2;
uint8_t   AppTick3;
uint8_t   AppTick4;
uint8_t   AppTick5;

/*****************************/
/*变量定义variable definition*/
/*****************************/
uint8_t  cntAppTick;
uint8_t  brightness1;
uint8_t  electricityBrightness;
uint8_t  demo_NO; //SN3237驱动使用

/**************************/
/*数组定义array definition*/
/**************************/

/******************************/
/*函数声明Function declaration*/
/******************************/

/****************************/
/*函数定义function definetion*/
/****************************/
/*******************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void PowerON_Reset(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	brightness1 = 2;
	electricityBrightness = 0x07;
	FireMode = MODE0_OFF_FIRE;
	PlayMode = PLAY_ON;

	demo_NO = 1; //SN3237驱动使用


	HAL_UART_Receive_IT(&huart1, Uart1_ReceiveBuffer, RECEIVELEN);
}

/*******************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void Scan_ONOFF(void)
{
	static uint8_t  BTPower_c;
	if (BTPower() != BTPower_c)
	{
		BTPower_c = BTPower();
		if (BTPower())
			PlayMode = PLAY_BT;
		else
		{
			PlayMode = PLAY_OFF;
			FireMode = MODE0_OFF_FIRE;
		}
	}
	if ((!KEY_LIGHT()) || (BTPower()) || (PlayMode != PLAY_OFF))
	{
		POWER_LED(1);
	}
	if (PlayMode == PLAY_OFF)
	{
		FireMode = MODE0_OFF_FIRE;
		POWER_LED(0);
	}
}

/*******************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void app_main(void)
{
	PowerON_Reset();

	while (1)
	{
		if (AppTick1ms)
		{
			AppTick1ms = 0;
			ADC_GetBuffer();

			//音量扫描
			volume_scan(VOLUME_STEP);

			Scan_ONOFF();
		}
		if (AppTick0)
		{
			AppTick0 = 0;
			KeyScan();
			KeyComMsg();
		}
		if (AppTick1)
		{
			AppTick1 = 0;
			BlueMode_Handle();
		}
		if (AppTick2)
		{
			AppTick2 = 0;
			FireMode_Handle();
		}
		if (AppTick3)
		{
			AppTick3 = 0;
			SN3236_Driver();
		}
	}
}


