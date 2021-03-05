/*==================================================================
	�������������[ 	�����[    	 	�����[	 �����������[ �����������������[	�������������[�����[  	�����[
	�����X�T�T�����[�����U  	�� 	�����U	�����X�T�T�����[�^�T�T�����X�T�T�a�����X�T�T�T�T�a�����U 	 	�����U
	�������������X�a�����U	 	���[ 	�����U	���������������U 	 �����U   �����U       �����������������U
	�����X�T�T�����[�����U	�������[	�����U	�����X�T�T�����U	 �����U   	�����U       �����X�T�T	�����U
	�����U 	 �����U�^�������X�������X�a	�����U  	�����U	   �����U   	�^�������������[ �����U 	 	�����U
	�^�T�a  	 �^�T�a 	�^�T�T�a�^�T�T�a 	�^�T�a  	�^�T�a	   �^�T�a    �^�T�T�T�T�T�a �^�T�a   	�^�T�a 
*====================================================================

ģ�����ݣ�
		1.3��OLED��ʾ�����ֱ���128*64
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
	delay_init(); 				//��ʱ��ʼ��
	IIC_GPIO_Init();			//IIC��ʼ��
	PCF8563_Init();				//PCF8563ʱ��ģ���ʼ��
	LED_Init(14399,9);		//led����0-4000��Խ�ߵ�Խ������ŵ�2000����
	button_init();				//��ʼ������
	OLED_Init();  				//OLED��ʼ��
	
	while(1)
	{
		PCF8563_Time_Update();	 //ʱ�����ڸ���
		Button_Sign_Update();    //����״̬����
		Menu_State_Switch();     //�˵���ʾ����
	}
}

