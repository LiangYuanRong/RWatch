#include "button.h"
#include "GUI.h"
#include "oled.h"
#include "oledfont.h"
#include "pcf8563.h"
#include "delay.h"

//======内部函数声明
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

	
//=======结构体声明
Button_Sign_t *button_sign;        //按键状态结构体指针
PCF8563_t *pcf8563;								 //时钟结构体指针

	
/********************菜单索引表（目前）*************************

	@主界面：显示时间和日期，以及手表状态（按确认键进入主菜单）
	@主菜单：
		1.手电筒功能
			1.1 亮度设置
		2.时间设置功能
			2.1 时间设置
			2.2 日期设置
		3.水平仪功能
			3.1 俯仰轴调平
			3.2 横滚轴调平
			3.3	偏航轴角度测量
			
**************************************************************/
Menu_index_struct menu_tab[12]=//菜单（1当前索引号，2下翻索引，3上翻索引，4进入索引，5返回索引，6功能函数指针）
{
	//now,dn,up,en,es,op
		{0 ,0	,0 ,1 ,0 ,(*Home_Screen)},  			//主界面
		{1 ,2	,3 ,4 ,0 ,(*Flashlignt_Screen)},	//手电筒		
		{2 ,3 ,1 ,5 ,0 ,(*Setting_Screen)},			//设置
		{3 ,1 ,2 ,7 ,0 ,(*Level_Screen)},				//水平仪
		{4 ,4 ,4 ,4 ,1 ,(*Flashlignt_set)},			//手电筒设置
		{5 ,5 ,5 ,5 ,2 ,(*Setting_Time)},			  //时间设置	
//		{6 ,5 ,5 ,6 ,2 ,(*Setting_Date)},				//日期设置
		{7 ,8 ,9 ,7 ,3 ,(*Level_Pitch)},				//俯仰角
		{8 ,9 ,7 ,8 ,3 ,(*Level_Raw)},					//横滚角
		{9 ,7 ,8 ,9 ,3 ,(*Level_Yaw)},					//偏航角
		
};
/**************************************************************/

//=========================菜单界面切换======================
unsigned char Key_fun=0;
unsigned char Last_Key_fun=0;

static void (*Key_fun_Pt)(void);

void Menu_State_Switch(void)
{
	button_sign = Return_button_add(); 	//获取按键结构体指针

	switch(button_sign->button_state)		//菜单索引登记
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
	
	if(Last_Key_fun != Key_fun)//切屏刷新
	{
		OLED_Clear();
	}
	
	Key_fun_Pt = menu_tab[Key_fun].operate; //更新执行函数指针
	(*Key_fun_Pt)();												//执行当前功能函数
	
	Last_Key_fun = Key_fun;									//记录上次界面状态							
}		


//=========================主界面===========================
static unsigned char sec_h,sec_l,min_h,min_l,hou_h,hou_l; 
static unsigned char last_hou_h=0;
static unsigned char screen_lock=0;//屏幕锁定

