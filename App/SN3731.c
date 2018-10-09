/*******************/
/*ͷ�ļ�header file*/
/*******************/
#include "app_main.h"
#include "SN3731_array.h"

/************************/
/*�궨��macro definition*/
/************************/
#ifdef STC_MCU
xdata unsigned char load_pwm[144];	//����װ��һ֡������pwmֵ
xdata unsigned char load_onoff[18];	//����װ������ֵ
xdata unsigned char pwm_temp1[32];
xdata unsigned char pwm_temp2[32];
#else
static unsigned char load_pwm[144];	//����װ��һ֡������pwmֵ
static unsigned char load_onoff[18];
static unsigned char pwm_temp1[32];
static unsigned char pwm_temp2[32];
#endif

//�Դ�����
#define g_MOVIE_fireworks  0x01 //�̻���������
#define g_MOVIE_lighting   0x02 //mode1��������
#define g_MOVIE_unlock	   0x03 //mode2��������
#define g_MOVIE_water	   0x04 //mode3��������
#define g_MOVIE_tingtaitu  0x05 //mode4��������
#define g_MOVIE_audio1	   0x06 //mode5��������
#define g_MOVIE_audio2	   0x07
#define g_MOVIE_audio3	   0x08

//�ڴ˶�����������
#define SN_REG_SWITCH_BASE 0x00
#define SN_REG_BLINK_BASE  0x12
#define SN_REG_PWM_BASE    0x24

#ifdef STC_MCU
extern unsigned char demo_NO;
#endif



/*************************/
/*���Ͷ���byte definition*/
/*************************/

/****************************/
/*��־λ����flags definetion*/
/****************************/
unsigned char ack;	     //Ӧ���־λ
unsigned char p_flag = 0;
unsigned char front_4_flag;     //1Ϊ����ǰ4���־��0Ϊ���ź�4���־

/*****************************/
/*��������variable definition*/
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



//static unsigned char g_SN3731_IIC_ADDRESS = g_SN3731_IIC_ADDRESS_M; //��Ϊ�˺궨��

unsigned char g_Poweron_ontime = 1;

unsigned char g_dram[36];	/*���ڼ�¼0x00h-0x23h�ļĴ�����ֵ����Ϊ��36���Ĵ�����ÿ��bit���ƶ�Ӧһ���Ƶ��������˸
								��Ϊ��36���Ĵ�����ÿ��bit���ƶ�Ӧһ���Ƶ��������˸���������һ�ŵƵ�״̬����ʱ����
								��Ӱ��ͬһ�ֽڵ�����bit���Ծ��б�Ҫ��¼����Щ�ֽڵ�״̬��Ϣ */

								//��������
uint8_t  g_fireworks_NO;	//���ڼ�¼�̻��������ŵ��ڼ�֡
uint8_t  g_fireworks_Temp;
uint8_t  g_lighting_NO;	//���ڼ�¼mode1�������ŵ��ڼ�֡
uint8_t  g_unlock_NO;		//���ڼ�¼mode2�������ŵ��ڼ�֡
uint8_t  g_water_NO;		//���ڼ�¼mode3�������ŵ��ڼ�֡
uint8_t  g_jingtaitu_NO;	//���ڼ�¼mode4�������ŵ��ڼ�֡
uint8_t  g_audio1_NO;		//���ڼ�¼mode4 picture ���ŵ��ڼ�֡
uint8_t  g_audio2_NO;
uint8_t  g_audio3_NO;
uint8_t  g_CurrentMovie_NO;
uint8_t  g_rail;
uint8_t  g_times;
uint8_t  g_MovieEnd_Flag;
uint8_t  g_MovieRestart_Flag;
uint8_t  demo_NO; //SN3237����ʹ��
uint8_t  timer_flag;

/**************************/
/*���鶨��array definition*/
/**************************/

/******************************/
/*��������Function declaration*/
/******************************/

