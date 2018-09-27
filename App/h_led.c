#include "h_led.h"

uint8_t				current_led_mode;//--------------��ǰLEDģʽ����
uint8_t				previous_led_mode;//-------------ǰһ��LEDģʽ���������ڱ���

//uint8_t 		mode_step[MODE_NUM];//-----------���Ʋ����������ǰģʽ��ʼ��������ˢ�µ�
uint8_t				mode_color[MODE_NUM];//----------������ɫ��������ǰģʽGREEN,RED,BLUE��

uint16_t			volume_ad_step[VOLUME_STEP];//---���ڶ�̬�ֲ�ÿһ������ADֵ					

uint8_t				increasing;//--------------------�ݹ�����

int8_t				current_volume_count;//----------����������������
int8_t				current_volume_max;//------------������������ֵ����

volumeTypeDef	volume;//------------------------�����ṹ�����
int8_t				volume_temp[SAV_LENGH];//--------�����������

uint16_t			volume_ad_mute_count;//----------����������ʱ����

ledTypeDef 		grb_led[LED_ROWS];//--------------��ɫ������

freqTypeDef		freqer1;//-----------------------Ƶ�ʽṹ�����

uint8_t				col_direction[LED_COLUMNS];//----���ڿ����е���ˮ��������

uint8_t				save_led_mode_flag;//------------���ڱ�Ǳ���ƹ�ģʽ


//���ؼӼ�(ԭֵ����������ֵ������ֵ)
void back_and_forth(uint8_t *source,uint8_t *direction,uint8_t min,uint8_t max)
{
	
	//����
	if(*direction )
	{
		
		//δ�������ֵ
		if((*source)<(max))
		{
			
			//�����Լ�1
			(*source)++;
		}
		//
		
		//�������ֵ
		else
		{
			//����ǯλ�����ֵ
			(*source)=max-1;
			
			//��λ��־λΪ�Լ�
			(*direction ) =0;
		}
		//
	}
	//
	
	//�Լ�
	else
	{
		
		//δ������Сֵ
		if((*source)>(min))
		{
			
			//�����Լ�һ
			(*source)--;
		}
		
		//������Сֵ
		else
		{
			
			//����ǯλ����Сֵ
			(*source)=min+1;
			
			//��λ��־λΪ����
			(*direction ) =1;
		}
		//
	}
	//
}
//


//��ת��������
void inversion_array(ledTypeDef  *source,uint8_t lengh)
{
	
	uint8_t i;
	
	//����ʱ����
	ledTypeDef temp;
	
	//����һ�볤��
	for(i=0;i<(lengh>>1);i++)
	{
		
		//��ʱ����
		temp =source[i] ;
		
		//�����滻����
		source[i]=source [lengh -1-i];
		
		//�ָ����򵽷���
		source [lengh -1-i]=temp;
	}
	//
}
//


//����
void breath(ledTypeDef *mled)
{
	
	//���ؼӼ�
	back_and_forth(&(mled->brightness),&(mled->direction),BRIGHTNESS_MIN,BRIGHTNESS_MAX);
}
//


//�Ȳ�ݹ�Ӽ�
void equal_difference(ledTypeDef *pre,ledTypeDef *cur,uint8_t operation,uint8_t min,uint8_t max)
{
	
	//�ų����ֵС�ڻ������Сֵ
	if(max <=min)
	{
		(*cur).brightness =min;
	}
	//
	
	//�������ֵ���������Сֵ
	else
	{
		
		//�ݹ��
		if((*pre).direction)
		{
			
			//���ǰһ������ֵ��operation���������ֵ
			if(((*pre).brightness+operation ) <=max)
			{
				
				//��ǰ����ֵ�����Լ�operation
				(*cur).brightness =(*pre).brightness +operation ;
				
				//��ǰ�ۼӷ������ǰһ��
				(*cur).direction =(*pre).direction;
			}
			//
			
			//�����operation�������ֵ
			else
			{
				
				//����ǯλ�����ֵ��ȥ������ֵ
				(*cur).brightness =max-(((*pre).brightness+operation )-max);
				
				//��λ��־λΪ�ݹ��
				(*cur).direction =!(*pre).direction;
			}
			//
		}
		//
		
		//�ݹ��
		else
		{
			
			//���ǰһ������ֵ��operation��С����Сֵ
			if(((*pre).brightness-operation) >=min)
			{
				
				//���Եݹ��ȥ��operation
				(*cur).brightness =(*pre).brightness -operation ;
				
				//��ǰ�ݹ鷽�����ǰһ��
				(*cur).direction =(*pre).direction;
			}
			//
			
			//����С����Сֵ
			else
			{
				
				//����ǯλ����Сֵ
				(*cur).brightness =min+(min-((*pre).brightness -operation));
				
				//��λ��־λΪ�ݹ��
				(*cur).direction =!(*pre).direction;
			}
			//
		}
		//
	}
	//
}
//


//���صȲ�ݹ�Ӽ�
void recursion(ledTypeDef *pre,ledTypeDef *cur,uint8_t operation)
{
	
	//��ǰ����ֵ��������������������֮�����ǰһ������ֵ�ݹ�Ӽ�
	equal_difference(pre,cur,operation,BRIGHTNESS_MIN,BRIGHTNESS_MAX);
	
	if((*cur).brightness ==BRIGHTNESS_MIN )
	{
		
		//��ɫ�������
		(*cur).color=mode_color[current_led_mode];
	}
	//
}
//


//������LEDд����
void write_all_led(void)
{
	extern uint8_t  Fire_Data[6][10]; 
	
	extern const uint8_t Map_Addr_H[6][10];
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i,j;
	
	for(i=0;i<LED_ROWS;i++)
	{

		Fire_Data[i][0]=grb_led [i].brightness;
	}
	
	for(j=0;j<LED_COLUMNS;j++)
	{

		Fire_Data[i][j]=Fire_Data[i][0];
	}
}
//


////��λLED����
//void reset_led_data(void)
//{
//	
//	//C89�涨�ֲ����������������ڿ�ͷ
//	uint8_t i;
//	
//	//��������LED
//	for(i=0;i<LED_NUM;i++)
//	{
//		
//		//��������
//		grb_led[i].brightness=0X00;
//		
//		//��������
//		grb_led[i].direction=BACKWARD;
//	}
//	//
//	
//	//���оƬ����
//	clear_sk6812Data();
//}
////


////��ʼ��LED
//void init_led(void)
//{
//	
////	//C89�涨�ֲ����������������ڿ�ͷ
////	uint8_t i;

