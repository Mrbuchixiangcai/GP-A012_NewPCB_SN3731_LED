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


uint16_t			volume_ad_step[VOLUME_STEP];//---���ڶ�̬�ֲ�ÿһ������ADֵ					
uint8_t				volume_renew_count;
uint16_t			volume_ad_mute_count;//----------����������ʱ����
volumeTypeDef	volume;//------------------------�����ṹ�����

Pulse_Volume	pulse_volume[SAV_LENGH];//-------��������

int8_t				volume_temp[SAV_LENGH];//--------��ʱ��������
int8_t				volume_save[SAV_LENGH];//--------������������

//uint8_t 			volume_tab_index;
//int8_t				volume_tab[VOL_TAB_LENGH][SAV_LENGH]={
//																					{4,5,3,6,2,7,1,8,0,9},//0
//																										//};//--------�����������
//																					{1,7,4,2,0,9,3,5,8,6},//1
//																					{8,0,2,7,6,1,3,9,5,4},//2
//																					{3,4,5,9,2,0,7,6,5,1},//3
//																					{0,2,3,5,7,4,8,1,6,9},//4
//																					{9,6,1,3,2,5,7,8,4,0},//5
//																					{7,9,6,0,4,5,3,2,1,8},//6
//																					{0,5,6,4,9,8,2,3,7,1},//7
//																					{2,0,1,8,3,6,4,7,5,9},//8
//																					{4,3,9,6,1,7,8,2,0,5}//9
//																										};//--------�����������
																									
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


//ˢ������
void renew_volume(int8_t vol)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	int16_t sum=0;
	
	//�����趨���ֵ
	volume.max=-127;
	
	//�����趨��Сֵ
	volume.min=127;
	
	//ˢ�뵱ǰ����
	volume_temp[volume_renew_count]=vol;
	
//	//����������������
//	for(i=1;i<SAV_LENGH;i++)
//	{
//		
//		//ˢ��ǰһ��
//		volume_temp[i-1]=volume_temp[i];
//	}
//	//
//	
//	//ˢ�����һ��
//	volume_temp[SAV_LENGH-1]=vol;
	
	//��ǰ����
	volume.current=volume_temp[volume_renew_count];
	
	//ˢ�����
	volume_renew_count++;
	
	//��������
	for(i=0;i<volume_renew_count;i++)
	{
		
		//�������ֵ
		if(volume_temp[i]>volume.max)
		{
			
			//�������ֵ
			volume.max =volume_temp[i];
		}
		//
		
		//��С��Сֵ
		if(volume_temp[i]<volume.min)
		{
			
			//������Сֵ
			volume.min =volume_temp[i];
		}
		//
		
		//�ۼ����
		sum=sum+volume_temp[i];
	}
	//
	
	//ƽ��ֵ
	volume.average=sum/volume_renew_count;
	
	//�������
	if(volume_renew_count>=(SAV_LENGH ))
	{
		
		//���¼���
		volume_renew_count=0;
		
		//��������
		for(i=0;i<SAV_LENGH;i++)
		{
			
			//���֮ǰ����
			volume_save[i]=volume_temp[i];
		}
		//
		
		//�������
		//memset(volume_temp,VOLUME_QUASI_MUTE,sizeof(volume_temp));
	}
	//
	
}
//


/*************************************************************************\
 *   Function name   : volume_scan(����ɨ��)                            				
 *   Returns 	    	 : void                                              	
 *   Parameters      : step(����ָ������������)                                     	
 *   Purpose         : �����ж���Ƶ�źŴ�С����Ӧ������                 		
\*************************************************************************/
//����ɨ��(ָ�����׼���)
void volume_scan(uint8_t step)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint16_t i,ad;
	
	//adcɨ��
	//adc_scan ();
	
	//adֵʹ��ƽ��ֵ
	ad=audio_adc_1.mean ;
	
	//��ǰADֵС��ʵ�⾲��AD����ֵ
	if(ad <NOISE_AD_AVERAGE)
	//if(audio_ad.current_average <NOISE_AD_PEAK)	
	{
		
		//���ﾲ��ʱ��
		if(volume_ad_mute_count>(MUTE_TIMES ))
		{

			//ˢ������
			renew_volume(VOLUME_MUTE);
			
			//ȷ�Ͼ���
			return ;
		}
		//
		
		//δ������ʱ��
		else
		{

			//��ʱ�ۼ�
			volume_ad_mute_count++;
			
			//ˢ������
			renew_volume(VOLUME_QUASI_MUTE);
			
			//׼����
			return ;
		}
		//
	}
	//
	
	//ADֵ���������ż�ֵ
	else
	{

		//���㾲����ʱ
		volume_ad_mute_count=0;
		
		//��������
		for(i=(VOLUME_QUASI_MUTE+1);i<(step) ;i++)
		{
			
			//ƽ����̬����ÿһ��������AD��Χ����̬������������ֲ��չ�������Ƕ�μ���
			
			//volume_ad_step[i]=NOISE_AD_AVERAGE+(AD_MAX-NOISE_AD_AVERAGE)*(i/(step-1))*(i/(step-1));
			//����NOISE_AD_AVERAGE�ǵ��룬(AD_MAX-NOISE_AD_AVERAGE)��AD��Ч��Χ��i*/(step-1)�ǵȷ�ƽ�����䣬i/(step-1)��У��ϵ��
			
			//volume_ad_step[i]=NOISE_AD_AVERAGE+((AD_MAX-NOISE_AD_AVERAGE)*i*i/((step-1)*(step-1)));
			//��ʽ�任��ֻ����һ�γ�������

			volume_ad_step[i]=NOISE_AD_AVERAGE+((AD_MAX-NOISE_AD_AVERAGE)*i*i/((step-1)*(step-1)));
			
			//�ж���������
			if(ad<=volume_ad_step[i])
			{
				
				//ˢ������
				renew_volume(i);
				
				//������Ӧ�����ȼ�
				return ;
			}
			//	
		}
		//
		
		//ˢ������
		renew_volume(step-1);
		
		//����AD_MAX����ֵ������������
		return ;
	}
	//
}
//

