#include "buzzer.h"
#include "main.h"

/*
PWM所用到的定时器的时钟频率分频系数，这个决定定时器的频率，例如系统时钟是72M，36分频，是2M
然后是重装寄存器，这个数值决定了PWM的最终频率与PWM分辨率，前面得到时钟是2M，此寄存器设置为999，那么2M除以1000，PWM频率就是2K，PWM占空值可由1到999变化。分辨率与频率成反比。
*/
void buzzer_init(unsigned int arr, unsigned int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能计时器时TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能	                                                                     	

   //设置该引脚为复用输出功能,输出PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;                 //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;               //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);             //CH1预装载使能	 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);                           //使能TIMx在ARR上的预装载寄存器 使能后可以用TIM_PrescalerConfig()修改预分频值
	TIM_Cmd(TIM3, ENABLE);                                        //使能TIM3
}

void buzzer_work(unsigned int value)
{
	
}
