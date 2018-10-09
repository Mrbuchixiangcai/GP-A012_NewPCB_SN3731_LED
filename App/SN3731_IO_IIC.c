/*******************/
/*头文件header file*/
/*******************/
#include "app_main.h"
#include "SN3731_IO_IIC.h"
#include "SN3731.h"

/************************/
/*宏定义macro definition*/
/************************/
uint8_t g_SN3731_IIC_ADDRESS = SN3731_IIC_ADDRESS_M;

/*************************/
/*类型定义byte definition*/
/*************************/

/****************************/
/*标志位定义flags definetion*/
/****************************/

/*****************************/
/*变量定义variable definition*/
/*****************************/

/**************************/
/*数组定义array definition*/
/**************************/

/******************************/
/*函数声明Function declaration*/
/******************************/
extern uint8_t g_SN3731_IIC_ADDRESS;
/*****************************/
/*函数定义function definetion*/
/*****************************/
/*******************************************************************
* 函数原型：void Delay_1us(u8 t_data)
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
//void Delay_1us(u8 t_data)  //在IO_IIC.c中已经定义了
//{
//	for (; t_data > 0; t_data--);
//	//	for(;t_data>0;t_data--);
//	//	for(;t_data>0;t_data--);
//	//	for(;t_data>0;t_data--);	
//}

/*******************************************************************
* 函数原型：static void SN3731_IIC_Start(void)
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_IIC_Start(void)
{
	SN3731_SDA5_OUT();//端口配置为输出
	SN3731_SDA6_OUT();
	SN3731_IIC_DA5(1); /*发送起始条件的数据信号，先拉高SDA线，data线输出高.*/
	SN3731_IIC_DA6(1);
	Delay_1us(7);
	SN3731_IIC_CLK(1); /*发送起始条件的时钟信号，先拉高SCL线*/
	Delay_1us(7);
	SN3731_IIC_DA5(0);  /*发送起始信号，在SCL高电平时拉低SDA并SCL在稍后随之降低为起始信号*/
	SN3731_IIC_DA6(0);
	Delay_1us(7);
	SN3731_IIC_CLK(0);  /*钳住I2C总线，准备发送或接收数据 */
	Delay_1us(7);
}

/*******************************************************************
 * 函数原型：static void SN3731_IIC_Stop(void)
 * 输入参数：
 * 输出参数：
 * 函数功能：
 * 返回值说明：
 * 创建日期：
 * 创建人：
 * 修改日期
 * 修改人：
 * 第N次修改：
 * 修改原因：
 * 备注：
 *******************************************************************/
void SN3731_IIC_Stop(void)
{
	SN3731_IIC_DA5(0);/*发送结束条件的数据信号*/
	SN3731_IIC_DA6(0);
	Delay_1us(7);
	SN3731_IIC_CLK(1);/*结束条件建立时间大于0.6μs*/
	Delay_1us(7);
	SN3731_IIC_DA5(1);/*发送I2C总线结束信号*/
	SN3731_IIC_DA6(1);
	Delay_1us(7);
}

