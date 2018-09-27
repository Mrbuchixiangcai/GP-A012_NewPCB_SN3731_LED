//ͷ�ļ�header file//
#include "app_main.h"


ledTypeDef 		grb_led[LED_ROWS][LED_COLUMNS];//----������

freqTypeDef		freqer[SAV_LENGH];//-----------------------Ƶ�ʽṹ�����

uint8_t const Music_Fire_tab[120][6][10]={  \
{
// L   H    L    H    L    H    L    H    L    H	
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
},                                                 
 
};
//


/*************************************************************************\
 *   Function name   : user_freq(�Զ���Ƶ����)                            				
 *   Returns 	  	   : Time_State(TIME_UPʱ�䵽/NOT_REACHEDδ����)                                              	
 *   Parameters      : *freq(Ƶ�ʽṹ��ָ��)                                             	
 *   Purpose         : ÿ����period��ִ��һ��                     		
\*************************************************************************/
Time_State user_freq(freqTypeDef *freq)
{
	
	(*freq).count ++;
	
	if((*freq).count>=(*freq).period )
	{
		(*freq).count=0;
	}
	//
	
	if((*freq).count==0)
	{
		
		return (TIME_UP);
	}
	//
	
	else
	{
		return (NOT_REACHED);
	}
	//
}
//


void write_all_led(void)
{
	extern uint8_t  Fire_Data[6][10]; 
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i,j;
	
	for(i=0;i<LED_ROWS;i++)
	{
		
		for(j=0;j<LED_COLUMNS;j++)
		{

			Fire_Data[i][j]=grb_led [LED_ROWS-1-i][j].brightness;
		}
	}
}
//

//�������ȣ��������ȣ�
void set_brightness(uint8_t p,uint8_t brightness)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	for(i=0;i<LED_ROWS;i++)
	{

			grb_led [i][p].brightness=brightness;
	}
}
//

//�����ٶ�
void set_speed(uint8_t p)
{
	
	//��ǰ������С��������
	if(volume_save[p]<volume.average )
	{
		
		//���¼�������
		freqer[p].period=7*(VOLUME_STEP-abs(pulse_volume[p].max-volume_save[p]));
	}
	//
	
//	//��ǰ�������䣬�н���
//	else if(volume_save[p]==volume.average )
//	{
//		
//		//���¼�������
//		freqer[p].period=4*(VOLUME_STEP-abs(pulse_volume[p].max-volume_save[p]));
//	}
//	//
		
	//��ǰ������󣬿����
	else
	{
		
		//���¼�������
		freqer[p].period=(VOLUME_STEP-abs(pulse_volume[p].max-volume_save[p]));
	}
	//
}
//


