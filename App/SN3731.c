/*******************/
/*头文件header file*/
/*******************/
#include "app_main.h"
#include "SN3731_array.h"

/************************/
/*宏定义macro definition*/
/************************/
#ifdef STC_MCU
xdata unsigned char load_pwm[144];	//用于装载一帧动画的pwm值
xdata unsigned char load_onoff[18];	//用于装载亮灭值
xdata unsigned char pwm_temp1[32];
xdata unsigned char pwm_temp2[32];
#else
static unsigned char load_pwm[144];	//用于装载一帧动画的pwm值
static unsigned char load_onoff[18];
static unsigned char pwm_temp1[32];
static unsigned char pwm_temp2[32];
#endif

//自带动画
#define g_MOVIE_fireworks  0x01 //烟花动画数据
#define g_MOVIE_lighting   0x02 //mode1动画数据
#define g_MOVIE_unlock	   0x03 //mode2动画数据
#define g_MOVIE_water	   0x04 //mode3动画数据
#define g_MOVIE_tingtaitu  0x05 //mode4动画数据
#define g_MOVIE_audio1	   0x06 //mode5动画数据
#define g_MOVIE_audio2	   0x07
#define g_MOVIE_audio3	   0x08

//在此定义其他动画
#define SN_REG_SWITCH_BASE 0x00
#define SN_REG_BLINK_BASE  0x12
#define SN_REG_PWM_BASE    0x24

#ifdef STC_MCU
extern unsigned char demo_NO;
#endif



/*************************/
/*类型定义byte definition*/
/*************************/

/****************************/
/*标志位定义flags definetion*/
/****************************/
unsigned char ack;	     //应答标志位
unsigned char p_flag = 0;
unsigned char front_4_flag;     //1为播放前4贞标志，0为播放后4贞标志

/*****************************/
/*变量定义variable definition*/
/*****************************/
unsigned char  g_reg_00h = 0;
unsigned char  g_reg_01h = 0;
unsigned char  g_reg_02h = 0;
unsigned char  g_reg_03h = 0;
unsigned char  g_reg_04h = 0;
unsigned char  g_reg_05h = 0;
unsigned char  g_reg_06h = 0;
unsigned char  g_reg_07h = 0;
unsigned char  g_reg_08h = 0;
unsigned char  g_reg_09h = 0;
unsigned char  g_reg_0bh = 0;



//static unsigned char g_SN3731_IIC_ADDRESS = g_SN3731_IIC_ADDRESS_M; //改为了宏定义

unsigned char g_Poweron_ontime = 1;

unsigned char g_dram[36];	/*用于记录0x00h-0x23h的寄存器的值，因为这36个寄存器的每个bit控制对应一个灯的亮灭和闪烁
								因为这36个寄存器的每个bit控制对应一个灯的亮灭和闪烁当调用针对一颗灯的状态操作时，不
								能影响同一字节的其他bit所以就有必要记录下这些字节的状态信息 */

								//其他动画
uint8_t  g_fireworks_NO;	//用于记录烟花动画播放到第几帧
uint8_t  g_fireworks_Temp;
uint8_t  g_lighting_NO;	//用于记录mode1动画播放到第几帧
uint8_t  g_unlock_NO;		//用于记录mode2动画播放到第几帧
uint8_t  g_water_NO;		//用于记录mode3动画播放到第几帧
uint8_t  g_jingtaitu_NO;	//用于记录mode4动画播放到第几帧
uint8_t  g_audio1_NO;		//用于记录mode4 picture 播放到第几帧
uint8_t  g_audio2_NO;
uint8_t  g_audio3_NO;
uint8_t  g_CurrentMovie_NO;
uint8_t  g_rail;
uint8_t  g_times;
uint8_t  g_MovieEnd_Flag;
uint8_t  g_MovieRestart_Flag;
uint8_t  demo_NO; //SN3237驱动使用
uint8_t  timer_flag;

/**************************/
/*数组定义array definition*/
/**************************/

/******************************/
/*函数声明Function declaration*/
/******************************/

