#include "h_led.h"

uint8_t				current_led_mode;//--------------当前LED模式变量
uint8_t				previous_led_mode;//-------------前一个LED模式变量，用于保存

//uint8_t 		mode_step[MODE_NUM];//-----------控制步骤变量，当前模式初始化，运行刷新等
uint8_t				mode_color[MODE_NUM];//----------控制颜色变量，当前模式GREEN,RED,BLUE等

uint16_t			volume_ad_step[VOLUME_STEP];//---用于动态分布每一级音量AD值					

uint8_t				increasing;//--------------------递归增量

int8_t				current_volume_count;//----------脉冲音量计数变量
int8_t				current_volume_max;//------------脉冲音量上限值变量

volumeTypeDef	volume;//------------------------音量结构体变量
int8_t				volume_temp[SAV_LENGH];//--------存放音量数组

uint16_t			volume_ad_mute_count;//----------音量静音计时变量

ledTypeDef 		grb_led[LED_ROWS];//--------------三色灯数组

freqTypeDef		freqer1;//-----------------------频率结构体变量

uint8_t				col_direction[LED_COLUMNS];//----用于控制列的流水呼吸方向

uint8_t				save_led_mode_flag;//------------用于标记保存灯光模式


//来回加减(原值，方向，下限值，上限值)
void back_and_forth(uint8_t *source,uint8_t *direction,uint8_t min,uint8_t max)
{
	
	//自增
	if(*direction )
	{
		
		//未到达最大值
		if((*source)<(max))
		{
			
			//可以自加1
			(*source)++;
		}
		//
		
		//到达最大值
		else
		{
			//立马钳位在最大值
			(*source)=max-1;
			
			//置位标志位为自减
			(*direction ) =0;
		}
		//
	}
	//
	
	//自减
	else
	{
		
		//未到达最小值
		if((*source)>(min))
		{
			
			//可以自减一
			(*source)--;
		}
		
		//到达最小值
		else
		{
			
			//立马钳位在最小值
			(*source)=min+1;
			
			//置位标志位为自增
			(*direction ) =1;
		}
		//
	}
	//
}
//


//反转数组索引
void inversion_array(ledTypeDef  *source,uint8_t lengh)
{
	
	uint8_t i;
	
	//灯临时变量
	ledTypeDef temp;
	
	//遍历一半长度
	for(i=0;i<(lengh>>1);i++)
	{
		
		//临时保存
		temp =source[i] ;
		
		//反序替换正序
		source[i]=source [lengh -1-i];
		
		//恢复正序到反序
		source [lengh -1-i]=temp;
	}
	//
}
//


//呼吸
void breath(ledTypeDef *mled)
{
	
	//来回加减
	back_and_forth(&(mled->brightness),&(mled->direction),BRIGHTNESS_MIN,BRIGHTNESS_MAX);
}
//


//等差递归加减
void equal_difference(ledTypeDef *pre,ledTypeDef *cur,uint8_t operation,uint8_t min,uint8_t max)
{
	
	//排除最大值小于或等于最小值
	if(max <=min)
	{
		(*cur).brightness =min;
	}
	//
	
	//否则最大值必须大于最小值
	else
	{
		
		//递归加
		if((*pre).direction)
		{
			
			//如果前一个亮度值加operation不超过最大值
			if(((*pre).brightness+operation ) <=max)
			{
				
				//当前亮度值可以自加operation
				(*cur).brightness =(*pre).brightness +operation ;
				
				//当前累加方向跟随前一个
				(*cur).direction =(*pre).direction;
			}
			//
			
			//否则加operation超过最大值
			else
			{
				
				//立马钳位在最大值减去超过的值
				(*cur).brightness =max-(((*pre).brightness+operation )-max);
				
				//置位标志位为递归减
				(*cur).direction =!(*pre).direction;
			}
			//
		}
		//
		
		//递归减
		else
		{
			
			//如果前一个亮度值减operation不小于最小值
			if(((*pre).brightness-operation) >=min)
			{
				
				//可以递归减去减operation
				(*cur).brightness =(*pre).brightness -operation ;
				
				//当前递归方向跟随前一个
				(*cur).direction =(*pre).direction;
			}
			//
			
			//否则小于最小值
			else
			{
				
				//立马钳位在最小值
				(*cur).brightness =min+(min-((*pre).brightness -operation));
				
				//置位标志位为递归加
				(*cur).direction =!(*pre).direction;
			}
			//
		}
		//
	}
	//
}
//


//重载等差递归加减
void recursion(ledTypeDef *pre,ledTypeDef *cur,uint8_t operation)
{
	
	//当前亮度值在上限亮度与下限亮度之间跟随前一个亮度值递归加减
	equal_difference(pre,cur,operation,BRIGHTNESS_MIN,BRIGHTNESS_MAX);
	
	if((*cur).brightness ==BRIGHTNESS_MIN )
	{
		
		//颜色跟随变量
		(*cur).color=mode_color[current_led_mode];
	}
	//
}
//


