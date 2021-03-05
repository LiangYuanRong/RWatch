#include "usart.h"	  


//���ڳ�ʼ��
void Usart1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStrue;   	//�ṹ�嶨��
	USART_InitTypeDef USART_InitStrue; 
  NVIC_InitTypeDef NVIC_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��gpioAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //ʹ�ܴ���1ʱ��

	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStrue);                      //GPIOA10���� 

	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStrue);                      //GPIOA9����
	
	
	USART_InitStrue.USART_BaudRate=115200;                  //������
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //������
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx ;           //����ģʽ
	USART_InitStrue.USART_Parity=USART_Parity_No;           //��żУ��
	USART_InitStrue.USART_StopBits=USART_StopBits_1;       //ֹͣλ
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;        //�ֳ�

//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);        //���������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);      //�����߿��У�������ж�

	USART_Init(USART1,&USART_InitStrue);           //ʹ�ܴ���1

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//��λ��2����
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;          //�����жϽṹ���Ա1
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStrue);

  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);     //����DMA����   

	USART_Cmd(USART1,ENABLE);                //���ڳ�ʼ��
}


////////////////////////////һЩ�ض����뷢���ַ����ĺ���/////////////////////////////////


/* ����һ���ֽ� */
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}

/* ���������ֽڵ����� */
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

/* ����8λ���ݵ����� */
void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for( i=0; i<num; i++ )
  {
		Usart_SendByte(pUSARTx, array[i]);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}
/* �����ַ��� */
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

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(USART1);
}


