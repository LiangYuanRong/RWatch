#ifndef _BUTTON_H
#define _BUTTON_H
#include "main.h"


//����IO��ȡ�궨��
#define KEY_ESC 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define KEY_UP 			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)
#define KEY_ENTER 	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define KEY_DOWN		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

//����stateֵ
#define CODE_ENTER  1
#define CODE_ESC    2
#define CODE_UP  		3
#define CODE_DOWN	  4


/*������Ϣ�ṹ��*/
typedef struct
{
	u8 press;						//��������
	u8 record;		 			//�������μ�¼
	uint32_t time;			//��������ʱ��
}Button_t;

typedef struct
{
	Button_t button_esc;
	Button_t button_up;
	Button_t button_enter;
	Button_t button_down;
	unsigned char button_state;
	
}Button_Sign_t;


/*ȫ�ֺ�������*/
void button_init(void);
void Button_Sign_Update(void);
Button_Sign_t* Return_button_add(void);



#endif
