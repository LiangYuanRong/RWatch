#include "button.h"
#include "GUI.h"
#include "oled.h"
#include "oledfont.h"
#include "pcf8563.h"
#include "delay.h"

//======�ڲ���������
static void Home_Screen(void);
static void Flashlignt_Screen(void);
	static void Flashlignt_set(void);
static void Setting_Screen(void);
	static void Setting_Time(void);
//	static void Setting_Date(void);
static void Level_Screen(void);
	static void Level_Pitch(void);
	static void Level_Raw(void);
	static void Level_Yaw(void);
	
static int Value_Limit(int val,int max,int min);
static int Value_circulation(int val,int max,int min);

	
//=======�ṹ������
Button_Sign_t *button_sign;        //����״̬�ṹ��ָ��
PCF8563_t *pcf8563;								 //ʱ�ӽṹ��ָ��

	
/********************�˵�������Ŀǰ��*************************

	@�����棺��ʾʱ������ڣ��Լ��ֱ�״̬����ȷ�ϼ��������˵���
	@���˵���
		1.�ֵ�Ͳ����
			1.1 ��������
		2.ʱ�����ù���
			2.1 ʱ������
			2.2 ��������
		3.ˮƽ�ǹ���
			3.1 �������ƽ
			3.2 ������ƽ
			3.3	ƫ����ǶȲ���
			
**************************************************************/
Menu_index_struct menu_tab[12]=//�˵���1��ǰ�����ţ�2�·�������3�Ϸ�������4����������5����������6���ܺ���ָ�룩
{
	//now,dn,up,en,es,op
		{0 ,0	,0 ,1 ,0 ,(*Home_Screen)},  			//������
		{1 ,2	,3 ,4 ,0 ,(*Flashlignt_Screen)},	//�ֵ�Ͳ		
		{2 ,3 ,1 ,5 ,0 ,(*Setting_Screen)},			//����
		{3 ,1 ,2 ,7 ,0 ,(*Level_Screen)},				//ˮƽ��
		{4 ,4 ,4 ,4 ,1 ,(*Flashlignt_set)},			//�ֵ�Ͳ����
		{5 ,5 ,5 ,5 ,2 ,(*Setting_Time)},			  //ʱ������	
//		{6 ,5 ,5 ,6 ,2 ,(*Setting_Date)},				//��������
		{7 ,8 ,9 ,7 ,3 ,(*Level_Pitch)},				//������
		{8 ,9 ,7 ,8 ,3 ,(*Level_Raw)},					//�����
		{9 ,7 ,8 ,9 ,3 ,(*Level_Yaw)},					//ƫ����
		
};
/**************************************************************/

//=========================�˵������л�======================
unsigned char Key_fun=0;
unsigned char Last_Key_fun=0;

static void (*Key_fun_Pt)(void);

void Menu_State_Switch(void)
{
	button_sign = Return_button_add(); 	//��ȡ�����ṹ��ָ��

	switch(button_sign->button_state)		//�˵������Ǽ�
	{
		case CODE_ENTER:	Key_fun = menu_tab[Key_fun].enter_index;
			break;
		case CODE_ESC:		Key_fun = menu_tab[Key_fun].esc_index;
			break;
		case CODE_UP:			Key_fun = menu_tab[Key_fun].up_index;
			break;
		case CODE_DOWN:		Key_fun = menu_tab[Key_fun].down_index;
			break;
		default: 
			break;
	}
	
	if(Last_Key_fun != Key_fun)//����ˢ��
	{
		OLED_Clear();
	}
	
	Key_fun_Pt = menu_tab[Key_fun].operate; //����ִ�к���ָ��
	(*Key_fun_Pt)();												//ִ�е�ǰ���ܺ���
	
	Last_Key_fun = Key_fun;									//��¼�ϴν���״̬							
}		


//=========================������===========================
static unsigned char sec_h,sec_l,min_h,min_l,hou_h,hou_l; 
static unsigned char last_hou_h=0;
static unsigned char screen_lock=0;//��Ļ����

static void Home_Screen(void)
{
	//���ν��븴λ�������ݣ���ֹȷ�ϼ��ظ�
	if(Last_Key_fun != Key_fun)
	{
		button_sign->button_state=0;
	}
	//��ȡʱ�ӽṹ��ָ��
	pcf8563 = Return_pcf8563_add();
	
	//�����ж�
	if(button_sign->button_state==CODE_ESC && screen_lock==0)//����
	{
		screen_lock=1;
		OLED_Display_Off();
	}
	else if(button_sign->button_state==CODE_ESC && screen_lock==1)
	{
		if(screen_lock==1)OLED_Display_On();  //ֻ��һ��
		screen_lock=0;		
	}
	
	if(screen_lock==0)//����
	{
		//ʱ�����ַֽ�
		sec_h = pcf8563->seconds/10;
		sec_l = pcf8563->seconds%10;
		min_h = pcf8563->minutes/10;
		min_l = pcf8563->minutes%10;
		hou_h = pcf8563->hours/10;
		hou_l = pcf8563->hours%10;	
		
		if(hou_h>2)hou_h=last_hou_h;//Ŀǰ����֪��ʲôbug��ʱ��ͻȻ����40
		last_hou_h=hou_h;
		
		//ʱ��oled��ʾ
		OLED_DrawBMP(0,2,18,8,digital_num_18x41[hou_h]); 	
		OLED_DrawBMP(19,2,37,8,digital_num_18x41[hou_l]); 	
		OLED_DrawBMP(38,3,44,6,colon_6x24); 	
		OLED_DrawBMP(45,2,63,8,digital_num_18x41[min_h]); 
		OLED_DrawBMP(64,2,82,8,digital_num_18x41[min_l]); 	
		OLED_DrawBMP(83,3,89,6,colon_6x24); 	
		OLED_DrawBMP(90,2,108,8,digital_num_18x41[sec_h]); 
		OLED_DrawBMP(109,2,127,8,digital_num_18x41[sec_l]); 
	}
}



//=========================�ֵ�Ͳ===========================
static int flash_state=0;//�ֵ�Ͳ����

static void Flashlignt_Screen(void)
{
	OLED_DrawBMP(32,0,96,8,BMP_flashlight); 	
}
static void Flashlignt_set(void)
{
	//���ν��븴λ�������ݣ���ֹȷ�ϼ��ظ�
	if(Last_Key_fun != Key_fun)
	{
		button_sign->button_state=0;
	}
	//ȷ�ϼ����ز�����
	if(button_sign->button_state==CODE_ENTER && flash_state==0)
	{	
		flash_state=1;
		TIM3->CCR1=700;//�ս�����ʼ����
		OLED_ShowString(30,4,(u8*)"OPEN ");	

	}
	else if(button_sign->button_state==CODE_ENTER && flash_state==1)
	{
		flash_state=0;
		OLED_ShowString(30,4,(u8*)"ClOSE");	
		TIM3->CCR1=14400;		
	}
	else
	{
		OLED_ShowString(19,2,(u8*)"Flash Lignt");			
	}
	
	//�޸�����
//	if(flash_state==1)//��������
//	{
//		if(button_sign->button_state==CODE_UP)//������
//		{
//			TIM3->CCR1-=200;
//			if(TIM3->CCR1<100)TIM3->CCR1=100;
//		}
//		if(button_sign->button_state==CODE_DOWN)//������
//		{
//			TIM3->CCR1+=200;
//			if(TIM3->CCR1>1500)TIM3->CCR1=1500;			
//		}
//	}
}


//=========================����===========================
 int set_timeordate=1;//01
 int into_set=0;//1�������ã���ֹ���¼����л���ͷ
 int setting_status=0;//ʱ��/���� 1 2
 int setting_num[4]={0};//���õ�ʱ�䣨0��ʱ��1���֣�2���£�3���գ�
static int setmin_h=0,setmin_l=0,sethou_h=0,sethou_l=0,
					 setmon_h=0,setmon_l=0,setday_h=0,setday_l=0;

static void Setting_Screen(void)
{
	OLED_DrawBMP(32,0,96,8,BMP_Setting); 	
} 