/****************************/
/*��������function definetion*/
/****************************/
/*******************************************************************
* ����ԭ�ͣ�void SN3731_Led_TurnOn_All(unsigned char i)
* ���������
* ���������
* �������ܣ�����LED����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Led_TurnOn_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));//Reg_MainAddress,0xFD(�˵�ַΪҳ����ƼĴ���)
	SN3731_IIC_WriteRegs(0x01, (uint8_t *)all_led_on, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_on, 144);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Led_TurnOff_All(unsigned char i)
* ���������
* ���������
* �������ܣ�����LEDд��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Led_TurnOff_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_blink_TurnOn_All(unsigned char i)
* ���������
* ��������� ����LED������˸
* �������ܣ�
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_blink_TurnOn_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_on, 18);
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_on, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_on, 144);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_blink_TurnOff_All(unsigned char i)
* ���������
* ���������
* �������ܣ�����LED�ص���˸
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_blink_TurnOff_All(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_off, 18);
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_DeleteFrame_pwm(unsigned char i)
* ���������
* ���������
* �������ܣ������ѡ��Ļ���PWMֵ
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_DeleteFrame_pwm(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_DeleteFrame_OnOff(unsigned char i)
* ���������
* ���������
* �������ܣ������ѡ��Ļ���onoffֵ
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_DeleteFrame_OnOff(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_DeleteFrame_blink(unsigned char i)
* ���������
* ���������
* �������ܣ������ѡ��Ļ���blinkֵ
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_DeleteFrame_blink(unsigned char i)
{
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
	SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_off, 18);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_AudioFrame_EN(unsigned char i)// i: 1 Enable,0 disable.
* ���������
* �������������AudioFrame
* �������ܣ�
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_AudioFrame_EN(unsigned char i)// i: 1 Enable,0 disable.
{
	g_reg_00h = g_reg_00h & 0xef;
	g_reg_00h = g_reg_00h | (i << 4);
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ModeConfig, g_reg_00h);

}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Movie_EN(unsigned char i)	 // i: 1 Enable,0 disable.
* ���������
* ���������
* �������ܣ�����moive ģʽ
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Movie_EN(unsigned char i)	 // i: 1 Enable,0 disable.
{
	g_reg_00h = (g_reg_00h & 0xf7);
	g_reg_00h = (g_reg_00h | (i << 3));
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ModeConfig, g_reg_00h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Set_StartFrame_Picture(unsigned char Start_Frame_Picture)//Start_Frame_Picture:0x00~0x07 ��8������
* ���������
* ���������
* �������ܣ�ѡ��movie mode����֡frame��ʼ����	Start_Frame_Picture:0x00~0x07
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Set_StartFrame_Picture(unsigned char Start_Frame_Picture)//Start_Frame_Picture:0x00~0x07 ��8������
{
	g_reg_00h = (g_reg_00h & 0xf8);
	g_reg_00h = (g_reg_00h | Start_Frame_Picture);
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ModeConfig, g_reg_00h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_DisplayFrame(unsigned char i)
* ���������
* ���������
* �������ܣ�ѡ��ǰ��ʾ����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_DisplayFrame(unsigned char i)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);//set display mode
	SN3731_IIC_Write_Reg(Reg_PictureAddress, i);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Set_LoopNum(unsigned char loopnum)
* ���������
* ���������
* �������ܣ�����movie ģʽ��movie���Ŵ�����
			loopnum 0:endless,
			loopnum 1-7:����1-7�Σ����Ž���INTB�����ж�
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Set_LoopNum(unsigned char loopnum)
{
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	g_reg_02h = g_reg_02h & 0x0f;
	g_reg_02h = g_reg_02h | (loopnum << 4);
	SN3731_IIC_Write_Reg(Reg_FramePlay, g_reg_02h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Set_FrameNum(unsigned char framenum)
* ���������
* ���������
* �������ܣ�����movie ģʽ��movie���ŵ�֡����framenum 0:0,1-7:���ŵ�֡��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Set_FrameNum(unsigned char framenum)
{
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	g_reg_02h = g_reg_02h & 0xf0;
	g_reg_02h = g_reg_02h | framenum;
	SN3731_IIC_Write_Reg(Reg_FramePlay, g_reg_02h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Set_FrameDelay(unsigned char A)
* ���������
* ���������
* �������ܣ�����movie ģʽ��movie���ŵ���ʱʱ�䣬A:0`63
			Delay between frame change in a movie
			A=0-63,��A=0��ʱ��������������ʱ��ΪA*30ms,���Ϊ30ms*63=1.89s
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Set_FrameDelay(unsigned char A)
{
	//SN3731_IIC_Write_Reg(Reg_MainAddress,Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_FrameTime, A);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_BlinkEN(unsigned char b_en)
* ���������
* ���������
* �������ܣ�blink enable function
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_BlinkEN(unsigned char b_en)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (b_en)g_reg_05h = g_reg_05h | (b_en << 3);
	else g_reg_05h = g_reg_05h & 0xf7;
	SN3731_IIC_Write_Reg(Reg_BlinkCtrl, g_reg_05h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Blink_Brightness(unsigned char brightness)
* ���������
* ���������
* �������ܣ���˸�Ҷ�ֵ���ú�����0:�Ե�ǰ֡��pwm��˸��1:��frame1 ��pwmֵ��˸
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Blink_Brightness(unsigned char brightness)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (brightness)g_reg_05h = g_reg_05h | (brightness << 5);
	else g_reg_05h = g_reg_05h & 0xbf;
	SN3731_IIC_Write_Reg(Reg_BlinkCtrl, g_reg_05h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Blink_period(unsigned char BPT)
* ���������
* ���������
* �������ܣ�������˸ʱ�䣬ֻ����ռ�ձ�Ϊ50%��˸
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Blink_period(unsigned char BPT)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_05h = g_reg_05h & 0xf8;
	g_reg_05h = g_reg_05h | BPT;
	SN3731_IIC_Write_Reg(Reg_BlinkCtrl, g_reg_05h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_AudioSyn(unsigned char a)
* ���������
* ���������
* �������ܣ�audioͬ��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_AudioSyn(unsigned char a)
{
	unsigned char j;
	j = j | a;
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_AudioSync, a);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Breath_RampUp(unsigned char rampup)
* ���������
* ���������
* �������ܣ�ȫ������ʱ ���ý���ʱ��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Breath_RampUp(unsigned char rampup)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_08h = g_reg_08h & 0xf0;
	g_reg_08h = g_reg_08h | rampup;
	SN3731_IIC_Write_Reg(Reg_BreathTime, g_reg_08h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Breath_RampDown(unsigned char rampdown)
* ���������
* ���������
* �������ܣ�ȫ������ʱ ���ý���ʱ��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Breath_RampDown(unsigned char rampdown)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_08h = g_reg_08h & 0x0f;
	g_reg_08h = g_reg_08h | (rampdown << 4);
	SN3731_IIC_Write_Reg(Reg_BreathTime, g_reg_08h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_BreathEN(unsigned char a)
* ���������
* ���������
* �������ܣ�����ȫ������ a 0:disable,1:enable
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_BreathEN(unsigned char a)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (a)g_reg_09h = g_reg_09h | 0x10;
	else g_reg_09h = 0;
	SN3731_IIC_Write_Reg(Reg_Breath, g_reg_09h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Breath_extinguish(unsigned char ET)
* ���������
* ���������
* �������ܣ����ú�����Ϩ��ʱ��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Breath_extinguish(unsigned char ET)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_09h = g_reg_09h & 0xf8;
	g_reg_09h = g_reg_09h | ET;
	SN3731_IIC_Write_Reg(Reg_Breath, g_reg_09h);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_SoftWareEn(unsigned char sw_en)
* ���������
* ���������
* �������ܣ����ʹ��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_SoftWareEn(unsigned char sw_en)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_ShutDown, sw_en);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Audio_AGCEN(unsigned char agc)
* ���������
* ���������
* �������ܣ�audio AGC Enable
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Audio_AGCEN(unsigned char agc)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (agc)g_reg_0bh = g_reg_0bh | (agc << 3);
	else g_reg_0bh = g_reg_0bh & 0xf7;
	SN3731_IIC_Write_Reg(Reg_AGC, g_reg_0bh);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Audio_AGCMode(unsigned char agc_mode)
* ���������
* ���������
* �������ܣ�audio AGC mode.0 slow mode, 1 fast mode
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Audio_AGCMode(unsigned char agc_mode)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	if (agc_mode)g_reg_0bh = g_reg_0bh | (agc_mode << 4);
	else g_reg_0bh = g_reg_0bh & 0xef;
	SN3731_IIC_Write_Reg(Reg_AGC, g_reg_0bh);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Audio_Gain(unsigned char gain)
* ���������
* ���������
* �������ܣ�audio gain,audio agc �����ȼ���audio gain��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Audio_Gain(unsigned char gain)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	g_reg_0bh = g_reg_0bh & 0xf8;
	g_reg_0bh = g_reg_0bh | (gain << 0);
	SN3731_IIC_Write_Reg(Reg_AGC, g_reg_0bh);
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Audio_SampleTime(unsigned char AAR)
* ���������
* ���������
* �������ܣ�����audio ����ʱ�䣬Ĭ��Ϊ0������ʱ������Ϊ19.635ms
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Audio_SampleTime(unsigned char AAR)
{
	SN3731_IIC_Write_Reg(Reg_MainAddress, Reg_Section_CTRLReg);
	SN3731_IIC_Write_Reg(Reg_AudioADCRate, AAR);
}

/*******************************************************************
* ����ԭ�ͣ�void Led_SelectChip(unsigned char i2c_addr)	//Ĭ��Ϊmaster����Ҫ�ı�I2C��ַ�ٵ���
* ���������
* ���������
* �������ܣ�ѡ��оƬ��I2C��ַ,���������оƬ����Ե��ô˺���
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void Led_SelectChip(unsigned char i2c_addr)	//Ĭ��Ϊmaster����Ҫ�ı�I2C��ַ�ٵ���
{
	g_SN3731_IIC_ADDRESS = i2c_addr;
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_LedTurnOnOff_WriteInSequence(unsigned char SequencePosition, unsigned char OnOFF_State, unsigned char Pwm_Intensity)
* ���������
* ���������
* �������ܣ�����datasheet ԭ��ͼlayout�Ļ�,�ú������԰����е�������һ��LED(1-144)
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��SequencePosition(����λ��):LED ������
		OnOFF_State(����״̬)	  :LED������״̬1:on,0:off
		Pwm_Intensity(PWMǿ��)	  :LED��PWMֵ
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
* ����ԭ�ͣ�void SN3731_LedTurnOnOff_WriteInMatrix(unsigned char row, unsigned char col, unsigned char OnOFF_State, unsigned char Pwm_Brightness)
* ���������
* ���������
* �������ܣ��Ծ��������������LED��λ��,ָ��LEDд��ָ����PWMֵ
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��row:��(1-9)
		col:��(1-16)
		OnOFF_State:LED������״̬1:on,0:off
		Pwm_Intensity:LED��PWMֵ
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
* ����ԭ�ͣ�void SN3731_LedBlinkTurnOnOff_WrtieInSequence(unsigned char SequencePosition, unsigned char Blink_OnOFF)
* ���������
* ���������
* �������ܣ�����datasheet ԭ��ͼlayout�Ļ�,�ú������԰����п���
			����һ��LED(1-144)�� ��˸,ע��ú�ֻ����LED����˸����,
			��û��дLED��PWMҲû�д�LED������,�����ڵ��øú���ʱ
			���ȵ���LED.
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��SequencePosition:LED ������
		Blink_OnOFF:LED����˸����1:on,0:off
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
* ����ԭ�ͣ�void SN3731_LedBlinkTurnOnOff_WrtieInMatrix(unsigned char row, unsigned char col, unsigned char Blink_OnOFF)
* ���������
* ���������
* �������ܣ��Ծ��������������LED��λ��,ָ����LEDд����˸��״̬��
			����SN3731_LedBlinkTurnOnOff_WrtieInSequence����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��row:��(1-9)
		col:��(1-16)
		Blink_OnOFF:LED������״̬1:on,0:off
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
* ����ԭ�ͣ�void SN3731_Init()
* ���������
* ���������
* �������ܣ�SN3731��ʼ��������
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Init(void)//�������main�б�����
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
	���ܣ��Ĵ�����ܼ�����:
		SN3731�ܹ��оŸ��Ĵ�������,ÿ������ĵ�ַ���� ��0x00-0xff����
		ͨ��һ������ַ��Ѱַ����FDh, ��FDhд0Bhʱ��ָ����ƼĴ�����ַ�ռ䣬
		��ʱ��0x00-0xff���Ĵ��ַֻ��0x00-0x0c��Ч;
		��FDhд0-7ʱ,ת��0-7֡ͼƬ�ĵ�ַ�ռ䣬����0x00-0x11Ϊ����Ĵ�����
		0x12-0x23Ϊ��˸�Ĵ�����0x24-0xB3λpwm���ݼĴ�����
	*******************************************************************/
	SN3731_IIC_Write_Reg(0xfd, 0x0b);/*FDhд0Bhʱ��ָ����ƼĴ�����ַ�ռ�*/
	SN3731_IIC_Write_Reg(0x00, 0x00);/*��Ϊ��12��[0x00-0x0c(ʵ��10��,���ֲ��֪��ΪɶҪ������)]��
								 ����д������ݶ���0x00�������ǿ��ƼĴ�������
								 //����̫�࣬���ֲᣬ�����İ�*/

	SN3731_IIC_Write_Reg(0x01, 0x00);//����̫�࣬���ֲᣬ�����İ�
								 //SN3731_IIC_Write_Reg(0x02,0x00);//����̫�࣬���ֲ�

	SN3731_IIC_Write_Reg(0x03, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x04, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x05, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x06, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x07, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x08, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x09, 0x00);//����̫�࣬���ֲ�
								 //SN3731_IIC_Write_Reg(0x0a,0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x0b, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x0c, 0x00);//����̫�࣬���ֲ�


	for (i = 1; i < 9; i++)//���л���Ĵ�������
	{
		SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));/*��FDhд0-7ʱ,ת��0-7֡ͼƬ�ĵ�ַ�ռ䣬����0x00-0x11Ϊ����Ĵ�����
																   0x12-0x23Ϊ��˸�Ĵ�����0x24-0xB3λpwm���ݼĴ�����
																   #define Reg_MainAddress	 0xFD //�Ĵ�������ַ
																   #define Reg_Section_Fram(i) ((i)-1) //i=1-8 */

		SN3731_IIC_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
		SN3731_IIC_WriteRegs(0x12, (uint8_t *)all_blink_off, 18);
		SN3731_IIC_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
	}


	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);/*ָ����ƼĴ�����#define Reg_Section_CTRLReg 0x0B*/
	SN3731_IIC_Write_Reg(Reg_ShutDown, Normal_Operation_Mode);//�������ض�
	SN3731_IIC_SelectSection(Reg_MainAddress, Reg_Section_Fram(1));//ָ���һ֡����

}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_Init2(void)
* ���������
* ���������
* �������ܣ�SN3731��ʼ��������
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
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
	*  �Ĵ�����ܼ�����:
	*	SN3731�ܹ��оŸ��Ĵ�������,ÿ������ĵ�ַ���� ��0x00-0xff����
	*	ͨ��һ������ַ��Ѱַ����FDh, ��FDhд0Bhʱ��ָ����ƼĴ�����ַ�ռ䣬
	*	��ʱ��0x00-0xff���Ĵ��ַֻ��0x00-0x0c��Ч;
	*	��FDhд0-7ʱ,ת��0-7֡ͼƬ�ĵ�ַ�ռ䣬����0x00-0x11Ϊ����Ĵ�����
	*	0x12-0x23Ϊ��˸�Ĵ�����0x24-0xB3λpwm���ݼĴ�����
	*****************************************************************************/
	SN3731_IIC_Write_Reg(0xfd, 0x0b);/*FDhд0Bhʱ��ָ����ƼĴ�����ַ�ռ�*/
	SN3731_IIC_Write_Reg(0x00, 0x00);/*��Ϊ��12��[0x00-0x0c(ʵ��10��,���ֲ��֪��ΪɶҪ������)]��
								   ����д������ݶ���0x00�������ǿ��ƼĴ�������
								   //����̫�࣬���ֲ�*/

	SN3731_IIC_Write_Reg(0x01, 0x00);//����̫�࣬���ֲ�
	//SN3731_IIC_Write_Reg(0x02,0x00);

	SN3731_IIC_Write_Reg(0x03, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x04, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x05, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x06, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x07, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x08, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x09, 0x00);//����̫�࣬���ֲ�
	//SN3731_IIC_Write_Reg(0x0a,0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x0b, 0x00);//����̫�࣬���ֲ�
	SN3731_IIC_Write_Reg(0x0c, 0x00);//����̫�࣬���ֲ�
}

