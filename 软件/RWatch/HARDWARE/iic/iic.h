#ifndef __IIC_H
#define __IIC_H

void IIC_GPIO_Init(void);
void IIC_Start(void);								  
void IIC_Stop(void);
int IIC_WaitAck(void);
void IIC_Ack(u8 re);					     
void IIC_WriteBit(u8 Temp);
u8 IIC_Read_Byte(unsigned char ack);

#endif