//对所有LED写数据
void write_all_led(void)
{
	extern uint8_t  Fire_Data[6][10]; 
	
	extern const uint8_t Map_Addr_H[6][10];
	//C89规定局部变量的声明必须在开头
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


////复位LED数据
//void reset_led_data(void)
//{
//	
//	//C89规定局部变量的声明必须在开头
//	uint8_t i;
//	
//	//遍历所有LED
//	for(i=0;i<LED_NUM;i++)
//	{
//		
//		//亮度清零
//		grb_led[i].brightness=0X00;
//		
//		//呼吸方向
//		grb_led[i].direction=BACKWARD;
//	}
//	//
//	
//	//清除芯片数据
//	clear_sk6812Data();
//}
////


////初始化LED
//void init_led(void)
//{
//	
////	//C89规定局部变量的声明必须在开头
////	uint8_t i;

////	//遍历灯光模式
////	for(i=0;i<(MODE_NUM); i++)
////	{
////		
////		//初始化每一个模式步骤为初始化步骤
////		mode_step[i]=STATE_INITIAL;
////		
////		//初始化每一种灯光模式的颜色
////		mode_color[i]=DEFAULT_COLOR;
////	}
////	//
//	
//	//初始化RAINBOW_BREATH模式颜色
//	mode_color[RAINBOW_BREATH]=GREEN_COLOR ;
//	
//	//初始化PARITY_BREATH模式颜色
//	mode_color[PARITY_BREATH]=RED_COLOR ;
//	
//	//初始化ENTIRE_BREATH模式颜色
//	mode_color[ENTIRE_BREATH]=YELLOW_COLOR ;
//	
//	//初始化HALF_PULSE_FLASH模式颜色
//	mode_color[HALF_PULSE_FLASH]=BLUE_COLOR ;
//	
//	//初始化PULSE_FLASH模式颜色
//	mode_color[PULSE_FLASH]=CYAN_COLOR ;
//	
//	//初始化MUSIC_FLASH模式颜色
//	mode_color[MUSIC_FLASH]=PURPLE_COLOR ;
//	
//	//初始化HALF_CHASE模式颜色
//	mode_color[HALF_CHASE]=WHITE_COLOR ;
//	
//	//初始化ENTIRE_CHASE模式颜色
//	mode_color[ENTIRE_CHASE]=GREEN_COLOR ;
//	
//	//初始化OPTICAL_DISK模式颜色
//	mode_color[OPTICAL_DISK]=RED_COLOR ;

//	//复位LED数据
//	reset_led_data();
//	
//	//初始化当前灯光模式
//	current_led_mode=LED_MODE_OFF;
//}
////


//一维坐标按指定参数复制所有灯
void one_coordinate_copy_all_led(ledTypeDef *mled)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//遍历一维坐标所有LED
	for(i=0;i<LED_NUM;i++)
	{
		
		//复制
		grb_led[i]=(*mled);
	}
	//
}
//


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
	
	//遍历保存音量数组
	for(i=1;i<SAV_LENGH;i++)
	{
		
		//刷新前一个
		volume_temp[i-1]=volume_temp[i];
	}
	//
	
	//刷新最后一个
	volume_temp[SAV_LENGH-1]=vol;
	
	//当前音量
	volume.current=volume_temp[SAV_LENGH-1];
	
	//遍历数组
	for(i=0;i<SAV_LENGH;i++)
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
	volume.average=sum/SAV_LENGH;
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
	uint8_t i;
	
	//adc扫描
	//adc_scan ();
	
	//当前AD值小于实测静音AD上限值
	if(audio_ad.current_average <NOISE_AD_AVERAGE)
	//if(audio_ad.current_average <NOISE_AD_PEAK)	
	{
		
		//到达静音时长
		if(volume_ad_mute_count>(LED_MUTE_TIMES ))
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
			if(audio_ad.current_average<=volume_ad_step[i])
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


//彩虹七色
void rainbow(uint8_t direction)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//遍历行
	for(i=1;i<LED_ROWS;i++)
	{

		//亮度跟随第一个
		grb_led[i].brightness=grb_led[0].brightness;
		
		//颜色跟随前一个颜色加一
		grb_led[i].color=value_loop (grb_led[i-1].color, direction , COLOR_MIN, LED_ROWS);

	}
	//
}
//


//整体呼吸(传参设定颜色)
void entire_breath(uint8_t color)
{

	//设定颜色
	grb_led[0].color=color;
	
	//呼吸
	breath (&grb_led[0]);

	//复制一维坐标所有LED跟随第一个
	one_coordinate_copy_all_led(&grb_led[0]);
	
	//对所有LED写数据
	write_all_led ();

}
//


//设置亮度（传参亮度）
void set_brightness(uint8_t brightness)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//遍历所有灯
	for(i=0;i<LED_NUM;i++)
	{
		
		//设定亮度
		grb_led[i].brightness =brightness ;
	}
	//
}
//


//设置颜色（传参颜色）
void set_color(uint8_t color)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//遍历所有灯
	for(i=0;i<LED_NUM;i++)
	{
		
		//设定亮度
		grb_led[i].color =color ;
	}
	//
}
//


//设置亮度和颜色
void set_brightness_color(uint8_t brightness,uint8_t color)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;

	for(i=0;i<LED_NUM;i++)
	{
		//设定颜色
		grb_led[i].color=mode_color[current_led_mode ];
		
		//设定亮度
		grb_led[i].brightness =brightness ;
	}
	//
}
//


//LED_MODE_START
//脉冲跟随音量(总级数,方向)
void pulse_follow_volume(uint8_t total_rows,uint8_t direction)//2\3
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//音量扫描
	volume_scan(total_rows+1);

	//静音
	if(volume.current==VOLUME_MUTE)
	{
		
		//频率计数时间到
		if(user_freq(&freqer1)==TIME_UP)
		{
		
			//设定速度
			freqer1.period=100;
			
			//设定亮度和颜色
			set_brightness_color (0X01,mode_color[current_led_mode ]);
			
			//对所有LED写数据
			write_all_led ();
		}
		//
	}
	//
			
	//非静音
	else 
	{

		//脉冲往上
		if(current_volume_count<current_volume_max)
		{
			
			//当前音量大于上限音量值
			if(volume.current>current_volume_max)
			{
				
				//更新上限值
				current_volume_max=volume.current;
			}
			//
			
			//频率计数时间到
			if(user_freq(&freqer1)==TIME_UP)
			{
			
				//点亮行计数加
				current_volume_count++;
				
				//遍历行前半部
				for(i=0;i<total_rows ;i++)
				{
					
					//设定颜色
					grb_led[i].color=mode_color[current_led_mode ];

					//设定亮度值
					grb_led[i].brightness =BRIGHTNESS_MAX*(i<current_volume_count);
				}
				//
				
				//反序
				if(!direction )
				{
					
					//反向排序
					inversion_array(grb_led ,total_rows);
				}
				//
				
				//遍历后半行
				for(i=total_rows;i<LED_ROWS ;i++)
				{
				
					//反向互补
					grb_led[i]=grb_led[i-total_rows];
				}
				//
				
				//对所有LED写数据
				write_all_led ();
			}
			//
		}
		//
		
		//脉冲往下
		else if(current_volume_count>current_volume_max)
		{
			
			//当前音量小于上限音量值
			if(volume.current<current_volume_max)
			{
				
				//更新上限值
				current_volume_max=volume.current;
			}
			//
			
			//频率计数时间到
			if(user_freq(&freqer1)==TIME_UP)
			{
			
				//点亮行计数加
				current_volume_count--;
				
				//遍历行前半部
				for(i=0;i<total_rows ;i++)
				{
					
					//设定颜色
					grb_led[i].color=mode_color[current_led_mode ];

					//设定亮度值
					grb_led[i].brightness =BRIGHTNESS_MAX*(i<current_volume_count);

				}
				//
				
				//反序
				if(!direction )
				{
					
					//反向排序
					inversion_array(grb_led ,total_rows);
				}
				//
				
				//遍历后半行
				for(i=total_rows;i<LED_ROWS ;i++)
				{
				
					//反向互补
					grb_led[i]=grb_led[i-total_rows];
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
			
			//更新上限值
			current_volume_max=volume.current;
		}
		//
		
//		//计数为准静音
//		if(current_volume_count==VOLUME_QUASI_MUTE)
//		{

//			//颜色从R到RGB循环
//			point_value_loop(&mode_color[current_led_mode],NEXT, COLOR_MIN, COLOR_MAX);
//			
//			//取反
//			//col_direction[0]=!col_direction[0];

//		}
//		//

		//音量变化率
		volume.slope=abs(current_volume_max-volume.current) ;

		//设定频率计数周期
		freqer1.period=120/total_rows*abs(total_rows-volume.slope);

	}
	//
}
//1


//渐渐变亮
void gradually_brighten(uint8_t step)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//当前计数的LED亮度未完全点亮
	if(grb_led[current_volume_count].brightness <BRIGHTNESS_MAX )
	{
		
		//速度控制	
		//频率计数时间到
		//if(user_freq(&freqer1)==TIME_UP)
		{
		
			//逐渐点亮
			grb_led[current_volume_count].brightness++;
			
			//遍历后半行
			for(i=step;i<LED_ROWS ;i++)
			{
			
				//反向互补
				grb_led[i]=grb_led[i-step];
			}
			//
			
			//对所有LED写数据
			write_all_led ();
		}
		//
	}
	//
	
	//当前计数的LED亮度完全点亮
	else
	{
		
		//当前计数还未超过上限值
		if(current_volume_count<(step-1))
		{
			
			//计数自加
			current_volume_count++;
			
		}
		//
		
		//超过数组上限边界
		else
		{
			
			//记录前一个音量
			//previous_volume=current_volume_max;
			
			//更新上限值
			current_volume_max=volume.current;
		}
		//
	}
	//
}
//


