#ifndef  __RGB_H__
#define  __RGB_H__
#include "app_main.h"

typedef struct
{
	uint8_t  path;
	uint8_t  step;
	uint8_t  data[2][6];
}LED_CAMPFIRE_TYPEDEF;

extern uint8_t  Fire_Data[6][10]; 
extern uint8_t  fire_Data[12][5]; 
void FireMode_Handle(void);

#endif