/****************************/
/*函数定义function definetion*/
/****************************/
/*******************************************************************
* 函数原型：void SN3731_Led_TurnOn_All(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：所有LED点亮
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Led_TurnOn_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));//Reg_MainAddress,0xFD(此地址为页面控制寄存器)
	SN3731_IIC_WriteRegs(0x01, (uint8_t *)all_led_on, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_on, 144);
}

/*******************************************************************
* 函数原型：void SN3731_Led_TurnOff_All(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：所有LED写灭
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Led_TurnOff_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
}

/*******************************************************************
* 函数原型：void SN3731_blink_TurnOn_All(unsigned char i)
* 输入参数：
* 输出参数： 所有LED开启闪烁
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_blink_TurnOn_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_on, 18);
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_on, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_on, 144);
}

/*******************************************************************
* 函数原型：void SN3731_blink_TurnOff_All(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：所有LED关掉闪烁
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_blink_TurnOff_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_off, 18);
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
}

/*******************************************************************
* 函数原型：void SN3731_DeleteFrame_pwm(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：清除所选择的画面PWM值
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_DeleteFrame_pwm(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
}

/*******************************************************************
* 函数原型：void SN3731_DeleteFrame_OnOff(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：清除所选择的画面onoff值
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_DeleteFrame_OnOff(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
}

/*******************************************************************
* 函数原型：void SN3731_DeleteFrame_blink(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：清除所选择的画面blink值
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_DeleteFrame_blink(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_off, 18);
}

/*******************************************************************
* 函数原型：void SN3731_AudioFrame_EN(unsigned char i)// i: 1 Enable,0 disable.
* 输入参数：
* 输出参数：开启AudioFrame
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_AudioFrame_EN(unsigned char i)// i: 1 Enable,0 disable.
{
	g_reg_00h = g_reg_00h & 0xef;
	g_reg_00h = g_reg_00h | (i << 4);
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ModeConfig, g_reg_00h);

}

/*******************************************************************
* 函数原型：void SN3731_Movie_EN(unsigned char i)	 // i: 1 Enable,0 disable.
* 输入参数：
* 输出参数：
* 函数功能：开启moive 模式
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Movie_EN(unsigned char i)	 // i: 1 Enable,0 disable.
{
	g_reg_00h = (g_reg_00h & 0xf7);
	g_reg_00h = (g_reg_00h | (i << 3));
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ModeConfig, g_reg_00h);
}

/*******************************************************************
* 函数原型：void SN3731_Set_StartFrame_Picture(unsigned char Start_Frame_Picture)//Start_Frame_Picture:0x00~0x07 共8幅画。
* 输入参数：
* 输出参数：
* 函数功能：选择movie mode从哪帧frame开始播放	Start_Frame_Picture:0x00~0x07
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Set_StartFrame_Picture(unsigned char Start_Frame_Picture)//Start_Frame_Picture:0x00~0x07 共8幅画。
{
	g_reg_00h = (g_reg_00h & 0xf8);
	g_reg_00h = (g_reg_00h | Start_Frame_Picture);
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ModeConfig, g_reg_00h);
}

/*******************************************************************
* 函数原型：void SN3731_DisplayFrame(unsigned char i)
* 输入参数：
* 输出参数：
* 函数功能：选择当前显示画面
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_DisplayFrame(unsigned char i)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);//set display mode
	SN3731_IIC_Write_Reg(Reg_PictureAddress, i);
}

/*******************************************************************
* 函数原型：void SN3731_Set_LoopNum(unsigned char loopnum)
* 输入参数：
* 输出参数：
* 函数功能：配置movie 模式，movie播放次数，
			loopnum 0:endless,
			loopnum 1-7:播放1-7次，播放结束INTB产生中断
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Set_LoopNum(unsigned char loopnum)
{
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	g_reg_02h = g_reg_02h & 0x0f;
	g_reg_02h = g_reg_02h | (loopnum << 4);
	SN3731_IIC_Write_Reg(Reg_FramePlay, g_reg_02h);
}

/*******************************************************************
* 函数原型：void SN3731_Set_FrameNum(unsigned char framenum)
* 输入参数：
* 输出参数：
* 函数功能：配置movie 模式，movie播放的帧数，framenum 0:0,1-7:播放的帧数
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Set_FrameNum(unsigned char framenum)
{
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	g_reg_02h = g_reg_02h & 0xf0;
	g_reg_02h = g_reg_02h | framenum;
	SN3731_IIC_Write_Reg(Reg_FramePlay, g_reg_02h);
}

/*******************************************************************
* 函数原型：void SN3731_Set_FrameDelay(unsigned char A)
* 输入参数：
* 输出参数：
* 函数功能：配置movie 模式，movie播放的延时时间，A:0`63
			Delay between frame change in a movie
			A=0-63,当A=0，时间最大，其他情况下时间为A*30ms,最大为30ms*63=1.89s
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Set_FrameDelay(unsigned char A)
{
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_FrameTime, A);
}

/*******************************************************************
* 函数原型：void SN3731_BlinkEN(unsigned char b_en)
* 输入参数：
* 输出参数：
* 函数功能：blink enable function
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_BlinkEN(unsigned char b_en)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (b_en)g_reg_05h = g_reg_05h | (b_en << 3);
	else g_reg_05h = g_reg_05h & 0xf7;
	SN3731_IIC_Write_Reg(Reg_BlinkCtrl, g_reg_05h);
}

/*******************************************************************
* 函数原型：void SN3731_Blink_Brightness(unsigned char brightness)
* 输入参数：
* 输出参数：
* 函数功能：闪烁灰度值配置函数，0:以当前帧的pwm闪烁，1:以frame1 的pwm值闪烁
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Blink_Brightness(unsigned char brightness)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (brightness)g_reg_05h = g_reg_05h | (brightness << 5);
	else g_reg_05h = g_reg_05h & 0xbf;
	SN3731_IIC_Write_Reg(Reg_BlinkCtrl, g_reg_05h);
}

/*******************************************************************
* 函数原型：void SN3731_Blink_period(unsigned char BPT)
* 输入参数：
* 输出参数：
* 函数功能：配置闪烁时间，只能以占空比为50%闪烁
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Blink_period(unsigned char BPT)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_05h = g_reg_05h & 0xf8;
	g_reg_05h = g_reg_05h | BPT;
	SN3731_IIC_Write_Reg(Reg_BlinkCtrl, g_reg_05h);
}

/*******************************************************************
* 函数原型：void SN3731_AudioSyn(unsigned char a)
* 输入参数：
* 输出参数：
* 函数功能：audio同步
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_AudioSyn(unsigned char a)
{
	unsigned char j;
	j = j | a;
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_AudioSync, a);
}

/*******************************************************************
* 函数原型：void SN3731_Breath_RampUp(unsigned char rampup)
* 输入参数：
* 输出参数：
* 函数功能：全屏呼吸时 配置渐亮时间
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Breath_RampUp(unsigned char rampup)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_08h = g_reg_08h & 0xf0;
	g_reg_08h = g_reg_08h | rampup;
	SN3731_IIC_Write_Reg(Reg_BreathTime, g_reg_08h);
}

/*******************************************************************
* 函数原型：void SN3731_Breath_RampDown(unsigned char rampdown)
* 输入参数：
* 输出参数：
* 函数功能：全屏呼吸时 配置渐灭时间
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Breath_RampDown(unsigned char rampdown)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_08h = g_reg_08h & 0x0f;
	g_reg_08h = g_reg_08h | (rampdown << 4);
	SN3731_IIC_Write_Reg(Reg_BreathTime, g_reg_08h);
}

/*******************************************************************
* 函数原型：void SN3731_BreathEN(unsigned char a)
* 输入参数：
* 输出参数：
* 函数功能：开启全屏呼吸 a 0:disable,1:enable
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_BreathEN(unsigned char a)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (a)g_reg_09h = g_reg_09h | 0x10;
	else g_reg_09h = 0;
	SN3731_IIC_Write_Reg(Reg_Breath, g_reg_09h);
}

/*******************************************************************
* 函数原型：void SN3731_Breath_extinguish(unsigned char ET)
* 输入参数：
* 输出参数：
* 函数功能：配置呼吸的熄灭时间
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Breath_extinguish(unsigned char ET)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_09h = g_reg_09h & 0xf8;
	g_reg_09h = g_reg_09h | ET;
	SN3731_IIC_Write_Reg(Reg_Breath, g_reg_09h);
}

/*******************************************************************
* 函数原型：void SN3731_SoftWareEn(unsigned char sw_en)
* 输入参数：
* 输出参数：
* 函数功能：软件使能
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_SoftWareEn(unsigned char sw_en)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ShutDown, sw_en);
}

/*******************************************************************
* 函数原型：void SN3731_Audio_AGCEN(unsigned char agc)
* 输入参数：
* 输出参数：
* 函数功能：audio AGC Enable
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Audio_AGCEN(unsigned char agc)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (agc)g_reg_0bh = g_reg_0bh | (agc << 3);
	else g_reg_0bh = g_reg_0bh & 0xf7;
	SN3731_IIC_Write_Reg(Reg_AGC, g_reg_0bh);
}

/*******************************************************************
* 函数原型：void SN3731_Audio_AGCMode(unsigned char agc_mode)
* 输入参数：
* 输出参数：
* 函数功能：audio AGC mode.0 slow mode, 1 fast mode
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Audio_AGCMode(unsigned char agc_mode)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (agc_mode)g_reg_0bh = g_reg_0bh | (agc_mode << 4);
	else g_reg_0bh = g_reg_0bh & 0xef;
	SN3731_IIC_Write_Reg(Reg_AGC, g_reg_0bh);
}

/*******************************************************************
* 函数原型：void SN3731_Audio_Gain(unsigned char gain)
* 输入参数：
* 输出参数：
* 函数功能：audio gain,audio agc 的优先级比audio gain高
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Audio_Gain(unsigned char gain)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_0bh = g_reg_0bh & 0xf8;
	g_reg_0bh = g_reg_0bh | (gain << 0);
	SN3731_IIC_Write_Reg(Reg_AGC, g_reg_0bh);
}

/*******************************************************************
* 函数原型：void SN3731_Audio_SampleTime(unsigned char AAR)
* 输入参数：
* 输出参数：
* 函数功能：配置audio 采样时间，默认为0，采样时间最慢为19.635ms
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Audio_SampleTime(unsigned char AAR)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_AudioADCRate, AAR);
}

/*******************************************************************
* 函数原型：void Led_SelectChip(unsigned char i2c_addr)	//默认为master，需要改变I2C地址再调用
* 输入参数：
* 输出参数：
* 函数功能：选择芯片的I2C地址,如果有主从芯片则可以调用此函数
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void Led_SelectChip(unsigned char i2c_addr)	//默认为master，需要改变I2C地址再调用
{
	g_SN3731_IIC_ADDRESS = i2c_addr;
}

/*******************************************************************
* 函数原型：void SN3731_LedTurnOnOff_WriteInSequence(unsigned char SequencePosition, unsigned char OnOFF_State, unsigned char Pwm_Intensity)
* 输入参数：
* 输出参数：
* 函数功能：按照datasheet 原理图layout的话,该函数可以按序列点亮任意一颗LED(1-144)
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：SequencePosition(序列位置):LED 的序列
		OnOFF_State(开关状态)	  :LED的亮灭状态1:on,0:off
		Pwm_Intensity(PWM强度)	  :LED的PWM值
*******************************************************************/
void SN3731_LedTurnOnOff_WriteInSequence(unsigned char SequencePosition, unsigned char OnOFF_State, unsigned char Pwm_Intensity)
{
	unsigned char i, j;
	i = SequencePosition / 8;
	j = SequencePosition % 8;
	if (j == 0)
	{
		j = 8;
		i = i - 1;
	}
	if (OnOFF_State)
	{
		g_dram[i] = ((1 << (j - 1)) | g_dram[i]);
		SN3731_IIC_Write_Reg(SN_REG_SWITCH_BASE + i, g_dram[i]);
	}
	else
	{

		g_dram[i] = ((~(1 << (j - 1)))&g_dram[i]);
		SN3731_IIC_Write_Reg(SN_REG_SWITCH_BASE + i, g_dram[i]);

	}
	SN3731_IIC_Write_Reg(SN_REG_PWM_BASE + SequencePosition - 1, Pwm_Intensity);
}