////	//�����ƹ�ģʽ
////	for(i=0;i<(MODE_NUM); i++)
////	{
////		
////		//��ʼ��ÿһ��ģʽ����Ϊ��ʼ������
////		mode_step[i]=STATE_INITIAL;
////		
////		//��ʼ��ÿһ�ֵƹ�ģʽ����ɫ
////		mode_color[i]=DEFAULT_COLOR;
////	}
////	//
//	
//	//��ʼ��RAINBOW_BREATHģʽ��ɫ
//	mode_color[RAINBOW_BREATH]=GREEN_COLOR ;
//	
//	//��ʼ��PARITY_BREATHģʽ��ɫ
//	mode_color[PARITY_BREATH]=RED_COLOR ;
//	
//	//��ʼ��ENTIRE_BREATHģʽ��ɫ
//	mode_color[ENTIRE_BREATH]=YELLOW_COLOR ;
//	
//	//��ʼ��HALF_PULSE_FLASHģʽ��ɫ
//	mode_color[HALF_PULSE_FLASH]=BLUE_COLOR ;
//	
//	//��ʼ��PULSE_FLASHģʽ��ɫ
//	mode_color[PULSE_FLASH]=CYAN_COLOR ;
//	
//	//��ʼ��MUSIC_FLASHģʽ��ɫ
//	mode_color[MUSIC_FLASH]=PURPLE_COLOR ;
//	
//	//��ʼ��HALF_CHASEģʽ��ɫ
//	mode_color[HALF_CHASE]=WHITE_COLOR ;
//	
//	//��ʼ��ENTIRE_CHASEģʽ��ɫ
//	mode_color[ENTIRE_CHASE]=GREEN_COLOR ;
//	
//	//��ʼ��OPTICAL_DISKģʽ��ɫ
//	mode_color[OPTICAL_DISK]=RED_COLOR ;

//	//��λLED����
//	reset_led_data();
//	
//	//��ʼ����ǰ�ƹ�ģʽ
//	current_led_mode=LED_MODE_OFF;
//}
////


//һά���갴ָ�������������е�
void one_coordinate_copy_all_led(ledTypeDef *mled)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//����һά��������LED
	for(i=0;i<LED_NUM;i++)
	{
		
		//����
		grb_led[i]=(*mled);
	}
	//
}
//


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
	
	//����������������
	for(i=1;i<SAV_LENGH;i++)
	{
		
		//ˢ��ǰһ��
		volume_temp[i-1]=volume_temp[i];
	}
	//
	
	//ˢ�����һ��
	volume_temp[SAV_LENGH-1]=vol;
	
	//��ǰ����
	volume.current=volume_temp[SAV_LENGH-1];
	
	//��������
	for(i=0;i<SAV_LENGH;i++)
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
	volume.average=sum/SAV_LENGH;
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
	uint8_t i;
	
	//adcɨ��
	//adc_scan ();
	
	//��ǰADֵС��ʵ�⾲��AD����ֵ
	if(audio_ad.current_average <NOISE_AD_AVERAGE)
	//if(audio_ad.current_average <NOISE_AD_PEAK)	
	{
		
		//���ﾲ��ʱ��
		if(volume_ad_mute_count>(LED_MUTE_TIMES ))
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
			if(audio_ad.current_average<=volume_ad_step[i])
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


//�ʺ���ɫ
void rainbow(uint8_t direction)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//������
	for(i=1;i<LED_ROWS;i++)
	{

		//���ȸ����һ��
		grb_led[i].brightness=grb_led[0].brightness;
		
		//��ɫ����ǰһ����ɫ��һ
		grb_led[i].color=value_loop (grb_led[i-1].color, direction , COLOR_MIN, LED_ROWS);

	}
	//
}
//


//�������(�����趨��ɫ)
void entire_breath(uint8_t color)
{

	//�趨��ɫ
	grb_led[0].color=color;
	
	//����
	breath (&grb_led[0]);

	//����һά��������LED�����һ��
	one_coordinate_copy_all_led(&grb_led[0]);
	
	//������LEDд����
	write_all_led ();

}
//


//�������ȣ��������ȣ�
void set_brightness(uint8_t brightness)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//�������е�
	for(i=0;i<LED_NUM;i++)
	{
		
		//�趨����
		grb_led[i].brightness =brightness ;
	}
	//
}
//


//������ɫ��������ɫ��
void set_color(uint8_t color)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//�������е�
	for(i=0;i<LED_NUM;i++)
	{
		
		//�趨����
		grb_led[i].color =color ;
	}
	//
}
//


//�������Ⱥ���ɫ
void set_brightness_color(uint8_t brightness,uint8_t color)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;

	for(i=0;i<LED_NUM;i++)
	{
		//�趨��ɫ
		grb_led[i].color=mode_color[current_led_mode ];
		
		//�趨����
		grb_led[i].brightness =brightness ;
	}
	//
}
//