/*******************************************************************
* ����ԭ�ͣ�void Load_Data_PWM(unsigned char movie_name)
* ���������
* ���������
* �������ܣ�
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
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
			load_pwm[143 - j] = fireworks_pwm[g_fireworks_NO][j];	//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
		}
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				pwm_temp1[8 * i + j] = load_pwm[0x18 + 16 * i + j]; //
			}
		}//ȡ��
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				pwm_temp2[8 * i + j] = load_pwm[0x50 + 16 * i + j];
			}
		}//ȡ��
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				load_pwm[0x18 + 16 * i + j] = pwm_temp2[8 * i + j];
			}
		}//����
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 8; j++)
			{
				load_pwm[0x50 + 16 * i + j] = pwm_temp1[8 * i + j];
			}
		}//����
		 /*
		 g_fireworks_Temp++;
		 if(g_fireworks_Temp<145)
		 g_fireworks_NO++;
		 if(g_fireworks_Temp>=145)
		 g_fireworks_NO--;

		 if(g_fireworks_Temp==289)
		 {
		 g_fireworks_Temp=0;
		 g_fireworks_NO=0;				//�̻�������220֡�����Ž���ʱ���²���
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
		load_onoff[j] = 0Xff;		  //����Ĵ���ȫ��
	}

	/*else if (movie_name == 2)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = lighting_pwm[g_lighting_NO][j];//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
		}
		g_lighting_NO++;
		if (g_lighting_NO == 20)
		{
			g_lighting_NO = 0;	      //���綯����20֡�����Ž���ʱ���²���
			g_MovieEnd_Flag = 1;
		}

	}
	else if (movie_name == 3)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = unlock_pwm[g_unlock_NO][j];//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
		}
		g_unlock_NO++;				  //����������36֡
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
			load_pwm[j] = water_pwm[g_water_NO][j];	//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
		}
		g_water_NO++;
		if (g_water_NO == 30)		  //ˮ�ζ�����30֡
		{
			g_water_NO = 0;
			g_MovieEnd_Flag = 1;
		}

	}
	else if (movie_name == 5)
	{
		if (g_jingtaitu_NO == 8)
		{						      //8����̬ͼ
			g_jingtaitu_NO = 0;
			g_MovieEnd_Flag = 1;

		}

		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = jingtaitu_pwm[g_jingtaitu_NO][j];	//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
		}
		g_jingtaitu_NO++;


	}
	else if (movie_name == 7)
	{								  			 //8����Ƶ����
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = a2_pwm[g_audio2_NO][j];//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
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
			load_pwm[j] = a3_pwm[g_audio3_NO][j];//װ��һ֡�����ĻҶ�ֵ��load_pwm[j]��
		}										 //72����Ƶ����
		g_audio3_NO++;							 //��STC_MCU�Ķ�ʱ������ʱˢ��SN3731��8������
		if (g_audio3_NO == 72)
		{
			g_audio3_NO = 0;
			g_MovieEnd_Flag = 1;
		}

	}




	for (j = 0; j < 18; j++)
	{
		load_onoff[j] = 0Xff;					 //����Ĵ���ȫ��
	}

	if (movie_name == 6)
	{
		for (j = 0; j < 144; j++)
		{
			load_pwm[j] = 0xff;
		}

		for (j = 0; j < 18; j++)
		{
			load_onoff[j] = a1_onoff[g_audio1_NO][j];//����Ĵ���ȫ��
		}
		g_audio1_NO++;						   		 //8����Ƶ����

		if (g_audio1_NO == 8)
		{
			g_audio1_NO = 0;
			g_MovieEnd_Flag = 1;
		}


	}*/

	//�ڴ˴��������ģʽ��������װ��
	//�ڴ˴��������ģʽ��������װ��
	//�ڴ˴��������ģʽ��������װ��
	/*if(movie_name==)*/
}

