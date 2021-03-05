#include "led.h"
#include "main.h"

void LED_Init(unsigned int arr, unsigned int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //ʹ�ܼ�ʱ��ʱTIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��	                                                                     	

   //���ø�����Ϊ�����������,���PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;                 //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;               //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);             //CH1Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);                           //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� ʹ�ܺ������TIM_PrescalerConfig()�޸�Ԥ��Ƶֵ
	TIM_Cmd(TIM3, ENABLE);                                        //ʹ��TIM3
	
	TIM3->CCR1=14400; //�ص�
}