static void Home_Screen(void)
{
	//初次进入复位按键数据，防止确认键重复
	if(Last_Key_fun != Key_fun)
	{
		button_sign->button_state=0;
	}
	//获取时钟结构体指针
	pcf8563 = Return_pcf8563_add();
	
	//锁屏判定
	if(button_sign->button_state==CODE_ESC && screen_lock==0)//锁屏
	{
		screen_lock=1;
		OLED_Display_Off();
	}
	else if(button_sign->button_state==CODE_ESC && screen_lock==1)
	{
		if(screen_lock==1)OLED_Display_On();  //只开一次
		screen_lock=0;		
	}
	
	if(screen_lock==0)//开屏
	{
		//时间数字分解
		sec_h = pcf8563->seconds/10;
		sec_l = pcf8563->seconds%10;
		min_h = pcf8563->minutes/10;
		min_l = pcf8563->minutes%10;
		hou_h = pcf8563->hours/10;
		hou_l = pcf8563->hours%10;	
		
		if(hou_h>2)hou_h=last_hou_h;//目前还不知道什么bug，时会突然跳到40
		last_hou_h=hou_h;
		
		//时间oled显示
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



//=========================手电筒===========================
static int flash_state=0;//手电筒开关

static void Flashlignt_Screen(void)
{
	OLED_DrawBMP(32,0,96,8,BMP_flashlight); 	
}
static void Flashlignt_set(void)
{
	//初次进入复位按键数据，防止确认键重复
	if(Last_Key_fun != Key_fun)
	{
		button_sign->button_state=0;
	}
	//确认键开关操作灯
	if(button_sign->button_state==CODE_ENTER && flash_state==0)
	{	
		flash_state=1;
		TIM3->CCR1=700;//刚进来初始亮度
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
	
	//修改亮度
//	if(flash_state==1)//正在亮灯
//	{
//		if(button_sign->button_state==CODE_UP)//加亮度
//		{
//			TIM3->CCR1-=200;
//			if(TIM3->CCR1<100)TIM3->CCR1=100;
//		}
//		if(button_sign->button_state==CODE_DOWN)//减亮度
//		{
//			TIM3->CCR1+=200;
//			if(TIM3->CCR1>1500)TIM3->CCR1=1500;			
//		}
//	}
}


//=========================设置===========================
 int set_timeordate=1;//01
 int into_set=0;//1进入设置，防止上下键再切换箭头
 int setting_status=0;//时分/月日 1 2
 int setting_num[4]={0};//设置的时间（0：时，1：分，2：月，3：日）
static int setmin_h=0,setmin_l=0,sethou_h=0,sethou_l=0,
					 setmon_h=0,setmon_l=0,setday_h=0,setday_l=0;

static void Setting_Screen(void)
{
	OLED_DrawBMP(32,0,96,8,BMP_Setting); 	
} 

static void Setting_Time(void)
{
	//初次进入初始化数据
	if(Last_Key_fun != Key_fun)
	{
		button_sign->button_state=0;
		setting_num[0]=pcf8563->hours;
		setting_num[1]=pcf8563->minutes;
		setting_num[2]=pcf8563->months;
		setting_num[3]=pcf8563->days;
	}
	//固定显示
	{
		OLED_ShowString(20,1,(u8*)"Time");
		OLED_ShowString(20,4,(u8*)"Date");		
		OLED_ShowString(75,1,(u8*)":");
		OLED_ShowString(75,4,(u8*)"-");
	}
	//确认进入修改项,确认进入后，只能设置完后才能回到选择状态继续
	if(button_sign->button_state==CODE_ENTER && into_set==0)
	{into_set=1;}
	
	//修改指示光标
	OLED_ShowString(8,set_timeordate,(u8*)">");//指示光标

	if(into_set==0)//未确认进入修改项，上下键选择修改时间或日期
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
	else if(into_set==1)//确认进入修改项
	{
		//上键
		if(button_sign->button_state==CODE_UP)
		{
				if(set_timeordate==1)//改时间
				{
						if(setting_status==1)//改时
						{
							setting_num[0]++;
							setting_num[0]=Value_circulation(setting_num[0],23,0);//限定范围0-23小时
						}
						else if(setting_status==2)//改分
						{
							setting_num[1]++;
							setting_num[1]=Value_circulation(setting_num[1],59,0);//限定范围0-59分钟					
						}
				}
				else if(set_timeordate==4)//改日期
				{
						if(setting_status==1)//改月
						{
							setting_num[2]++;
							setting_num[2]=Value_circulation(setting_num[2],12,1);//限定范围1-12月
						}
						else if(setting_status==2)//改日
						{
							setting_num[3]++;
							setting_num[3]=Value_circulation(setting_num[3],31,1);//限定范围1-31日（大小月自己算）					
						}				
				}
		}
		//下键
		else if(button_sign->button_state==CODE_DOWN)
		{
				if(set_timeordate==1)//改时间
				{
						if(setting_status==1)//改时
						{
							setting_num[0]--;
							setting_num[0]=Value_circulation(setting_num[0],23,0);//限定范围0-23小时
						}
						else if(setting_status==2)//改分
						{
							setting_num[1]--;
							setting_num[1]=Value_circulation(setting_num[1],59,0);//限定范围0-59分钟					
						}
				}
				else if(set_timeordate==4)//改日期
				{
						if(setting_status==1)//改月
						{
							setting_num[2]--;
							setting_num[2]=Value_circulation(setting_num[2],12,1);//限定范围1-12月
						}
						else if(setting_status==2)//改日
						{
							setting_num[3]--;
							setting_num[3]=Value_circulation(setting_num[3],31,1);//限定范围1-31日（大小月自己算）					
						}				
				}	
		}
		//确认键
		else if(button_sign->button_state==CODE_ENTER)
		{
			OLED_Clear();
			button_sign->button_state=0;
			setting_status++;
			if(setting_status>2)
			{
				setting_status=0;
				//将修改值写入芯片PCF8563_Time_Set(unsigned char hou, unsigned char min, unsigned char sec)
				PCF8563_Time_Set(setting_num[0],setting_num[1],0);
				PCF8563_Date_Set(1,setting_num[2],setting_num[3]);
				
				delay_ms(100);							//加这个防止一按下按键就又进入时间设置
				button_sign->button_state=0;//复位按键信息
				into_set=0;									//退出当前选项设置
			}
		}
		
	}
	
	//绘制目标线
	if(into_set==1)
		OLED_ShowString((59+(setting_status-1)*21),(set_timeordate+1),(u8*)"--");
	
  //
	//setmin_h=0,setmin_l=0,sethou_h=0,sethou_l=0;setmon_h=0,setmon_l=0,setyea_h=0,setyea_l=0;    setting_num
	//数字分解
	sethou_h=setting_num[0]/10;  sethou_l=setting_num[0]%10;
	setmin_h=setting_num[1]/10;  setmin_l=setting_num[1]%10;
	setmon_h=setting_num[2]/10;  setmon_l=setting_num[2]%10;
	setday_h=setting_num[3]/10;	 setday_l=setting_num[3]%10;
	
	//显示数字
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

//=========================水平仪===========================
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







//===============================================数学函数===================================================
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
