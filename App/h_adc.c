#include "app_main.h"
#include "h_adc.h"

uint8_t   Adc_Dma_Irq_f;
uint8_t   Audio_level;
uint16_t  cntAudioNone;		
uint16_t  Audio_brightness;
uint16_t  AdcDma_Buf[ADC_SIZE];	 
uint16_t  AdcAudio_Buf[ADC_SIZE];  
ADC_VAL_TypeDef audio_adc_1;
ADC_VAL_TypeDef audio_adc_2;
ADC_VAL_TypeDef audio_adc_3;
ADC_VAL_TypeDef audio_adc_4;
ADC_VAL_TypeDef audio_adc_5;
ADC_VAL_TypeDef audio_adc_6;
ADC_VAL_TypeDef audio_adc_7;
ADC_VAL_TypeDef audio_adc_8;
AUDIO_LEVEL_TypeDef sAudio;


uint16_t			volume_ad_step[VOLUME_STEP];//---用于动态分布每一级音量AD值					
uint8_t				volume_renew_count;
uint16_t			volume_ad_mute_count;//----------音量静音计时变量
volumeTypeDef	volume;//------------------------音量结构体变量

Pulse_Volume	pulse_volume[SAV_LENGH];//-------脉冲音量

int8_t				volume_temp[SAV_LENGH];//--------临时音量数组
int8_t				volume_save[SAV_LENGH];//--------保存音量数组

//uint8_t 			volume_tab_index;
//int8_t				volume_tab[VOL_TAB_LENGH][SAV_LENGH]={
//																					{4,5,3,6,2,7,1,8,0,9},//0
//																										//};//--------存放音量数组
//																					{1,7,4,2,0,9,3,5,8,6},//1
//																					{8,0,2,7,6,1,3,9,5,4},//2
//																					{3,4,5,9,2,0,7,6,5,1},//3
//																					{0,2,3,5,7,4,8,1,6,9},//4
//																					{9,6,1,3,2,5,7,8,4,0},//5
//																					{7,9,6,0,4,5,3,2,1,8},//6
//																					{0,5,6,4,9,8,2,3,7,1},//7
//																					{2,0,1,8,3,6,4,7,5,9},//8
//																					{4,3,9,6,1,7,8,2,0,5}//9
//																										};//--------存放音量数组
																									
void Audio_Average(ADC_VAL_TypeDef *audio_adc, uint16_t adc_val) 
{
    uint8_t  i;
    uint32_t sum=0;
	audio_adc->max=0; 
	audio_adc->mini=0xffff; 
	audio_adc->Data[audio_adc->index]=adc_val;
	if(++audio_adc->index>=ADC_SIZE)
		audio_adc->index=0;
	for(i=0;i<ADC_SIZE;i++)
	{
	 if(audio_adc->Data[i] > audio_adc->max)
		 audio_adc->max=audio_adc->Data[i];
	 if(audio_adc->Data[i] < audio_adc->mini)
		 audio_adc->mini=audio_adc->Data[i];
	 sum += audio_adc->Data[i];
	}
	audio_adc->mean = sum / ADC_SIZE;	
}
 uint8_t  iii,jjj;