/*******************************************************************
* 函数原型：static unsigned char SN3731_IIC_SDA_Read(void)
* 输入参数：
* 输出参数：
* 函数功能：判断I2C是否通信正常,即通信应答信号的返回函数，ack那个应答
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
static unsigned char SN3731_IIC_SDA_Read(void)
{
	unsigned char back;
//	SN3731_SDA5_IN();//因为要读取PA5电平状态，检查是否要ack回应，所以要配置为输入
//	SN3731_SDA6_IN();
//	SN3731_IIC_CLK(1);
//	back=SN3731_READ_SDA5();
//	SN3731_IIC_CLK(0);

//	SN3731_SDA5_OUT();//要继续输出，所以配置为输出
//	SN3731_SDA6_OUT();
	//return back;//本来是返回back的，但是默认通信正常，如果是比较严谨的就要换上这句
	back=0;		  //本来是返回back的，但是默认通信正常，如果是比较严谨的就要换上这句
	return back;
}

/*******************************************************************
* 函数原型：static unsigned char SN3731_IIC_SDA_Read(void)
* 输入参数：
* 输出参数：
* 函数功能：判断I2C是否通信正常,即通信应答信号的返回函数，ack那个应答
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
static unsigned char SN3731_IIC_Wait_Ack(void)
{
	SN3731_IIC_CLK(1);
	Delay_1us(5);
	SN3731_IIC_CLK(0);
	Delay_1us(5);
	return 0;
}

/*******************************************************************
* 函数原型：static void SN3731_IIC_SendByte(unsigned char c)
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
static void SN3731_IIC_SendByte(unsigned char txData)
{
	for (uint8_t t = 0; t < 8; t++)
	{
		if (txData & 0x80) 
		{ 
			SN3731_IIC_DA5(1);
		}
		else 
		{ 
			SN3731_IIC_DA5(0);
		}
		if (txData & 0x80) 
		{ 
			SN3731_IIC_DA6(1);
		}
		else 
		{ 
			SN3731_IIC_DA6(0);
		}
		Delay_1us(5);
		SN3731_IIC_CLK(1);
		Delay_1us(5);
		txData <<= 1;
		SN3731_IIC_CLK(0);
		Delay_1us(5);
	}
//	Delay_1us(5);
//	SN3731_IIC_DA5(1);
//	SN3731_IIC_DA6(1);	 /* 8位发送完后释放数据线，准备接收应答位(释放就是拉高，这个高是在
//						 SCL检测到第八位的电平状态之后无论第八位是高电平还是低都拉高，然后SCL在发送
//						 脉冲期间SDA被拉低，所以和上面数据第一位(从最高位开始发送)的高电平(也有可能
//						 为低电平)形成了一个脉冲型，但如果为低就不是脉冲型)*/
//						 //for (j=0; j < SN3731_DELAY; j++);
//						 //for (j=0; j < SN3731_DELAY; j++);
//						 //for (j=0; j < SN3731_DELAY; j++);
//						 //for (j=0; j < SN3731_DELAY; j++);
//	__NOP();
//	SN3731_IIC_CLK(1);	 /* *脉冲开始* 释放SDA线为高电平，然后使SCL发送一个脉冲。如果SN3731正确的接收
//						 到8位数据，在SCL的脉冲期间它将使SDA拉低；如果SDA线不为低，则表示数据没有正
//						 确接收，主控器件将会发送一个“停止”信号（稍候介 绍）并且中断数据传递)*/
//						 //for (j=0; j < SN3731_DELAY; j++);
//	__NOP();

//	//#if STC_MCU_ACK	   //#define STC_MCU 1  //1需要ACK信号的话，0忽略ACK信号的话 ,在程序中STC测试ACK与其他平台的不一样
//	//	if (DATA == 1) //读取DATA的值，判断是否接收到应答信号
//	//		ack = 0;   //没有接收到应答信号时就是没有拉低，所以DATA为1，那ack为0，
//	//	else
//	//		ack = 1;   //接收到应答信号时ack要为1
//	//#endif

//	#if SN_IIC_ACK	//#define SN_IIC_ACK  0  //1需要ACK信号的话，0 忽略ACK信号的话  //有应答功能
//		SN3731_SDA5_IN(); //在第九个SCL之前将SDA设置为输入，准备接收应答信号
//		SN3731_SDA6_IN();
//		__NOP();
//		SN3731_IIC_CLK(1);
//		__NOP();
//		ack = SN3731_IIC_SDA_Read();//读取应答信号
//		__NOP();
//		SN3731_IIC_CLK(0);
//		__NOP();
//		SN3731_SDA5_OUT(); 	//应答信号在第九个SCL下降沿接除了，这个设置为输出
//		SN3731_SDA6_OUT();
//		SN3731_IIC_DA5(0);	//同时将SDA拉低
//		SN3731_IIC_DA6(0);
//		__NOP();
//		if (ack)
//		{
//			SN3731_IIC_CLK(0); //停止IIC总线.stop.
//			__NOP();
//			SN3731_IIC_DA5(1);
//			SN3731_IIC_DA6(1);
//			__NOP();
//			return ack;
//		}
//	#else
//		{
//			SN3731_IIC_DA5(0); //在第九个SCL之前直接将SDA拉低，忽视应答
//			SN3731_IIC_DA6(0);
//			__NOP();
//			SN3731_IIC_CLK(1);
//			__NOP();
//			SN3731_IIC_CLK(0);
//			__NOP();
//		}
//	#endif
//	ack=SN3731_IIC_SDA_Read();

