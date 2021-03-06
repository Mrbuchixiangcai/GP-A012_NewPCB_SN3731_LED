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

/****************************/
/*标志位定义flags definetion*/
/****************************/

/*****************************/
/*变量定义variable definition*/
/*****************************/

/**************************/
/*数组定义array definition*/
/**************************/
u8 SN3236_Pwm2_1[36];
u8 SN3236_Pwm2_2[36];
u8 SN3236_RegBuffer[0x4B];

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
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3236_RegWrite(u8 *p)
{
	u8 i;
	IIC_Start();
	IIC_Send_Byte(0x78);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	for (i = 0; i<0x4B; i++)
	{
		if ((i>0) && (i<0x25))
			IIC_Send_Byte_Mix(i);
		else
			IIC_Send_Byte(p[i]);
		IIC_Wait_Ack();
	}
	IIC_Stop();
}

/*******************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3236_Init1(void)
{
	u8 i;
	for (i = 0; i<0x4B; i++)
		SN3236_RegBuffer[i] = 0;
	for (i = 0x26; i<0x4A; i++)
		SN3236_RegBuffer[i] = electricityBrightness;
	SN3236_RegBuffer[REG_00H_TURN_ON] = 0x01;
}

/*******************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3236_Driver()
{
	SN3236_Init1();
	SN3236_RegWrite(SN3236_RegBuffer);
}

/*******************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
//u8  xxxx;
//u8  yyyy;
//	u8 i;
//	for(i=0;i<36;i++)
//		SN3236_Pwm2_1[i]=0x00;
//	for(i=0;i<36;i++)
//		SN3236_Pwm2_1[i]=0x00;
//	SN3236_Pwm2_1[xxxx]=0xF0;
//	SN3236_Pwm2_2[yyyy]=0xF0;	


