void ADC_GetBuffer(void) 
{
 uint8_t  i;
// static  uint16_t tmp;		
 if((Adc_Dma_Irq_f))
 {
	Adc_Dma_Irq_f=0;
	if (HAL_ADC_Stop_DMA(&hadc) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	for(i=0;i<ADC_SIZE;i++)
     AdcAudio_Buf[i]=AdcDma_Buf[i]; 
	if (HAL_ADC_Start_DMA(&hadc, (uint32_t*)AdcDma_Buf, ADC_SIZE) != HAL_OK)
	{
	 _Error_Handler(__FILE__, __LINE__);
	}
	for(i=0;i<ADC_SIZE;i++)
    Audio_Average(&audio_adc_1, AdcAudio_Buf[i]); 
//    Audio_Average(&audio_adc_2, audio_adc_1.mean); 
//    Audio_Average(&audio_adc_3, audio_adc_2.mean); 
//    Audio_Average(&audio_adc_4, audio_adc_3.mean); 	
//    Audio_Average(&audio_adc_5, audio_adc_4.mean); 
//    Audio_Average(&audio_adc_6, audio_adc_5.mean);
//    Audio_Average(&audio_adc_7, audio_adc_6.mean); 
//    Audio_Average(&audio_adc_8, audio_adc_7.mean);
//	if(audio_adc_1.max>10)
//	{
//	 cntAudioNone=0;
//	}
//	if(!audio_adc_1.max)
//	{
//	 if(cntAudioNone<2000)
//	   cntAudioNone++;
//    }
//    if(audio_adc_1.max>((audio_adc_8.mini)+(audio_adc_8.mini/5)))
//    {
//     tmp=(audio_adc_1.max-audio_adc_8.mini);
//    }
//    else
//    {
//     tmp=0;	
//    }	
//    if(audio_adc_8.mean<50)
//	{
//	 if(++sAudio.cntLow>=200)
//	 {		 
//	  sAudio.cntLow=0;	
//      sAudio.Range=LOW;		 
//	 }
//	 sAudio.cntMid=0;
//	 sAudio.cntHigh=0;
//	}
//	else if(audio_adc_8.mean<150)
//	{
//	 if(++sAudio.cntMid>=200)
//	 {		 
//	  sAudio.cntMid=0;
//	  sAudio.Range=MID;	 
//	 }	
//	 sAudio.cntLow =0;
//	 sAudio.cntHigh=0;
//	}
//	else
//	{
//	 if(++sAudio.cntHigh>=200)
//	 {		 
//	  sAudio.cntHigh=0;
//	  sAudio.Range=HIGH;	 
//	 }
//	 sAudio.cntLow=0;
//	 sAudio.cntMid=0;
//	}

//    if(sAudio.Range==LOW)
//	{
//     tmp=tmp*35;
//     if(audio_adc_1.max>((audio_adc_1.mean)))//+(audio_adc_1.mean/10))) 
//     {
//      Audio_level=(audio_adc_1.max-audio_adc_2.mean)*16;
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}		
//	else if(sAudio.Range==MID)
//	{
//     tmp=tmp*30;
//     if(audio_adc_1.max>((audio_adc_1.mean)))
//     {
//      Audio_level=(audio_adc_1.max-audio_adc_2.mean)*12;
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}		
//	else 
//	{
//     tmp=tmp*20;
//     if(audio_adc_1.max>((audio_adc_3.mean)))
//     {
//      Audio_level=(audio_adc_1.max-audio_adc_2.mean)*16;	
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}		
//	if(Audio_level>=12)
//	 Audio_level=12;
//	tmp=tmp/2;	
//	if(Audio_brightness<tmp)
//	 Audio_brightness++;	
//	if(Audio_brightness<tmp)
//	 Audio_brightness++;	  
//	if(Audio_brightness>tmp)
//	 Audio_brightness--;	
//	if(Audio_brightness>=255)
//	 Audio_brightness=255;	
//   //Fire_Data[iii][jjj]=Audio_brightness;
//   if(++jjj>=6)	
//   {
//	 jjj=0; 
//    if(++iii>=12)	
//	 iii=0; 
//   }   
 }
}






