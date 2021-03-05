#ifndef _BUTTON_H
#define _BUTTON_H
#include "main.h"


//按键IO读取宏定义
#define KEY_ESC 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define KEY_UP 			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)
#define KEY_ENTER 	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define KEY_DOWN		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

//按键state值
#define CODE_ENTER  1
#define CODE_ESC    2
#define CODE_UP  		3
#define CODE_DOWN	  4


/*按键信息结构体*/
typedef struct
{
	u8 press;						//按键按下
	u8 record;		 			//按键单次记录
	uint32_t time;			//按键长按时间
}Button_t;

typedef struct
{
	Button_t button_esc;
	Button_t button_up;
	Button_t button_enter;
	Button_t button_down;
	unsigned char button_state;
	
}Button_Sign_t;


/*全局函数声明*/
void button_init(void);
void Button_Sign_Update(void);
Button_Sign_t* Return_button_add(void);



#endif
