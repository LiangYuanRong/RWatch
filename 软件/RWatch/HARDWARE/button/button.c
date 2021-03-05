#include "button.h"
#include "timer.h"

static Button_Sign_t button_sign;

/*
*函数功能：初始化按键IO口
*/
void button_init(void)
{
		//定义端口初始化结构体
    GPIO_InitTypeDef  GPIO_InitStructure;
	 //使能IO口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能PA端口时钟
	 //初始化结构体成员
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//按键io
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	  //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
	 //初始化IO
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 			//根据设定参数初始化GPIOB
		
	 //初始化结构体
		button_sign.button_down.press=0;
		button_sign.button_down.record=0;
		button_sign.button_down.time=0;
		button_sign.button_up.press=0;
		button_sign.button_up.record=0;
		button_sign.button_up.time=0;
		button_sign.button_enter.press=0;
		button_sign.button_enter.record=0;
		button_sign.button_enter.time=0;	
		button_sign.button_esc.press=0;
		button_sign.button_esc.record=0;
		button_sign.button_esc.time=0;		
}
 
/*
*函数功能：更新按键结构体button_sign信息
*备注：按下按键时，press子成员置1，state成员置当前键位值(从0到1，若一直按则回0)，反之0
*/
static int count_esc,count_up,count_enter,count_down; //按键消抖计数

void Button_Sign_Update(void)
{
//==========================按键消抖=============================//
//检测esc按键	
	if(!KEY_ESC)
	{
		count_esc++;
		if(count_esc>=10)
		{
			count_esc=10;
			if(button_sign.button_esc.press==0)
			{
				button_sign.button_esc.press=1;
				button_sign.button_state = CODE_ESC;
			}
			else 
				button_sign.button_state = 0;
		}
	}
	else
	{
		count_esc=0;
		button_sign.button_esc.press=0;
	} 
//检测up按键
	if(!KEY_UP)
	{
		count_up++;
		if(count_up>=10)
		{
			count_up=10;
			if(button_sign.button_up.press==0)
			{
					button_sign.button_up.press=1;
					button_sign.button_state = CODE_UP;
			}
			else 
				button_sign.button_state = 0;			
		}
	}
	else
	{
		count_up=0;
		button_sign.button_up.press=0;
	}
//检测mid按键
	if(!KEY_ENTER)
	{
		count_enter++;
		if(count_enter>=10)
		{
			count_enter=10;
			if(button_sign.button_enter.press==0)
			{
					button_sign.button_enter.press=1;
					button_sign.button_state = CODE_ENTER;
			}
			else 
				button_sign.button_state = 0;			
		}
	}
	else
	{
		count_enter=0;
		button_sign.button_enter.press=0;
	}
//检测down按键	
	if(!KEY_DOWN)
	{
		count_down++;
		if(count_down>=10)
		{
			count_down=10;
			if(button_sign.button_down.press==0)
			{
					button_sign.button_down.press=1;
					button_sign.button_state = CODE_DOWN;
			}
			else 
				button_sign.button_state = 0;			
		}
	}
	else
	{
		count_down=0;
		button_sign.button_down.press=0;
	}

////==========================状态值记录=============================//
//	if(button_sign.button_enter.press==1)
//		button_sign.button_state = CODE_ENTER;
//	else if(button_sign.button_esc.press==1)
//		button_sign.button_state = CODE_ESC;
//	else if(button_sign.button_up.press==1)
//		button_sign.button_state = CODE_UP;
//	else if(button_sign.button_down.press==1)
//		button_sign.button_state = CODE_DOWN;
//	else
//		button_sign.button_state = 0;
}
/*
*函数功能：返回按键结构体地址
*/
Button_Sign_t* Return_button_add(void)
{
    return &button_sign;
}