//渐渐变暗
void gradually_darken(uint8_t step)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//当前计数的LED亮度未完全熄灭
	if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
	{
		
		//速度控制	
		//频率计数时间到
		//if(user_freq(&freqer1)==TIME_UP)
		{
		
			//逐渐熄灭
			grb_led[current_volume_count].brightness--;
			
			//遍历后半行
			for(i=step;i<LED_ROWS ;i++)
			{
			
				//反向互补
				grb_led[i]=grb_led[i-step];
			}
			//
			
			//对所有LED写数据
			write_all_led ();
		}
		//
	}
	//
	
	//当前计数的LED亮度完全熄灭
	else
	{
		
		//当前计数还未超过下限值
		if(current_volume_count>0)
		{
			
			//计数自减
			current_volume_count--;
		}
		//
		
		//超过数组下限边界
		else
		{
			
			//记录前一个音量
			//previous_volume=current_volume_max;
			
			//更新上限值
			current_volume_max=volume.current;
		}
		//
	}
	//
}
//


//脉冲呼吸跟随音量(总级数)
//void pulse_breath_follow_volume(uint8_t step)//2/3
//{
//	
//	//C89规定局部变量的声明必须在开头
//	uint8_t i;

//	//音量扫描
//	current_volume=volume_scan(step);

//	//当前静音
//	if(current_volume==VOLUME_MUTE)
//	{
//		
//		//频率计数时间到
//		if(user_freq(&freqer1)==TIME_UP)
//		{
//		
//			//设定周期
//			freqer1.period =100;
//			
//			//设定亮度和颜色
//			set_brightness_color (0X01,mode_color[current_led_mode ]);
//			
//			//对所有LED写数据
//			write_all_led ();
//		}
//		//
//	}
//	//

//	//当前非静音
//	else 
//	{
//		
//		//音量变化率
//		//volume_slope=abs(current_volume_max-previous_volume) ;
//		
//		//设定周期
//		//freqer1.period=(step-volume_slope);
//		
//		//设置颜色
//		set_color(mode_color[current_led_mode ]);
//		
//		//从前往后遍历所有音阶
//		for(i=0;i<step;i++)
//		{
//			
//			//前面灯
//			if(i<current_volume_count)
//			{
//				
//				//没有完全点亮
//				if(grb_led[i].brightness <BRIGHTNESS_MAX )
//				{
//					
//					//从没有完全点亮的位置开始
//					current_volume_count=i;
//					
//					break;
//				}
//				//
//			}
//			//

//			//后面灯
//			else if(i>current_volume_count)
//			{
//				
//				//没有完全熄灭
//				if(grb_led[i].brightness >BRIGHTNESS_OFF )
//				{
//					
//					//从没有完全熄灭的位置开始
//					current_volume_count=i;
//				}
//				//
//			}
//			//
//		}
//		//
//		
//		//脉冲准静音//熄灭所有灯
//		if(current_volume_max==VOLUME_QUASI_MUTE)
//		{
//			
//			//当前计数的LED亮度未完全熄灭
//			if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
//			{
//				
//				//速度控制	
//				//频率计数时间到
//				//if(user_freq(&freqer1)==TIME_UP)
//				{
//				
//					//逐渐熄灭
//					grb_led[current_volume_count].brightness--;
//					
//					//遍历后半行
//					for(i=step;i<LED_ROWS ;i++)
//					{
//					
//						//反向互补
//						grb_led[i]=grb_led[i-step];
//					}
//					//
//					
//					//对所有LED写数据
//					write_all_led ();
//				}
//				//
//			}
//			//
//			
//			//当前计数的LED亮度完全熄灭
//			else
//			{
//				
//				//当前计数还未超过下限值
//				if(current_volume_count>0)
//				{
//					
//					//计数自减
//					current_volume_count--;
//				}
//				//
//				
//				//超过数组下限边界
//				else
//				{
//					
//					//记录前一个音量
//					//previous_volume=current_volume_max;
//					
//					//更新上限值
//					current_volume_max=current_volume;
//				}
//				//
//			}
//			//
//		}
//		//
//		
//		//非准静音
//		else
//		{
//		
//			//脉冲到顶
//			if(current_volume_count==(current_volume_max))
//			{
//				
//				//当前计数的LED亮度未完全点亮
//				if(grb_led[current_volume_count-1].brightness <BRIGHTNESS_MAX )
//				{
//					
//					//速度控制	
//					//频率计数时间到
//					//if(user_freq(&freqer1)==TIME_UP)
//					{
//					
//						//逐渐点亮
//						grb_led[current_volume_count-1].brightness++;
//						
//						//遍历后半行
//						for(i=step;i<LED_ROWS ;i++)
//						{
//						
//							//反向互补
//							grb_led[i]=grb_led[i-step];
//						}
//						//
//						
//						//对所有LED写数据
//						write_all_led ();
//					}
//					//
//				}
//				//
//				
//				//当前计数的LED亮度完全点亮
//				else
//				{
//					
//					//当前计数还未超过上限值
//					if(current_volume_count<(step-1))
//					{
//						
//						//计数自加
//						current_volume_count++;
//						
//					}
//					//
//					
//					//超过数组上限边界
//					else
//					{
//						
//						//记录前一个音量
//						//previous_volume=current_volume_max;
//						
//						//更新上限值
//						current_volume_max=current_volume;
//					}
//					//
//				}
//				//
//				
//			}
//			//
//			
//			//脉冲往下
//			else if(current_volume_count>(current_volume_max))
//			{
//				
//				//渐渐变暗
//				gradually_darken(step);
//			}
//			//
//			
//			//脉冲往上
//			else  if(current_volume_count<(current_volume_max))
//			{
//				
//				//渐渐变亮
//				gradually_brighten(step);
//			}
//			//
//		}
//		//
//	}
//	//
//}
////2/3