/*******************************************************************
* 函数原型：void SN3731_LedTurnOnOff_WriteInMatrix(unsigned char row, unsigned char col, unsigned char OnOFF_State, unsigned char Pwm_Brightness)
* 输入参数：
* 输出参数：
* 函数功能：以矩阵的行列来定义LED的位置,指定LED写入指定的PWM值
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：row:行(1-9)
		col:列(1-16)
		OnOFF_State:LED的亮灭状态1:on,0:off
		Pwm_Intensity:LED的PWM值
*******************************************************************/
void SN3731_LedTurnOnOff_WriteInMatrix(unsigned char row, unsigned char col, unsigned char OnOFF_State, unsigned char Pwm_Brightness)
{
	long unsigned int i, j;
	//unsigned char	pos;
	if ((row < 1) || (row > 9)) return;
	if ((col < 1) || (col > 16)) return;

	if (OnOFF_State)
	{


		if (col <= 8)
		{
			g_dram[row] |= (0x0001 << (col - 1));
			i = g_dram[row];
			j = i;
			SN3731_IIC_Write_Reg(SN_REG_SWITCH_BASE + (row - 1) * 2, j);
		}
		else
		{
			g_dram[row + 1] |= (0x0001 << (col - 1));
			i = g_dram[row + 1];
			j = (i >> 8);
			SN3731_IIC_Write_Reg(SN_REG_SWITCH_BASE + (row - 1) * 2 + 1, j);
		}
	}
	else
	{


		if (col <= 8)
		{
			g_dram[row] &= (~(0x0001 << (col - 1)));
			i = g_dram[row];
			j = i;
			SN3731_IIC_Write_Reg(SN_REG_SWITCH_BASE + (row - 1) * 2, j);
		}
		else
		{
			g_dram[row + 1] &= (~(0x0001 << (col - 1)));
			i = g_dram[row + 1];
			j = (i >> 8);
			SN3731_IIC_Write_Reg(SN_REG_SWITCH_BASE + (row - 1) * 2 + 1, j);
		}

	}

	SN3731_IIC_Write_Reg(SN_REG_PWM_BASE + (row - 1) * 16 + col - 1, Pwm_Brightness);
}

