#ifndef  __IO_IIC_H__
#define  __IO_IIC_H__

//头文件header file//


//宏定义macro definition//
typedef unsigned        char uint8_t;
typedef unsigned short  int  uint16_t;
typedef unsigned        int  uint32_t;

#define uchar  uint8_t
#define uint   uint16_t
#define ulong  uint32_t

#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t

//IO方向设置,这样可以快速设置IO方向，而不影响其他位
#define  SDA5_IN()    {GPIOA->MODER &= (~((u32)0x03<<(5*2 )));GPIOA->MODER |= ((u32)00<<(5*2 ));}//GPIOA_Pin5设置为输入
#define  SDA5_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(5*2 )));GPIOA->MODER |= ((u32)01<<(5*2 ));}//GPIOA_Pin5设置为输出
#define  SDA6_IN()    {GPIOA->MODER &= (~((u32)0x03<<(6*2 )));GPIOA->MODER |= ((u32)00<<(6*2 ));}
#define  SDA6_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(6*2 )));GPIOA->MODER |= ((u32)01<<(6*2 ));}
#define  SDA7_IN()    {GPIOA->MODER &= (~((u32)0x03<<(7*2 )));GPIOA->MODER |= ((u32)00<<(7*2 ));}
#define  SDA7_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(7*2 )));GPIOA->MODER |= ((u32)01<<(7*2 ));}
#define  SDA8_IN()    {GPIOA->MODER &= (~((u32)0x03<<(8*2 )));GPIOA->MODER |= ((u32)00<<(8*2 ));}
#define  SDA8_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(8*2 )));GPIOA->MODER |= ((u32)01<<(8*2 ));}
#define  SDA9_IN()    {GPIOA->MODER &= (~((u32)0x03<<(9*2 )));GPIOA->MODER |= ((u32)00<<(9*2 ));}
#define  SDA9_OUT()   {GPIOA->MODER &= (~((u32)0x03<<(9*2 )));GPIOA->MODER |= ((u32)01<<(9*2 ));}
#define  SDA10_IN()   {GPIOA->MODER &= (~((u32)0x03<<(10*2)));GPIOA->MODER |= ((u32)00<<(10*2));}
#define  SDA10_OUT()  {GPIOA->MODER &= (~((u32)0x03<<(10*2)));GPIOA->MODER |= ((u32)01<<(10*2));}
#define  SDA11_IN()   {GPIOA->MODER &= (~((u32)0x03<<(11*2)));GPIOA->MODER |= ((u32)00<<(11*2));}
#define  SDA11_OUT()  {GPIOA->MODER &= (~((u32)0x03<<(11*2)));GPIOA->MODER |= ((u32)01<<(11*2));}
#define  SDA12_IN()   {GPIOA->MODER &= (~((u32)0x03<<(12*2)));GPIOA->MODER |= ((u32)00<<(12*2));}
#define  SDA12_OUT()  {GPIOA->MODER &= (~((u32)0x03<<(12*2)));GPIOA->MODER |= ((u32)01<<(12*2));}

#define  IIC_SDB(a)  if(a)    					\
						GPIOB->BSRR=GPIO_PIN_1; \
					 else     					\
						GPIOB->BRR=GPIO_PIN_1
#define  IIC_SCL(a)  if(a)    					\
						GPIOB->BSRR=GPIO_PIN_0; \
					 else     					\
						GPIOB->BRR=GPIO_PIN_0
#define  IIC_SDA5(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_5; \
					 else     					\
						GPIOA->BRR=GPIO_PIN_5				 
#define  IIC_SDA6(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_6; \
					 else     					\
						GPIOA->BRR=GPIO_PIN_6
#define  IIC_SDA7(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_7; \
					 else     					\
						GPIOA->BRR=GPIO_PIN_7
#define  IIC_SDA8(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_8; \
					 else     					\
						GPIOA->BRR=GPIO_PIN_8				 
#define  IIC_SDA9(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_9; \
					 else     					\
						GPIOA->BRR=GPIO_PIN_9
#define  IIC_SDA10(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_10;\
					 else     					\
						GPIOA->BRR=GPIO_PIN_10
#define  IIC_SDA11(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_11;\
					 else     					\
						GPIOA->BRR=GPIO_PIN_11				 
#define  IIC_SDA12(a) if(a)    					\
						GPIOA->BSRR=GPIO_PIN_12;\
					 else     					\
						GPIOA->BRR=GPIO_PIN_12

					 
		
					 
#define  READ_SDA5    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)
#define  READ_SDA6    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define  READ_SDA7	  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
#define  READ_SDA8    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)
#define  READ_SDA9    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9)
#define  READ_SDA10	  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)
#define  READ_SDA11   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)
#define  READ_SDA12   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)
					 
//类型定义byte definition//


//变量定义variable definition//


//标志位定义flags definetion//


//函数定义function definetion//
//IIC所有操作函数
void Delay_1us(u8 t_data);     	   
void IIC_Start(void);			 	  
void IIC_Stop(void);				  
void IIC_Send_Byte(u8 txd);   
void IIC_Send_Byte_Mix(u8 p);
u8   IIC_Wait_Ack(void);
#endif