//脉冲呼吸跟随音量(总级数)
void pulse_breath_follow_volume(uint8_t step)//
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;

	//音量扫描
	volume_scan(step+1);

	//当前静音
	if(volume.current==VOLUME_MUTE)
	{
		
		//频率计数时间到
		if(user_freq(&freqer1)==TIME_UP)
		{
		
			//设定周期
			freqer1.period =100;
			
			//设定亮度和颜色
			set_brightness (0X01);
			
			//对所有LED写数据
			write_all_led ();
		}
		//
	}
	//

	//当前非静音
	else 
	{
		
		//上限没有音量//熄灭所有灯
		if(current_volume_max<=VOLUME_QUASI_MUTE)
		{
			
			//从前往后遍历所有音阶
			for(i=0;i<step;i++)
			{

				//没有完全熄灭
				if(grb_led[i].brightness >BRIGHTNESS_OFF )
				{
					
					//从没有完全熄灭的位置开始
					current_volume_count=i;
				}
				//
			}
			//从前往后遍历所有音阶
			
			//当前计数的LED亮度未完全熄灭
			if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
			{
				
				//速度控制	
				//频率计数时间到
				//if(user_freq(&freqer1)==TIME_UP)
				{
				
					//慢慢熄灭
					grb_led[current_volume_count].brightness--;
					
					//遍历后半行
					for(i=step;i<LED_ROWS ;i++)
					{
					
						//反向互补
						grb_led[i]=grb_led[i-step];
					}
					//
					
					//对所有LED写数据
					write_all_led ();
				}
				//
			}
			//
			
			//当前计数的LED亮度完全熄灭
			else
			{
				
				//更新上限值
				current_volume_max=volume.max;
			}
			//
		}
		//
		
		//上限有音量
		else if(current_volume_max>VOLUME_QUASI_MUTE)
		{
			
			//从前往后遍历所有音阶
			for(i=0;i<step;i++)
			{
				
				//前面灯
				if(i<current_volume_count)
				{
					
					//没有完全点亮
					if(grb_led[i].brightness <BRIGHTNESS_MAX )
					{
						
						//从没有完全点亮的位置开始
						current_volume_count=i;
						
						break;
					}
					//
				}
				//

				//后面灯
				else if(i>current_volume_count)
				{
					
					//没有完全熄灭
					if(grb_led[i].brightness >BRIGHTNESS_OFF )
					{
						
						//从没有完全熄灭的位置开始
						current_volume_count=i;
					}
					//
				}
				//
			}
			//从前往后遍历所有音阶

			//脉冲到顶
			if(current_volume_count==(current_volume_max-1))
			{
				
				//当前计数的LED亮度未完全点亮
				if(grb_led[current_volume_count].brightness <BRIGHTNESS_MAX )
				{
					
					//速度控制	
					//频率计数时间到
					//if(user_freq(&freqer1)==TIME_UP)
					{
					
						//慢慢点亮
						grb_led[current_volume_count].brightness++;
						
						//遍历后半行
						for(i=step;i<LED_ROWS ;i++)
						{
						
							//反向互补
							grb_led[i]=grb_led[i-step];
						}
						//
						
						//对所有LED写数据
						write_all_led ();
					}
					//
				}
				//
				
				//当前计数的LED亮度完全点亮
				else
				{
					
					//更新上限值
					current_volume_max=volume.current;
				}
				//
			}
			//脉冲到顶
			
			//脉冲往下
			else if((current_volume_max-1)<current_volume_count)
			{
				
				//当前计数的LED亮度未完全熄灭
				if(grb_led[current_volume_count].brightness >BRIGHTNESS_OFF )
				{
					
					//速度控制	
					//频率计数时间到
					//if(user_freq(&freqer1)==TIME_UP)
					{
					
						//慢慢熄灭
						grb_led[current_volume_count].brightness--;
						
						//遍历后半行
						for(i=step;i<LED_ROWS ;i++)
						{
						
							//反向互补
							grb_led[i]=grb_led[i-step];
						}
						//
						
						//对所有LED写数据
						write_all_led ();
					}
					//
				}
				//
				
				//当前计数的LED亮度完全熄灭
				else
				{
					
					//音量有变化
					//if(volume.current<current_volume_max)
					{
						
						//更新上限值
						current_volume_max=volume.min;
					}
					//
					
					//当前计数还未超过下限值
					if(current_volume_count>0)
					{
						
						//自减
						current_volume_count--;
					}
					//
				}
				//
			}
			//脉冲往下
			
			//脉冲往上
			else  if((current_volume_max-1)>current_volume_count)
			{
				
				//当前计数的LED亮度未完全点亮
				if(grb_led[current_volume_count].brightness <BRIGHTNESS_MAX )
				{
					
					//速度控制	
					//频率计数时间到
					//if(user_freq(&freqer1)==TIME_UP)
					{
					
						//慢慢点亮
						grb_led[current_volume_count].brightness++;
						
						//遍历后半行
						for(i=step;i<LED_ROWS ;i++)
						{
						
							//反向互补
							grb_led[i]=grb_led[i-step];
						}
						//
						
						//对所有LED写数据
						write_all_led ();
					}
					//
				}
				//
				
				//当前计数的LED亮度完全点亮
				else
				{
					
					//音量有变化
					//if(volume.current>current_volume_max)
					{
						
						//更新上限值
						current_volume_max=volume.max ;
					}
					//

					//当前计数还未超过上限值
					if(current_volume_count<(current_volume_max-1))
					{
						
						//自加
						current_volume_count++;
						
					}
					//
				}
				//
			}
			//脉冲往上
		}
		//脉冲非准静音

		//音量变化率
		//volume_slope=abs(current_volume_max-previous_volume) ;
		
		//设定周期
		//freqer1.period=(step-volume_slope);
		//freqer1.period=0;
	}
	//
}
//2/3


