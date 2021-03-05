#include "main.h"
#include "iic.h"

#define IIC_SCL_H()      GPIO_SetBits(GPIOB,GPIO_Pin_6)  
#define IIC_SCL_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define IIC_SDA_H()      GPIO_SetBits(GPIOB,GPIO_Pin_7)  
#define IIC_SDA_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define IIC_SDA_Read()   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

//=========================================IIC�������ú���=================================================//
//iicר����ʱ
static void IIC_Delay(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a = 6;//6
		while(a--);
	}
}

//IIC�˿ڳ�ʼ��  PB6->SCL   PB7->SDA
void IIC_GPIO_Init(void) 
{
	GPIO_InitTypeDef   gpio;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6; 
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &gpio);
}

//SDA���ݷ��ͳ�ʼ��
static void IIC_SDA_Out(void)
{
	GPIO_InitTypeDef   gpio;    
	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &gpio);
}

//SDA���ݽ��ճ�ʼ��
static void IIC_SDA_In(void)
{
    GPIO_InitTypeDef   gpio;    
	  gpio.GPIO_Pin = GPIO_Pin_7;    
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &gpio);
}

//=======================================IICӦ�ú���==============================================//

//����IIC��ʼ�ź�
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

//����IICֹͣ�ź�
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

//�ȴ�Ӧ���ź�
//����ֵ��1->Ӧ��ʧ��
//			 0->Ӧ��ɹ�
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

//����ACKӦ��
//��ڲ�����0->����Ӧ��
//				 1->������Ӧ��
void IIC_Ack(u8 re)					     
{
	IIC_SDA_L();
	IIC_SDA_Out();
	if(re)
	   IIC_SDA_H(); //������Ӧ��
	else
	   IIC_SDA_L();	//����Ӧ��
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_Delay(1);	
	IIC_SCL_L();
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��1��Ӧ��0��Ӧ��
void IIC_WriteBit(u8 Temp)
{
	u8 i;
	IIC_SDA_Out();
	IIC_SCL_L();//����ʱ�ӿ�ʼ���ݴ���
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

//��1���ֽ�
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_In();//SDA����Ϊ����
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
        IIC_Ack(1);//����nACK
    else
        IIC_Ack(0); //����ACK   
    return receive;
}


