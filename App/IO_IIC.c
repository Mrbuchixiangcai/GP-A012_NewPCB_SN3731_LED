//头文件header file//
#include "app_main.h"
#include "IO_IIC.h"

void Delay_1us(u8 t_data)
{
	for(;t_data>0;t_data--);
//	for(;t_data>0;t_data--);
//	for(;t_data>0;t_data--);
//	for(;t_data>0;t_data--);	
}

void IIC_Init(void)  
{
	SDA5_OUT();  
	SDA6_OUT(); 
	SDA7_OUT(); 
	SDA8_OUT();  
	IIC_SDA5(1); 
	IIC_SDA6(1); 
	IIC_SDA7(1); 
	IIC_SDA8(1); 
	IIC_SCL(1); 
}

void IIC_Start(void) 
{
	IIC_Init();
	IIC_SDA5(0); 
	IIC_SDA6(0); 
	IIC_SDA7(0); 
	IIC_SDA8(0); 
	Delay_1us(0);
	IIC_SCL(0); 
}
void IIC_Stop(void) 
{
	IIC_SDA5(0); 
	IIC_SDA6(0); 
	IIC_SDA7(0); 
	IIC_SDA8(0); 
	Delay_1us(0); 
	IIC_SCL(1); 
	Delay_1us(0); 
	IIC_SDA5(1); 
	IIC_SDA6(1); 
	IIC_SDA7(1); 
	IIC_SDA8(1); 
}
u8 IIC_Wait_Ack(void)
{
	IIC_SCL(1); 
	Delay_1us(0);  
	IIC_SCL(0); 
	return 0;
}


void IIC_Send_Byte(u8 txd)//专门写寄存器地址的发送IIC函数
{
	for(uint8_t t=0;t<8;t++)
	{
		if(txd & 0x80){IIC_SDA5(1);}
		else{IIC_SDA5(0);}
		if(txd & 0x80){IIC_SDA6(1);}
		else{IIC_SDA6(0);}
		if(txd & 0x80){IIC_SDA7(1);}
		else{IIC_SDA7(0);}
		if(txd & 0x80){IIC_SDA8(1);}
		else{IIC_SDA8(0);}
		Delay_1us(0);
		IIC_SCL(1);
		Delay_1us(0);
		txd<<=1;
		IIC_SCL(0);
	}
}

void IIC_Send_Byte_Mix(u8 p)
{
	for(uint8_t t=0;t<8;t++)
	{
		if(SN3236_Pwm2_1[p-1] & 0x80)
     	{IIC_SDA7(1);IIC_SDA5(1);}
		else {IIC_SDA7(0);IIC_SDA5(0);}
		if(SN3236_Pwm2_2[p-1] & 0x80)
    	{IIC_SDA8(1);IIC_SDA6(1);}
		else {IIC_SDA8(0);IIC_SDA6(0);}
		Delay_1us(0); 
		IIC_SCL(1); 
		SN3236_Pwm2_1[p-1] <<= 1;
		SN3236_Pwm2_2[p-1] <<= 1;
		IIC_SCL(0); 
	}
}




