//彩虹七色
void rainbow_different_rotate_color(void)//8
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//音量扫描
	volume_scan(VOLUME_STEP);

	//频率计数时间到
	if(user_freq(&freqer1)==TIME_UP)
	{
//		
//		//当前呼吸
//		breath(&grb_led[mode_color[current_led_mode]]);
//		
		//呼吸到下限值
		//if(grb_led[mode_color[current_led_mode]].brightness==BRIGHTNESS_MIN)
		{
			
			//颜色从R到RGB循环
			back_and_forth(&mode_color[current_led_mode],&col_direction[0], 0, 1);
		}
		//
		
		//遍历前三分之一
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
		
		//遍历剩余
		for(i=LED_ONE_THIRD;i<(LED_ROWS);i++)
		{

			//亮度跟随第一个
			grb_led[i].brightness=grb_led[i-2].brightness;
			grb_led[i].color =grb_led[i-2].color<<1 ;
		}
		//

		//设定频率计数周期
		freqer1.period=50*(VOLUME_STEP-volume.current);
		
		//对所有LED写数据
		write_all_led ();
	}
	//
}
//8


//彩虹呼吸
void rainbow_breath(void)//1
{
	
	//音量扫描
	volume_scan(VOLUME_STEP);

	//频率计数时间到
	if(user_freq(&freqer1)==TIME_UP)
	{
		
		//设定第一个亮度呼吸
		breath (&grb_led[0]);
		
		//呼吸到下限亮度时
		if(grb_led[0].brightness<=BRIGHTNESS_MIN)
		{
			
//			//设定频率计数周期
//			freqer1.period=(VOLUME_STEP-current_volume);
			
			//切换颜色
			point_value_loop(&mode_color[current_led_mode], col_direction[0], COLOR_MIN, LED_ROWS);
			
		}
		//
		
//		//呼吸到上限亮度时
//		else if(grb_led[0].brightness>=BRIGHTNESS_MAX)
//		{
//			
//			//设定频率计数周期
//			freqer1.period=(VOLUME_STEP-current_volume);
//			
//		}
//		//

		//设定第一个LED颜色
		grb_led[0].color=mode_color[current_led_mode];
		
		//彩虹七色
		rainbow(col_direction[0]);
		
		//对所有LED写数据
		write_all_led ();
		
		//设定频率计数周期
		freqer1.period=(VOLUME_STEP-volume.average);
		
	}
	//
	
	
}
//1


//奇偶呼吸
void parity_breath(void)//2
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;
	
	//音量扫描
	volume_scan(VOLUME_STEP);
	
	//频率计数时间到
	if(user_freq(&freqer1)==TIME_UP)
	{

		//整体呼吸(指定颜色)
		//entire_breath(mode_color [current_led_mode ]);
		
		//设定颜色
		grb_led[0].color=mode_color [current_led_mode ];
		
		//呼吸
		breath(&grb_led[0]);
		
		//在最低亮度时
		if(grb_led[0].brightness==BRIGHTNESS_MIN)
		{
			
			//自动切换颜色
			point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);
			
			//自动切换颜色
			point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);
			//一次切换两色
		}
		//

		//遍历所有LED
		for(i=0;i<LED_NUM;i++)
		{

			//偶数
			if(i%2==0)
			{
				
				//亮度
				grb_led[i]=grb_led[0];
			}
			//
			
			//奇数
			else
			{
				
				//在最低亮度时
				if(grb_led[i].brightness==BRIGHTNESS_MIN )
				{
				
					//设定颜色跟随前一个
					grb_led[i].color=value_loop(grb_led[i-1].color,NEXT,COLOR_MIN,COLOR_MAX);
				}
				//
				
				//亮度互补
				grb_led[i].brightness=BRIGHTNESS_MAX-grb_led[0].brightness;
			}
			//
		}
		//

		//对所有LED写数据
		write_all_led ();
		
		//设定频率计数周期
		freqer1.period=(VOLUME_STEP-volume.average);
	}
	//
}
//2



//LED模式-七彩呼吸自动变化
void breath_seven_color_change(void)//3
{

	//音量扫描
	volume_scan(VOLUME_STEP);
	
	//频率计数时间到
	if(user_freq(&freqer1)==TIME_UP)
	{
		
		//整体呼吸(指定颜色)
		entire_breath(mode_color [current_led_mode ]);
		
		//在最低亮度时
		if(grb_led[0].brightness==BRIGHTNESS_MIN )
		{
			
			//自动切换颜色
			point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);

		}
		//
		
		//设定频率计数周期
		freqer1.period=(VOLUME_STEP-volume.average);
	}
	//
}
//3


//LED模式-所有灯常量一种颜色
void still_all_led_one_color(void)//5
{
	
	//频率计数时间到
	if(user_freq(&freqer1)==TIME_UP)
	{
	
		//设定频率计数周期
		freqer1.period=100 ;

		//设定亮度
		//grb_led[0].brightness =max_brightness ;
		
		//设定颜色值
		//grb_led[0].color=mode_color[current_led_mode];
		
		//复制一维坐标所有LED跟随第一个
		//one_coordinate_copy_all_led(&grb_led[0]);
		
		//设置亮度和颜色
		set_brightness_color (BRIGHTNESS_MAX,mode_color[current_led_mode]);
		
		//对所有LED写数据
		write_all_led ();
	}
	//
}
//5


