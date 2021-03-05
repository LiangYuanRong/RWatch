/*==================================================================
	██████╗ 	██╗    	 	██╗	 █████╗ ████████╗	██████╗██╗  	██╗
	██╔══██╗██║  	█ 	██║	██╔══██╗╚══██╔══╝██╔════╝██║ 	 	██║
	██████╔╝██║	 	█╗ 	██║	███████║ 	 ██║   ██║       ████████║
	██╔══██╗██║	███╗	██║	██╔══██║	 ██║   	██║       ██╔══	██║
	██║ 	 ██║╚███╔███╔╝	██║  	██║	   ██║   	╚██████╗ ██║ 	 	██║
	╚═╝  	 ╚═╝ 	╚══╝╚══╝ 	╚═╝  	╚═╝	   ╚═╝    ╚═════╝ ╚═╝   	╚═╝ 
*====================================================================

模块数据：
		1.3寸OLED显示屏：分辨率128*64
*/
#include "stdio.h"
#include "main.h"
#include "delay.h"
#include "led.h"
#include "button.h"
#include "buzzer.h" 
#include "usart.h"
#include "iic.h"
#include "pcf8563.h"
#include "oled.h"
#include "GUI.h"

u8 week=0;
u8 flag=0;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	delay_init(); 				//延时初始化
	IIC_GPIO_Init();			//IIC初始化
	PCF8563_Init();				//PCF8563时钟模块初始化
	LED_Init(14399,9);		//led亮度0-4000，越高灯越暗，大概到2000灯灭
	button_init();				//初始化按键
	OLED_Init();  				//OLED初始化
	
	while(1)
	{
		PCF8563_Time_Update();	 //时间日期更新
		Button_Sign_Update();    //按键状态更新
		Menu_State_Switch();     //菜单显示更新
	}
}