//LED_MODE_START
//�����������(�ܼ���,����)
void pulse_follow_volume(uint8_t total_rows,uint8_t direction)//2\3
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//����ɨ��
	volume_scan(total_rows+1);

	//����
	if(volume.current==VOLUME_MUTE)
	{
		
		//Ƶ�ʼ���ʱ�䵽
		if(user_freq(&freqer1)==TIME_UP)
		{
		
			//�趨�ٶ�
			freqer1.period=100;
			
			//�趨���Ⱥ���ɫ
			set_brightness_color (0X01,mode_color[current_led_mode ]);
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//
			
	//�Ǿ���
	else 
	{

		//��������
		if(current_volume_count<current_volume_max)
		{
			
			//��ǰ����������������ֵ
			if(volume.current>current_volume_max)
			{
				
				//��������ֵ
				current_volume_max=volume.current;
			}
			//
			
			//Ƶ�ʼ���ʱ�䵽
			if(user_freq(&freqer1)==TIME_UP)
			{
			
				//�����м�����
				current_volume_count++;
				
				//������ǰ�벿
				for(i=0;i<total_rows ;i++)
				{
					
					//�趨��ɫ
					grb_led[i].color=mode_color[current_led_mode ];

					//�趨����ֵ
					grb_led[i].brightness =BRIGHTNESS_MAX*(i<current_volume_count);
				}
				//
				
				//����
				if(!direction )
				{
					
					//��������
					inversion_array(grb_led ,total_rows);
				}
				//
				
				//���������
				for(i=total_rows;i<LED_ROWS ;i++)
				{
				
					//���򻥲�
					grb_led[i]=grb_led[i-total_rows];
				}
				//
				
				//������LEDд����
				write_all_led ();
			}
			//
		}
		//
		
		//��������
		else if(current_volume_count>current_volume_max)
		{
			
			//��ǰ����С����������ֵ
			if(volume.current<current_volume_max)
			{
				
				//��������ֵ
				current_volume_max=volume.current;
			}
			//
			
			//Ƶ�ʼ���ʱ�䵽
			if(user_freq(&freqer1)==TIME_UP)
			{
			
				//�����м�����
				current_volume_count--;
				
				//������ǰ�벿
				for(i=0;i<total_rows ;i++)
				{
					
					//�趨��ɫ
					grb_led[i].color=mode_color[current_led_mode ];

					//�趨����ֵ
					grb_led[i].brightness =BRIGHTNESS_MAX*(i<current_volume_count);

				}
				//
				
				//����
				if(!direction )
				{
					
					//��������
					inversion_array(grb_led ,total_rows);
				}
				//
				
				//���������
				for(i=total_rows;i<LED_ROWS ;i++)
				{
				
					//���򻥲�
					grb_led[i]=grb_led[i-total_rows];
				}
				//
				
				//������LEDд����
				write_all_led ();
			}
			//
		}
		//
		
		//���嵽��
		else
		{
			
			//��������ֵ
			current_volume_max=volume.current;
		}
		//
		
//		//����Ϊ׼����
//		if(current_volume_count==VOLUME_QUASI_MUTE)
//		{

//			//��ɫ��R��RGBѭ��
//			point_value_loop(&mode_color[current_led_mode],NEXT, COLOR_MIN, COLOR_MAX);
//			
//			//ȡ��
//			//col_direction[0]=!col_direction[0];

//		}
//		//

		//�����仯��
		volume.slope=abs(current_volume_max-volume.current) ;

		//�趨Ƶ�ʼ�������
		freqer1.period=120/total_rows*abs(total_rows-volume.slope);

	}
	//
}
//1


//��������
void gradually_brighten(uint8_t step)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//��ǰ������LED����δ��ȫ����
	if(grb_led[current_volume_count].brightness <BRIGHTNESS_MAX )
	{
		
		//�ٶȿ���	
		//Ƶ�ʼ���ʱ�䵽
		//if(user_freq(&freqer1)==TIME_UP)
		{
		
			//�𽥵���
			grb_led[current_volume_count].brightness++;
			
			//���������
			for(i=step;i<LED_ROWS ;i++)
			{
			
				//���򻥲�
				grb_led[i]=grb_led[i-step];
			}
			//
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//
	
	//��ǰ������LED������ȫ����
	else
	{
		
		//��ǰ������δ��������ֵ
		if(current_volume_count<(step-1))
		{
			
			//�����Լ�
			current_volume_count++;
			
		}
		//
		
		//�����������ޱ߽�
		else
		{
			
			//��¼ǰһ������
			//previous_volume=current_volume_max;
			
			//��������ֵ
			current_volume_max=volume.current;
		}
		//
	}
	//
}
//


//�����䰵
void gradually_darken(uint8_t step)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//��ǰ������LED����δ��ȫϨ��
	if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
	{
		
		//�ٶȿ���	
		//Ƶ�ʼ���ʱ�䵽
		//if(user_freq(&freqer1)==TIME_UP)
		{
		
			//��Ϩ��
			grb_led[current_volume_count].brightness--;
			
			//���������
			for(i=step;i<LED_ROWS ;i++)
			{
			
				//���򻥲�
				grb_led[i]=grb_led[i-step];
			}
			//
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//
	
	//��ǰ������LED������ȫϨ��
	else
	{
		
		//��ǰ������δ��������ֵ
		if(current_volume_count>0)
		{
			
			//�����Լ�
			current_volume_count--;
		}
		//
		
		//�����������ޱ߽�
		else
		{
			
			//��¼ǰһ������
			//previous_volume=current_volume_max;
			
			//��������ֵ
			current_volume_max=volume.current;
		}
		//
	}
	//
}
//


//���������������(�ܼ���)
//void pulse_breath_follow_volume(uint8_t step)//2/3
//{
//	
//	//C89�涨�ֲ����������������ڿ�ͷ
//	uint8_t i;

//	//����ɨ��
//	current_volume=volume_scan(step);

//	//��ǰ����
//	if(current_volume==VOLUME_MUTE)
//	{
//		
//		//Ƶ�ʼ���ʱ�䵽
//		if(user_freq(&freqer1)==TIME_UP)
//		{
//		
//			//�趨����
//			freqer1.period =100;
//			
//			//�趨���Ⱥ���ɫ
//			set_brightness_color (0X01,mode_color[current_led_mode ]);
//			
//			//������LEDд����
//			write_all_led ();
//		}
//		//
//	}
//	//

//	//��ǰ�Ǿ���
//	else 
//	{
//		
//		//�����仯��
//		//volume_slope=abs(current_volume_max-previous_volume) ;
//		
//		//�趨����
//		//freqer1.period=(step-volume_slope);
//		
//		//������ɫ
//		set_color(mode_color[current_led_mode ]);
//		
//		//��ǰ���������������
//		for(i=0;i<step;i++)
//		{
//			
//			//ǰ���
//			if(i<current_volume_count)
//			{
//				
//				//û����ȫ����
//				if(grb_led[i].brightness <BRIGHTNESS_MAX )
//				{
//					
//					//��û����ȫ������λ�ÿ�ʼ
//					current_volume_count=i;
//					
//					break;
//				}
//				//
//			}
//			//

//			//�����
//			else if(i>current_volume_count)
//			{
//				
//				//û����ȫϨ��
//				if(grb_led[i].brightness >BRIGHTNESS_OFF )
//				{
//					
//					//��û����ȫϨ���λ�ÿ�ʼ
//					current_volume_count=i;
//				}
//				//
//			}
//			//
//		}
//		//
//		
//		//����׼����//Ϩ�����е�
//		if(current_volume_max==VOLUME_QUASI_MUTE)
//		{
//			
//			//��ǰ������LED����δ��ȫϨ��
//			if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
//			{
//				
//				//�ٶȿ���	
//				//Ƶ�ʼ���ʱ�䵽
//				//if(user_freq(&freqer1)==TIME_UP)
//				{
//				
//					//��Ϩ��
//					grb_led[current_volume_count].brightness--;
//					
//					//���������
//					for(i=step;i<LED_ROWS ;i++)
//					{
//					
//						//���򻥲�
//						grb_led[i]=grb_led[i-step];
//					}
//					//
//					
//					//������LEDд����
//					write_all_led ();
//				}
//				//
//			}
//			//
//			
//			//��ǰ������LED������ȫϨ��
//			else
//			{
//				
//				//��ǰ������δ��������ֵ
//				if(current_volume_count>0)
//				{
//					
//					//�����Լ�
//					current_volume_count--;
//				}
//				//
//				
//				//�����������ޱ߽�
//				else
//				{
//					
//					//��¼ǰһ������
//					//previous_volume=current_volume_max;
//					
//					//��������ֵ
//					current_volume_max=current_volume;
//				}
//				//
//			}
//			//
//		}
//		//
//		
//		//��׼����
//		else
//		{
//		
//			//���嵽��
//			if(current_volume_count==(current_volume_max))
//			{
//				
//				//��ǰ������LED����δ��ȫ����
//				if(grb_led[current_volume_count-1].brightness <BRIGHTNESS_MAX )
//				{
//					
//					//�ٶȿ���	
//					//Ƶ�ʼ���ʱ�䵽
//					//if(user_freq(&freqer1)==TIME_UP)
//					{
//					
//						//�𽥵���
//						grb_led[current_volume_count-1].brightness++;
//						
//						//���������
//						for(i=step;i<LED_ROWS ;i++)
//						{
//						
//							//���򻥲�
//							grb_led[i]=grb_led[i-step];
//						}
//						//
//						
//						//������LEDд����
//						write_all_led ();
//					}
//					//
//				}
//				//
//				
//				//��ǰ������LED������ȫ����
//				else
//				{
//					
//					//��ǰ������δ��������ֵ
//					if(current_volume_count<(step-1))
//					{
//						
//						//�����Լ�
//						current_volume_count++;
//						
//					}
//					//
//					
//					//�����������ޱ߽�
//					else
//					{
//						
//						//��¼ǰһ������
//						//previous_volume=current_volume_max;
//						
//						//��������ֵ
//						current_volume_max=current_volume;
//					}
//					//
//				}
//				//
//				
//			}
//			//
//			
//			//��������
//			else if(current_volume_count>(current_volume_max))
//			{
//				
//				//�����䰵
//				gradually_darken(step);
//			}
//			//
//			
//			//��������
//			else  if(current_volume_count<(current_volume_max))
//			{
//				
//				//��������
//				gradually_brighten(step);
//			}
//			//
//		}
//		//
//	}
//	//
//}
////2/3


//���������������(�ܼ���)
void pulse_breath_follow_volume(uint8_t step)//
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;

	//����ɨ��
	volume_scan(step+1);

	//��ǰ����
	if(volume.current==VOLUME_MUTE)
	{
		
		//Ƶ�ʼ���ʱ�䵽
		if(user_freq(&freqer1)==TIME_UP)
		{
		
			//�趨����
			freqer1.period =100;
			
			//�趨���Ⱥ���ɫ
			set_brightness (0X01);
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//

	//��ǰ�Ǿ���
	else 
	{
		
		//����û������//Ϩ�����е�
		if(current_volume_max<=VOLUME_QUASI_MUTE)
		{
			
			//��ǰ���������������
			for(i=0;i<step;i++)
			{

				//û����ȫϨ��
				if(grb_led[i].brightness >BRIGHTNESS_OFF )
				{
					
					//��û����ȫϨ���λ�ÿ�ʼ
					current_volume_count=i;
				}
				//
			}
			//��ǰ���������������
			
			//��ǰ������LED����δ��ȫϨ��
			if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
			{
				
				//�ٶȿ���	
				//Ƶ�ʼ���ʱ�䵽
				//if(user_freq(&freqer1)==TIME_UP)
				{
				
					//����Ϩ��
					grb_led[current_volume_count].brightness--;
					
					//���������
					for(i=step;i<LED_ROWS ;i++)
					{
					
						//���򻥲�
						grb_led[i]=grb_led[i-step];
					}
					//
					
					//������LEDд����
					write_all_led ();
				}
				//
			}
			//
			
			//��ǰ������LED������ȫϨ��
			else
			{
				
				//��������ֵ
				current_volume_max=volume.max;
			}
			//
		}
		//
		
		//����������
		else if(current_volume_max>VOLUME_QUASI_MUTE)
		{
			
			//��ǰ���������������
			for(i=0;i<step;i++)
			{
				
				//ǰ���
				if(i<current_volume_count)
				{
					
					//û����ȫ����
					if(grb_led[i].brightness <BRIGHTNESS_MAX )
					{
						
						//��û����ȫ������λ�ÿ�ʼ
						current_volume_count=i;
						
						break;
					}
					//
				}
				//

				//�����
				else if(i>current_volume_count)
				{
					
					//û����ȫϨ��
					if(grb_led[i].brightness >BRIGHTNESS_OFF )
					{
						
						//��û����ȫϨ���λ�ÿ�ʼ
						current_volume_count=i;
					}
					//
				}
				//
			}
			//��ǰ���������������

			//���嵽��
			if(current_volume_count==(current_volume_max-1))
			{
				
				//��ǰ������LED����δ��ȫ����
				if(grb_led[current_volume_count].brightness <BRIGHTNESS_MAX )
				{
					
					//�ٶȿ���	
					//Ƶ�ʼ���ʱ�䵽
					//if(user_freq(&freqer1)==TIME_UP)
					{
					
						//��������
						grb_led[current_volume_count].brightness++;
						
						//���������
						for(i=step;i<LED_ROWS ;i++)
						{
						
							//���򻥲�
							grb_led[i]=grb_led[i-step];
						}
						//
						
						//������LEDд����
						write_all_led ();
					}
					//
				}
				//
				
				//��ǰ������LED������ȫ����
				else
				{
					
					//��������ֵ
					current_volume_max=volume.current;
				}
				//
			}
			//���嵽��
			
			//��������
			else if((current_volume_max-1)<current_volume_count)
			{
				
				//��ǰ������LED����δ��ȫϨ��
				if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
				{
					
					//�ٶȿ���	
					//Ƶ�ʼ���ʱ�䵽
					//if(user_freq(&freqer1)==TIME_UP)
					{
					
						//����Ϩ��
						grb_led[current_volume_count].brightness--;
						
						//���������
						for(i=step;i<LED_ROWS ;i++)
						{
						
							//���򻥲�
							grb_led[i]=grb_led[i-step];
						}
						//
						
						//������LEDд����
						write_all_led ();
					}
					//
				}
				//
				
				//��ǰ������LED������ȫϨ��
				else
				{
					
					//�����б仯
					//if(volume.current<current_volume_max)
					{
						
						//��������ֵ
						current_volume_max=volume.min;
					}
					//
					
					//��ǰ������δ��������ֵ
					if(current_volume_count>0)
					{
						
						//�Լ�
						current_volume_count--;
					}
					//
				}
				//
			}
			//��������
			
			//��������
			else  if((current_volume_max-1)>current_volume_count)
			{
				
				//��ǰ������LED����δ��ȫ����
				if(grb_led[current_volume_count].brightness <BRIGHTNESS_MAX )
				{
					
					//�ٶȿ���	
					//Ƶ�ʼ���ʱ�䵽
					//if(user_freq(&freqer1)==TIME_UP)
					{
					
						//��������
						grb_led[current_volume_count].brightness++;
						
						//���������
						for(i=step;i<LED_ROWS ;i++)
						{
						
							//���򻥲�
							grb_led[i]=grb_led[i-step];
						}
						//
						
						//������LEDд����
						write_all_led ();
					}
					//
				}
				//
				
				//��ǰ������LED������ȫ����
				else
				{
					
					//�����б仯
					//if(volume.current>current_volume_max)
					{
						
						//��������ֵ
						current_volume_max=volume.max ;
					}
					//

					//��ǰ������δ��������ֵ
					if(current_volume_count<(current_volume_max-1))
					{
						
						//�Լ�
						current_volume_count++;
						
					}
					//
				}
				//
			}
			//��������
		}
		//�����׼����

		//�����仯��
		//volume_slope=abs(current_volume_max-previous_volume) ;
		
		//�趨����
		//freqer1.period=(step-volume_slope);
		//freqer1.period=0;
	}
	//
}
//2/3


//�ʺ���ɫ
void rainbow_different_rotate_color(void)//8
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//����ɨ��
	volume_scan(VOLUME_STEP);

	//Ƶ�ʼ���ʱ�䵽
	if(user_freq(&freqer1)==TIME_UP)
	{
//		
//		//��ǰ����
//		breath(&grb_led[mode_color[current_led_mode]]);
//		
		//����������ֵ
		//if(grb_led[mode_color[current_led_mode]].brightness==BRIGHTNESS_MIN)
		{
			
			//��ɫ��R��RGBѭ��
			back_and_forth(&mode_color[current_led_mode],&col_direction[0], 0, 1);
		}
		//
		
		//����ǰ����֮һ
		for(i=0;i<LED_ONE_THIRD;i++)
		{

			//
			if(i==(mode_color[current_led_mode]))
			{
				
				grb_led[i].brightness=BRIGHTNESS_MAX;
				grb_led[i].color =COLOR_MIN ;
			}
			//
			
			else
			{
				
				grb_led[i].brightness=BRIGHTNESS_OFF;
				grb_led[i].color =NO_COLOR ;
			}
			//
		}
		//
		
		//����ʣ��
		for(i=LED_ONE_THIRD;i<(LED_ROWS);i++)
		{

			//���ȸ����һ��
			grb_led[i].brightness=grb_led[i-2].brightness;
			grb_led[i].color =grb_led[i-2].color<<1 ;
		}
		//

		//�趨Ƶ�ʼ�������
		freqer1.period=50*(VOLUME_STEP-volume.current);
		
		//������LEDд����
		write_all_led ();
	}
	//
}
//8


//�ʺ����
void rainbow_breath(void)//1
{
	
	//����ɨ��
	volume_scan(VOLUME_STEP);

	//Ƶ�ʼ���ʱ�䵽
	if(user_freq(&freqer1)==TIME_UP)
	{
		
		//�趨��һ�����Ⱥ���
		breath (&grb_led[0]);
		
		//��������������ʱ
		if(grb_led[0].brightness<=BRIGHTNESS_MIN)
		{
			
//			//�趨Ƶ�ʼ�������
//			freqer1.period=(VOLUME_STEP-current_volume);
			
			//�л���ɫ
			point_value_loop(&mode_color[current_led_mode], col_direction[0], COLOR_MIN, LED_ROWS);
			
		}
		//
		
//		//��������������ʱ
//		else if(grb_led[0].brightness>=BRIGHTNESS_MAX)
//		{
//			
//			//�趨Ƶ�ʼ�������
//			freqer1.period=(VOLUME_STEP-current_volume);
//			
//		}
//		//

		//�趨��һ��LED��ɫ
		grb_led[0].color=mode_color[current_led_mode];
		
		//�ʺ���ɫ
		rainbow(col_direction[0]);
		
		//������LEDд����
		write_all_led ();
		
		//�趨Ƶ�ʼ�������
		freqer1.period=(VOLUME_STEP-volume.average);
		
	}
	//
	
	
}
//1


//��ż����
void parity_breath(void)//2
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//����ɨ��
	volume_scan(VOLUME_STEP);
	
	//Ƶ�ʼ���ʱ�䵽
	if(user_freq(&freqer1)==TIME_UP)
	{

		//�������(ָ����ɫ)
		//entire_breath(mode_color [current_led_mode ]);
		
		//�趨��ɫ
		grb_led[0].color=mode_color [current_led_mode ];
		
		//����
		breath(&grb_led[0]);
		
		//���������ʱ
		if(grb_led[0].brightness==BRIGHTNESS_MIN)
		{
			
			//�Զ��л���ɫ
			point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);
			
			//�Զ��л���ɫ
			point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);
			//һ���л���ɫ
		}
		//

		//��������LED
		for(i=0;i<LED_NUM;i++)
		{

			//ż��
			if(i%2==0)
			{
				
				//����
				grb_led[i]=grb_led[0];
			}
			//
			
			//����
			else
			{
				
				//���������ʱ
				if(grb_led[i].brightness==BRIGHTNESS_MIN )
				{
				
					//�趨��ɫ����ǰһ��
					grb_led[i].color=value_loop(grb_led[i-1].color,NEXT,COLOR_MIN,COLOR_MAX);
				}
				//
				
				//���Ȼ���
				grb_led[i].brightness=BRIGHTNESS_MAX-grb_led[0].brightness;
			}
			//
		}
		//

		//������LEDд����
		write_all_led ();
		
		//�趨Ƶ�ʼ�������
		freqer1.period=(VOLUME_STEP-volume.average);
	}
	//
}
//2



