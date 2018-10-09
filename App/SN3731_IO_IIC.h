/*****************************************************************************
*版本所有 (C)2011 矽恩微电子（厦门）有限公司
*****************************************************************************
*  模块名	    ：
*  文件名	    ：SN3731drv.h
*  文件标识    ：
*  相关文件		：
*  实现功能    ：本文件包含被系统调用的API函数的具体定义
*  作者		：
*  版本		：V1.0
*  完成日期	：20111114
*  其他说明	：
*****************************************************************************/

/*****************************************************************************
*  寄存器框架简单描述:
*	SN3731总共有九个寄存器区域,每个区域的地址都是 （0x00-0xff），
*	通过一个主地址来寻址，即FDh, 当FDh写0bh时，指向控制寄存器地址空间，
*	这时（0x00-0xff）寄存地址只有0x00-0x0c有效;
*	当FDh写0-7时,转向0-7桢的地址空间，其中0x00-0x11为亮灭寄存器，
*	0x12-0x23为闪烁寄存器，0x24-0xB3位pwm数据寄存器。
*****************************************************************************/


/**********************************/
/*头文件定义header file definition*/
/**********************************/
#ifndef  __SN3731_IO_IIC_H__
#define  __SN3731_IO_IIC_H__

/*******************/
/*头文件header file*/
/*******************/
#include "app_main.h"

/************************/
/*宏定义macro definition*/
/************************/
#define  SN3731_SDA5_IN()    {GPIOA->MODER &= (~((u32)0x03<<(5*2 )));GPIOA->MODER |= ((u32)00<<(5*2 ));}//GPIOA_Pin5设置为输入
#define  SN3731_SDA5_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(5*2 )));GPIOA->MODER |= ((u32)01<<(5*2 ));}//GPIOA_Pin5设置为输出
#define  SN3731_SDA6_IN()    {GPIOA->MODER &= (~((u32)0x03<<(6*2 )));GPIOA->MODER |= ((u32)00<<(6*2 ));}
#define  SN3731_SDA6_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(6*2 )));GPIOA->MODER |= ((u32)01<<(6*2 ));}
#define  SN3731_SDA7_IN()    {GPIOA->MODER &= (~((u32)0x03<<(7*2 )));GPIOA->MODER |= ((u32)00<<(7*2 ));}

#define  SN3731_IIC_DA5(a)	if(a)    					\
								GPIOA->BSRR=GPIO_PIN_5; \
							else     					\
								GPIOA->BRR=GPIO_PIN_5 
#define  SN3731_IIC_DA6(a)  if(a)    					\
								GPIOA->BSRR=GPIO_PIN_6; \
							else     					\
								GPIOA->BRR=GPIO_PIN_6
#define  SN3731_IIC_CLK(a)  if(a)    					\
								GPIOB->BSRR=GPIO_PIN_0; \
							else     					\
								GPIOB->BRR=GPIO_PIN_0

#define  SN3731_READ_SDA5()   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)//读取PA5的电平状态
#define  SN3731_READ_SDA6()   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define  SN3731_READ_INTB()   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)							


#define Register_MainAddress	0xFD//寄存器主地址

///////级联应用2颗3731的的时候需要定义ADDR_M和ADDR_S////////////////////////////////////////////////////////

/************************************/
// SN3731的从地址:
// AdPin Connect to GND 0xE8
// AdPin Connect to VDD 0xEE
// AdPin Connect to SCL 0xEA
// AdPin Connect to SDA 0xEC
/************************************/

#define	SN3731_IIC_ADDRESS_M   	0xe8 		//SN3731的从地址,请参考DATASHEET，AD0接GND(0xe8),VDD(0xee)，SCL(0xea)，SDA(0xec)
#define	SN3731_IIC_ADDRESS_S  	0xe8 		//SN3731的从地址,请参考DATASHEET，AD0接GND(0xe8),VDD(0xee)，SCL(0xea)，SDA(0xec)
#define	SN_IIC_ACK				0  			//0	//1需要ACK信号的话，0 忽略ACK信号的话
#define STC_MCU_ACK            	1			//0	//1需要ACK信号的话，0 忽略ACK信号的话 ,在程序中STC测试ACK与其他平台的不一样

//#define STC_MCU
//#define MOBILE_PLATEFORM_MTK625X 0  	//如果是MTK平台，请打开此宏
//#define MOBILE_PLATEFORM_SC66X0  	 	//如果是Spreadturm平台，请打开此宏