//随音乐闪烁
void follow_music_flash(void)//6
{
	
	//记录前一个音量
	//previous_volume=current_volume;
	
	//音量扫描
	volume_scan(VOLUME_STEP);

	//静音
	if(volume.current ==VOLUME_MUTE)
	{
		
		//频率计数时间到
		if(user_freq(&freqer1)==TIME_UP)
		{
			
			//设定周期
			freqer1.period=100;
			
			//设定亮度和颜色
			set_brightness_color (0X01,mode_color[current_led_mode ]);
			
			//对所有LED写数据
			write_all_led ();
		}
		//
	}
	//
	
	//非静音
	else
	{

		//设定颜色
		grb_led[0].color=mode_color [current_led_mode ];
		
		//当前亮度值小于当前音量亮度值
		if(grb_led[0].brightness<(current_volume_max *FLASH_PER_STEP))
		{

			//速度控制	
			//频率计数时间到
			//if(user_freq(&freqer1)==TIME_UP)
			{
		
				//当前亮度值渐渐变大
				grb_led[0].brightness++;
				
				//复制所有LED跟随第一个
				one_coordinate_copy_all_led(&grb_led[0]);
				
				//对所有LED写数据
				write_all_led ();
			}
			//
		}
		//
		
		//当前亮度值大于当前音量亮度值
		else if(grb_led[0].brightness>(current_volume_max *FLASH_PER_STEP))
		{
			
			//速度控制	
			//频率计数时间到
			//if(user_freq(&freqer1)==TIME_UP)
			{
				
				//当前亮度值渐渐变小
				grb_led[0].brightness--;
				
				//复制所有LED跟随第一个
				one_coordinate_copy_all_led(&grb_led[0]);
				
				//对所有LED写数据
				write_all_led ();
			}
			//
		}
		//
		
		//当前亮度值等于当前音量亮度值
		else
		{
			
			//高音量时
			if(volume.current>(VOLUME_STEP>>1))
			{
				
				//音量突变
				if(abs(volume.current-current_volume_max) >=(VOLUME_STEP>>1))
				{
					
					//更新音量变化率
					volume.slope=abs(volume.current-current_volume_max);
					
					//更新音量
					current_volume_max=volume.current;
					
					//设定频率计数周期
					freqer1.period=(VOLUME_STEP-volume.slope);
				}
				//
				
				//音量没有突变
				else
				{
					
					//更新音量变化率
					volume.slope=current_volume_max;
					
					//强制更新音量
					current_volume_max=volume.current-(VOLUME_STEP>>1);
					
					//设定频率计数周期
					freqer1.period=(VOLUME_STEP-volume.slope);
				}
				//
			}
			//
			
			//
			else
			{
				
				//更新音量变化率
				volume.slope=abs(volume.current-current_volume_max);
				
				//更新音量
				current_volume_max=volume.current;
				
				//设定频率计数周期
				freqer1.period=(VOLUME_STEP-volume.slope);
			}
			//
			
//			//准静音
//			if(current_volume_max==VOLUME_QUASI_MUTE)
//			{
//				
//				//第一个LED七彩循环切换
//				point_value_loop(&mode_color[current_led_mode ], NEXT, COLOR_MIN, COLOR_MAX);
//			}
//			//
		}
		//
	}
	//
}
//8



////随音乐闪烁
//void follow_music_flash(void)//8
//{
//	
//	//记录前一个音量
//	//previous_volume=current_volume;
//	
//	//音量扫描
//	current_volume=volume_scan(VOLUME_STEP);

//	//静音
//	if(current_volume ==VOLUME_MUTE)
//	{
//		
//		//频率计数时间到
//		if(user_freq(&freqer1)==TIME_UP)
//		{
//			
//			//设定周期
//			freqer1.period=100;
//			
//			//设置亮度和颜色
//			set_brightness_color (0X01,mode_color[current_led_mode]);
//			
//			//对所有LED写数据
//			write_all_led ();
//		}
//		//
//	}
//	//
//	
//	//非静音
//	else
//	{
//		
//		
//		
//		//准静音
//		if(current_volume ==VOLUME_QUASI_MUTE)
//		{
//			
//			//设定频率计数周期
//			freqer1.period=2*(VOLUME_STEP-current_volume);
//			
//			//设置亮度和颜色
//			set_brightness_color (0X00,mode_color[current_led_mode]);
//			
//			//对所有LED写数据
//			write_all_led ();
//		}
//		//
//		
//		//非准静音
//		else
//		{
//			
//			//设定频率计数周期
//			freqer1.period=2*(VOLUME_STEP-current_volume);
//			
//			//设定颜色
//			grb_led[0].color=mode_color [current_led_mode ];

//			//当前亮度值小于上限亮度值
//			if(grb_led[0].brightness<max_brightness)
//			{
//				
//				//如果产生了新上限亮度值
//				if(max_brightness<(audio_ad.current_average/10))
//				{
//					
//					//更新上限亮度值
//					max_brightness=(audio_ad.current_average/10);

//				}
//				//
//				
//				//速度控制	
//				if(user_freq(&freqer1)==TIME_UP)
//				{
//					
//					//当前亮度值渐渐变大
//					grb_led[0].brightness++;
//					
//					//复制所有LED跟随第一个
//					one_coordinate_copy_all_led(&grb_led[0]);
//					
//					//对所有LED写数据
//					write_all_led ();
//				}
//				//
//			}
//			//
//			
//			//当前亮度值大于上限亮度值
//			else if(grb_led[0].brightness>max_brightness)
//			{
//				
//				//如果产生了新上限亮度值
//				if(max_brightness>(audio_ad.current_average/10))
//				{
//					
//					//更新上限亮度值
//					max_brightness=(audio_ad.current_average/10);

//				}
//				//
//				
//				//速度控制	
//				if(user_freq(&freqer1)==TIME_UP)
//				{
//					
//					//当前亮度值渐渐变小
//					grb_led[0].brightness--;
//					
//					//复制所有LED跟随第一个
//					one_coordinate_copy_all_led(&grb_led[0]);
//					
//					//对所有LED写数据
//					write_all_led ();
//				}
//				//
//			}
//			//
//			
//			//当前亮度值等于上限亮度值
//			else
//			{

//				//更新上限亮度值
//				max_brightness=(audio_ad.current_average/10);

//			}
//			//
//		}
//		//
//	}
//	//
//}
////8


////逐光呼吸（总行数）
//void chase_breath(uint8_t total_rows)
//{
//	
//	//C89规定局部变量的声明必须在开头
//	uint8_t i;
//	
//	//状态
//	switch (mode_step[current_led_mode ])
//	{
//		
//		//初始化状态
//		case STATE_INITIAL:
//		{