//static  uint16_t tmp;	
//void ADC_GetBuffer(void) 
//{
// uint8_t  i;
// //uint16_t tmp;		
// if((Adc_Dma_Irq_f))
// {
//	Adc_Dma_Irq_f=0;
//	if (HAL_ADC_Stop_DMA(&hadc) != HAL_OK)
//	{
//		_Error_Handler(__FILE__, __LINE__);
//	}
//	for(i=0;i<ADC_SIZE;i++)
//     AdcAudio_Buf[i]=AdcDma_Buf[i]; 
//	if (HAL_ADC_Start_DMA(&hadc, (uint32_t*)AdcDma_Buf, ADC_SIZE) != HAL_OK)
//	{
//	 _Error_Handler(__FILE__, __LINE__);
//	}
//	for(i=0;i<ADC_SIZE;i++)
//    Audio_Average(&audio_adc_1, AdcAudio_Buf[i]>>4); 
//    Audio_Average(&audio_adc_2, audio_adc_1.mean); 
//    Audio_Average(&audio_adc_3, audio_adc_2.mean); 
//    Audio_Average(&audio_adc_4, audio_adc_3.mean); 	
//    Audio_Average(&audio_adc_5, audio_adc_4.mean); 
//    Audio_Average(&audio_adc_6, audio_adc_5.mean);
//    Audio_Average(&audio_adc_7, audio_adc_6.mean); 
//    Audio_Average(&audio_adc_8, audio_adc_7.mean);
//    if(audio_adc_1.max>((audio_adc_8.mini)+(audio_adc_8.mini/5)))
//    {
//     tmp=(audio_adc_1.max-audio_adc_8.mini);
//    }
//    else
//    {
//     tmp=0;	
//    }	
//	if(audio_adc_8.mean<20)
//	{
//	 if(++sAudio.cntLowest>=200)
//	 {		 
//	  sAudio.cntLowest=0;
//	  sAudio.Range=LOWEST;	 
//	 }
//	  //sAudio.cntLowest=0;
//	  sAudio.cntLow=0;
//	  sAudio.cntMid=0;
//	  sAudio.cntHigh=0;
//	  sAudio.cntHighest=0;
//	}
//	else if(audio_adc_8.mean<80)
//	{
//	 if(++sAudio.cntLow>=200)
//	 {		 
//	  sAudio.cntLow=0;	
//      sAudio.Range=LOW;		 
//	 }
//	  sAudio.cntLowest=0;
//	  //sAudio.cntLow=0;
//	  sAudio.cntMid=0;
//	  sAudio.cntHigh=0;
//	  sAudio.cntHighest=0;	 
//	}
//	else if(audio_adc_8.mean<140)
//	{
//	 if(++sAudio.cntMid>=200)
//	 {		 
//	  sAudio.cntMid=0;
//	  sAudio.Range=MID;	 
//	 }	
//	  sAudio.cntLowest=0;
//	  sAudio.cntLow=0;
//	  //sAudio.cntMid=0;
//	  sAudio.cntHigh=0;
//	  sAudio.cntHighest=0;	 
//	}
//	else if(audio_adc_8.mean<200)
//	{
//	 if(++sAudio.cntHigh>=200)
//	 {		 
//	  sAudio.cntHigh=0;
//	  sAudio.Range=HIGH;	 
//	 }
//	  sAudio.cntLowest=0;
//	  sAudio.cntLow=0;
//	  sAudio.cntMid=0;
//	  //sAudio.cntHigh=0;
//	  sAudio.cntHighest=0;	 
//	}
//	else 
//	{
//	 if(++sAudio.cntHighest>=200)
//	 {		 
//	  sAudio.cntHighest=0;
//	  sAudio.Range=HIGHEST;	 
//	 }
//	  sAudio.cntLowest=0;
//	  sAudio.cntLow=0;
//	  sAudio.cntMid=0;
//	  sAudio.cntHigh=0;
//	  //sAudio.cntHighest=0;
//	}
//	if(sAudio.Range==LOWEST)
//	{
//     tmp=tmp*40;
//     if(audio_adc_1.max>((audio_adc_1.mean)))//+(audio_adc_1.mean/10))) 
//     {
//      Audio_level++;Audio_level++;//(audio_adc_1.max-audio_adc_1.mean)*32;
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}
//	else if(sAudio.Range==LOW)
//	{
//     tmp=tmp*35;
//     if(audio_adc_1.max>((audio_adc_1.mean)))//+(audio_adc_1.mean/10))) 
//     {
//      Audio_level++;Audio_level++;//(audio_adc_1.max-audio_adc_8.mean)*16;
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}		
//	else if(sAudio.Range==MID)
//	{
//     tmp=tmp*30;
//     if(audio_adc_1.max>((audio_adc_1.mean)))//+(audio_adc_1.mean/10))) 
//     {
//      Audio_level++;Audio_level++;//(audio_adc_1.max-audio_adc_8.mean)*16;
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}		
//	else if(sAudio.Range==HIGH)
//	{
//     tmp=tmp*20;
//     if(audio_adc_1.max>((audio_adc_1.mean)))//+(audio_adc_1.mean/10))) 
//     {
//      Audio_level++;Audio_level++;//(audio_adc_1.max-audio_adc_8.mean)*16;	
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}		
//	else 
//	{
//     tmp=tmp*20;
//     if(audio_adc_1.max>((audio_adc_1.mean)))//+(audio_adc_1.mean/10))) 
//     {
//      Audio_level++;Audio_level++;//(audio_adc_1.max-audio_adc_8.mean)*32;
//     }
//     else if(Audio_level)
//	 {		 
//	   Audio_level--; 
//	 }
//	}	
//  if(Audio_level>=12)
//   Audio_level=12;
//  tmp=tmp/2;	
//  if(Audio_brightness<tmp)
//    Audio_brightness++;	
//  if(Audio_brightness<tmp)
//    Audio_brightness++;	  
//  if(Audio_brightness>tmp)
//    Audio_brightness--;	
//  if(Audio_brightness>=255)
//    Audio_brightness=255;	
// }
//}


