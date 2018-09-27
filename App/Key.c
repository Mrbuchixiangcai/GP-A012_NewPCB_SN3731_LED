//ͷ�ļ�header file//
#include "app_main.h"

uint8_t   gbKeyPress;     //��������
uint8_t   gbKeyNone;      //û�а�������
uint8_t   KeyValue;       //����ֵ
uint8_t   KeyEvent;       //�����¼�
uint8_t   cntKeyLoop;     //��������ѭ�����̰����������������İ���ʱ��
uint8_t   cntKeyLong;     //��������ʱ������������ڳ�����������






uint8_t SyncNum_Table[7][6]= 
{
	"B0050\n", //�صƹ�
	"B0051\n", //С��
	"B0052\n", //���
	"B0053\n", //�������
	"B0054\n", //����������
	"B0055\n", //��������
	"B0056\n", //��������
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
    static uint8_t KeyCurValue; //������ѹ��ǰֵ��current��ǰ��
    static uint8_t KeyLastValue;//������ѹ��һ�ε�ֵ
	gbKeyPress=0;
	if(KeyCurValue != KeyTmp)//���if�����ȥ�����ᱻִ�����Σ������ν���ʱֱ������
	{//KeyTmpΪ��ʱֵ
	  KeyCurValue=KeyTmp;
	  return;
	}
	if(KeyCurValue == KeyLastValue)
	{
		if((KeyLastValue==0) || (cntKeyLoop==0))
			return;
		if(--cntKeyLoop==0)//����ڵڶ��ν�KeyEventPorc()��������һ�ν���if(KeyCurValue==KeyLastValue)��֮ǰ��ʱ�����渳ֵΪ5
		{//cntKeyLoopΪ5����50msʱ��
			KeyValue=KeyLastValue;//����֮���ȷ���İ�����ֵ��KeyValue
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

//������������
void KeyScan(void)
{
	KeyEventPorc(GetKeyValue());
	
}

//��������
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
				else if(PlayMode==PLAY_ON)//�����ػ������lihgt��ֻ��С��͹ر��������
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






























