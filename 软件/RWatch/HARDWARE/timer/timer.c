#include "timer.h"

static void TIM2_Start(void);   //TIM2��ʼ����
static void TIM2_End(void);     //TIM2��������

/*
*�������ܣ���ʼ��TIM2��ʱ��
*/
void TIM2_init(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 72-1;    //72M/7200 = 10K 36-1;    //APB1 is 36MHz
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 1000-1;   // ms       
    TIM_TimeBaseInit(TIM2,&tim);
}

/*������ʱ��*/
static void TIM2_Start(void)   
{
    TIM_Cmd(TIM2, ENABLE);	 
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
}

/*�رն�ʱ��*/
static void TIM2_End(void)     
{
    TIM_Cmd(TIM2, DISABLE);	 
    TIM_ITConfig(TIM2, TIM_IT_Update,DISABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
}

/*TIM2�жϺ���*/
int TIM2_count=0;
void TIM2_IRQHandler(void)
{

  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)//�������ź�
  {
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//��������־
		TIM2_count++;
	}
}

/*
*�������ܣ�����10ms
*/
int get_10ms(void)
{
	if(TIM2_count==0)
		TIM2_Start();
	if(TIM2_count>=10)
	{
		TIM2_count=0;
		TIM2_End();
		return 1;
	}
	return 0;
}
