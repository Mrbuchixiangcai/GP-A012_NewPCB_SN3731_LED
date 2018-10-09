/*****************************************************************************
*�汾���� (C)2011 ����΢���ӣ����ţ����޹�˾
*****************************************************************************
*  ģ����	    ��
*  �ļ���	    ��SN3731drv.h
*  �ļ���ʶ    ��
*  ����ļ�		��
*  ʵ�ֹ���    �����ļ�������ϵͳ���õ�API�����ľ��嶨��
*  ����		��
*  �汾		��V1.0
*  �������	��20111114
*  ����˵��	��
*****************************************************************************/

/*****************************************************************************
*  �Ĵ�����ܼ�����:
*	SN3731�ܹ��оŸ��Ĵ�������,ÿ������ĵ�ַ���� ��0x00-0xff����
*	ͨ��һ������ַ��Ѱַ����FDh, ��FDhд0bhʱ��ָ����ƼĴ�����ַ�ռ䣬
*	��ʱ��0x00-0xff���Ĵ��ַֻ��0x00-0x0c��Ч;
*	��FDhд0-7ʱ,ת��0-7��ĵ�ַ�ռ䣬����0x00-0x11Ϊ����Ĵ�����
*	0x12-0x23Ϊ��˸�Ĵ�����0x24-0xB3λpwm���ݼĴ�����
*****************************************************************************/


/**********************************/
/*ͷ�ļ�����header file definition*/
/**********************************/
#ifndef  __SN3731_IO_IIC_H__
#define  __SN3731_IO_IIC_H__

/*******************/
/*ͷ�ļ�header file*/
/*******************/
#include "app_main.h"

/************************/
/*�궨��macro definition*/
/************************/
#define  SN3731_SDA5_IN()    {GPIOA->MODER &= (~((u32)0x03<<(5*2 )));GPIOA->MODER |= ((u32)00<<(5*2 ));}//GPIOA_Pin5����Ϊ����
#define  SN3731_SDA5_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(5*2 )));GPIOA->MODER |= ((u32)01<<(5*2 ));}//GPIOA_Pin5����Ϊ���
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

#define  SN3731_READ_SDA5()   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)//��ȡPA5�ĵ�ƽ״̬
#define  SN3731_READ_SDA6()   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define  SN3731_READ_INTB()   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)							


#define Register_MainAddress	0xFD//�Ĵ�������ַ

///////����Ӧ��2��3731�ĵ�ʱ����Ҫ����ADDR_M��ADDR_S////////////////////////////////////////////////////////

/************************************/
// SN3731�Ĵӵ�ַ:
// AdPin Connect to GND 0xE8
// AdPin Connect to VDD 0xEE
// AdPin Connect to SCL 0xEA
// AdPin Connect to SDA 0xEC
/************************************/

#define	SN3731_IIC_ADDRESS_M   	0xe8 		//SN3731�Ĵӵ�ַ,��ο�DATASHEET��AD0��GND(0xe8),VDD(0xee)��SCL(0xea)��SDA(0xec)
#define	SN3731_IIC_ADDRESS_S  	0xe8 		//SN3731�Ĵӵ�ַ,��ο�DATASHEET��AD0��GND(0xe8),VDD(0xee)��SCL(0xea)��SDA(0xec)
#define	SN_IIC_ACK				0  			//0	//1��ҪACK�źŵĻ���0 ����ACK�źŵĻ�
#define STC_MCU_ACK            	1			//0	//1��ҪACK�źŵĻ���0 ����ACK�źŵĻ� ,�ڳ�����STC����ACK������ƽ̨�Ĳ�һ��

//#define STC_MCU
//#define MOBILE_PLATEFORM_MTK625X 0  	//�����MTKƽ̨����򿪴˺�
//#define MOBILE_PLATEFORM_SC66X0  	 	//�����Spreadturmƽ̨����򿪴˺�

#ifdef MOBILE_PLATEFORM_MTK625X

	#define	SN_SDA_PIN_High()	GPIO_WriteIO(1,SN_SDA_PIN)  	//DATA=1	//data�������.
	#define	SN_SDA_PIN_Low()	GPIO_WriteIO(0,SN_SDA_PIN)  	//DATA=0	//data�������.
	#define	SN_SCL_PIN_High()	GPIO_WriteIO(1,SN_SCL_PIN)  	//CLK=1		//clock�������.
	#define	SN_SCL_PIN_Low()	GPIO_WriteIO(0,SN_SCL_PIN)  	//CLK=0		//clock�������.

	#define	SN_SDB_PIN_High()	GPIO_WriteIO(1,SN_SDB_PIN)	//chip shut down�������.
	#define	SN_SDB_PIN_Low()	GPIO_WriteIO(0,SN_SDB_PIN)	//chip shut down�������.

	#define	SN_SDA_SET_INPUT()      //��Ϊ����
	#define	SN_SDA_SET_OUTPUT()     //��Ϊ���

#endif

#ifdef MOBILE_PLATEFORM_SC66X0

	#define	SN_SDA_PIN_High()	GPIO_SetValue(SN_SDA_PIN,1)  	//DATA=1       //data�������.
	#define	SN_SDA_PIN_Low()	GPIO_SetValue(SN_SDA_PIN,0)  	//DATA=0	//data�������.
	#define	SN_SCL_PIN_High()	GPIO_SetValue(SN_SCL_PIN,1)  	//CLK=1	//clock�������.
	#define	SN_SCL_PIN_Low()	GPIO_SetValue(SN_SCL_PIN,0)  	//CLK=0	//clock�������.

	#define	SN_SDB_PIN_High()	GPIO_SetValue(SN_SDB_PIN,1)	//chip shut down�������.
	#define	SN_SDB_PIN_Low()	GPIO_SetValue(SN_SDB_PIN,0)	//chip shut down�������.

	#define	SN_SDA_SET_INPUT()      	//��Ϊ����
	#define	SN_SDA_SET_OUTPUT()     //��Ϊ���

#endif

#ifdef MOBILE_PLATEFORM_MSTAR
	#define   SN_SDA_PIN   		PAD_NUM_BT_GPIO1_GPIO_8_OUT  	//SYS GPIO ����GPIO���,��ο�ԭ��ͼ
	#define   SN_SCL_PIN   		PAD_NUM_BT_GPIO2_GPIO_9_OUT 	 //SYS GPIO ����GPIO���,��ο�ԭ��ͼ
	#define   SN_SDB_PIN   		PAD_NUM_I2SBCLK_GPIO_65_OUT  	//SYS GPIO ����GPIO���,��ο�ԭ��ͼ

	#define	SN_SCL_PIN_High()      DrvIoChipUpGpio(SN_SCL_PIN)
	#define	SN_SCL_PIN_Low()       DrvIoChipDwnGpio(SN_SCL_PIN)
	#define	SN_SDA_PIN_High()     DrvIoChipUpGpio(SN_SDA_PIN)
	#define	SN_SDA_PIN_Low()      DrvIoChipDwnGpio(SN_SDA_PIN)
	#define	SN_SDB_PIN_High()     DrvIoChipUpGpio(SN_SDB_PIN)
	#define	SN_SDB_PIN_Low()      DrvIoChipDwnGpio(SN_SDB_PIN)

	#define	SN_SDA_SET_INPUT()      //��Ϊ����
	#define	SN_SDA_SET_OUTPUT()     //��Ϊ���

#endif


#ifdef MOBILE_PLATEFORM_MTK625X
	extern void EINT_Registration(unsigned char eintno, char Dbounce_En, char ACT_Polarity, \
								  void (reg_hisr)(void), char auto_umask);
	extern unsigned long int EINT_SW_Debounce_Modify(unsigned char eintno, unsigned char debounce_time);
	extern void EINT_Mask(unsigned char eintno);
	extern void EINT_UnMask(unsigned char eintno);
#endif

#ifdef STC_MCU
	#define	SN_SDA_PIN_High()	DATA=1		//data�������.
	#define	SN_SDA_PIN_Low()	DATA=0		//data�������.
	#define	SN_SCL_PIN_High()	CLK=1		//clock�������.
	#define	SN_SCL_PIN_Low()	CLK=0		//clock�������.
#endif

//���ݼĴ�������

//����8���ͼƬ�Ĵ�����ַ��ѡ���һ�����ƼĴ�����ַ�ε�ѡ��
#define Reg_MainAddress			0xFD//�Ĵ�������ַ
//�Ĵ�����ַ��ѡ��(������������Ϊ1-8�������)
#define Reg_Section_Fram(i)		((i)-1)//i=1-8
#define Reg_Section_CTRLReg		0x0B		
//һ��SN3731���ؼĴ�������
//i=1-9
#define Reg_ON_OFF_CS        	0x00
#define Reg_ON_OFF_CSA(i)     	(Matrix_ON_OFF_CS+2*(i)-2)  //A�������б���
#define Reg_ON_OFF_CSB(i)     	(Matrix_ON_OFF_CS+2*(i)-1)	//B�������б���
//һ��SN3731��˸�Ĵ�������
//i=1-9
#define Reg_Blink_CS       		0x12
#define Reg_Blink_CSA(i)       	(Matrix_Blink_CS+2*(i)-2)	//A����˸�б���
#define Reg_Blink_CSB(i)      	(Matrix_Blink_CS+2*(i)-1)	//B����˸�б���
//һ��SN3731ÿ�ŵƻҶȼĴ�������
//1-144								
#define Reg_PWM(i)       		(0x25+(i)-2) //PWM��ַƫ�Ʊ���
//���ƼĴ�����ַ����
#define Reg_ModeConfig			0x00 	//ģʽ���üĴ���
#define Reg_PictureAddress		0x01	//ͼƬѡ��Ĵ���
#define Reg_FramePlay			0x02	//�����Զ����żĴ���
#define Reg_FrameTime			0x03	//������ʱ�Ĵ��� 
#define Reg_0x04H				0x04	//NC
#define Reg_BlinkCtrl			0x05	//��˸ģʽ�Ĵ���
#define Reg_AudioSync			0x06	//����ͬ���Ĵ���
#define Reg_StatusReg			0x07	//����״̬�Ĵ���(ֻ��)
#define Reg_BreathTime			0x08	//����ʱ�����üĴ���
#define Reg_Breath				0x09	//�������ƼĴ���
#define Reg_ShutDown			0x0A	//�ضϼĴ���
#define Reg_AGC					0x0B	//AGC���ƼĴ���
#define Reg_AudioADCRate		0x0C	//��Ƶ�����Ĵ���

//
#define Config_Mode_Picture     0x00
#define Config_Mode_Movie       (0x01<3)
#define Config_Mode_AudioSync   (0x02<3)
#define Software_Shutdown_Mode	0x00
#define Normal_Operation_Mode	0x01
//Start frame in Movie Mode and Picture Mode
//�� i frame
#define Config_Strat_Frame(i)    (Config_Strat_Frame+(i)-1)
//Num of Loop in Moive mode
//�� i frame
#define Config_LoopNum_Movie(i)  ((Config_LoopNum_Movie+(i))<3)
//Num of frame in Moive mode
//�� i frame
#define Config_FrameNum_Movie(i) (Config_FrameNum_Movie+(i))
//Delay between frame change in a movie
//i=0-63,��i=0��ʱ��������������ʱ��Ϊi*30ms,���Ϊ30ms*63=1.89s
#define Config_Delay_BetweenFrame_Movie(i)	(i)//2.394s

extern uint8_t g_SN3731_IIC_ADDRESS;

/*************************/
/*���Ͷ���byte definition*/
/*************************/

/****************************/
/*��־λ����flags definetion*/
/****************************/

/*****************************/
/*��������variable definition*/
/*****************************/

/**************************/
/*���鶨��array definition*/
/**************************/

/******************************/
/*��������Function declaration*/
/******************************/
void SN3731_IIC_Start(void);
void SN3731_IIC_Stop(void);
unsigned char SN3731_IIC_SDA_Read(void);
static void SN3731_IIC_SendByte(unsigned char c);
unsigned char SN3731_IIC_SelectSection(unsigned char MainAddress, unsigned char SectionReg);
unsigned char SN3731_IIC_WriteRegs(unsigned char reg, unsigned char *s, unsigned char no);
unsigned char SN3731_IIC_Write_Reg(unsigned char reg, unsigned char dat);
unsigned char SN3731_IIC_Read8Bit(void);				//����8��������λ
unsigned char SN3731_IIC_ReadReg(unsigned char add);
void SN3731_WriteRegs(unsigned char RegFristAdd, unsigned char *s, unsigned char no);
void SN3731_WriteReg(unsigned char reg, unsigned char dat);
void SN3731_SelectSection(unsigned char MainAddress, unsigned char SectionReg);
unsigned char SN3731_ReadReg(unsigned char add);

/****************************/
/*��������function definetion*/
/****************************/

/********************************/
/*�ⲿ����_�궨��macro definition*/
/********************************/

/*********************************/
/*�ⲿ����_���Ͷ���byte definition*/
/*********************************/

/************************************/
/*�ⲿ����_��־λ����flags definetion*/
/************************************/

/*************************************/
/*�ⲿ����_��������variable definition*/
/*************************************/

/**********************************/
/*�ⲿ����_���鶨��array definition*/
/**********************************/

/**************************************/
/*�ⲿ����_��������Function declaration*/
/**************************************/

/*************************************/
/*�ⲿ����_��������function definetion*/
/*************************************/

#endif 