/*******************************************************************
* 函数原型：void SN3731_LedBlinkTurnOnOff_WrtieInSequence(unsigned char SequencePosition, unsigned char Blink_OnOFF)
* 输入参数：
* 输出参数：
* 函数功能：按照datasheet 原理图layout的话,该函数可以按序列开启
			任意一颗LED(1-144)的 闪烁,注意该函只开启LED的闪烁开关,
			并没有写LED的PWM也没有打开LED的亮灭,所以在调用该函数时
			请先点亮LED.
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：SequencePosition:LED 的序列
		Blink_OnOFF:LED的闪烁开关1:on,0:off
*******************************************************************/
void SN3731_LedBlinkTurnOnOff_WrtieInSequence(unsigned char SequencePosition, unsigned char Blink_OnOFF)
{
	unsigned char i, j;
	i = SequencePosition / 8;
	j = SequencePosition % 8;
	if (j == 0)
	{
		j = 8;
		i = i - 1;
	}

	if (Blink_OnOFF)
	{
		g_dram[SN_REG_BLINK_BASE + i] = ((1 << (j - 1)) | g_dram[SN_REG_BLINK_BASE + i]);
		//temp=SN3731_ReadReg(SN_REG_BLINK_BASE+i);
		//temp=((1<<(j-1))|temp);
		SN3731_IIC_Write_Reg(SN_REG_BLINK_BASE + i, g_dram[SN_REG_BLINK_BASE + i]);
	}
	else
	{
		//temp=SN3731_ReadReg(SN_REG_BLINK_BASE+i);
		g_dram[SN_REG_BLINK_BASE + i] = ((~(1 << (j - 1)))&g_dram[SN_REG_BLINK_BASE + i]);
		SN3731_IIC_Write_Reg(SN_REG_BLINK_BASE + i, g_dram[SN_REG_BLINK_BASE + i]);

	}

}

/*******************************************************************
* 函数原型：void SN3731_LedBlinkTurnOnOff_WrtieInMatrix(unsigned char row, unsigned char col, unsigned char Blink_OnOFF)
* 输入参数：
* 输出参数：
* 函数功能：以矩阵的行列来定义LED的位置,指定的LED写入闪烁的状态功
			能与SN3731_LedBlinkTurnOnOff_WrtieInSequence类似
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：row:行(1-9)
		col:列(1-16)
		Blink_OnOFF:LED的亮灭状态1:on,0:off
*******************************************************************/
void SN3731_LedBlinkTurnOnOff_WrtieInMatrix(unsigned char row, unsigned char col, unsigned char Blink_OnOFF)
{
	long unsigned int i, j, temp;
	//unsigned char pos;
	if ((row < 1) || (row > 9)) return;
	if ((col < 1) || (col > 16)) return;

	if (Blink_OnOFF)
	{
		//temp=SN3731_ReadReg(SN_REG_BLINK_BASE+pos);
		//temp|= (0x0001<<(col-1));
		//i = temp;

		if (col <= 8)
		{
			g_dram[SN_REG_BLINK_BASE + row] |= (0x0001 << (col - 1));
			i = g_dram[SN_REG_BLINK_BASE + row];
			j = i;
			SN3731_IIC_Write_Reg(SN_REG_BLINK_BASE + (row - 1) * 2, j);
		}
		else
		{
			g_dram[SN_REG_BLINK_BASE + row + 1] |= (0x0001 << (col - 1));
			i = g_dram[SN_REG_BLINK_BASE + row + 1];
			j = (i >> 8);
			SN3731_IIC_Write_Reg(SN_REG_BLINK_BASE + (row - 1) * 2 + 1, j);
		}
	}
	else
	{
		//temp=SN3731_ReadReg(SN_REG_BLINK_BASE+pos);
		//temp&= (~(0x0001<<(col-1)));
		//i = temp;

		if (col <= 8)
		{
			g_dram[SN_REG_BLINK_BASE + row] &= (~(0x0001 << (col - 1)));
			i = g_dram[SN_REG_BLINK_BASE + row];
			j = i;
			SN3731_IIC_Write_Reg(SN_REG_BLINK_BASE + (row - 1) * 2, j);
		}
		else
		{
			g_dram[SN_REG_BLINK_BASE + row + 1] &= (~(0x0001 << (col - 1)));
			i = g_dram[SN_REG_BLINK_BASE + row + 1];
			j = (i >> 8);
			SN3731_IIC_Write_Reg(SN_REG_BLINK_BASE + (row - 1) * 2 + 1, j);
		}

	}
}