//����������
void volume_counter(uint8_t p)
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;
	
	//��ǰ�����Ǿ���
	if((volume_save[p] != VOLUME_MUTE))
	{
	
		//��������
		if((pulse_volume[p].count)<(pulse_volume[p].max))
		{
			
			//��ǰ����������������ֵ
			if(volume_save[p]>(pulse_volume[p].max))
			{
				
				//�����ٶ�
				set_speed(p);
				
				//��������ֵ
				(pulse_volume[p].max)=volume_save[p];
			}
			//
			
			//Ƶ�ʼ���ʱ�䵽
			if(user_freq(&freqer[p])==TIME_UP)
			{
			
				//�����м�����
				(pulse_volume[p].count)++;
				
				//������
				for(i=0;i<LED_ROWS ;i++)
				{

					//�趨����ֵ
					//grb_led[i][p].brightness =BRIGHTNESS_STEP*(pulse_volume[p].max)*(i<(pulse_volume[p].count));
					grb_led[i][p].brightness =BRIGHTNESS_MAX*(i<(pulse_volume[p].count));
				}
				//
				
				//������LEDд����
				write_all_led ();
			}
			//
		}
		//
		
		//��������
		else if((pulse_volume[p].count)>(pulse_volume[p].max))
		{
			
			//��ǰ����С����������ֵ
			if(volume_save[p]<(pulse_volume[p].max))
			{
				
				//�����ٶ�
				set_speed(p);
				
				//��������ֵ
				(pulse_volume[p].max)=volume_save[p];
			}
			//
			
			//Ƶ�ʼ���ʱ�䵽
			if(user_freq(&freqer[p])==TIME_UP)
			{
			
				//�����м�����
				(pulse_volume[p].count)--;
				
				//������ǰ�벿
				for(i=0;i<LED_ROWS ;i++)
				{

					//�趨����ֵ
					//grb_led[i][p].brightness =BRIGHTNESS_STEP*(pulse_volume[p].max)*(i<(pulse_volume[p].count));
					grb_led[i][p].brightness =BRIGHTNESS_MAX*(i<(pulse_volume[p].count));

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
			
			//�����ٶ�
			set_speed(p);
			
			//��ǰ����δ�����仯
			if(volume_save[p]==pulse_volume[p].max)
			{
				
				if(volume_save[p]==LED_ROWS)
				{
					pulse_volume[p].max=volume_save[p]-3;
				}
				//
			}
			//
			
			//��ǰ���������仯
			else
			{
			
				//��������ֵ
				(pulse_volume[p].max)=volume_save[p];
				
			}
			//
		}
		//
	}
	//
	
	//����
	else //if(volume.current==VOLUME_MUTE)
	{
		
		//Ƶ�ʼ���ʱ�䵽
		if(user_freq(&freqer[p])==TIME_UP)
		{
		
			//�趨�ٶ�
			freqer[p].period=16;
			
			//�趨���Ⱥ���ɫ
			set_brightness(p,BRIGHTNESS_MUTE);
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//
}
//

//�����������
void pulse_follow_volume(void)//4
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i;

	//����������
	for(i=0;i<LED_COLUMNS;i++)
	{
		
		//��������
		volume_counter (i);
	}
	//
}
//4


//�����������
void pulse_follow_volume_2(void)//5
{
	
	//C89�涨�ֲ����������������ڿ�ͷ
	uint8_t i,j;

	//����������
	for(i=0;i<LED_COLUMNS;i++)
	{
		
		if(i%LED_TEAM==0)
		{
			
			//��������
			volume_counter (i);
		}
		//
		
		//
		else
		{
			
			//������
			for(j=0;j<LED_ROWS;j++)
			{
				
				//ͬ���������ǰһ��
				grb_led [LED_ROWS-1-j][i].brightness=grb_led [LED_ROWS-1-j][i-1].brightness;
			}
			//
			
			//������LEDд����
			write_all_led ();
		}
		//
	}
	//
}
//5

/*******************************************************************************************/
/*******************************************************************************************/ 
 u8 ptrMove; 
 u8 cntMoveSpeed;  
 const uint8_t RRC_MOVE_Tab[12] =
 {
  0,0,255,0,0,255,0,0,255,0,0,255,
 };
extern const uint8_t Map_Addr_H[6][10];
uint8_t music_speed_max;
uint8_t music_speed_count;
uint8_t big_small_flag;
uint8_t small_index;	
uint8_t big_index;	
 
void Music_Fire_Mode(void)
{
 u8 i,j;
// static uint8_t speed;
//	static uint8_t small_index;	
//	static uint8_t big_index;	
	
	//����
	if(volume.current==VOLUME_MUTE)
	{
			for(i=0;i<6;i++)
			{
			 for(j=0;j<10;j++)
				Fire_Data[i][j]=0;
			} 
		j=ptrMove;
		for(i=0;i<10;i++)
		{

		 Fire_Data[5][i]=RRC_MOVE_Tab[j];
		 if(++j>=12)
			j=0;
		}
		if(++cntMoveSpeed>=50)
		{
		 cntMoveSpeed=0;	
		 if(++ptrMove>=12)
			ptrMove=0;		 
		}	
	 //
		
		//�Ӿ�������Ǿ�����ʹ��С��
		big_small_flag=0;
	}
	//

	//�Ǿ���
	else 
	{
		
		//������
		if(volume.current<volume.average )
		{
			
			//�л���С��
			big_small_flag=0;
			
			music_speed_max=15;
							
		}
		
		else if(volume.current==volume.average )
		{
			
			//�л���С��
			big_small_flag=0;
			
			music_speed_max=10;
		}
		//
		
		//�����
		else
		{
			
			//�л������
			big_small_flag=1;
			
			music_speed_max=5;
		}
		//
			
		//�ٶȿ���
		if(++music_speed_count>=music_speed_max)
		{
		 
			music_speed_count=0;

			//С��
			if(big_small_flag==0)
			{

				for(i=0;i<6;i++)
				{
				 for(j=0;j<10;j++)
					Fire_Data[i][j]=Small_Fire_tab_array[small_index][i][j];
				}
				//
				
				//��һ֡
				if(++small_index>=SMALL_FRAME_END)
				{
				 small_index=0; 
					
				}
			}
			//
			
			//���
			else
			{

				for(i=0;i<6;i++)
				{
				 for(j=0;j<10;j++)
					Fire_Data[i][j]=Big_Fire_tab_array[big_index][i][j];
				}
				//
				
				//��һ֡
				if(++big_index>=H_BIG_FRAME_END)
				{
				 big_index=0; 
					
					
				}
			}
			//
		}
		//
//  for(i=0;i<6;i++)
//  {
//   for(j=0;j<10;j++)
//  	Fire_Data[i][j]=Music_Fire_tab[index][i][j];
//  }

//  if(++index>=120)
//   index=0; 
	}
}
/*******************************************************************************************/
/*******************************************************************************************/ 