#ifdef MOBILE_PLATEFORM_MTK625X

	#define	SN_SDA_PIN_High()	GPIO_WriteIO(1,SN_SDA_PIN)  	//DATA=1	//data线输出高.
	#define	SN_SDA_PIN_Low()	GPIO_WriteIO(0,SN_SDA_PIN)  	//DATA=0	//data线输出低.
	#define	SN_SCL_PIN_High()	GPIO_WriteIO(1,SN_SCL_PIN)  	//CLK=1		//clock线输出高.
	#define	SN_SCL_PIN_Low()	GPIO_WriteIO(0,SN_SCL_PIN)  	//CLK=0		//clock线输出低.

	#define	SN_SDB_PIN_High()	GPIO_WriteIO(1,SN_SDB_PIN)	//chip shut down线输出高.
	#define	SN_SDB_PIN_Low()	GPIO_WriteIO(0,SN_SDB_PIN)	//chip shut down线输出低.

	#define	SN_SDA_SET_INPUT()      //置为输入
	#define	SN_SDA_SET_OUTPUT()     //置为输出

#endif

#ifdef MOBILE_PLATEFORM_SC66X0

	#define	SN_SDA_PIN_High()	GPIO_SetValue(SN_SDA_PIN,1)  	//DATA=1       //data线输出高.
	#define	SN_SDA_PIN_Low()	GPIO_SetValue(SN_SDA_PIN,0)  	//DATA=0	//data线输出低.
	#define	SN_SCL_PIN_High()	GPIO_SetValue(SN_SCL_PIN,1)  	//CLK=1	//clock线输出高.
	#define	SN_SCL_PIN_Low()	GPIO_SetValue(SN_SCL_PIN,0)  	//CLK=0	//clock线输出低.

	#define	SN_SDB_PIN_High()	GPIO_SetValue(SN_SDB_PIN,1)	//chip shut down线输出高.
	#define	SN_SDB_PIN_Low()	GPIO_SetValue(SN_SDB_PIN,0)	//chip shut down线输出低.

	#define	SN_SDA_SET_INPUT()      	//置为输入
	#define	SN_SDA_SET_OUTPUT()     //置为输出

#endif

#ifdef MOBILE_PLATEFORM_MSTAR
	#define   SN_SDA_PIN   		PAD_NUM_BT_GPIO1_GPIO_8_OUT  	//SYS GPIO 具体GPIO编号,请参考原理图
	#define   SN_SCL_PIN   		PAD_NUM_BT_GPIO2_GPIO_9_OUT 	 //SYS GPIO 具体GPIO编号,请参考原理图
	#define   SN_SDB_PIN   		PAD_NUM_I2SBCLK_GPIO_65_OUT  	//SYS GPIO 具体GPIO编号,请参考原理图

	#define	SN_SCL_PIN_High()      DrvIoChipUpGpio(SN_SCL_PIN)
	#define	SN_SCL_PIN_Low()       DrvIoChipDwnGpio(SN_SCL_PIN)
	#define	SN_SDA_PIN_High()     DrvIoChipUpGpio(SN_SDA_PIN)
	#define	SN_SDA_PIN_Low()      DrvIoChipDwnGpio(SN_SDA_PIN)
	#define	SN_SDB_PIN_High()     DrvIoChipUpGpio(SN_SDB_PIN)
	#define	SN_SDB_PIN_Low()      DrvIoChipDwnGpio(SN_SDB_PIN)

	#define	SN_SDA_SET_INPUT()      //置为输入
	#define	SN_SDA_SET_OUTPUT()     //置为输出

#endif


#ifdef MOBILE_PLATEFORM_MTK625X
	extern void EINT_Registration(unsigned char eintno, char Dbounce_En, char ACT_Polarity, \
								  void (reg_hisr)(void), char auto_umask);
	extern unsigned long int EINT_SW_Debounce_Modify(unsigned char eintno, unsigned char debounce_time);
	extern void EINT_Mask(unsigned char eintno);
	extern void EINT_UnMask(unsigned char eintno);
#endif

#ifdef STC_MCU
	#define	SN_SDA_PIN_High()	DATA=1		//data线输出高.
	#define	SN_SDA_PIN_Low()	DATA=0		//data线输出低.
	#define	SN_SCL_PIN_High()	CLK=1		//clock线输出高.
	#define	SN_SCL_PIN_Low()	CLK=0		//clock线输出低.
