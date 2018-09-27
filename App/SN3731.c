//Í·ÎÄ¼þheader file//
#include "app_main.h"

u8 SN3236_Pwm2_1[36];
u8 SN3236_Pwm2_2[36];
u8 SN3236_RegBuffer[0x4B];

void SN3236_RegWrite(u8 *p)
{
	u8 i;
	IIC_Start();
	IIC_Send_Byte(0x78);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	for(i=0;i<0x4B;i++)
	{
	 if((i>0)&&(i<0x25))
		IIC_Send_Byte_Mix(i);
	 else
	  IIC_Send_Byte(p[i]);
	 IIC_Wait_Ack();
	}
	IIC_Stop();
}
void SN3236_Init1(void)
{
	u8 i;
	for(i=0;i<0x4B;i++)
	 SN3236_RegBuffer[i]=0;
	for(i=0x26;i<0x4A;i++)
	 SN3236_RegBuffer[i]=electricityBrightness;
	SN3236_RegBuffer[REG_00H_TURN_ON]=0x01;
}

void SN3236_Driver()
{
 SN3236_Init1();	
 SN3236_RegWrite(SN3236_RegBuffer);	
}
//u8  xxxx;
//u8  yyyy;
//	u8 i;
//	for(i=0;i<36;i++)
//		SN3236_Pwm2_1[i]=0x00;
//	for(i=0;i<36;i++)
//		SN3236_Pwm2_1[i]=0x00;
//	SN3236_Pwm2_1[xxxx]=0xF0;
//	SN3236_Pwm2_2[yyyy]=0xF0;	


