//LEDģʽ-�߲ʺ����Զ��仯
void breath_seven_color_change(void)//3
{

	//����ɨ��
	volume_scan(VOLUME_STEP);
	
	//Ƶ�ʼ���ʱ�䵽
	if(user_freq(&freqer1)==TIME_UP)
	{
		
		//�������(ָ����ɫ)
		entire_breath(mode_color [current_led_mode ]);
		
		//���������ʱ
		if(grb_led[0].brightness==BRIGHTNESS_MIN )
		{
			
			//�Զ��л���ɫ
			point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);

		}
		//
		
		//�趨Ƶ�ʼ�������
		freqer1.period=(VOLUME_STEP-volume.average);
	}
	//
}
//3


//LEDģʽ-���еƳ���һ����ɫ
void still_all_led_one_color(void)//5
{
	
	//Ƶ�ʼ���ʱ�䵽
	if(user_freq(&freqer1)==TIME_UP)
	{
	
		//�趨Ƶ�ʼ�������
		freqer1.period=100 ;

		//�趨����
		//grb_led[0].brightness =max_brightness ;
		
		//�趨��ɫֵ
		//grb_led[0].color=mode_color[current_led_mode];
		
		//����һά��������LED�����һ��
		//one_coordinate_copy_all_led(&grb_led[0]);
		
		//�������Ⱥ���ɫ
		set_brightness_color (BRIGHTNESS_MAX,mode_color[current_led_mode]);
		
		//������LEDд����
		write_all_led ();
	}
	//
}
//5


