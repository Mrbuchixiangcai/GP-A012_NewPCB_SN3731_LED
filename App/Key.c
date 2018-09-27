//头文件header file//
#include "app_main.h"

uint8_t   gbKeyPress;     //按键按下
uint8_t   gbKeyNone;      //没有按键按下
uint8_t   KeyValue;       //按键值
uint8_t   KeyEvent;       //按键事件
uint8_t   cntKeyLoop;     //计数按键循环，短按，长按，超长按的按下时间
uint8_t   cntKeyLong;     //按键按下时间计数，可用于长按键和其他






uint8_t SyncNum_Table[7][6]= 
{
	"B0050\n", //关灯光
	"B0051\n", //小火
	"B0052\n", //大火
	"B0053\n", //火焰随风
	"B0054\n", //火焰随音乐
	"B0055\n", //单列脉冲
	"B0056\n", //三列脉冲
};

uint8_t GetKeyValue(void)
{
	uint8_t  KeyNum=0;
	if(!KEY_LIGHT())
		KeyNum=T_LIGHT;
	return KeyNum;
}

void KeyEventPorc(uint8_t KeyTmp)
{
    static uint8_t KeyCurValue; //按键电压当前值，current当前的
    static uint8_t KeyLastValue;//按键电压上一次的值
	gbKeyPress=0;
	if(KeyCurValue != KeyTmp)//这个if语句是去抖，会被执行两次，第三次进来时直接跳过
	{//KeyTmp为临时值
	  KeyCurValue=KeyTmp;
	  return;
	}
	if(KeyCurValue == KeyLastValue)
	{
		if((KeyLastValue==0) || (cntKeyLoop==0))
			return;
		if(--cntKeyLoop==0)//这个在第二次进KeyEventPorc()函数（第一次进“if(KeyCurValue==KeyLastValue)”之前）时被下面赋值为5
		{//cntKeyLoop为5就是50ms时间
			KeyValue=KeyLastValue;//防抖之后把确定的按键赋值给KeyValue
			if(gbKeyNone==0)
				gbKeyPress=1;
			switch(KeyEvent)
			{
				case 0:
					KeyEvent=1;
					KeyValue |= K_D;
					cntKeyLoop=cKEY_HOLD;
					break;
				case 1:
					KeyEvent=2;
					KeyValue |= K_H;
					cntKeyLoop=cKEY_RATE_VOL;
					break;
				case 2:
				case 3:
					KeyEvent=3;
					KeyValue |= K_R;
					cntKeyLoop=cKEY_RATE_VOL;
					if(cntKeyLong<250)
						cntKeyLong++;
					break;
			}
		}
	}
	else
	{
		if(KeyLastValue)
		{
			KeyValue=KeyLastValue;
			if(gbKeyNone==0)
				gbKeyPress=1;
			switch(KeyEvent)
			{
				case 1:KeyValue |= K_U; break;
				case 2:KeyValue |= K_LU;break;
				case 3:KeyValue |= K_LU;break;
			}
			KeyEvent=0;
		}
		else
		{
			gbKeyNone=0;
			cntKeyLong=0;
		}
		cntKeyLoop=cKEY_CLICK;
		KeyLastValue=KeyCurValue;
	}
}

//被主函数调用
void KeyScan(void)
{
	KeyEventPorc(GetKeyValue());
	
}

//按键处理
void KeyComMsg(void)
{
	static uint16_t  cntUart_Send=0;
	if(gbKeyPress)
	{
		if(PlayMode==PLAY_OFF)
			PlayMode=PLAY_ON;
		switch(KeyValue)
		{
//			case KU(T_BT_POWER):
//			{}
//			case KLU(T_BT_POWER):
//			{
//				if(cntKeyLong>=10)
//					break;
//				PlayMode=PLAY_BT;
//				break;
//			}
			case KU(T_LIGHT): 
			{
				if(PlayMode==PLAY_BT)
				{
					FireMode++;
					if(FireMode>FIRE_SIZE_MAX)
						FireMode=MODE0_OFF_FIRE;
					if(FireMode == MODE0_OFF_FIRE)
					{
						if(BTPower())
							PlayMode=PLAY_BT;
						else
							PlayMode=PLAY_OFF;
					}
				}
				else if(PlayMode==PLAY_ON)//蓝牙关机情况打开lihgt，只有小火和关闭两种情况
				{
					//PlayMode=PLAY_ON;
					if(FireMode==MODE0_OFF_FIRE)
					{	
						FireMode=MODE1_SMALL_FIRE1;
						
					}
					else
					{	
						FireMode=MODE0_OFF_FIRE;
						PlayMode=PLAY_OFF;
					}
				}
				enUart_Send=1;
				if(cntUart_Send==600)
				{
					enUart_Send=0;	
					cntUart_Send=0;	
					Usart1SendData(&SyncNum_Table[FireMode][0]);
				}					
				break;
			}
			case KLU(T_LIGHT):
			{
				if (cntKeyLong >= 50) 
					break;
				if(brightness1==1)
					brightness1=3;
				else
					brightness1=1;
				break;
			}
		}
	}
	if(enUart_Send)
	{	
		if(++cntUart_Send>600)
		{	 
			enUart_Send=0;  
			cntUart_Send=0;	  
			Usart1SendData(&SyncNum_Table[FireMode][0]);
		}	  
	}
	else if(cntUart_Send<600)
		cntUart_Send++;	 
}






