/*******************************************************************
* 函数原型：void SN3731_Init()
* 输入参数：
* 输出参数：
* 函数功能：SN3731初始化函数，
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Init(void)//这个是在main中被调用
{
	unsigned char i;

	g_reg_00h = 0;
	g_reg_01h = 0;
	g_reg_02h = 0;
	g_reg_05h = 0;
	g_reg_06h = 0;
	g_reg_08h = 0;
	g_reg_09h = 0;
	g_reg_0bh = 0;

	//g_fireworks_NO=0;
	g_lighting_NO = 0;
	g_unlock_NO = 0;
	g_water_NO = 0;
	g_jingtaitu_NO = 0;
	g_audio1_NO = 0;
	g_audio2_NO = 0;
	g_audio3_NO = 0;
	g_jingtaitu_NO = 0;

	g_CurrentMovie_NO = 0;
	g_rail = 0;
	g_times = 0;
	g_MovieEnd_Flag = 0;

	/*******************************************************************
	功能：寄存器框架简单描述:
		SN3731总共有九个寄存器区域,每个区域的地址都是 （0x00-0xff），
		通过一个主地址来寻址，即FDh, 当FDh写0Bh时，指向控制寄存器地址空间，
		这时（0x00-0xff）寄存地址只有0x00-0x0c有效;
		当FDh写0-7时,转向0-7帧图片的地址空间，其中0x00-0x11为亮灭寄存器，
		0x12-0x23为闪烁寄存器，0x24-0xB3位pwm数据寄存器。
	*******************************************************************/
	SN3731_IIC_Write_Reg(0xfd, 0x0b);/*FDh写0Bh时，指向控制寄存器地址空间*/
	SN3731_IIC_Write_Reg(0x00, 0x00);/*因为这12个[0x00-0x0c(实际10个,看手册就知道为啥要屏蔽了)]寄
								 存器写入的数据都是0x00，所以是控制寄存器清零
								 //功能太多，看手册，有中文版*/

	SN3731_IIC_Write_Reg(0x01, 0x00);//功能太多，看手册，有中文版
								 //SN3731_IIC_Write_Reg(0x02,0x00);//功能太多，看手册

	SN3731_IIC_Write_Reg(0x03, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x04, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x05, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x06, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x07, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x08, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x09, 0x00);//功能太多，看手册
								 //SN3731_IIC_Write_Reg(0x0a,0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x0b, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x0c, 0x00);//功能太多，看手册


	for (i = 1; i < 9; i++)//所有画面寄存器清零
	{
		SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));/*当FDh写0-7时,转向0-7帧图片的地址空间，其中0x00-0x11为亮灭寄存器，
																   0x12-0x23为闪烁寄存器，0x24-0xB3位pwm数据寄存器。
																   #define Reg_MainAddress	 0xFD //寄存器主地址
																   #define Reg_Section_Fram(i) ((i)-1) //i=1-8 */

		SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
		SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_off, 18);
		SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
	}


	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);/*指向控制寄存器，#define Reg_Section_CTRLReg 0x0B*/
	SN3731_IIC_Write_Reg(Reg_ShutDown, Normal_Operation_Mode);//解除软件关断
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(1));//指向第一帧画面

}

/*******************************************************************
* 函数原型：void SN3731_Init2(void)
* 输入参数：
* 输出参数：
* 函数功能：SN3731初始化函数，
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Init2(void)
{
	g_reg_00h = 0;
	g_reg_01h = 0;
	g_reg_02h = 0;
	g_reg_05h = 0;
	g_reg_06h = 0;
	g_reg_08h = 0;
	g_reg_09h = 0;
	g_reg_0bh = 0;

	//g_fireworks_NO=0;
	g_lighting_NO = 0;
	g_unlock_NO = 0;
	g_water_NO = 0;
	g_jingtaitu_NO = 0;
	g_audio1_NO = 0;
	g_audio2_NO = 0;
	g_audio3_NO = 0;
	g_jingtaitu_NO = 0;

	g_CurrentMovie_NO = 0;
	g_rail = 0;
	g_times = 0;
	g_MovieEnd_Flag = 0;


	/*****************************************************************************
	*  寄存器框架简单描述:
	*	SN3731总共有九个寄存器区域,每个区域的地址都是 （0x00-0xff），
	*	通过一个主地址来寻址，即FDh, 当FDh写0Bh时，指向控制寄存器地址空间，
	*	这时（0x00-0xff）寄存地址只有0x00-0x0c有效;
	*	当FDh写0-7时,转向0-7帧图片的地址空间，其中0x00-0x11为亮灭寄存器，
	*	0x12-0x23为闪烁寄存器，0x24-0xB3位pwm数据寄存器。
	*****************************************************************************/
	SN3731_IIC_Write_Reg(0xfd, 0x0b);/*FDh写0Bh时，指向控制寄存器地址空间*/
	SN3731_IIC_Write_Reg(0x00, 0x00);/*因为这12个[0x00-0x0c(实际10个,看手册就知道为啥要屏蔽了)]寄
								   存器写入的数据都是0x00，所以是控制寄存器清零
								   //功能太多，看手册*/

	SN3731_IIC_Write_Reg(0x01, 0x00);//功能太多，看手册
	//SN3731_IIC_Write_Reg(0x02,0x00);

	SN3731_IIC_Write_Reg(0x03, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x04, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x05, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x06, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x07, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x08, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x09, 0x00);//功能太多，看手册
	//SN3731_IIC_Write_Reg(0x0a,0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x0b, 0x00);//功能太多，看手册
	SN3731_IIC_Write_Reg(0x0c, 0x00);//功能太多，看手册
}

