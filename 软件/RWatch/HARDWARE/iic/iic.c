#include "main.h"
#include "iic.h"

#define IIC_SCL_H()      GPIO_SetBits(GPIOB,GPIO_Pin_6)  
#define IIC_SCL_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define IIC_SDA_H()      GPIO_SetBits(GPIOB,GPIO_Pin_7)  
#define IIC_SDA_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define IIC_SDA_Read()   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

//=========================================IIC基本配置函数=================================================//
//iic专用延时
static void IIC_Delay(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a = 6;//6
		while(a--);
	}
}

//IIC端口初始化  PB6->SCL   PB7->SDA
void IIC_GPIO_Init(void) 
{
	GPIO_InitTypeDef   gpio;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6; 
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &gpio);
}

//SDA数据发送初始化
static void IIC_SDA_Out(void)
{
	GPIO_InitTypeDef   gpio;    
	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &gpio);
}

//SDA数据接收初始化
static void IIC_SDA_In(void)
{
    GPIO_InitTypeDef   gpio;    
	  gpio.GPIO_Pin = GPIO_Pin_7;    
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &gpio);
}

//=======================================IIC应用函数==============================================//

//产生IIC起始信号
void IIC_Start(void)								  
{
	IIC_SDA_Out();
	IIC_SDA_H();
	IIC_SCL_H();
	IIC_Delay(1);
	IIC_SDA_L();
	IIC_Delay(1);
	IIC_SCL_L();
}

//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SDA_Out();
	IIC_SCL_L();
	IIC_SDA_L();
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_SDA_H();
	IIC_Delay(1);
}

//等待应答信号
//返回值：1->应答失败
//			 0->应答成功
int IIC_WaitAck(void)
{
	u16 Out_Time=250;    
	IIC_SDA_H();
	IIC_SDA_In();
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_Delay(1);
	while(IIC_SDA_Read())
	{
		if(--Out_Time)
		{
			IIC_Stop();
      return 1;
		}
	}
	IIC_SCL_L();
  return 0;
}

//产生ACK应答
//入口参数：0->产生应答
//				 1->不产生应答
void IIC_Ack(u8 re)					     
{
	IIC_SDA_L();
	IIC_SDA_Out();
	if(re)
	   IIC_SDA_H(); //不产生应答
	else
	   IIC_SDA_L();	//产生应答
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_Delay(1);	
	IIC_SCL_L();
}

//IIC发送一个字节
//返回从机有无应答：1有应答，0无应答
void IIC_WriteBit(u8 Temp)
{
	u8 i;
	IIC_SDA_Out();
	IIC_SCL_L();//拉低时钟开始数据传输
	for(i=0;i<8;i++)
	{
		if(Temp&0x80)
		{
			IIC_SDA_H();
		}
		else
		{
			IIC_SDA_L();
		}
		Temp<<=1;
		IIC_Delay(1);
		IIC_SCL_H();
		IIC_Delay(1);
		IIC_SCL_L();
	}
}

//读1个字节
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_In();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
    IIC_SCL_L(); 
    IIC_Delay(2);
		IIC_SCL_H();
     receive<<=1;
    if(IIC_SDA_Read())receive++;   
		IIC_Delay(1); 
    }					 
    if (!ack)
        IIC_Ack(1);//发送nACK
    else
        IIC_Ack(0); //发送ACK   
    return receive;
}


