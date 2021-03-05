#include "usart.h"	  


//串口初始化
void Usart1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStrue;   	//结构体定义
	USART_InitTypeDef USART_InitStrue; 
  NVIC_InitTypeDef NVIC_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能gpioA时钟
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //使能串口1时钟

	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStrue);                      //GPIOA10配置 

	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStrue);                      //GPIOA9配置
	
	
	USART_InitStrue.USART_BaudRate=115200;                  //波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //数据流
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx ;           //设置模式
	USART_InitStrue.USART_Parity=USART_Parity_No;           //奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1;       //停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;        //字长

//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);        //开启接受中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);      //若总线空闲，则产生中断

	USART_Init(USART1,&USART_InitStrue);           //使能串口1

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//两位是2个数
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;          //设置中断结构体成员1
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStrue);

  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);     //开启DMA接收   

	USART_Cmd(USART1,ENABLE);                //串口初始化
}


////////////////////////////一些重定义与发送字符串的函数/////////////////////////////////


/* 发送一个字节 */
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}

/* 发送两个字节的数据 */
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t temp_h,temp_l;
	
	temp_h = (data&0xff00) >> 8 ;
	temp_l = data&0xff;
	
	USART_SendData(pUSARTx, temp_h);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	
	USART_SendData(pUSARTx, temp_l);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}

/* 发送8位数据的数组 */
void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for( i=0; i<num; i++ )
  {
		Usart_SendByte(pUSARTx, array[i]);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}
/* 发送字符串 */
void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str)
{
	uint8_t i=0;
	do
  {
		Usart_SendByte(pUSARTx, *(str+i));
		i++;
	}while(*(str+i) != '\0');
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART1, (uint8_t) ch);
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(USART1);
}