/*******************************************************************
* 函数原型：void Load_Data_PWM(unsigned char movie_name)
* 输入参数：
* 输出参数：
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void Load_Data_PWM(unsigned char movie_name)
{
	unsigned char j, i;
	//unsigned char p_flag=0;
	if (movie_name == 1)
	{
		/*for(j=0;j<144;j++)
		{
		load_pwm[j]=0x00;
		}

		for(i=0;i<4;i++)
		{
		for(j=0;j<8;j++)
		{
		load_pwm[0x10+16*i+j]=0xff;//pwm_temp1[8*i+j];
		}
		}*/
		for (j = 0; j < 144; j++)
		{
			load_pwm[143 - j] = fireworks_pwm[g_fireworks_NO][j];	//装载一帧动画的灰度值到load_pwm[j]中
		}
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				pwm_temp1[8 * i + j] = load_pwm[0x18 + 16 * i + j]; //
			}
		}//取出
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				pwm_temp2[8 * i + j] = load_pwm[0x50 + 16 * i + j];
			}
		}//取出
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				load_pwm[0x18 + 16 * i + j] = pwm_temp2[8 * i + j];
			}
		}//填入
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				load_pwm[0x50 + 16 * i + j] = pwm_temp1[8 * i + j];
			}
		}//填入
		 /*
		 g_fireworks_Temp++;
		 if(g_fireworks_Temp<145)
		 g_fireworks_NO++;
		 if(g_fireworks_Temp>=145)
		 g_fireworks_NO--;

		 if(g_fireworks_Temp==289)
		 {
		 g_fireworks_Temp=0;
		 g_fireworks_NO=0;				//烟花动画有220帧，播放结束时重新播放
		 //while(1);
		 //g_MovieEnd_Flag=1;
		 } */
		if (p_flag == 0)
		{
			g_fireworks_NO++;
		}

		if ((g_fireworks_NO == 145) && (p_flag == 0))
		{
			p_flag = 1;
			//while(1);				
			//g_MovieEnd_Flag=1;
		}
		if (p_flag == 1)
		{
			g_fireworks_NO--;
			//while(1);	
		}
		if ((g_fireworks_NO == 0) && (p_flag == 1))
		{
			p_flag = 0;
			//g_fireworks_NO=1;
			//while(1);	
		}
	}
	for (j = 0; j < 18; j++)
	{
		load_onoff[j] = 0Xff;		  //亮灭寄存器全开
	}

	/*else if (movie_name == 2)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = lighting_pwm[g_lighting_NO][j];//装载一帧动画的灰度值到load_pwm[j]中
		}
		g_lighting_NO++;
		if (g_lighting_NO == 20)
		{
			g_lighting_NO = 0;	      //闪电动画有20帧，播放结束时重新播放
			g_MovieEnd_Flag = 1;
		}

	}
	else if (movie_name == 3)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = unlock_pwm[g_unlock_NO][j];//装载一帧动画的灰度值到load_pwm[j]中
		}
		g_unlock_NO++;				  //解锁动画有36帧
		if (g_unlock_NO == 36)
		{
			g_unlock_NO = 0;
			g_MovieEnd_Flag = 1;
		}

	}
	else if (movie_name == 4)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = water_pwm[g_water_NO][j];	//装载一帧动画的灰度值到load_pwm[j]中
		}
		g_water_NO++;
		if (g_water_NO == 30)		  //水滴动画有30帧
		{
			g_water_NO = 0;
			g_MovieEnd_Flag = 1;
		}

	}
	else if (movie_name == 5)
	{
		if (g_jingtaitu_NO == 8)
		{						      //8幅静态图
			g_jingtaitu_NO = 0;
			g_MovieEnd_Flag = 1;

		}

		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = jingtaitu_pwm[g_jingtaitu_NO][j];	//装载一帧动画的灰度值到load_pwm[j]中
		}
		g_jingtaitu_NO++;


	}
	else if (movie_name == 7)
	{								  			 //8幅音频数据
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = a2_pwm[g_audio2_NO][j];//装载一帧动画的灰度值到load_pwm[j]中
		}
		g_audio2_NO++;
		if (g_audio2_NO == 8)
		{
			g_audio2_NO = 0;
			g_MovieEnd_Flag = 1;
		}

	}
	else if (movie_name == 8)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = a3_pwm[g_audio3_NO][j];//装载一帧动画的灰度值到load_pwm[j]中
		}										 //72幅音频数据
		g_audio3_NO++;							 //用STC_MCU的定时器来定时刷新SN3731的8幅数据
		if (g_audio3_NO == 72)
		{
			g_audio3_NO = 0;
			g_MovieEnd_Flag = 1;
		}

	}




	for (j = 0; j < 18; j++)
	{
		load_onoff[j] = 0Xff;					 //亮灭寄存器全开
	}

	if (movie_name == 6)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = 0xff;
		}

		for (j = 0; j < 18; j++)
		{
			load_onoff[j] = a1_onoff[g_audio1_NO][j];//亮灭寄存器全开
		}
		g_audio1_NO++;						   		 //8幅音频数据

		if (g_audio1_NO == 8)
		{
			g_audio1_NO = 0;
			g_MovieEnd_Flag = 1;
		}


	}*/

	//在此处添加其他模式动画数据装载
	//在此处添加其他模式动画数据装载
	//在此处添加其他模式动画数据装载
	/*if(movie_name==)*/
}

