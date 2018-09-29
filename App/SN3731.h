/**********************************/
/*头文件定义header file definition*/
/**********************************/
#ifndef  __SN3731_H__
#define  __SN3731_H__

/*******************/
/*头文件header file*/
/*******************/
#include "app_main.h"

/************************/
/*宏定义macro definition*/
/************************/
#define	SN3731_DELAY	0x00		//此延时根据需要进行相应的调整
//#define g_SN3731_IIC_ADDRESS  SN3731_IIC_ADDRESS_M  //器件地址,#define	SN3731_IIC_ADDRESS_M 0xe8

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

/******************************/
/*函数声明Function declaration*/
/******************************/
void SN3731_Init(void);     //完全初始化函数
/************************************************************************
*************************第一部分声明函数********************************
****************以下声明函数为被系统调用的I2C读写SN3731的函数************
************************************************************************/
void SN3731_WriteRegs(unsigned char RegFristAdd, unsigned char *s, unsigned char no);//连续写函数
void SN3731_WriteReg(unsigned char reg, unsigned char dat);							 //写指定寄存器函数
void SN3731_SelectSection(unsigned char MainAddress, unsigned char SectionReg);		 //选择页寄存器函数
unsigned char SN3731_ReadReg(unsigned char add);									 //读一个指定寄存器函数

/************************************************************************
*************************第二部分声明函数********************************
**********************以下声明函数为初始化函数***************************
************************************************************************/
void SN3731_Led_TurnOn_All(unsigned char i);	//LED全亮函数
void SN3731_Led_TurnOff_All(unsigned char i);	//LED全灭函数
void SN3731_blink_TurnOn_All(unsigned char i);	//LED闪烁全开
void SN3731_blink_TurnOff_All(unsigned char i); //LED闪烁全关
void SN3731_DeleteFrame_pwm(unsigned char i);	//仅清除pwm数据
void SN3731_DeleteFrame_OnOff(unsigned char i);	//仅清除onoff数据
void SN3731_DeleteFrame_blink(unsigned char i);	//仅清除blink数据								

/************************************************************************
*************************第三部分声明函数********************************
******************以下声明函数为控制寄存器的配置函数*********************
************************************************************************/

/*****************以下为0x00控制寄存器**********************************/
void SN3731_AudioFrame_EN(unsigned char i);	//开启audio_frame
void SN3731_Movie_EN(unsigned char i);		//开启movie,audio_frame优先级比movie高 
void SN3731_Set_StartFrame_Picture(unsigned char Start_Frame_Picture);//设置movie 模式播放的开始贞  

/*****************以下为0x01控制寄存器**********************************/
void SN3731_DisplayFrame(unsigned char i);//选择当前显示画面

/*****************以下为0x02控制寄存器**********************************/
void SN3731_Set_LoopNum(unsigned char loopnum);	 //movie播放次数
void SN3731_Set_FrameNum(unsigned char framenum);//movie播放的贞数

/*****************以下为0x03控制寄存器**********************************/
void SN3731_Set_FrameDelay(unsigned char A);//Delay between frame change in a movie

/*****************以下为0x05控制寄存器**********************************/
void SN3731_BlinkEN(unsigned char b_en);				//blink enable function
void SN3731_Blink_Brightness(unsigned char brightness); //闪烁灰度值配置函数
void SN3731_Blink_period(unsigned char BPT);			//配置闪烁周期

/*****************以下为0x06控制寄存器**********************************/
void SN3731_AudioSyn(unsigned char a);//audio同步

/*****************以下为0x08控制寄存器**********************************/
void SN3731_Breath_RampUp(unsigned char rampup);	//渐亮时间
void SN3731_Breath_RampDown(unsigned char rampdown);//渐灭时间

/*****************以下为0x09控制寄存器**********************************/
void SN3731_BreathEN(unsigned char a);			//呼吸使能
void SN3731_Breath_extinguish(unsigned char ET);//设置呼吸熄灭时间

/*****************以下为0x0a控制寄存器**********************************/
void SN3731_SoftWareEn(unsigned char sw_en);//软件解除SD

/*****************以下为0x0b控制寄存器**********************************/
void SN3731_Audio_AGCEN(unsigned char agc);		  //开启AGC
void SN3731_Audio_AGCMode(unsigned char agc_mode);//0:慢速AGC,1:快速AGC
void SN3731_Audio_Gain(unsigned char gain);		  //gain  000   001   010   011  100  101   110   111
												  //0dB  -3dB   3dB   6dB  9dB  12dB  15dB  18dB

/*****************以下为0x0c控制寄存器**********************************/
void SN3731_Audio_SampleTime(unsigned char AAR);//采样率 AAR=t*A,t=77uS


/*****************以下为4贞4贞写动画的函数**********************************/
void SN3731_PingPang_BufferWrite(unsigned char Movie_Name, unsigned char delay_time);

/*****************以下函数为选择主从寄存器的I2C地址**********************************/
void Led_SelectChip(unsigned char i2c_addr);


/*****************以下两个函数为点亮或者熄灭指定位置LED的函数************************/
void SN3731_LedTurnOnOff_WriteInSequence(unsigned char position, unsigned char OnOFF_State, unsigned char Pwm_Intensity);
void SN3731_LedTurnOnOff_WriteInMatrix(unsigned char row, unsigned char col, unsigned char OnOFF_State, unsigned char Pwm_Intensity);//A=0~255

/*****************以下两个函数为使能指定位置LED闪烁的函数************************/
void SN3731_LedBlinkTurnOnOff_WrtieInSequence(unsigned char SequencePosition, unsigned char Blink_OnOFF);
void SN3731_LedBlinkTurnOnOff_WrtieInMatrix(unsigned char row, unsigned char col, unsigned char Blink_OnOFF);

/*****************以下两个函数为写8幅画到8frame中的函数************************/
void SN3731_Write8Picture_Into8FrameReg(unsigned char static_picture_name);

/*****************以下两个函数为装载动画数据函数************************/
void load(unsigned char movie_name);

/*************************************************************************************
*	//函数名    : void SN3731_DEMO(unsigned char demo_NO)
*	//功能      : 本函数是实现多贞动画函数，
: 采用4贞ping-pang buffer方式实现
*************************************************************************************/
void SN3731_DEMO(unsigned char demo_NO);
/****************************/
/*函数定义function definetion*/
/****************************/

/********************************/
/*外部调用_宏定义macro definition*/
/********************************/

/*********************************/
/*外部调用_类型定义byte definition*/
/*********************************/

/************************************/
/*外部调用_标志位定义flags definetion*/
/************************************/
extern unsigned char ack;	     //应答标志位
/*************************************/
/*外部调用_变量定义variable definition*/
/*************************************/
extern unsigned char g_SN3731_IIC_ADDRESS;
/**********************************/
/*外部调用_数组定义array definition*/
/**********************************/

/**************************************/
/*外部调用_函数声明Function declaration*/
/**************************************/

/*************************************/
/*外部调用_函数定义function definetion*/
/*************************************/


#endif











