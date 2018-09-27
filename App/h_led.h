#ifndef __H_LED_H__
#define __H_LED_H__

//#include "h_funtion.h"				//
#include "h_adc.h"						//adc_scan()
//#include "h_sk6812.h"					//LED����
#include "stdlib.h"						//abs

#define	LED_COLUMNS				(10)//-----------------------10��
#define	LED_ROWS					(6)//------------------------6��
#define	LED_HALF_ROWS			(3)//------------------------����֮һ
#define	LED_ONE_THIRD			(2)//------------------------����֮һ
#define LED_NUM						(LED_COLUMNS*LED_ROWS)//-----�ܵ���
#define COLOR_TYPE				(3)//------------------------һ��LED���ֻ�����ɫ

#define STATE_ON						1								//��
#define STATE_OFF						0								//��

#define NEXT								1								//��һ��
#define PRES								0								//��һ��
	
#define _FORWARD						1								//�������
#define BACKWARD						0								//�������
	
//#define STATE_INITIAL				0								//״̬��ʼ��
//#define STATE_REFLASH				1								//״̬��ˢ��

#define VOLUME_MUTE					(-1)						//����
#define VOLUME_QUASI_MUTE		(0)							//׼����

#define CURRENT							(1)							//������������������λ
#define PREVIOUS						(0)							//ǰһ����������������λ


#define G_BIT 							(0)							//����ɫ��ɫ
#define R_BIT 							(1)							//����ɫ��ɫ
#define B_BIT								(2)							//����ɫ��ɫ
#define H_BIT								B_BIT						//��λ

#define NO_COLOR 						(0)
#define GREEN_COLOR 				((0X01<<G_BIT))									//1��ɫ
#define RED_COLOR 					((0X01<<R_BIT))									//2��ɫ
#define YELLOW_COLOR				((0X01<<R_BIT)+(0X01<<G_BIT))		//3��ɫ
#define BLUE_COLOR 					((0X01<<B_BIT))									//4��ɫ
#define CYAN_COLOR 					((0X01<<G_BIT)+(0X01<<B_BIT))		//5��ɫ
#define PURPLE_COLOR				((0X01<<R_BIT)+(0X01<<B_BIT))		//6��ɫ
#define WHITE_COLOR					((0X01<<R_BIT)+(0X01<<G_BIT)+(0X01<<B_BIT))//7��ɫ

#define COLOR_MIN						GREEN_COLOR			//1
#define COLOR_MAX						WHITE_COLOR			//7

#define DEFAULT_COLOR				GREEN_COLOR			//Ĭ����ɫ				

#define BRIGHTNESS_MUTE			0X0F//----------��������
#define BRIGHTNESS_OFF			0//-------------���ȹ�
#define BRIGHTNESS_MIN			0//-------------������С
#define BRIGHTNESS_MAX 			0XFF//-----------�������

#define VOLUME_MUTE					(-1)						//����
#define VOLUME_QUASI_MUTE		(0)							//׼����

#define BRIGHTNESS_STEP			(BRIGHTNESS_MAX/LED_ROWS)							//255/6
#define VOL_TAB_LENGH				10


#define LED_MODE_MIN				RAINBOW_BREATH	//ģʽ��Сֵ�����ڿ���ģʽ�л�����ֵ
#define LED_MODE_MAX				OPTICAL_DISK		//ģʽ���ֵ�����ڿ���ģʽ�л�����ֵ
#define MODE_NUM 						(LED_MODE_MAX-LED_MODE_MIN+1+1)//ģʽ����������������ģʽ�������ر�ģʽ

#define MUTE_TIMES					2000						//�����������ڴ���������1ms��2000ms
#define RENEW_TIMES					10							//ˢ�£�����6ms��1800ms


#define	VOLUME_STEP					(LED_ROWS+1)		//��������
#define FLASH_PER_STEP			(42)						//BRIGHTNESS_MAX/(VOLUME_STEP-1)=255/6

#define LED_TEAM						(3)

typedef struct freq_struct//----------Ƶ�ʽṹ��
{
	uint16_t		count;//----------------��������
	uint16_t		period;//---------------��������
}freqTypeDef;
//



typedef enum//---------------------ʱ��ö��
{
	
	TIME_UP=0,//ʱ�䵽��
	NOT_REACHED//ʱ��δ��
}Time_State;
//


typedef enum
{
	RAINBOW_BREATH=0,			//�ʺ�����Զ������ɫ0
	PARITY_BREATH,				//ө������Զ������ɫ1
	ENTIRE_BREATH,				//��������Զ������ɫ2
	HALF_PULSE_FLASH,			//�԰������Զ������ɫ3
	PULSE_FLASH,					//���������Զ������ɫ4
	MUSIC_FLASH,					//������������˸5
	HALF_CHASE,						//�԰�����Զ������ɫ6
	ENTIRE_CHASE,					//��Ȧ����Զ������ɫ7
	OPTICAL_DISK,					//������ɫ����8
	LED_MODE_OFF,					//�ر�ģʽ
	
}led_mode;
//

typedef struct led_struct//------------------LED�ṹ��
{
	uint8_t		brightness;//--------------------����ֵ//0-255
//	uint8_t		color;//-------------------------��ɫ���//B,R,BR,G,GB,GR,GRB
//	uint8_t		direction;//---------------------�������//�����Լ�
//	uint32_t 	grb_value;//---------------------��ͨ����ɫֵ
 
}ledTypeDef;
//




extern uint8_t current_led_mode;//------------��ǰLEDģʽ����
extern uint8_t previous_led_mode;//-----------ǰһ��LEDģʽ���������ڱ���

//extern void	save_led_mode(void);//----------����LEDģʽ
extern void led_handle_process(void);//-------LED�������
extern void switch_led_mode(uint8_t);//-------�л�LEDģʽ
extern void switch_led_color(uint8_t);//------�л�LED��ɫ
//extern void switch_led_brightness(bool);
extern void	switch_power(void);
extern void	init_led(void);//-----------------��ʼ��LED
extern void close_all_led(void);//------------OFF
extern void pulse_follow_volume(void);
extern void pulse_follow_volume_2(void);
#endif