/*******************************************************************
* 函数原型：void Load_Data_PWM(unsigned char movie_name)
* 输入参数：
* 输出参数：
* 函数功能：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_Write8Picture_Into8FrameReg(unsigned char static_picture_name)
{
	unsigned char i;
	for (i = 1; i < 9; i++)
	{
		Load_Data_PWM(static_picture_name);
		SN3731_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
		SN3731_WriteRegs(0x00, load_onoff, 18);	//所有的亮灭寄存器打开
		SN3731_WriteRegs(0x24, load_pwm, 144);	//写入烟花动画数据的pwm值

	}
}

/*******************************************************************
* 函数原型：void SN3731_PingPang_BufferWrite( unsigned char Movie_Name,unsigned char delay_time)
* 输入参数：Movie_Name:要播放动画的名称,这里的参数与资源的动画参数,可以不一样
* 输出参数：
* 函数功能：一次写入4帧动画数据到IC中,等中断到来播放已经写的画面,同时再写4帧动画到IC的另外4帧SRAM中
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注： ************************函数的说明******************************************
	完备的函数是要适用不同帧数的数据动画的流畅播放，芯片在
	动画播放结束的时候总是停在最后一帧的下一帧，而且不同动
	画的总帧数可能为4的偶数倍或者奇数倍，而这几个因素多是影
	响本函数的一些关键性因素，下面的程序考虑了情况。
*******************************************************************/
void SN3731_PingPang_BufferWrite(unsigned char Movie_Name, unsigned char delay_time)
{
	unsigned char i;
	if (g_CurrentMovie_NO == 0)//当前动画如果为0，则认为动画是刚开始播放后者播放结束
	{
		if (g_MovieEnd_Flag)   //判断动画是否为结束的尾帧，如果是则播放完该动画的尾帧，然后再重新开始播放，形成循环
		{
			if (g_rail != 0)   //轨道，电极，机匣
			{
				//如果动画不是恰好为4的整数倍，那么就需要在动画结束时，播放最后几帧
				if (g_times & 0x01)//判断为奇偶数，此处为当前动画总帧数为4的奇数倍，偶数与上0x01 bit0一定为1
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//播放后4帧动画
					SN3731_Set_StartFrame_Picture(4);							//从第5帧开始播放
					SN3731_Set_FrameNum(g_rail);								//4帧(5-8)画面参与循环
					SN3731_SelectSection(Reg_MainAddress, (g_rail + 4));		//如果尾数不为零，则把尾数的下一帧寄存器清空
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);	   		//因为SN3731在播放结束时，图片是停在最后一帧的下一帧
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);		//而如果不清零的话，很可能在movie的末尾与下一动画衔接时会闪烁一下。
				}
				else//当前动画的总帧数为4的偶数倍
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//播放前4帧动画
					SN3731_Set_StartFrame_Picture(0);							//从第1帧开始播放
					SN3731_Set_FrameNum(g_rail);								//4帧(1-4)画面参与循环
					SN3731_SelectSection(Reg_MainAddress, g_rail);
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);	   		//如果尾数不为零，且当前动画的总帧数为4的偶数倍则为数帧清零
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
				}
			}
			else
			{
				//如果动画恰好为4的帧数倍，那么程序把最后4帧认为是尾数，在此播放最后4帧
				if (g_times & 0x01)//判断为奇偶数，此处为当前动画总帧数为4的奇数倍
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//播放前4帧动画
					SN3731_Set_StartFrame_Picture(0);							//从第1帧开始播放
					SN3731_Set_FrameNum(4);									    //4帧画面参与循环
					SN3731_SelectSection(Reg_MainAddress, 4);
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);

				}
				else//当前动画的总帧数为4的偶数倍
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//播放后4帧动画
					SN3731_Set_StartFrame_Picture(4);							//从第5帧开始播放
					SN3731_Set_FrameNum(4);									    //4帧(5-8)画面参与循环环
					SN3731_SelectSection(Reg_MainAddress, 0);					//因为从第4帧播放4帧结束，frame停在frame0，所以把frame0清0
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);			//因为SN3731在播放结束时，图片是停在最后一帧的下一帧
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);		//而如果不清零的话，很可能在movie的末尾与下一动画衔接时会闪烁一下。
				}
			}
			g_MovieRestart_Flag = 1;
			g_MovieEnd_Flag = 0;

		}
		else//不是动画结束，则就是动画播放的开始，或重新播放的开始
		{
			if (g_Poweron_ontime)
			{
				SN3731_SoftWareEn(0);	//关断模式，芯片软件SD，以防止重新播放时可能出现的闪烁
				SN3731_Init();			//完全初始化芯片
				SN3731_SoftWareEn(1);	//工作模式，SN3731软件使能
				g_Poweron_ontime = 0;
			}

			SN3731_SoftWareEn(0);		//芯片软件SD，以防止重新播放时可能出现的闪烁
			SN3731_Init2();				//完全初始化芯片
			SN3731_SoftWareEn(1);

			for (i = 0; i < 8; i++)		//刚开始播放时写动画的前8帧，
			{
				g_CurrentMovie_NO++;	//记录当前播放动画播放到第几帧
				Load_Data_PWM(Movie_Name);				 //装载烟花动画，由传进来的参数决定，参数由宏定义确定				
				SN3731_SelectSection(Reg_MainAddress, i);//指向第i页寄存器空间，i(0-7)所以是画面寄存器
				SN3731_WriteRegs(0x00, load_onoff, 18);	 //所有的亮灭寄存器打开
				SN3731_WriteRegs(0x24, load_pwm, 144);	 //写入烟花动画数据的pwm值	
				if (g_MovieEnd_Flag)
				{
					g_rail = g_CurrentMovie_NO % 4;		 //如果动画数据在此结束时，则求出动画的尾数和动画为4的倍数
					g_times = g_CurrentMovie_NO / 4;
					g_CurrentMovie_NO = 0;
					break;								 //跳出循环，在主程序等待中断，然后转向播放尾数动画
				}
			}
			SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);//开始播放前4帧
			SN3731_Movie_EN(1);										   //开启movie模式
			SN3731_Set_StartFrame_Picture(0);						   //从第1帧开始播放
			SN3731_Set_LoopNum(1);									   //循环一次
			SN3731_Set_FrameNum(4);									   //4帧画面参与循环
			SN3731_Set_FrameDelay(delay_time);						   //帧的自动切换时间为45ms左右，
		}
	}																   //1:15ms,2:30ms,3:45ms,以此类推
	else
	{
		if (front_4_flag)	//1为播放前4帧标志，0为播放后4帧标志	   //中断内部改变front_4_flag的值
		{
			SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//播放前4帧动画
			SN3731_Set_StartFrame_Picture(0);			 //从第1帧开始播放
			for (i = 4; i < 8; i++)		//这里为什么写后四帧看不懂
			{											 //写后4帧的动画
				g_CurrentMovie_NO++;					 //记录当前播放动画播放到第几帧
				Load_Data_PWM(Movie_Name);				 //装载烟花动画，参数由宏定义确定
				SN3731_SelectSection(Reg_MainAddress, i);//指向第i页寄存器空间
				SN3731_WriteRegs(0x00, load_onoff, 18);	 //所有的亮灭寄存器打开
				SN3731_WriteRegs(0x24, load_pwm, 144);	 //写入烟花动画数据的pwm值
				if (g_MovieEnd_Flag)
				{
					g_rail = g_CurrentMovie_NO % 4;		 //如果动画数据在此结束时，则求出动画的尾数和动画为4的倍数
					g_times = g_CurrentMovie_NO / 4;
					g_CurrentMovie_NO = 0;
					break;	//跳出循环，在主程序等待中断，然后转向播放尾数动画
				}
			}
		}
		else
		{
			SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);//播放后4帧的动画
			SN3731_Set_StartFrame_Picture(4);			 //从第4帧开始播放

			for (i = 0; i < 4; i++)						 //写前4帧的动画，为什么，看不懂，不是应该后四帧吗
			{
				g_CurrentMovie_NO++;					 //记录当前播放动画播放到第几帧
				Load_Data_PWM(Movie_Name);				 //装载烟花动画，					 
				SN3731_SelectSection(Reg_MainAddress, i);//指向第i页寄存器空间
				SN3731_WriteRegs(0x00, load_onoff, 18);	 //所有的亮灭寄存器打开
				SN3731_WriteRegs(0x24, load_pwm, 144);	 //写入烟花动画数据的pwm值
				if (g_MovieEnd_Flag)
				{
					g_rail = g_CurrentMovie_NO % 4;		 //如果动画数据在此结束时，则求出动画的尾数和动画为4的倍数
					g_times = g_CurrentMovie_NO / 4;
					g_CurrentMovie_NO = 0;
					break;	//跳出循环，在主程序等待中断，然后转向播放尾数动画
				}
			}
		}
	}
}

