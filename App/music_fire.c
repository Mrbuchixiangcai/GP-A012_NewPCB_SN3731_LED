//头文件header file//
#include "app_main.h"


ledTypeDef 		grb_led[LED_ROWS][LED_COLUMNS];//----灯数组

freqTypeDef		freqer[SAV_LENGH];//-----------------------频率结构体变量

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
 *   Function name   : user_freq(自定义频率器)                            				
 *   Returns 	  	   : Time_State(TIME_UP时间到/NOT_REACHED未到达)                                              	
 *   Parameters      : *freq(频率结构体指针)                                             	
 *   Purpose         : 每计数period次执行一次                     		
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
	
	//C89规定局部变量的声明必须在开头
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

//设置亮度（传参亮度）
void set_brightness(uint8_t p,uint8_t brightness)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	for(i=0;i<LED_ROWS;i++)
	{

			grb_led [i][p].brightness=brightness;
	}
}
//

//设置速度
void set_speed(uint8_t p)
{
	
	//当前音量变小，慢节奏
	if(volume_save[p]<volume.average )
	{
		
		//更新计数周期
		freqer[p].period=7*(VOLUME_STEP-abs(pulse_volume[p].max-volume_save[p]));
	}
	//
	
//	//当前音量不变，中节奏
//	else if(volume_save[p]==volume.average )
//	{
//		
//		//更新计数周期
//		freqer[p].period=4*(VOLUME_STEP-abs(pulse_volume[p].max-volume_save[p]));
//	}
//	//
		
	//当前音量变大，快节奏
	else
	{
		
		//更新计数周期
		freqer[p].period=(VOLUME_STEP-abs(pulse_volume[p].max-volume_save[p]));
	}
	//
}
//


//音量计数器
void volume_counter(uint8_t p)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//当前音量非静音
	if((volume_save[p] != VOLUME_MUTE))
	{
	
		//脉冲往上
		if((pulse_volume[p].count)<(pulse_volume[p].max))
		{
			
			//当前音量大于上限音量值
			if(volume_save[p]>(pulse_volume[p].max))
			{
				
				//设置速度
				set_speed(p);
				
				//更新上限值
				(pulse_volume[p].max)=volume_save[p];
			}
			//
			
			//频率计数时间到
			if(user_freq(&freqer[p])==TIME_UP)
			{
			
				//点亮行计数加
				(pulse_volume[p].count)++;
				
				//遍历行
				for(i=0;i<LED_ROWS ;i++)
				{

					//设定亮度值
					//grb_led[i][p].brightness =BRIGHTNESS_STEP*(pulse_volume[p].max)*(i<(pulse_volume[p].count));
					grb_led[i][p].brightness =BRIGHTNESS_MAX*(i<(pulse_volume[p].count));
				}
				//
				
				//对所有LED写数据
				write_all_led ();
			}
			//
		}
		//
		
		//脉冲往下
		else if((pulse_volume[p].count)>(pulse_volume[p].max))
		{
			
			//当前音量小于上限音量值
			if(volume_save[p]<(pulse_volume[p].max))
			{
				
				//设置速度
				set_speed(p);
				
				//更新上限值
				(pulse_volume[p].max)=volume_save[p];
			}
			//
			
			//频率计数时间到
			if(user_freq(&freqer[p])==TIME_UP)
			{
			
				//点亮行计数减
				(pulse_volume[p].count)--;
				
				//遍历行前半部
				for(i=0;i<LED_ROWS ;i++)
				{

					//设定亮度值
					//grb_led[i][p].brightness =BRIGHTNESS_STEP*(pulse_volume[p].max)*(i<(pulse_volume[p].count));
					grb_led[i][p].brightness =BRIGHTNESS_MAX*(i<(pulse_volume[p].count));

				}
				//
				
				//对所有LED写数据
				write_all_led ();
			}
			//
		}
		//
		
		//脉冲到顶
		else
		{
			
			//设置速度
			set_speed(p);
			
			//当前音量未发生变化
			if(volume_save[p]==pulse_volume[p].max)
			{
				
				if(volume_save[p]==LED_ROWS)
				{
					pulse_volume[p].max=volume_save[p]-3;
				}
				//
			}
			//
			
			//当前音量发生变化
			else
			{
			
				//更新上限值
				(pulse_volume[p].max)=volume_save[p];
				
			}
			//
		}
		//
	}
	//
	
	//静音
	else //if(volume.current==VOLUME_MUTE)
	{
		
		//频率计数时间到
		if(user_freq(&freqer[p])==TIME_UP)
		{
		
			//设定速度
			freqer[p].period=16;
			
			//设定亮度和颜色
			set_brightness(p,BRIGHTNESS_MUTE);
			
			//对所有LED写数据
			write_all_led ();
		}
		//
	}
	//
}
//

//脉冲跟随音量
void pulse_follow_volume(void)//4
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;

	//遍历所有列
	for(i=0;i<LED_COLUMNS;i++)
	{
		
		//音量计数
		volume_counter (i);
	}
	//
}
//4


//脉冲跟随音量
void pulse_follow_volume_2(void)//5
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i,j;

	//遍历所有列
	for(i=0;i<LED_COLUMNS;i++)
	{
		
		if(i%LED_TEAM==0)
		{
			
			//音量计数
			volume_counter (i);
		}
		//
		
		//
		else
		{
			
			//遍历行
			for(j=0;j<LED_ROWS;j++)
			{
				
				//同组其余跟随前一个
				grb_led [LED_ROWS-1-j][i].brightness=grb_led [LED_ROWS-1-j][i-1].brightness;
			}
			//
			
			//对所有LED写数据
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
	
	//静音
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
		
		//从静音进入非静音先使用小火
		big_small_flag=0;
	}
	//

	//非静音
	else 
	{
		
		//慢节奏
		if(volume.current<volume.average )
		{
			
			//切换到小火
			big_small_flag=0;
			
			music_speed_max=15;
							
		}
		
		else if(volume.current==volume.average )
		{
			
			//切换到小火
			big_small_flag=0;
			
			music_speed_max=10;
		}
		//
		
		//快节奏
		else
		{
			
			//切换到大火
			big_small_flag=1;
			
			music_speed_max=5;
		}
		//
			
		//速度控制
		if(++music_speed_count>=music_speed_max)
		{
		 
			music_speed_count=0;

			//小火
			if(big_small_flag==0)
			{

				for(i=0;i<6;i++)
				{
				 for(j=0;j<10;j++)
					Fire_Data[i][j]=Small_Fire_tab_array[small_index][i][j];
				}
				//
				
				//下一帧
				if(++small_index>=SMALL_FRAME_END)
				{
				 small_index=0; 
					
				}
			}
			//
			
			//大火
			else
			{

				for(i=0;i<6;i++)
				{
				 for(j=0;j<10;j++)
					Fire_Data[i][j]=Big_Fire_tab_array[big_index][i][j];
				}
				//
				
				//下一帧
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


