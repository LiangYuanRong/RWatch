#include "button.h"
#include "timer.h"

static Button_Sign_t button_sign;

/*
*�������ܣ���ʼ������IO��
*/
void button_init(void)
{
		//����˿ڳ�ʼ���ṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;
	 //ʹ��IO��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PA�˿�ʱ��
	 //��ʼ���ṹ���Ա
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//����io
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	  //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
	 //��ʼ��IO
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 			//�����趨������ʼ��GPIOB
		
	 //��ʼ���ṹ��
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
*�������ܣ����°����ṹ��button_sign��Ϣ
*��ע�����°���ʱ��press�ӳ�Ա��1��state��Ա�õ�ǰ��λֵ(��0��1����һֱ�����0)����֮0
*/
static int count_esc,count_up,count_enter,count_down; //������������

void Button_Sign_Update(void)
{
//==========================��������=============================//
//���esc����	
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
//���up����
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
//���mid����
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
//���down����	
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

////==========================״ֵ̬��¼=============================//
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
*�������ܣ����ذ����ṹ���ַ
*/
Button_Sign_t* Return_button_add(void)
{
    return &button_sign;
}