static void Setting_Time(void)
{
	//���ν����ʼ������
	if(Last_Key_fun != Key_fun)
	{
		button_sign->button_state=0;
		setting_num[0]=pcf8563->hours;
		setting_num[1]=pcf8563->minutes;
		setting_num[2]=pcf8563->months;
		setting_num[3]=pcf8563->days;
	}
	//�̶���ʾ
	{
		OLED_ShowString(20,1,(u8*)"Time");
		OLED_ShowString(20,4,(u8*)"Date");		
		OLED_ShowString(75,1,(u8*)":");
		OLED_ShowString(75,4,(u8*)"-");
	}
	//ȷ�Ͻ����޸���,ȷ�Ͻ����ֻ�����������ܻص�ѡ��״̬����
	if(button_sign->button_state==CODE_ENTER && into_set==0)
	{into_set=1;}
	
	//�޸�ָʾ���
	OLED_ShowString(8,set_timeordate,(u8*)">");//ָʾ���

	if(into_set==0)//δȷ�Ͻ����޸�����¼�ѡ���޸�ʱ�������
	{
		if((button_sign->button_state==CODE_UP||button_sign->button_state==CODE_DOWN) && set_timeordate==1)
		{
			set_timeordate=4;	
			OLED_Clear();
		}
		else if((button_sign->button_state==CODE_UP||button_sign->button_state==CODE_DOWN) && set_timeordate==4)
		{
			set_timeordate=1; 
			OLED_Clear();
		}
	}
	else if(into_set==1)//ȷ�Ͻ����޸���
	{
		//�ϼ�
		if(button_sign->button_state==CODE_UP)
		{
				if(set_timeordate==1)//��ʱ��
				{
						if(setting_status==1)//��ʱ
						{
							setting_num[0]++;
							setting_num[0]=Value_circulation(setting_num[0],23,0);//�޶���Χ0-23Сʱ
						}
						else if(setting_status==2)//�ķ�
						{
							setting_num[1]++;
							setting_num[1]=Value_circulation(setting_num[1],59,0);//�޶���Χ0-59����					
						}
				}
				else if(set_timeordate==4)//������
				{
						if(setting_status==1)//����
						{
							setting_num[2]++;
							setting_num[2]=Value_circulation(setting_num[2],12,1);//�޶���Χ1-12��
						}
						else if(setting_status==2)//����
						{
							setting_num[3]++;
							setting_num[3]=Value_circulation(setting_num[3],31,1);//�޶���Χ1-31�գ���С���Լ��㣩					
						}				
				}
		}
		//�¼�
		else if(button_sign->button_state==CODE_DOWN)
		{
				if(set_timeordate==1)//��ʱ��
				{
						if(setting_status==1)//��ʱ
						{
							setting_num[0]--;
							setting_num[0]=Value_circulation(setting_num[0],23,0);//�޶���Χ0-23Сʱ
						}
						else if(setting_status==2)//�ķ�
						{
							setting_num[1]--;
							setting_num[1]=Value_circulation(setting_num[1],59,0);//�޶���Χ0-59����					
						}
				}
				else if(set_timeordate==4)//������
				{
						if(setting_status==1)//����
						{
							setting_num[2]--;
							setting_num[2]=Value_circulation(setting_num[2],12,1);//�޶���Χ1-12��
						}
						else if(setting_status==2)//����
						{
							setting_num[3]--;
							setting_num[3]=Value_circulation(setting_num[3],31,1);//�޶���Χ1-31�գ���С���Լ��㣩					
						}				
				}	
		}
		//ȷ�ϼ�
		else if(button_sign->button_state==CODE_ENTER)
		{
			OLED_Clear();
			button_sign->button_state=0;
			setting_status++;
			if(setting_status>2)
			{
				setting_status=0;
				//���޸�ֵд��оƬPCF8563_Time_Set(unsigned char hou, unsigned char min, unsigned char sec)
				PCF8563_Time_Set(setting_num[0],setting_num[1],0);
				PCF8563_Date_Set(1,setting_num[2],setting_num[3]);
				
				delay_ms(100);							//�������ֹһ���°������ֽ���ʱ������
				button_sign->button_state=0;//��λ������Ϣ
				into_set=0;									//�˳���ǰѡ������
			}
		}
		
	}
	
	//����Ŀ����
	if(into_set==1)
		OLED_ShowString((59+(setting_status-1)*21),(set_timeordate+1),(u8*)"--");
	
  //
	//setmin_h=0,setmin_l=0,sethou_h=0,sethou_l=0;setmon_h=0,setmon_l=0,setyea_h=0,setyea_l=0;    setting_num
	//���ַֽ�
	sethou_h=setting_num[0]/10;  sethou_l=setting_num[0]%10;
	setmin_h=setting_num[1]/10;  setmin_l=setting_num[1]%10;
	setmon_h=setting_num[2]/10;  setmon_l=setting_num[2]%10;
	setday_h=setting_num[3]/10;	 setday_l=setting_num[3]%10;
	
	//��ʾ����
	OLED_ShowNum(59,1,sethou_h,1,8);   OLED_ShowNum(66,1,sethou_l,1,8);
	OLED_ShowNum(82,1,setmin_h,1,8);   OLED_ShowNum(90,1,setmin_l,1,8);
	OLED_ShowNum(59,4,setmon_h,1,8);   OLED_ShowNum(66,4,setmon_l,1,8);
	OLED_ShowNum(82,4,setday_h,1,8);   OLED_ShowNum(90,4,setday_l,1,8);
	
//	OLED_ShowString(59,1,(u8*)"00");
//	OLED_ShowString(82,1,(u8*)"00");
//	
//	OLED_ShowString(59,4,(u8*)"00");
//	OLED_ShowString(82,4,(u8*)"00");	
  
	
	
}
//static void Setting_Date(void)
//{
//	OLED_ShowString(19,2,(u8*)"set date");	
//	
//}

//=========================ˮƽ��===========================
static void Level_Screen(void)
{
	OLED_DrawBMP(32,0,96,8,BMP_Level); 	
	
}
static void Level_Pitch(void)
{
	OLED_ShowString(19,2,(u8*)"pitch angle");	
	
}
static void Level_Raw(void)
{
	OLED_ShowString(19,2,(u8*)"raw angle");	
	
}
static void Level_Yaw(void)
{
	OLED_ShowString(19,2,(u8*)"yaw angle");	
	
}







//===============================================��ѧ����===================================================
static int Value_Limit(int val,int max,int min)
{
	if(val<=min)val=min;
	else if(val>=max)val=max;
	return val;
}

static int Value_circulation(int val,int max,int min)
{
	if(val<min)val=max;
	else if(val>max)val=min;
	return val;
}