//			//计算等差亮度值
//			increasing=(((BRIGHTNESS_MAX-BRIGHTNESS_MIN)<<1)/(total_rows));
//			
//			//正向遍历
//			for(i=0;i<(total_rows);i++)
//			{
//				
//				//设定颜色;
//				grb_led[i].color=mode_color[current_led_mode];
//				
//				//防止溢出
//				if((i*increasing)>BRIGHTNESS_MAX)
//				{
//					
//					//最大值减去超过最大值的数
//					grb_led[i].brightness=(BRIGHTNESS_MAX<<1)-i*increasing;
//					
//					//反转反向
//					grb_led[i].direction=!grb_led[i].direction;
//				}
//				//
//				
//				else
//				{
//					//初始化等差亮度
//					grb_led[i].brightness=i*increasing;
//				}
//				//
//			}
//			//
//			
//			//下一个状态
//			mode_step[current_led_mode ]++;
//			
//		}
//		//
//		break;
//		
//		//刷新状态
//		case STATE_REFLASH:
//		{
//			
//			//音量扫描
//			current_volume =volume_scan(total_rows);

//			//频率计数时间到
//			if(user_freq(&freqer1)==TIME_UP)
//			{
//				
//				//设定周期
//				freqer1.period=(total_rows-current_volume);

//				//遍历前半部分
//				for(i=0;i<(total_rows);i++)
//				{

//					//呼吸
//					breath (&grb_led[i]);
//			
//					//在最低亮度时
//					if(grb_led[i].brightness==BRIGHTNESS_MIN)
//					{
//						
//						if(i==0)
//						{
//							
//							//自动切换颜色
//							point_value_loop(&mode_color[current_led_mode], NEXT, COLOR_MIN, COLOR_MAX);
//						
//						}
//						//
//						
//						//设定颜色;
//						grb_led[i].color=mode_color[current_led_mode];
//					}
//					//
//				}
//				//
//				
//				//遍历后半部分
//				for(i=total_rows;i<(LED_ROWS);i++)
//				{
//					
//					//反向互补
//					grb_led[i]=grb_led[i-LED_HALF_ROWS];
//				}
//				//

//				//对所有LED写数据
//				write_all_led ();
//			}
//			//
//		}
//		//
//		break;
//		
//		//出错
//		default :
//		{
//			
//			//设置模式状态为初始化
//			mode_step[current_led_mode ]=STATE_INITIAL;
//			
//		}
//		//
//		break;
//	}
//	//
//}
////10


//逐光呼吸（总行数，方向）
void chase_breath(uint8_t total_rows,uint8_t direction)
{
	
	//C89规定局部变量的声明必须在开头
	uint8_t i;

	//用于记录递归前一个灯的编码
	uint8_t previous;
	
	//音量扫描
	volume_scan(total_rows);

	//频率计数时间到
	if(user_freq(&freqer1)==TIME_UP)
	{
		
		//设定周期
		freqer1.period=(total_rows-volume.current);
	
		//计算等差亮度值
		increasing=(((BRIGHTNESS_MAX-BRIGHTNESS_MIN)<<1)/total_rows);
		
		//设定颜色;
		grb_led[0].color=mode_color[current_led_mode];

		//模拟呼吸
		breath (&grb_led[0]);
		
		//呼吸一遍
		if(grb_led[0].brightness ==BRIGHTNESS_MIN)
		{

			//自动切换颜色
			point_value_loop(&mode_color[current_led_mode], NEXT, COLOR_MIN, COLOR_MAX);
		}
		//

		//向前遍历行后半部分,用于递归操作
		for(i=(total_rows-1);i>(0);i--)
		{
		
			//下一个编码，并判断是否越界
			previous=value_loop (i,direction,0,total_rows-1);
			
			//递归后一个
			recursion(&grb_led[previous],&grb_led[i],increasing);
		}
		//
		
		//遍历剩余部分
		for(i=total_rows;i<(LED_ROWS);i++)
		{
			
			//反向互补
			grb_led[i]=grb_led[i-LED_HALF_ROWS];
		}
		//

		//对所有LED写数据
		write_all_led ();
	}
	//	
//	//C89规定局部变量的声明必须在开头
//	uint8_t i;
//	
//	//状态
//	switch (mode_step[current_led_mode ])
//	{
//		
//		//初始化状态
//		case STATE_INITIAL:
//		{

//			//计算等差亮度值
//			increasing=(((BRIGHTNESS_MAX-BRIGHTNESS_MIN)<<1)/(total_rows));
//			
//			//正向遍历
//			for(i=0;i<(total_rows);i++)
//			{
//				
//				//设定颜色;
//				grb_led[i].color=mode_color[current_led_mode];
//				
//				//防止溢出
//				if((i*increasing)>BRIGHTNESS_MAX)
//				{
//					
//					//最大值减去超过最大值的数
//					grb_led[i].brightness=(BRIGHTNESS_MAX<<1)-i*increasing;
//					
//					//反转反向
//					grb_led[i].direction=!grb_led[i].direction;
//				}
//				//
//				
//				else
//				{
//					//初始化等差亮度
//					grb_led[i].brightness=i*increasing;
//				}
//				//
//			}
//			//
//			
//			//反序
//			if(!direction )
//			{
//				
//				//反向排序
//				inversion_array(grb_led ,total_rows);
//			}
//			//
//			
//			//下一个状态
//			mode_step[current_led_mode ]++;
//			
//		}
//		//
//		break;
//		
//		//刷新状态
//		case STATE_REFLASH:
//		{
//			
//			//音量扫描
//			current_volume =volume_scan(total_rows);

//			//频率计数时间到
//			if(user_freq(&freqer1)==TIME_UP)
//			{
//				
//				//设定周期
//				freqer1.period=(total_rows-current_volume);

//				//遍历前半部分
//				for(i=0;i<(total_rows);i++)
//				{

//					//呼吸
//					breath (&grb_led[i]);
//			
//					//在最低亮度时
//					if(grb_led[i].brightness==BRIGHTNESS_MIN)
//					{
//						
//						if(i==0)
//						{
//							
//							//自动切换颜色
//							point_value_loop(&mode_color[current_led_mode], NEXT, COLOR_MIN, COLOR_MAX);
//						
//						}
//						//
//						
//						//设定颜色;
//						grb_led[i].color=mode_color[current_led_mode];
//					}
//					//
//				}
//				//
//				
//				//遍历后半部分
//				for(i=total_rows;i<(LED_ROWS);i++)
//				{
//					
//					//反向互补
//					grb_led[i]=grb_led[i-LED_HALF_ROWS];
//				}
//				//

//				//对所有LED写数据
//				write_all_led ();
//			}
//			//
//		}
//		//
//		break;
//		
//		//出错
//		default :
//		{
//			
//			//设置模式状态为初始化
//			mode_step[current_led_mode ]=STATE_INITIAL;
//			
//		}
//		//
//		break;
//	}
//	//
}
//10