/*******************************************************************
* ����ԭ�ͣ�void Load_Data_PWM(unsigned char movie_name)
* ���������
* ���������
* �������ܣ�
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************/
void SN3731_Write8Picture_Into8FrameReg(unsigned char static_picture_name)
{
	unsigned char i;
	for (i = 1; i < 9; i++)
	{
		Load_Data_PWM(static_picture_name);
		SN3731_SelectSection(Reg_MainAddress, Reg_Section_Fram(i));
		SN3731_WriteRegs(0x00, load_onoff, 18);	//���е�����Ĵ�����
		SN3731_WriteRegs(0x24, load_pwm, 144);	//д���̻��������ݵ�pwmֵ

	}
}

/*******************************************************************
* ����ԭ�ͣ�void SN3731_PingPang_BufferWrite( unsigned char Movie_Name,unsigned char delay_time)
* ���������Movie_Name:Ҫ���Ŷ���������,����Ĳ�������Դ�Ķ�������,���Բ�һ��
* ���������
* �������ܣ�һ��д��4֡�������ݵ�IC��,���жϵ��������Ѿ�д�Ļ���,ͬʱ��д4֡������IC������4֡SRAM��
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע�� ************************������˵��******************************************
	�걸�ĺ�����Ҫ���ò�ͬ֡�������ݶ������������ţ�оƬ��
	�������Ž�����ʱ������ͣ�����һ֡����һ֡�����Ҳ�ͬ��
	������֡������Ϊ4��ż�������������������⼸�����ض���Ӱ
	�챾������һЩ�ؼ������أ�����ĳ������������
*******************************************************************/
void SN3731_PingPang_BufferWrite(unsigned char Movie_Name, unsigned char delay_time)
{
	unsigned char i;
	if (g_CurrentMovie_NO == 0)//��ǰ�������Ϊ0������Ϊ�����Ǹտ�ʼ���ź��߲��Ž���
	{
		if (g_MovieEnd_Flag)   //�ж϶����Ƿ�Ϊ������β֡��������򲥷���ö�����β֡��Ȼ�������¿�ʼ���ţ��γ�ѭ��
		{
			if (g_rail != 0)   //������缫����ϻ
			{
				//�����������ǡ��Ϊ4������������ô����Ҫ�ڶ�������ʱ���������֡
				if (g_times & 0x01)//�ж�Ϊ��ż�����˴�Ϊ��ǰ������֡��Ϊ4����������ż������0x01 bit0һ��Ϊ1
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//���ź�4֡����
					SN3731_Set_StartFrame_Picture(4);							//�ӵ�5֡��ʼ����
					SN3731_Set_FrameNum(g_rail);								//4֡(5-8)�������ѭ��
					SN3731_SelectSection(Reg_MainAddress, (g_rail + 4));		//���β����Ϊ�㣬���β������һ֡�Ĵ������
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);	   		//��ΪSN3731�ڲ��Ž���ʱ��ͼƬ��ͣ�����һ֡����һ֡
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);		//�����������Ļ����ܿ�����movie��ĩβ����һ�����ν�ʱ����˸һ�¡�
				}
				else//��ǰ��������֡��Ϊ4��ż����
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//����ǰ4֡����
					SN3731_Set_StartFrame_Picture(0);							//�ӵ�1֡��ʼ����
					SN3731_Set_FrameNum(g_rail);								//4֡(1-4)�������ѭ��
					SN3731_SelectSection(Reg_MainAddress, g_rail);
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);	   		//���β����Ϊ�㣬�ҵ�ǰ��������֡��Ϊ4��ż������Ϊ��֡����
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);
				}
			}
			else
			{
				//�������ǡ��Ϊ4��֡��������ô��������4֡��Ϊ��β�����ڴ˲������4֡
				if (g_times & 0x01)//�ж�Ϊ��ż�����˴�Ϊ��ǰ������֡��Ϊ4��������
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//����ǰ4֡����
					SN3731_Set_StartFrame_Picture(0);							//�ӵ�1֡��ʼ����
					SN3731_Set_FrameNum(4);									    //4֡�������ѭ��
					SN3731_SelectSection(Reg_MainAddress, 4);
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);

				}
				else//��ǰ��������֡��Ϊ4��ż����
				{
					SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//���ź�4֡����
					SN3731_Set_StartFrame_Picture(4);							//�ӵ�5֡��ʼ����
					SN3731_Set_FrameNum(4);									    //4֡(5-8)�������ѭ����
					SN3731_SelectSection(Reg_MainAddress, 0);					//��Ϊ�ӵ�4֡����4֡������frameͣ��frame0�����԰�frame0��0
					SN3731_WriteRegs(0x00, (uint8_t *)all_led_off, 18);			//��ΪSN3731�ڲ��Ž���ʱ��ͼƬ��ͣ�����һ֡����һ֡
					SN3731_WriteRegs(0x24, (uint8_t *)all_pwm_off, 144);		//�����������Ļ����ܿ�����movie��ĩβ����һ�����ν�ʱ����˸һ�¡�
				}
			}
			g_MovieRestart_Flag = 1;
			g_MovieEnd_Flag = 0;

		}
		else//���Ƕ�������������Ƕ������ŵĿ�ʼ�������²��ŵĿ�ʼ
		{
			if (g_Poweron_ontime)
			{
				SN3731_SoftWareEn(0);	//�ض�ģʽ��оƬ���SD���Է�ֹ���²���ʱ���ܳ��ֵ���˸
				SN3731_Init();			//��ȫ��ʼ��оƬ
				SN3731_SoftWareEn(1);	//����ģʽ��SN3731���ʹ��
				g_Poweron_ontime = 0;
			}

			SN3731_SoftWareEn(0);		//оƬ���SD���Է�ֹ���²���ʱ���ܳ��ֵ���˸
			SN3731_Init2();				//��ȫ��ʼ��оƬ
			SN3731_SoftWareEn(1);

			for (i = 0; i < 8; i++)		//�տ�ʼ����ʱд������ǰ8֡��
			{
				g_CurrentMovie_NO++;	//��¼��ǰ���Ŷ������ŵ��ڼ�֡
				Load_Data_PWM(Movie_Name);				 //װ���̻��������ɴ������Ĳ��������������ɺ궨��ȷ��				
				SN3731_SelectSection(Reg_MainAddress, i);//ָ���iҳ�Ĵ����ռ䣬i(0-7)�����ǻ���Ĵ���
				SN3731_WriteRegs(0x00, load_onoff, 18);	 //���е�����Ĵ�����
				SN3731_WriteRegs(0x24, load_pwm, 144);	 //д���̻��������ݵ�pwmֵ	
				if (g_MovieEnd_Flag)
				{
					g_rail = g_CurrentMovie_NO % 4;		 //������������ڴ˽���ʱ�������������β���Ͷ���Ϊ4�ı���
					g_times = g_CurrentMovie_NO / 4;
					g_CurrentMovie_NO = 0;
					break;								 //����ѭ������������ȴ��жϣ�Ȼ��ת�򲥷�β������
				}
			}
			SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);//��ʼ����ǰ4֡
			SN3731_Movie_EN(1);										   //����movieģʽ
			SN3731_Set_StartFrame_Picture(0);						   //�ӵ�1֡��ʼ����
			SN3731_Set_LoopNum(1);									   //ѭ��һ��
			SN3731_Set_FrameNum(4);									   //4֡�������ѭ��
			SN3731_Set_FrameDelay(delay_time);						   //֡���Զ��л�ʱ��Ϊ45ms���ң�
		}
	}																   //1:15ms,2:30ms,3:45ms,�Դ�����
	else
	{
		if (front_4_flag)	//1Ϊ����ǰ4֡��־��0Ϊ���ź�4֡��־	   //�ж��ڲ��ı�front_4_flag��ֵ
		{
			SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);	//����ǰ4֡����
			SN3731_Set_StartFrame_Picture(0);			 //�ӵ�1֡��ʼ����
			for (i = 4; i < 8; i++)		//����Ϊʲôд����֡������
			{											 //д��4֡�Ķ���
				g_CurrentMovie_NO++;					 //��¼��ǰ���Ŷ������ŵ��ڼ�֡
				Load_Data_PWM(Movie_Name);				 //װ���̻������������ɺ궨��ȷ��
				SN3731_SelectSection(Reg_MainAddress, i);//ָ���iҳ�Ĵ����ռ�
				SN3731_WriteRegs(0x00, load_onoff, 18);	 //���е�����Ĵ�����
				SN3731_WriteRegs(0x24, load_pwm, 144);	 //д���̻��������ݵ�pwmֵ
				if (g_MovieEnd_Flag)
				{
					g_rail = g_CurrentMovie_NO % 4;		 //������������ڴ˽���ʱ�������������β���Ͷ���Ϊ4�ı���
					g_times = g_CurrentMovie_NO / 4;
					g_CurrentMovie_NO = 0;
					break;	//����ѭ������������ȴ��жϣ�Ȼ��ת�򲥷�β������
				}
			}
		}
		else
		{
			SN3731_SelectSection(Reg_MainAddress, Reg_Section_CTRLReg);//���ź�4֡�Ķ���
			SN3731_Set_StartFrame_Picture(4);			 //�ӵ�4֡��ʼ����

			for (i = 0; i < 4; i++)						 //дǰ4֡�Ķ�����Ϊʲô��������������Ӧ�ú���֡��
			{
				g_CurrentMovie_NO++;					 //��¼��ǰ���Ŷ������ŵ��ڼ�֡
				Load_Data_PWM(Movie_Name);				 //װ���̻�������					 
				SN3731_SelectSection(Reg_MainAddress, i);//ָ���iҳ�Ĵ����ռ�
				SN3731_WriteRegs(0x00, load_onoff, 18);	 //���е�����Ĵ�����
				SN3731_WriteRegs(0x24, load_pwm, 144);	 //д���̻��������ݵ�pwmֵ
				if (g_MovieEnd_Flag)
				{
					g_rail = g_CurrentMovie_NO % 4;		 //������������ڴ˽���ʱ�������������β���Ͷ���Ϊ4�ı���
					g_times = g_CurrentMovie_NO / 4;
					g_CurrentMovie_NO = 0;
					break;	//����ѭ������������ȴ��жϣ�Ȼ��ת�򲥷�β������
				}
			}
		}
	}
}

