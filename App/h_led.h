#ifndef __H_LED_H__
#define __H_LED_H__

//#include "h_funtion.h"				//
#include "h_adc.h"						//adc_scan()
//#include "h_sk6812.h"					//LED驱动
#include "stdlib.h"						//abs

#define	LED_COLUMNS				(10)//-----------------------10列
#define	LED_ROWS					(6)//------------------------6行
#define	LED_HALF_ROWS			(3)//------------------------二分之一
#define	LED_ONE_THIRD			(2)//------------------------三分之一
#define LED_NUM						(LED_COLUMNS*LED_ROWS)//-----总灯数
#define COLOR_TYPE				(3)//------------------------一个LED三种基本颜色

#define STATE_ON						1								//开
#define STATE_OFF						0								//关

#define NEXT								1								//下一个
#define PRES								0								//上一个
	
#define _FORWARD						1								//正向呼吸
#define BACKWARD						0								//反向呼吸
	
//#define STATE_INITIAL				0								//状态初始化
//#define STATE_REFLASH				1								//状态重刷新

#define VOLUME_MUTE					(-1)						//静音
#define VOLUME_QUASI_MUTE		(0)							//准静音

#define CURRENT							(1)							//最新音量的数组索引位
#define PREVIOUS						(0)							//前一个音量的数组索引位


#define G_BIT 							(0)							//三基色绿色
#define R_BIT 							(1)							//三基色红色
#define B_BIT								(2)							//三基色蓝色
#define H_BIT								B_BIT						//高位

#define NO_COLOR 						(0)
#define GREEN_COLOR 				((0X01<<G_BIT))									//1绿色
#define RED_COLOR 					((0X01<<R_BIT))									//2红色
#define YELLOW_COLOR				((0X01<<R_BIT)+(0X01<<G_BIT))		//3黄色
#define BLUE_COLOR 					((0X01<<B_BIT))									//4蓝色
#define CYAN_COLOR 					((0X01<<G_BIT)+(0X01<<B_BIT))		//5青色
#define PURPLE_COLOR				((0X01<<R_BIT)+(0X01<<B_BIT))		//6紫色
#define WHITE_COLOR					((0X01<<R_BIT)+(0X01<<G_BIT)+(0X01<<B_BIT))//7白色

#define COLOR_MIN						GREEN_COLOR			//1
#define COLOR_MAX						WHITE_COLOR			//7

#define DEFAULT_COLOR				GREEN_COLOR			//默认颜色				

#define BRIGHTNESS_MUTE			0X0F//----------静音亮度
#define BRIGHTNESS_OFF			0//-------------亮度关
#define BRIGHTNESS_MIN			0//-------------亮度最小
#define BRIGHTNESS_MAX 			0XFF//-----------亮度最大

#define VOLUME_MUTE					(-1)						//静音
#define VOLUME_QUASI_MUTE		(0)							//准静音

#define BRIGHTNESS_STEP			(BRIGHTNESS_MAX/LED_ROWS)							//255/6
#define VOL_TAB_LENGH				10


#define LED_MODE_MIN				RAINBOW_BREATH	//模式最小值，用于控制模式切换下限值
#define LED_MODE_MAX				OPTICAL_DISK		//模式最大值，用于控制模式切换上限值
#define MODE_NUM 						(LED_MODE_MAX-LED_MODE_MIN+1+1)//模式个数，包含上下限模式，包含关闭模式

#define MUTE_TIMES					2000						//静音计数周期次数，周期1ms，2000ms
#define RENEW_TIMES					10							//刷新，周期6ms，1800ms


#define	VOLUME_STEP					(LED_ROWS+1)		//音量阶数
#define FLASH_PER_STEP			(42)						//BRIGHTNESS_MAX/(VOLUME_STEP-1)=255/6

#define LED_TEAM						(3)

typedef struct freq_struct//----------频率结构体
{
	uint16_t		count;//----------------计数变量
	uint16_t		period;//---------------计数周期
}freqTypeDef;
//



typedef enum//---------------------时间枚举
{
	
	TIME_UP=0,//时间到了
	NOT_REACHED//时间未到
}Time_State;
//


typedef enum
{
	RAINBOW_BREATH=0,			//彩虹呼吸自动变幻颜色0
	PARITY_BREATH,				//萤火呼吸自动变幻颜色1
	ENTIRE_BREATH,				//整体呼吸自动变幻颜色2
	HALF_PULSE_FLASH,			//对半脉冲自动变幻颜色3
	PULSE_FLASH,					//整行脉冲自动变幻颜色4
	MUSIC_FLASH,					//渐变随音乐闪烁5
	HALF_CHASE,						//对半逐光自动变幻颜色6
	ENTIRE_CHASE,					//整圈逐光自动变幻颜色7
	OPTICAL_DISK,					//常亮三色跑马8
	LED_MODE_OFF,					//关闭模式
	
}led_mode;
//

typedef struct led_struct//------------------LED结构体
{
	uint8_t		brightness;//--------------------亮度值//0-255
//	uint8_t		color;//-------------------------三色组合//B,R,BR,G,GB,GR,GRB
//	uint8_t		direction;//---------------------方向变量//自增自减
//	uint32_t 	grb_value;//---------------------三通道颜色值
 
}ledTypeDef;
//




extern uint8_t current_led_mode;//------------当前LED模式变量
extern uint8_t previous_led_mode;//-----------前一个LED模式变量，用于保存

//extern void	save_led_mode(void);//----------保存LED模式
extern void led_handle_process(void);//-------LED处理进程
extern void switch_led_mode(uint8_t);//-------切换LED模式
extern void switch_led_color(uint8_t);//------切换LED颜色
//extern void switch_led_brightness(bool);
extern void	switch_power(void);
extern void	init_led(void);//-----------------初始化LED
extern void close_all_led(void);//------------OFF
extern void pulse_follow_volume(void);
extern void pulse_follow_volume_2(void);
#endif

