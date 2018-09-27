#ifndef __h_adc_h__
#define __h_adc_h__

#include "app_main.h"

#define ADC_SIZE 50

#define	NOISE_AD_AVERAGE	0X020//----------------------实测噪音AD平均门槛值
#define	NOISE_AD_PEAK			0X100//----------------------实测噪音AD波峰门槛值
#define	AD_MAX						0XFFF//----------------------AD最大值
#define SAV_LENGH						(10)						//保存音量数组长度10ms内的10个音量


typedef struct 
{
	uint16_t max;
	uint16_t mean;
	uint16_t mini;
	uint16_t index;
	uint16_t Data[ADC_SIZE];
} ADC_VAL_TypeDef;


typedef struct volume_struct//---------------音量结构体
{
	int8_t		average;//------------------------平均值
	int8_t		current;//-----------------------当前音量
	int8_t		previous;//----------------------前一个音量
	int8_t		slope;//-------------------------音量变化变量
	int8_t		max;//---------------------------音量最大值
	int8_t		min;//---------------------------音量最小值

}volumeTypeDef;
//
extern volumeTypeDef	volume;//------------------------音量结构体变量

typedef struct pulse_volume_struct//----------频率结构体
{
	int8_t		count;//----------------计数变量
	int8_t		max;//------------------计数上限
}Pulse_Volume;
//
extern Pulse_Volume	pulse_volume[SAV_LENGH];//-------脉冲音量

extern int8_t				volume_save[SAV_LENGH];//--------存放音量数组

extern uint8_t  Adc_Dma_Irq_f;
extern uint8_t  Audio_level;
extern uint16_t cntAudioNone;		
extern uint16_t Audio_brightness;
extern ADC_VAL_TypeDef audio_adc_1;
extern ADC_VAL_TypeDef audio_adc_2;
extern ADC_VAL_TypeDef audio_adc_3;
extern ADC_VAL_TypeDef audio_adc_4;
extern ADC_VAL_TypeDef audio_adc_5;
extern ADC_VAL_TypeDef audio_adc_6;
extern ADC_VAL_TypeDef audio_adc_7;
extern ADC_VAL_TypeDef audio_adc_8;

typedef enum 
{
	LOWEST = 0,
	LOW		  ,
	MID		  ,
	HIGH	  ,
	HIGHEST	  ,
} TYPE_RANGE; 
typedef struct 
{
	uint16_t cntLowest;
	uint16_t cntLow;
	uint16_t cntMid;
	uint16_t cntHigh;
	uint16_t cntHighest;	
	TYPE_RANGE Range;
} AUDIO_LEVEL_TypeDef;
extern AUDIO_LEVEL_TypeDef sAudio;
extern uint16_t AdcDma_Buf[ADC_SIZE];	 
extern uint16_t AdcAudio_Buf[ADC_SIZE];  
extern void ADC_GetBuffer(void);
extern void volume_scan(uint8_t step);

#endif