#endif

//数据寄存器定义

//包括8桢的图片寄存器地址段选择和一个控制寄存器地址段的选择
#define Reg_MainAddress			0xFD//寄存器主地址
//寄存器地址段选择(定义数据区域为1-8桢的数据)
#define Reg_Section_Fram(i)		((i)-1)//i=1-8
#define Reg_Section_CTRLReg		0x0B		
//一幅SN3731开关寄存器定义
//i=1-9
#define Reg_ON_OFF_CS        	0x00
#define Reg_ON_OFF_CSA(i)     	(Matrix_ON_OFF_CS+2*(i)-2)  //A屏亮灭行变量
#define Reg_ON_OFF_CSB(i)     	(Matrix_ON_OFF_CS+2*(i)-1)	//B屏亮灭行变量
//一幅SN3731闪烁寄存器定义
//i=1-9
#define Reg_Blink_CS       		0x12
#define Reg_Blink_CSA(i)       	(Matrix_Blink_CS+2*(i)-2)	//A屏闪烁行变量
#define Reg_Blink_CSB(i)      	(Matrix_Blink_CS+2*(i)-1)	//B屏闪烁行变量
//一幅SN3731每颗灯灰度寄存器定义
//1-144								
#define Reg_PWM(i)       		(0x25+(i)-2) //PWM地址偏移变量
//控制寄存器地址定义
#define Reg_ModeConfig			0x00 	//模式配置寄存器
#define Reg_PictureAddress		0x01	//图片选择寄存器
#define Reg_FramePlay			0x02	//画面自动播放寄存器
#define Reg_FrameTime			0x03	//画面延时寄存器 
#define Reg_0x04H				0x04	//NC
#define Reg_BlinkCtrl			0x05	//闪烁模式寄存器
#define Reg_AudioSync			0x06	//音乐同步寄存器
#define Reg_StatusReg			0x07	//画面状态寄存器(只读)
#define Reg_BreathTime			0x08	//呼吸时间设置寄存器
#define Reg_Breath				0x09	//呼吸控制寄存器
#define Reg_ShutDown			0x0A	//关断寄存器
#define Reg_AGC					0x0B	//AGC控制寄存器
#define Reg_AudioADCRate		0x0C	//音频采样寄存器

//
#define Config_Mode_Picture     0x00
#define Config_Mode_Movie       (0x01<3)
#define Config_Mode_AudioSync   (0x02<3)
#define Software_Shutdown_Mode	0x00
#define Normal_Operation_Mode	0x01
//Start frame in Movie Mode and Picture Mode
//第 i frame
#define Config_Strat_Frame(i)    (Config_Strat_Frame+(i)-1)
//Num of Loop in Moive mode
//第 i frame
#define Config_LoopNum_Movie(i)  ((Config_LoopNum_Movie+(i))<3)
//Num of frame in Moive mode
//第 i frame
#define Config_FrameNum_Movie(i) (Config_FrameNum_Movie+(i))
//Delay between frame change in a movie
//i=0-63,当i=0，时间最大，其他情况下时间为i*30ms,最大为30ms*63=1.89s
#define Config_Delay_BetweenFrame_Movie(i)	(i)//2.394s

extern uint8_t g_SN3731_IIC_ADDRESS;

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
void SN3731_IIC_Start(void);
void SN3731_IIC_Stop(void);
unsigned char SN3731_IIC_SDA_Read(void);
static void SN3731_IIC_SendByte(unsigned char c);
unsigned char SN3731_IIC_SelectSection(unsigned char MainAddress, unsigned char SectionReg);
unsigned char SN3731_IIC_WriteRegs(unsigned char reg, unsigned char *s, unsigned char no);
unsigned char SN3731_IIC_Write_Reg(unsigned char reg, unsigned char dat);
unsigned char SN3731_IIC_Read8Bit(void);				//读出8个二进制位
unsigned char SN3731_IIC_ReadReg(unsigned char add);
void SN3731_WriteRegs(unsigned char RegFristAdd, unsigned char *s, unsigned char no);
void SN3731_WriteReg(unsigned char reg, unsigned char dat);
void SN3731_SelectSection(unsigned char MainAddress, unsigned char SectionReg);
unsigned char SN3731_ReadReg(unsigned char add);

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

/*************************************/
/*外部调用_变量定义variable definition*/
/*************************************/

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