//	SN3731_IIC_CLK(0);	 /* *脉冲结束*，对应上面的脉冲开始*/
//	__NOP();
//	SN3731_IIC_Wait_Ack();
//	ack=1;
}

/*******************************************************************
* 函数原型：SN3731_IIC_SelectSection(unsigned char MainAddress,unsigned char SectionReg)
* 输入参数：
* 输出参数：
* 函数功能：用户接口函数 ：选择寄存器(maybe),向SN3731连续发送多个(应该是单个)数据的全过
			程,从器件地址已经包含在函数中，寄存器地址suba，发送内容是s指向的内容，
			发送no个字节。
			如果返回1表示操作成功，否则操作有误。
* 返回值说明：如果返回1表示操作成功，否则操作有误。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
unsigned char SN3731_IIC_SelectSection(unsigned char MainAddress, unsigned char SectionReg)
{
	SN3731_IIC_Start();               /*启动总线*/
	SN3731_IIC_SendByte(g_SN3731_IIC_ADDRESS);/*发送器件地址,0xe8*/
//	if (ack == 0)
//		return(0);
	SN3731_IIC_Wait_Ack();
	SN3731_IIC_SendByte(MainAddress); /*发送寄存器主地址,*/
//	if (ack == 0)
//		return(0);
	SN3731_IIC_Wait_Ack();
	SN3731_IIC_SendByte(SectionReg);  /*主寄存器数据*/
//	if (ack == 0)
//		return(0);
	SN3731_IIC_Wait_Ack();
	SN3731_IIC_Stop();
	return(1);
}


/*******************************************************************
* 函数原型：unsigned char SN3731_IIC_WriteRegs(unsigned char reg,unsigned char *s,unsigned char no)
* 输入参数：
* 输出参数：
* 函数功能：用户接口函数 ：向SN3731连续发送多个数据的全过程,从器件
			地址已经包含在函数中，寄存器地址suba，发送内容是s指向的内容，
			发送no个字节。
			如果返回1表示操作成功，否则操作有误。
* 返回值说明：如果返回1表示操作成功，否则操作有误。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：使用前必须已结束总线,从周立功的IIC程序中修改过来的
*******************************************************************/
unsigned char SN3731_IIC_WriteRegs(unsigned char reg, unsigned char *s, unsigned char no)
{
	unsigned char i;
	SN3731_IIC_Start();         /*启动总线*/
	SN3731_IIC_SendByte(g_SN3731_IIC_ADDRESS);/*发送器件地址*/
//	if (ack == 0)
//		return(0);
	SN3731_IIC_Wait_Ack();
	SN3731_IIC_SendByte(reg);   /*发送器件子地址*/
//	if (ack == 0)
//		return(0);
	SN3731_IIC_Wait_Ack();
	for (i = 0; i < no; i++)
	{
//		SN3731_IIC_SendByte(*s);/*发送数据*/
//		if (ack == 0)
//			return(0);
		SN3731_IIC_Wait_Ack();
		s++;
	}
	SN3731_IIC_Stop();          /*结束总线*/
	return (1);
}
/*******************************************************************
* 函数原型：unsigned char SN3731_IIC_Write_Reg(unsigned char reg,unsigned char dat)
* 输入参数：
* 输出参数：
* 函数功能：用户接口函数 ：向SN3731发送一个数据的全过程,从器件
			地址已经包含在函数中，寄存器地址reg，发送内容是dat.
			如果返回1表示操作成功，否则操作有误。
* 返回值说明：如果返回1表示操作成功，否则操作有误。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：使用前必须已结束总线，从周立功的IIC程序中修改过来的
*******************************************************************/
unsigned char  SN3731_IIC_Write_Reg(unsigned char reg, unsigned char dat)
{
	SN3731_IIC_Start();    //启动总线*/
	SN3731_IIC_SendByte(g_SN3731_IIC_ADDRESS);//发送器件地址，g_SN3731_IIC_ADDRESS是器件地址
	if (ack == 0)
		return(0);
	SN3731_IIC_SendByte(reg);  //发送器件子地址
	if (ack == 0)
		return(0);
	SN3731_IIC_SendByte(dat);  //发送数据
	if (ack == 0)
		return(0);
	SN3731_IIC_Stop();     //结束总线
	return(1);
}

