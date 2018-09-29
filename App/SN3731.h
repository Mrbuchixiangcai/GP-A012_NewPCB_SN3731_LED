/**********************************/
/*ͷ�ļ�����header file definition*/
/**********************************/
#ifndef  __SN3731_H__
#define  __SN3731_H__

/*******************/
/*ͷ�ļ�header file*/
/*******************/
#include "app_main.h"

/************************/
/*�궨��macro definition*/
/************************/
#define	SN3731_DELAY	0x00		//����ʱ������Ҫ������Ӧ�ĵ���
//#define g_SN3731_IIC_ADDRESS  SN3731_IIC_ADDRESS_M  //������ַ,#define	SN3731_IIC_ADDRESS_M 0xe8

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
void SN3731_Init(void);     //��ȫ��ʼ������
/************************************************************************
*************************��һ������������********************************
****************������������Ϊ��ϵͳ���õ�I2C��дSN3731�ĺ���************
************************************************************************/
void SN3731_WriteRegs(unsigned char RegFristAdd, unsigned char *s, unsigned char no);//����д����
void SN3731_WriteReg(unsigned char reg, unsigned char dat);							 //дָ���Ĵ�������
void SN3731_SelectSection(unsigned char MainAddress, unsigned char SectionReg);		 //ѡ��ҳ�Ĵ�������
unsigned char SN3731_ReadReg(unsigned char add);									 //��һ��ָ���Ĵ�������

/************************************************************************
*************************�ڶ�������������********************************
**********************������������Ϊ��ʼ������***************************
************************************************************************/
void SN3731_Led_TurnOn_All(unsigned char i);	//LEDȫ������
void SN3731_Led_TurnOff_All(unsigned char i);	//LEDȫ����
void SN3731_blink_TurnOn_All(unsigned char i);	//LED��˸ȫ��
void SN3731_blink_TurnOff_All(unsigned char i); //LED��˸ȫ��
void SN3731_DeleteFrame_pwm(unsigned char i);	//�����pwm����
void SN3731_DeleteFrame_OnOff(unsigned char i);	//�����onoff����
void SN3731_DeleteFrame_blink(unsigned char i);	//�����blink����								

/************************************************************************
*************************����������������********************************
******************������������Ϊ���ƼĴ��������ú���*********************
************************************************************************/

/*****************����Ϊ0x00���ƼĴ���**********************************/
void SN3731_AudioFrame_EN(unsigned char i);	//����audio_frame
void SN3731_Movie_EN(unsigned char i);		//����movie,audio_frame���ȼ���movie�� 
void SN3731_Set_StartFrame_Picture(unsigned char Start_Frame_Picture);//����movie ģʽ���ŵĿ�ʼ��  

/*****************����Ϊ0x01���ƼĴ���**********************************/
void SN3731_DisplayFrame(unsigned char i);//ѡ��ǰ��ʾ����

/*****************����Ϊ0x02���ƼĴ���**********************************/
void SN3731_Set_LoopNum(unsigned char loopnum);	 //movie���Ŵ���
void SN3731_Set_FrameNum(unsigned char framenum);//movie���ŵ�����

/*****************����Ϊ0x03���ƼĴ���**********************************/
void SN3731_Set_FrameDelay(unsigned char A);//Delay between frame change in a movie

/*****************����Ϊ0x05���ƼĴ���**********************************/
void SN3731_BlinkEN(unsigned char b_en);				//blink enable function
void SN3731_Blink_Brightness(unsigned char brightness); //��˸�Ҷ�ֵ���ú���
void SN3731_Blink_period(unsigned char BPT);			//������˸����

/*****************����Ϊ0x06���ƼĴ���**********************************/
void SN3731_AudioSyn(unsigned char a);//audioͬ��

/*****************����Ϊ0x08���ƼĴ���**********************************/
void SN3731_Breath_RampUp(unsigned char rampup);	//����ʱ��
void SN3731_Breath_RampDown(unsigned char rampdown);//����ʱ��

/*****************����Ϊ0x09���ƼĴ���**********************************/
void SN3731_BreathEN(unsigned char a);			//����ʹ��
void SN3731_Breath_extinguish(unsigned char ET);//���ú���Ϩ��ʱ��

/*****************����Ϊ0x0a���ƼĴ���**********************************/
void SN3731_SoftWareEn(unsigned char sw_en);//������SD

/*****************����Ϊ0x0b���ƼĴ���**********************************/
void SN3731_Audio_AGCEN(unsigned char agc);		  //����AGC
void SN3731_Audio_AGCMode(unsigned char agc_mode);//0:����AGC,1:����AGC
void SN3731_Audio_Gain(unsigned char gain);		  //gain  000   001   010   011  100  101   110   111
												  //0dB  -3dB   3dB   6dB  9dB  12dB  15dB  18dB

/*****************����Ϊ0x0c���ƼĴ���**********************************/
void SN3731_Audio_SampleTime(unsigned char AAR);//������ AAR=t*A,t=77uS


/*****************����Ϊ4��4��д�����ĺ���**********************************/
void SN3731_PingPang_BufferWrite(unsigned char Movie_Name, unsigned char delay_time);

/*****************���º���Ϊѡ�����ӼĴ�����I2C��ַ**********************************/
void Led_SelectChip(unsigned char i2c_addr);


/*****************������������Ϊ��������Ϩ��ָ��λ��LED�ĺ���************************/
void SN3731_LedTurnOnOff_WriteInSequence(unsigned char position, unsigned char OnOFF_State, unsigned char Pwm_Intensity);
void SN3731_LedTurnOnOff_WriteInMatrix(unsigned char row, unsigned char col, unsigned char OnOFF_State, unsigned char Pwm_Intensity);//A=0~255

/*****************������������Ϊʹ��ָ��λ��LED��˸�ĺ���************************/
void SN3731_LedBlinkTurnOnOff_WrtieInSequence(unsigned char SequencePosition, unsigned char Blink_OnOFF);
void SN3731_LedBlinkTurnOnOff_WrtieInMatrix(unsigned char row, unsigned char col, unsigned char Blink_OnOFF);

/*****************������������Ϊд8������8frame�еĺ���************************/
void SN3731_Write8Picture_Into8FrameReg(unsigned char static_picture_name);

/*****************������������Ϊװ�ض������ݺ���************************/
void load(unsigned char movie_name);

/*************************************************************************************
*	//������    : void SN3731_DEMO(unsigned char demo_NO)
*	//����      : ��������ʵ�ֶ��궯��������
: ����4��ping-pang buffer��ʽʵ��
*************************************************************************************/
void SN3731_DEMO(unsigned char demo_NO);
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
extern unsigned char ack;	     //Ӧ���־λ
/*************************************/
/*�ⲿ����_��������variable definition*/
/*************************************/
extern unsigned char g_SN3731_IIC_ADDRESS;
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