//刷新音量
void renew_volume(int8_t vol)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	int16_t sum=0;
	
	//重新设定最大值
	volume.max=-127;
	
	//重新设定最小值
	volume.min=127;
	
	//刷入当前计数
	volume_temp[volume_renew_count]=vol;
	
//	//遍历保存音量数组
//	for(i=1;i<SAV_LENGH;i++)
//	{
//		
//		//刷新前一个
//		volume_temp[i-1]=volume_temp[i];
//	}
//	//
//	
//	//刷新最后一个
//	volume_temp[SAV_LENGH-1]=vol;
	
	//当前音量
	volume.current=volume_temp[volume_renew_count];
	
	//刷入计数
	volume_renew_count++;
	
	//遍历数组
	for(i=0;i<volume_renew_count;i++)
	{
		
		//大于最大值
		if(volume_temp[i]>volume.max)
		{
			
			//更新最大值
			volume.max =volume_temp[i];
		}
		//
		
		//大小最小值
		if(volume_temp[i]<volume.min)
		{
			
			//更新最小值
			volume.min =volume_temp[i];
		}
		//
		
		//累计求和
		sum=sum+volume_temp[i];
	}
	//
	
	//平均值
	volume.average=sum/volume_renew_count;
	
	//如果存满
	if(volume_renew_count>=(SAV_LENGH ))
	{
		
		//重新计数
		volume_renew_count=0;
		
		//遍历数组
		for(i=0;i<SAV_LENGH;i++)
		{
			
			//清除之前保存
			volume_save[i]=volume_temp[i];
		}
		//
		
		//清除数组
		//memset(volume_temp,VOLUME_QUASI_MUTE,sizeof(volume_temp));
	}
	//
	
}
//


/*************************************************************************\
 *   Function name   : volume_scan(音量扫描)                            				
 *   Returns 	    	 : void                                              	
 *   Parameters      : step(传参指定总音量阶数)                                     	
 *   Purpose         : 用于判断音频信号大小所对应的音量                 		
\*************************************************************************/
//音量扫描(指定音阶级数)
void volume_scan(uint8_t step)
{
	
	//C89规定局部变量的声明必须在开头
	uint16_t i,ad;
	
	//adc扫描
	//adc_scan ();
	
	//ad值使用平均值
	ad=audio_adc_1.mean ;
	
	//当前AD值小于实测静音AD上限值
	if(ad <NOISE_AD_AVERAGE)
	//if(audio_ad.current_average <NOISE_AD_PEAK)	
	{
		
		//到达静音时长
		if(volume_ad_mute_count>(MUTE_TIMES ))
		{

			//刷新音量
			renew_volume(VOLUME_MUTE);
			
			//确认静音
			return ;
		}
		//
		
		//未到静音时长
		else
		{

			//计时累加
			volume_ad_mute_count++;
			
			//刷新音量
			renew_volume(VOLUME_QUASI_MUTE);
			
			//准静音
			return ;
		}
		//
	}
	//
	
	//AD值超过静音门槛值
	else
	{

		//清零静音计时
		volume_ad_mute_count=0;
		
		//遍历级数
		for(i=(VOLUME_QUASI_MUTE+1);i<(step) ;i++)
		{
			
			//平均动态分配每一级音量的AD范围，动态分配优势是移植扩展，劣势是多次计算
			
			//volume_ad_step[i]=NOISE_AD_AVERAGE+(AD_MAX-NOISE_AD_AVERAGE)*(i/(step-1))*(i/(step-1));
			//其中NOISE_AD_AVERAGE是底噪，(AD_MAX-NOISE_AD_AVERAGE)是AD有效范围，i*/(step-1)是等分平均分配，i/(step-1)是校正系数
			
			//volume_ad_step[i]=NOISE_AD_AVERAGE+((AD_MAX-NOISE_AD_AVERAGE)*i*i/((step-1)*(step-1)));
			//等式变换，只计算一次除法运算

			volume_ad_step[i]=NOISE_AD_AVERAGE+((AD_MAX-NOISE_AD_AVERAGE)*i*i/((step-1)*(step-1)));
			
			//判断音量级数
			if(ad<=volume_ad_step[i])
			{
				
				//刷新音量
				renew_volume(i);
				
				//返回相应音量等级
				return ;
			}
			//	
		}
		//
		
		//刷新音量
		renew_volume(step-1);
		
		//大于AD_MAX上限值，返回最大阶数
		return ;
	}
	//
}
//