////LED模式-关闭所有LED
//void close_all_led(void)//OFF
//{
//	
//	//频率计数时间到
//	if(user_freq(&freqer1)==TIME_UP)
//	{
//	
//		//if(!led_off_flag)
//		{
//			
//			//led_off_flag=1;
//			
//			//设定周期
//			freqer1.period =100;
//		
//			//清除LED数据
//			//clear_ws2811Data();
//			
//			//设置亮度关，无颜色
//			set_brightness_color(BRIGHTNESS_OFF ,NO_COLOR );
//			
//			//对所有LED写数据
//			write_all_led ();
//			
//			//GP222需要关闭灯电源IO口
//			power_control(L);
//		}
//		//
//	}
//	//
//}
////OFF
////LED_MODE_END


////external_fuction_start
////切换LED颜色(传参方向，true下一个模式，false上一个模式)
//void switch_led_color(uint8_t direction)
//{
//	
//	//第一个LED七彩手动循环切换
//	point_value_loop(&mode_color[current_led_mode ], direction, COLOR_MIN, COLOR_MAX);
//}
////


////切换LED模式(传参方向，true下一个模式，false上一个模式)
//void switch_led_mode(uint8_t direction)
//{
//	
//	//蓝牙开
//	if(read_bt_state()==STATE_ON)
//	{
//		
//		//需要打开LED电源
//		power_control(H);
//		
//		//保存过灯光模式
//		if(save_led_mode_flag)
//		{
//			
//			//复位标志位
//			save_led_mode_flag=0;
//		
//			//恢复保存的模式
//			current_led_mode=previous_led_mode ;
//		}
//		//
//		
//		//切换
//		else
//		{
//		
//			//切换模式（当前LED模式，方向，LED_MODE_MIN到LED_MODE_MAX）
//			point_value_loop (&current_led_mode,direction ,LED_MODE_MIN,LED_MODE_MAX);
//			
//			//如果当前灯光模式处于开启状态
//			if(current_led_mode !=LED_MODE_OFF)
//			{
//				
//				//更新要保存的灯光模式
//				previous_led_mode=current_led_mode ;
//			}
//			//
//		}
//		//
//		
//		//设定周期，立即响应
//		freqer1.period  =0;
//		
//		//设置模式状态为初始化
//		//mode_step[current_led_mode ]=STATE_INITIAL;

//		//复位灯数据
//		reset_led_data();
//	}
//	//
//}
////

////切换电源
//void switch_power(void)
//{

//	//蓝牙开
//	if(read_bt_state()==STATE_ON)
//	{
//				
//		//灯光模式开
//		if(current_led_mode!=LED_MODE_OFF)
//		{
//			
//			//保存标志
//			save_led_mode_flag=1;
//			
//			//保存关闭前的灯光模式
//			previous_led_mode=current_led_mode;
//			
//			//切换灯光模式关
//			current_led_mode=LED_MODE_OFF;
//		}
//		//
//		
//		//灯光模式关
//		else
//		{
//			
//			//切换下一个模式
//			switch_led_mode(NEXT);
//		}
//		//
//		
//		//复位灯数据
//		reset_led_data();
//	}
//	//
//}
////


////LED处理进程
//void led_handle_process(void)
//{

//	//LED模式选择
//	switch (current_led_mode)
//	{

//		case RAINBOW_BREATH:
//		{
//			
//			//彩虹呼吸
//			rainbow_breath();
//			
//		}
//		//
//		break;

//		case PARITY_BREATH:
//		{	
//			
//			//奇偶呼吸
//			parity_breath();

//		}
//		//
//		break;
//		
//		case ENTIRE_BREATH:
//		{
//			
//			//所有灯常量一种颜色
//			//still_all_led_one_color();
//			
//			//呼吸七彩变化
//			breath_seven_color_change();

//		}
//		//
//		break;
//		
//		case HALF_PULSE_FLASH:
//		{
//			
//			//脉冲半行
//			
//			//脉冲呼吸跟随音量（半行）
//			pulse_breath_follow_volume(LED_HALF_ROWS);
//			
//			//脉冲跟随音量（前半行,反向）
//			//pulse_follow_volume(LED_HALF_ROWS,col_direction[0]);
//		}
//		//
//		break;
//		
//		case PULSE_FLASH:
//		{
//			
//			//脉冲整行
//			
//			//脉冲呼吸跟随音量（整行）
//			pulse_breath_follow_volume(LED_ROWS);
//			
//			//脉冲跟随音量（整行，正向）
//			//pulse_follow_volume(LED_ROWS,col_direction[0]);
//			
//		}
//		//
//		break;
//		
//		case MUSIC_FLASH:
//		{
//			
//			//亮度跟随AD呼吸
//			//brightness_follow_ad();
//			
//			//随音乐闪烁
//			follow_music_flash();
//		}
//		//
//		break;
//		
//		case HALF_CHASE:
//		{	
//			
//			
//			//逐光呼吸（半行）
//			chase_breath(LED_HALF_ROWS ,col_direction[0]);

//		}
//		//
//		break;
//		
//		case ENTIRE_CHASE:
//		{
//			
//			//逐光呼吸（所有行）
//			chase_breath(LED_ROWS ,col_direction[0]);
//			
//		}
//		//
//		break;
//		
//		case OPTICAL_DISK:
//		{
//			
//			//彩虹七色
//			rainbow_different_rotate_color();
//		}
//		//
//		break;

//		case LED_MODE_OFF:
//		{
//			
//			//关闭所有LED
//			close_all_led();
//			
//		}
//		//
//		break;
//		
//		default:
//		{
//			
//			//重新初始化led数据
//			init_led();
//		}
//		//
//		break;
//	}
//	//
//}
////
//external_fuction_end