//��������˸
void follow_music_flash(void)//6
{
	
	//��¼ǰһ������
	//previous_volume=current_volume;
	
	//����ɨ��
	volume_scan(VOLUME_STEP);

	//����
	if(volume.current ==VOLUME_MUTE)
	{
		
		//Ƶ�ʼ���ʱ�䵽
		if(user_freq(&freqer1)==TIME_UP)
		{
			
			//�趨����
			freqer1.period=100;
			
			//�趨���Ⱥ���ɫ
			set_brightness_color (0X01,mode_color[current_led_mode ]);
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//
	
	//�Ǿ���
	else
	{

		//�趨��ɫ
		grb_led[0].color=mode_color [current_led_mode ];
		
		//��ǰ����ֵС�ڵ�ǰ��������ֵ
		if(grb_led[0].brightness<(current_volume_max *FLASH_PER_STEP))
		{

			//�ٶȿ���	
			//Ƶ�ʼ���ʱ�䵽
			//if(user_freq(&freqer1)==TIME_UP)
			{
		
				//��ǰ����ֵ�������
				grb_led[0].brightness++;
				
				//��������LED�����һ��
				one_coordinate_copy_all_led(&grb_led[0]);
				
				//������LEDд����
				write_all_led ();
			}
			//
		}
		//
		
		//��ǰ����ֵ���ڵ�ǰ��������ֵ
		else if(grb_led[0].brightness>(current_volume_max *FLASH_PER_STEP))
		{
			
			//�ٶȿ���	
			//Ƶ�ʼ���ʱ�䵽
			//if(user_freq(&freqer1)==TIME_UP)
			{
				
				//��ǰ����ֵ������С
				grb_led[0].brightness--;
				
				//��������LED�����һ��
				one_coordinate_copy_all_led(&grb_led[0]);
				
				//������LEDд����
				write_all_led ();
			}
			//
		}
		//
		
		//��ǰ����ֵ���ڵ�ǰ��������ֵ
		else
		{
			
			//������ʱ
			if(volume.current>(VOLUME_STEP>>1))
			{
				
				//����ͻ��
				if(abs(volume.current-current_volume_max) >=(VOLUME_STEP>>1))
				{
					
					//���������仯��
					volume.slope=abs(volume.current-current_volume_max);
					
					//��������
					current_volume_max=volume.current;
					
					//�趨Ƶ�ʼ�������
					freqer1.period=(VOLUME_STEP-volume.slope);
				}
				//
				
				//����û��ͻ��
				else
				{
					
					//���������仯��
					volume.slope=current_volume_max;
					
					//ǿ�Ƹ�������
					current_volume_max=volume.current-(VOLUME_STEP>>1);
					
					//�趨Ƶ�ʼ�������
					freqer1.period=(VOLUME_STEP-volume.slope);
				}
				//
			}
			//
			
			//
			else
			{
				
				//���������仯��
				volume.slope=abs(volume.current-current_volume_max);
				
				//��������
				current_volume_max=volume.current;
				
				//�趨Ƶ�ʼ�������
				freqer1.period=(VOLUME_STEP-volume.slope);
			}
			//
			
//			//׼����
//			if(current_volume_max==VOLUME_QUASI_MUTE)
//			{
//				
//				//��һ��LED�߲�ѭ���л�
//				point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);
//			}
//			//
		}
		//
	}
	//
}
//8



////��������˸
//void follow_music_flash(void)//8
//{
//	
//	//��¼ǰһ������
//	//previous_volume=current_volume;
//	
//	//����ɨ��
//	current_volume=volume_scan(VOLUME_STEP);

//	//����
//	if(current_volume ==VOLUME_MUTE)
//	{
//		
//		//Ƶ�ʼ���ʱ�䵽
//		if(user_freq(&freqer1)==TIME_UP)
//		{
//			
//			//�趨����
//			freqer1.period=100;
//			
//			//�������Ⱥ���ɫ
//			set_brightness_color (0X01,mode_color[current_led_mode]);
//			
//			//������LEDд����
//			write_all_led ();
//		}
//		//
//	}
//	//
//	
//	//�Ǿ���
//	else
//	{
//		
//		
//		
//		//׼����
//		if(current_volume ==VOLUME_QUASI_MUTE)
//		{
//			
//			//�趨Ƶ�ʼ�������
//			freqer1.period=2*(VOLUME_STEP-current_volume);
//			
//			//�������Ⱥ���ɫ
//			set_brightness_color (0X00,mode_color[current_led_mode]);
//			
//			//������LEDд����
//			write_all_led ();
//		}
//		//
//		
//		//��׼����
//		else
//		{
//			
//			//�趨Ƶ�ʼ�������
//			freqer1.period=2*(VOLUME_STEP-current_volume);
//			
//			//�趨��ɫ
//			grb_led[0].color=mode_color [current_led_mode ];

//			//��ǰ����ֵС����������ֵ
//			if(grb_led[0].brightness<max_brightness)
//			{
//				
//				//�������������������ֵ
//				if(max_brightness<(audio_ad.current_average/10))
//				{
//					
//					//������������ֵ
//					max_brightness=(audio_ad.current_average/10);

//				}
//				//
//				
//				//�ٶȿ���	
//				if(user_freq(&freqer1)==TIME_UP)
//				{
//					
//					//��ǰ����ֵ�������
//					grb_led[0].brightness++;
//					
//					//��������LED�����һ��
//					one_coordinate_copy_all_led(&grb_led[0]);
//					
//					//������LEDд����
//					write_all_led ();
//				}
//				//
//			}
//			//
//			
//			//��ǰ����ֵ������������ֵ
//			else if(grb_led[0].brightness>max_brightness)
//			{
//				
//				//�������������������ֵ
//				if(max_brightness>(audio_ad.current_average/10))
//				{
//					
//					//������������ֵ
//					max_brightness=(audio_ad.current_average/10);

//				}
//				//
//				
//				//�ٶȿ���	
//				if(user_freq(&freqer1)==TIME_UP)
//				{
//					
//					//��ǰ����ֵ������С
//					grb_led[0].brightness--;
//					
//					//��������LED�����һ��
//					one_coordinate_copy_all_led(&grb_led[0]);
//					
//					//������LEDд����
//					write_all_led ();
//				}
//				//
//			}
//			//
//			
//			//��ǰ����ֵ������������ֵ
//			else
//			{

//				//������������ֵ
//				max_brightness=(audio_ad.current_average/10);

//			}
//			//
//		}
//		//
//	}
//	//
//}
////8


////����������������
//void chase_breath(uint8_t total_rows)
//{
//	
//	//C89�涨�ֲ����������������ڿ�ͷ
//	uint8_t i;
//	
//	//״̬
//	switch (mode_step[current_led_mode ])
//	{
//		
//		//��ʼ��״̬
//		case STATE_INITIAL:
//		{

//			//����Ȳ�����ֵ
//			increasing=(((BRIGHTNESS_MAX-BRIGHTNESS_MIN)<<1)/(total_rows));
//			
//			//�������
//			for(i=0;i<(total_rows);i++)
//			{
//				
//				//�趨��ɫ;
//				grb_led[i].color=mode_color[current_led_mode];
//				
//				//��ֹ���
//				if((i*increasing)>BRIGHTNESS_MAX)
//				{
//					
//					//���ֵ��ȥ�������ֵ����
//					grb_led[i].brightness=(BRIGHTNESS_MAX<<1)-i*increasing;
//					
//					//��ת����
//					grb_led[i].direction=!grb_led[i].direction;
//				}
//				//
//				
//				else
//				{
//					//��ʼ���Ȳ�����
//					grb_led[i].brightness=i*increasing;
//				}
//				//
//			}
//			//
//			
//			//��һ��״̬
//			mode_step[current_led_mode ]++;
//			
//		}
//		//
//		break;
//		
//		//ˢ��״̬
//		case STATE_REFLASH:
//		{
//			
//			//����ɨ��
//			current_volume =volume_scan(total_rows);

//			//Ƶ�ʼ���ʱ�䵽
//			if(user_freq(&freqer1)==TIME_UP)
//			{
//				
//				//�趨����
//				freqer1.period=(total_rows-current_volume);

//				//����ǰ�벿��
//				for(i=0;i<(total_rows);i++)
//				{

//					//����
//					breath (&grb_led[i]);
//			
//					//���������ʱ
//					if(grb_led[i].brightness==BRIGHTNESS_MIN)
//					{
//						
//						if(i==0)
//						{
//							
//							//�Զ��л���ɫ
//							point_value_loop(&mode_color[current_led_mode], NEXT, COLOR_MIN, COLOR_MAX);
//						
//						}
//						//
//						
//						//�趨��ɫ;
//						grb_led[i].color=mode_color[current_led_mode];
//					}
//					//
//				}
//				//
//				
//				//������벿��
//				for(i=total_rows;i<(LED_ROWS);i++)
//				{
//					
//					//���򻥲�
//					grb_led[i]=grb_led[i-LED_HALF_ROWS];
//				}
//				//

//				//������LEDд����
//				write_all_led ();
//			}
//			//
//		}
//		//
//		break;
//		
//		//����
//		default :
//		{
//			
//			//����ģʽ״̬Ϊ��ʼ��
//			mode_step[current_led_mode ]=STATE_INITIAL;
//			
//		}
//		//
//		break;
//	}
//	//
//}
////10


//��������������������
void chase_breath(uint8_t total_rows,uint8_t direction)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;

	//���ڼ�¼�ݹ�ǰһ���Ƶı���
	uint8_t previous;
	
	//����ɨ��
	volume_scan(total_rows);

	//Ƶ�ʼ���ʱ�䵽
	if(user_freq(&freqer1)==TIME_UP)
	{
		
		//�趨����
		freqer1.period=(total_rows-volume.current);
	
		//����Ȳ�����ֵ
		increasing=(((BRIGHTNESS_MAX-BRIGHTNESS_MIN)<<1)/total_rows);
		
		//�趨��ɫ;
		grb_led[0].color=mode_color[current_led_mode];

		//ģ�����
		breath (&grb_led[0]);
		
		//����һ��
		if(grb_led[0].brightness ==BRIGHTNESS_MIN)
		{

			//�Զ��л���ɫ
			point_value_loop(&mode_color[current_led_mode], NEXT, COLOR_MIN, COLOR_MAX);
		}
		//

		//��ǰ�����к�벿��,���ڵݹ����
		for(i=(total_rows-1);i>(0);i--)
		{
		
			//��һ�����룬���ж��Ƿ�Խ��
			previous=value_loop (i,direction,0,total_rows-1);
			
			//�ݹ��һ��
			recursion(&grb_led[previous],&grb_led[i],increasing);
		}
		//
		
		//����ʣ�ಿ��
		for(i=total_rows;i<(LED_ROWS);i++)
		{
			
			//���򻥲�
			grb_led[i]=grb_led[i-LED_HALF_ROWS];
		}
		//

		//������LEDд����
		write_all_led ();
	}
	//	
//	//C89�涨�ֲ����������������ڿ�ͷ
//	uint8_t i;
//	
//	//״̬
//	switch (mode_step[current_led_mode ])
//	{
//		
//		//��ʼ��״̬
//		case STATE_INITIAL:
//		{

//			//����Ȳ�����ֵ
//			increasing=(((BRIGHTNESS_MAX-BRIGHTNESS_MIN)<<1)/(total_rows));
//			
//			//�������
//			for(i=0;i<(total_rows);i++)
//			{
//				
//				//�趨��ɫ;
//				grb_led[i].color=mode_color[current_led_mode];
//				
//				//��ֹ���
//				if((i*increasing)>BRIGHTNESS_MAX)
//				{
//					
//					//���ֵ��ȥ�������ֵ����
//					grb_led[i].brightness=(BRIGHTNESS_MAX<<1)-i*increasing;
//					
//					//��ת����
//					grb_led[i].direction=!grb_led[i].direction;
//				}
//				//
//				
//				else
//				{
//					//��ʼ���Ȳ�����
//					grb_led[i].brightness=i*increasing;
//				}
//				//
//			}
//			//
//			
//			//����
//			if(!direction )
//			{
//				
//				//��������
//				inversion_array(grb_led ,total_rows);
//			}
//			//
//			
//			//��һ��״̬
//			mode_step[current_led_mode ]++;
//			
//		}
//		//
//		break;
//		
//		//ˢ��״̬
//		case STATE_REFLASH:
//		{
//			
//			//����ɨ��
//			current_volume =volume_scan(total_rows);

//			//Ƶ�ʼ���ʱ�䵽
//			if(user_freq(&freqer1)==TIME_UP)
//			{
//				
//				//�趨����
//				freqer1.period=(total_rows-current_volume);

//				//����ǰ�벿��
//				for(i=0;i<(total_rows);i++)
//				{

//					//����
//					breath (&grb_led[i]);
//			
//					//���������ʱ
//					if(grb_led[i].brightness==BRIGHTNESS_MIN)
//					{
//						
//						if(i==0)
//						{
//							
//							//�Զ��л���ɫ
//							point_value_loop(&mode_color[current_led_mode], NEXT, COLOR_MIN, COLOR_MAX);
//						
//						}
//						//
//						
//						//�趨��ɫ;
//						grb_led[i].color=mode_color[current_led_mode];
//					}
//					//
//				}
//				//
//				
//				//������벿��
//				for(i=total_rows;i<(LED_ROWS);i++)
//				{
//					
//					//���򻥲�
//					grb_led[i]=grb_led[i-LED_HALF_ROWS];
//				}
//				//

//				//������LEDд����
//				write_all_led ();
//			}
//			//
//		}
//		//
//		break;
//		
//		//����
//		default :
//		{
//			
//			//����ģʽ״̬Ϊ��ʼ��
//			mode_step[current_led_mode ]=STATE_INITIAL;
//			
//		}
//		//
//		break;
//	}
//	//
}
//10


////LEDģʽ-�ر�����LED
//void close_all_led(void)//OFF
//{
//	
//	//Ƶ�ʼ���ʱ�䵽
//	if(user_freq(&freqer1)==TIME_UP)
//	{
//	
//		//if(!led_off_flag)
//		{
//			
//			//led_off_flag=1;
//			
//			//�趨����
//			freqer1.period =100;
//		
//			//���LED����
//			//clear_ws2811Data();
//			
//			//�������ȹأ�����ɫ
//			set_brightness_color(BRIGHTNESS_OFF ,NO_COLOR );
//			
//			//������LEDд����
//			write_all_led ();
//			
//			//GP222��Ҫ�رյƵ�ԴIO��
//			power_control(L);
//		}
//		//
//	}
//	//
//}
////OFF
////LED_MODE_END


////external_fuction_start
////�л�LED��ɫ(���η���true��һ��ģʽ��false��һ��ģʽ)
//void switch_led_color(uint8_t direction)
//{
//	
//	//��һ��LED�߲��ֶ�ѭ���л�
//	point_value_loop(&mode_color[current_led_mode ], direction, COLOR_MIN, COLOR_MAX);
//}
////


////�л�LEDģʽ(���η���true��һ��ģʽ��false��һ��ģʽ)
//void switch_led_mode(uint8_t direction)
//{
//	
//	//������
//	if(read_bt_state()==STATE_ON)
//	{
//		
//		//��Ҫ��LED��Դ
//		power_control(H);
//		
//		//������ƹ�ģʽ
//		if(save_led_mode_flag)
//		{
//			
//			//��λ��־λ
//			save_led_mode_flag=0;
//		
//			//�ָ������ģʽ
//			current_led_mode=previous_led_mode ;
//		}
//		//
//		
//		//�л�
//		else
//		{
//		
//			//�л�ģʽ����ǰLEDģʽ������LED_MODE_MIN��LED_MODE_MAX��
//			point_value_loop (&current_led_mode,direction ,LED_MODE_MIN,LED_MODE_MAX);
//			
//			//�����ǰ�ƹ�ģʽ���ڿ���״̬
//			if(current_led_mode !=LED_MODE_OFF)
//			{
//				
//				//����Ҫ����ĵƹ�ģʽ
//				previous_led_mode=current_led_mode ;
//			}
//			//
//		}
//		//
//		
//		//�趨���ڣ�������Ӧ
//		freqer1.period  =0;
//		
//		//����ģʽ״̬Ϊ��ʼ��
//		//mode_step[current_led_mode ]=STATE_INITIAL;

//		//��λ������
//		reset_led_data();
//	}
//	//
//}
////

////�л���Դ
//void switch_power(void)
//{

//	//������
//	if(read_bt_state()==STATE_ON)
//	{
//				
//		//�ƹ�ģʽ��
//		if(current_led_mode!=LED_MODE_OFF)
//		{
//			
//			//�����־
//			save_led_mode_flag=1;
//			
//			//����ر�ǰ�ĵƹ�ģʽ
//			previous_led_mode=current_led_mode;
//			
//			//�л��ƹ�ģʽ��
//			current_led_mode=LED_MODE_OFF;
//		}
//		//
//		
//		//�ƹ�ģʽ��
//		else
//		{
//			
//			//�л���һ��ģʽ
//			switch_led_mode(NEXT);
//		}
//		//
//		
//		//��λ������
//		reset_led_data();
//	}
//	//
//}
////


////LED�������
//void led_handle_process(void)
//{

//	//LEDģʽѡ��
//	switch (current_led_mode)
//	{

//		case RAINBOW_BREATH:
//		{
//			
//			//�ʺ����
//			rainbow_breath();
//			
//		}
//		//
//		break;

//		case PARITY_BREATH:
//		{	
//			
//			//��ż����
//			parity_breath();

//		}
//		//
//		break;
//		
//		case ENTIRE_BREATH:
//		{
//			
//			//���еƳ���һ����ɫ
//			//still_all_led_one_color();
//			
//			//�����߲ʱ仯
//			breath_seven_color_change();

//		}
//		//
//		break;
//		
//		case HALF_PULSE_FLASH:
//		{
//			
//			//�������
//			
//			//��������������������У�
//			pulse_breath_follow_volume(LED_HALF_ROWS);
//			
//			//�������������ǰ����,����
//			//pulse_follow_volume(LED_HALF_ROWS,col_direction[0]);
//		}
//		//
//		break;
//		
//		case PULSE_FLASH:
//		{
//			
//			//��������
//			
//			//��������������������У�
//			pulse_breath_follow_volume(LED_ROWS);
//			
//			//����������������У�����
//			//pulse_follow_volume(LED_ROWS,col_direction[0]);
//			
//		}
//		//
//		break;
//		
//		case MUSIC_FLASH:
//		{
//			
//			//���ȸ���AD����
//			//brightness_follow_ad();
//			
//			//��������˸
//			follow_music_flash();
//		}
//		//
//		break;
//		
//		case HALF_CHASE:
//		{	
//			
//			
//			//�����������У�
//			chase_breath(LED_HALF_ROWS ,col_direction[0]);

//		}
//		//
//		break;
//		
//		case ENTIRE_CHASE:
//		{
//			
//			//�������������У�
//			chase_breath(LED_ROWS ,col_direction[0]);
//			
//		}
//		//
//		break;
//		
//		case OPTICAL_DISK:
//		{
//			
//			//�ʺ���ɫ
//			rainbow_different_rotate_color();
//		}
//		//
//		break;

//		case LED_MODE_OFF:
//		{
//			
//			//�ر�����LED
//			close_all_led();
//			
//		}
//		//
//		break;
//		
//		default:
//		{
//			
//			//���³�ʼ��led����
//			init_led();
//		}
//		//
//		break;
//	}
//	//
//}
////
//external_fuction_end