/*******************************************************************
 * ����ԭ�ͣ�void SN3731_DEMO(unsigned char demo_index)
 * ���������
 * ���������
 * �������ܣ�����ģʽ��ʵ�ֺ������ͻ��ڴ����ģʽ
 * �������ڣ�
 * �����ˣ�
 * �޸�����
 * �޸��ˣ�
 * ��N���޸ģ�
 * �޸�ԭ��
 * ��ע�� **************************���ģʽ�����ļ�Ҫ˵��*************************
	 ����ͻ�Ҫ����ĩβ���mode22������������Ҫ����SN3731res.h�а�
	 mode22�Ķ���������ӽ�ȥ��Ȼ���ڸ��ļ��ж���һ����¼��
	 ����֡����ȫ�ֱ���g_mode22_NO��ͬʱ��������������:
	 #define g_MOVIE_mode22			0x16��Ȼ����void Load_Data_PWM(unsigned char movie_name)
	 ���������װ��mode22�������ݴ��룬���������ӿɲ�������
	 ��������ڱ�������ĩβ���һ��ģʽ������:
	 SN3731_PingPang_BufferWrite(g_mode22_NO,g_MOVIE_mode22,0x04)����
*******************************************************************/
void SN3731_DEMO(unsigned char demo_index)
{

	switch (demo_index)
	{
		case 1:
			#ifdef STC_MCU
					TR0 = 0;//��STC_MCU ��ʱ��
					EX0 = 1;
			#endif
				SN3731_PingPang_BufferWrite(g_MOVIE_fireworks, 0x03);//play fireworks�����̻�
				break;
		/*case 2:
			SN3731_PingPang_BufferWrite(g_MOVIE_lighting, 0x06);//play lighting���ŵƹ�
			break;
		case 3:

			SN3731_PingPang_BufferWrite(g_MOVIE_unlock, 0x04);
			break;
		case 4:

			SN3731_PingPang_BufferWrite(g_MOVIE_water, 0x04);
			break;
		case 5:
			#ifdef STC_MCU
				TR0 = 0;//��STC_MCU ��ʱ��
				EX0 = 0;
			#endif
			SN3731_Init();		 //ȫ���ʼ��
			SN3731_SoftWareEn(1);//����ģʽ��SN3731���ʹ��
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_tingtaitu);			//д8֡���ݵ�IC��8�����ݾִ�����
			SN3731_SelectSection(Register_MainAddress, Reg_Section_Fram(2));//���ģʽ�еĵڶ��������һЩ�㿪��˸
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(30, 1);   //��30�㿪��˸
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(45, 1);   //��45�㿪��˸
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(47, 1);   //��47�㿪��˸
			SN3731_LedBlinkTurnOnOff_WrtieInSequence(62, 1);   //��62�㿪��˸
			SN3731_LedBlinkTurnOnOff_WrtieInMatrix(8, 3, 1);   //��8�С���3�п���˸
			SN3731_LedBlinkTurnOnOff_WrtieInMatrix(9, 2, 1);   //��9�С���2�п���˸
			SN3731_LedBlinkTurnOnOff_WrtieInMatrix(9, 4, 1);   //��9�С���4�п���˸
			SN3731_Blink_period(1);							   //������˸ʱ��
			SN3731_BlinkEN(1);								   //��˸ʹ��
			SN3731_BreathEN(1);			//LED����ʹ��
			SN3731_Breath_extinguish(4);//����Ϩ��ʱ��
			SN3731_Breath_RampDown(4);	//���ý���ʱ��
			SN3731_Breath_RampUp(4);	//���ý���ʱ��
			SN3731_Set_FrameNum(0);		//8֡ѭ��
			SN3731_Set_LoopNum(0);		//һֱѭ������
			SN3731_Movie_EN(1);			//movie ����ʹ��
			break;

		case 6:
			SN3731_Init();
			SN3731_SoftWareEn(1);
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_audio1);
			SN3731_AudioFrame_EN(1);//����frameʹ��
			break;
		case 7:
			SN3731_Init();
			SN3731_SoftWareEn(1);
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_audio2);
			SN3731_AudioFrame_EN(1);//����frameʹ��
			break;
		case 8:
			SN3731_SoftWareEn(1);
			SN3731_Write8Picture_Into8FrameReg(g_MOVIE_audio3);
			SN3731_AudioFrame_EN(1);//����frameʹ��
			#ifdef STC_MCU
					TR0 = 1;//��STC_MCU ��ʱ��
			#endif
			break;*/

			/****************************************************
			�ڴ˴����ģʽ����(�û�д�ģ������ҵĴ��С��...��������)
			****************************************************/
		case 9:
			break;
}

	//if(mode!=1)break;		
}








