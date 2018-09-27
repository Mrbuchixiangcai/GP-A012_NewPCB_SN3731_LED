//头文件header file//
#include "app_main.h"

//typedef struct
//{
//	uint8_t  path;
//	uint8_t  step;
//	uint8_t  data[3][36];
//}LED_CAMPFIRE_TypeDef;
//LED_CAMPFIRE_TypeDef campFire1;
//LED_CAMPFIRE_TypeDef campFire2;

uint8_t  RGB_Work;
uint8_t  step;
uint8_t  Fire_Data[6][10]; 
uint8_t  fire_Data[12][5]; 

 const uint8_t Map_Addr_H[6][10] =
 {
  58,59,46,47,34,35,22,23,10,11,	
  56,57,44,45,32,33,20,21, 8, 9,	
  54,55,42,43,30,31,18,19, 6, 7,
  52,53,40,41,28,29,16,17, 4, 5,
  50,51,38,39,26,27,14,15, 2, 3,
  48,49,36,37,24,25,12,13, 0, 1,
 };
 const uint8_t Map_Addr_V[12][5] = 
 {                                
   59,47,35,23,11,                 
  58,46,34,22,10,                 
  57,45,33,21, 9,                 
  56,44,32,20, 8,                 
  55,43,31,19, 7,	          
  54,42,30,18, 6,                 
  53,41,29,17, 5,                 
  52,40,28,16, 4,                 
  51,39,27,15, 3,                 
  50,38,26,14, 2,                 
  49,37,25,13, 1,                 
  48,36,24,12, 0,                 
 }; 
/*******************************************************************************************/
/*******************************************************************************************/ 
void LoadData_to_SN3236Driver_V(void)
{
 u8 i,j,addr;
 for(i=0;i<12;i++)
 {
  for(j=0;j<5;j++)
  {
   addr= Map_Addr_V[i][j];
   if(addr<36)
	SN3236_Pwm2_1[addr]=fire_Data[i][j];
   else
	SN3236_Pwm2_2[addr-36]=fire_Data[i][j];
  }
 }		
}

void LoadData_to_SN3236Driver_H(void)
{
 u8 i,j,addr;
 for(i=0;i<6;i++)
 {
  for(j=0;j<10;j++)
  {
   addr= Map_Addr_H[i][j];
   if(addr<36)
	SN3236_Pwm2_1[addr]=Fire_Data[i][j];
   else
	SN3236_Pwm2_2[addr-36]=Fire_Data[i][j];
  }
 }		
}
/*******************************************************************************************/
/*******************************************************************************************/ 
void Off_Fire_Mode(void)
{
 u8 i,j;
 for(i=0;i<6;i++)
 {
  for(j=0;j<10;j++)
	 {
   Fire_Data[i][j]=0;
		 fire_Data[i][j]=0;
	 }
 }		
}
/*******************************************************************************************/
/*******************************************************************************************/ 
void FireMode_Handle(void)
{	

	//灯光模式关
 if (FireMode == MODE0_OFF_FIRE)
 {
  Off_Fire_Mode();
	 
	LoadData_to_SN3236Driver_H();
 }
 
 //小火
 else if (FireMode == MODE1_SMALL_FIRE1)
 {
  Small_Fire_Mode();
	LoadData_to_SN3236Driver_V();
	 
 }
 //
 
// //小火
// else if (FireMode == H_SMALL_FIRE1)
// {
//	 
//	 h_Small_Fire_Mode();
//	 LoadData_to_SN3236Driver_H();
// }
// //
// 
 //大火
 else if (FireMode == MODE3_BIG_FIRE1)
 {
//  Big_Fire_Mode();
//	LoadData_to_SN3236Driver_V();
	 
	 h_Big_Fire_Mode();
	 LoadData_to_SN3236Driver_H();
 }
 //
// 
// //大火
// else if (FireMode == H_BIG_FIRE1)
// {
//	 
//	 h_Big_Fire_Mode();
//	 LoadData_to_SN3236Driver_H();
// }
// //
 
 //随风
 else if (FireMode == FOLLOW_WIND)
 {
  Follow_Wind_Fire_Mode();
	 
	LoadData_to_SN3236Driver_V();
 }
 
 //火焰节奏
 else if (FireMode == MODE4_PULSATE_MUSIC)
 {
  Music_Fire_Mode();
	 
	LoadData_to_SN3236Driver_H();
 }
 //
 
 
 else if (FireMode == MUSIC_PULSE_FLASH)
 {
  //脉冲跟随音量
	pulse_follow_volume();
	 
	LoadData_to_SN3236Driver_H();
	 
 }
 //
 else if (FireMode == MUSIC_PULSE_FLASH_2)
 {
  //脉冲跟随音量
	pulse_follow_volume_2();
	 
	LoadData_to_SN3236Driver_H();
 }
 //
 
}
/*******************************************************************************************/
/*******************************************************************************************/ 