/*******************************************************************
 * 函数原型：void SN3731_DEMO(unsigned char demo_index)
 * 输入参数：
 * 输出参数：
 * 函数功能：各种模式的实现函数，客户在此添加模式
 * 创建日期：
 * 创建人：
 * 修改日期
 * 修改人：
 * 第N次修改：
 * 修改原因：
 * 备注： **************************添加模式动画的简要说明*************************
	 例如客户要在最末尾添加mode22动画，则首先要先在SN3731res.h中把
	 mode22的动画数据添加进去，然后在该文件中定义一个记录该
	 动画帧数的全局变量g_mode22_NO，同时给这个动画编号如:
	 #define g_MOVIE_mode22			0x16，然后在void Load_Data_PWM(unsigned char movie_name)
	 函数中添加装载mode22动画数据代码，具体如何添加可参照已有
	 程序，最后在本函数的末尾添加一个模式，调用:
	 SN3731_PingPang_BufferWrite(g_mode22_NO,g_MOVIE_mode22,0x04)即可
*******************************************************************/
void SN3731_DEMO(unsigned char demo_index)
{

	switch (demo_index)
	{
		case 1:
			#ifdef STC_MCU
					TR0 = 0;//关STC_MCU 定时器
					EX0 = 1;
			#endif
				SN3731_PingPang_BufferWrite(g_MOVIE_fireworks, 0x03);//play fireworks播放烟火
				break;
		/*case 2:
			SN3731_PingPang_BufferWrite(g_MOVIE_lighting, 0x06);//play lighting播放灯光
			break;
		case 3:

			SN3731_PingPang_BufferWrite(g_MOVIE_unlock, 0x04);
			break;
		case 4:

			SN3731_PingPang_BufferWrite(g_MOVIE_water, 0x04);
			break;
		case 5:
			#ifdef STC_MCU
				TR0 = 0;//关STC_MCU 定时器
				EX0 = 0;
			#endif
			SN3731_Init();		 //全面初始化
			SN3731_SoftWareEn(1);//工作模式，SN3731软件使能
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_tingtaitu);			//写8帧数据到IC的8幅数据局存器中
			SN3731_SelectSection(Register_MainAddress, Reg_Section_Fram(2));//这个模式中的第二幅画面的一些点开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(30, 1);   //第30点开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(45, 1);   //第45点开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(47, 1);   //第47点开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(62, 1);   //第62点开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInMatrix(8, 3, 1);   //第8行、第3列开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInMatrix(9, 2, 1);   //第9行、第2列开闪烁
			SN3731_LedBlinkTurnOnOff_WrtieInMatrix(9, 4, 1);   //第9行、第4列开闪烁
			SN3731_Blink_period(1);							   //设置闪烁时间
			SN3731_BlinkEN(1);								   //闪烁使能
			SN3731_BreathEN(1);			//LED呼吸使能
			SN3731_Breath_extinguish(4);//设置熄灭时间
			SN3731_Breath_RampDown(4);	//设置渐灭时间
			SN3731_Breath_RampUp(4);	//设置渐亮时间
			SN3731_Set_FrameNum(0);		//8帧循环
			SN3731_Set_LoopNum(0);		//一直循环播放
			SN3731_Movie_EN(1);			//movie 播放使能
			break;

		case 6:
			SN3731_Init();
			SN3731_SoftWareEn(1);
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_audio1);
			SN3731_AudioFrame_EN(1);//音乐frame使能
			break;
		case 7:
			SN3731_Init();
			SN3731_SoftWareEn(1);
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_audio2);
			SN3731_AudioFrame_EN(1);//音乐frame使能
			break;
		case 8:
			SN3731_SoftWareEn(1);
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_audio3);
			SN3731_AudioFrame_EN(1);//音乐frame使能
			#ifdef STC_MCU
					TR0 = 1;//开STC_MCU 定时器
			#endif
			break;*/

			/****************************************************
			在此处添加模式代码(用户写的，比如我的大火，小火...放在这里)
			****************************************************/
		case 9:
			break;
}

	//if(mode!=1)break;		
}