/*******************************************************************
* 函数原型：unsigned char SN3731_IIC_Read8Bit()
* 输入参数：
* 输出参数：
* 函数功能：读出8个unsigned char
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
unsigned char SN3731_IIC_Read8Bit(void)//读出8个二进制位
{
	unsigned char temp, j, returnByte = 0;
	SN3731_SDA5_IN();//因为要读取，所以要配置为输入，
	//SN3731_SDA6_IN();//之所以配置一个，因为配置两个也是返回一个数据
	for (temp = 8; temp != 0; temp--)
	{
		SN3731_IIC_CLK(1);
		for (j = 0; j < SN3731_DELAY + 2; j++)
			;
		returnByte = returnByte << 1;
		returnByte = (returnByte | ((unsigned char)(SN3731_READ_SDA5())));
		SN3731_IIC_CLK(0);
		for (j = 0; j < SN3731_DELAY + 2; j++)
			;
	}
	SN3731_SDA5_OUT();//读取完了之后配置为输出
	return	(returnByte);
}

/*******************************************************************
* 函数原型：SN3731_IIC_ReadReg(unsigned char add)
* 输入参数：寄存器的地址
* 输出参数：
* 函数功能：读出一个指定寄存器的值
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
unsigned char SN3731_IIC_ReadReg(unsigned char add)
{
	unsigned char current_data, j;
	SN3731_IIC_Start();
	SN3731_IIC_SendByte(g_SN3731_IIC_ADDRESS);
	if (ack == 0)
		return(0);
	SN3731_IIC_SendByte(add);
	if (ack == 0)
		return(0);
	SN3731_IIC_Stop();
	Delay_1us(5);

	SN3731_IIC_Start();
	SN3731_IIC_SendByte(g_SN3731_IIC_ADDRESS + 1);
	if (ack == 0)
		return(0);
	for (j = 0; j < SN3731_DELAY + 2; j++)
		;
	current_data = SN3731_IIC_Read8Bit();
	SN3731_IIC_CLK(1);
	for (j = 0; j < SN3731_DELAY + 2; j++)
		;
	SN3731_IIC_CLK(0);
	for (j = 0; j < SN3731_DELAY + 2; j++)
		;
	SN3731_IIC_Stop();   /*结束总线*/
	return(current_data);
}

/*******************************************************************
* 函数原型：SN3731_WriteRegs(unsigned char RegFristAdd,unsigned char *s,unsigned char no)
* 输入参数：
* 输出参数：
* 函数功能：向SN3731连续发送多个数据的全过程,从器件
			地址已经包含在函数中，寄存器地址suba，发送内容是s指向的内容，
			发送no个字节，重新封装，供上层调用。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void  SN3731_WriteRegs(unsigned char RegFristAdd, unsigned char *s, unsigned char no)
{
	SN3731_IIC_WriteRegs(RegFristAdd, s, no);

}

/*******************************************************************
* 函数原型：SN3731_WriteReg(unsigned char reg,unsigned char dat)
* 输入参数：
* 输出参数：
* 函数功能：写指定寄存器指定值，重新封装，供上层调用。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_WriteReg(unsigned char reg, unsigned char dat)
{
	SN3731_IIC_Write_Reg(reg, dat);
}

/*******************************************************************
* 函数原型：SN3731_SelectSection(unsigned char MainAddress,unsigned char SectionReg)
* 输入参数：
* 输出参数：
* 函数功能：指向（SectionReg）指定的寄存器页，重新封装，供上层调用。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
void SN3731_SelectSection(unsigned char MainAddress, unsigned char SectionReg)
{
	SN3731_IIC_SelectSection(MainAddress, SectionReg);
}

/*******************************************************************
* 函数原型：unsigned char SN3731_ReadReg(unsigned char add)
* 输入参数：
* 输出参数：
* 函数功能：读出一个指定寄存器的值，重新封装，供上层调用。
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************/
unsigned char SN3731_ReadReg(unsigned char add)
{
	unsigned char RegData;
	do
	{
		RegData = SN3731_IIC_ReadReg(add);
	} while (!ack);
	return(RegData);
}


































