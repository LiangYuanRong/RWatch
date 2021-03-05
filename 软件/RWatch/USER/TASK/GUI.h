#ifndef __GUI_H
#define __GUI_H

//用数组进行菜单切换

#define MAX_MENU_FIR_LEVEL 3    //一级菜单最大数量
#define MAX_MENU_SEC_LEVEL 2	  //二级菜单最大数量
//#define MAX_MENU_THR_LEVEL 2


typedef struct
{
		unsigned char index;        // 1 ,存放当前界面的索引号
		unsigned char down_index; 	// 2 ,按下“down（向下）”键时需要跳转到的索引号 
		unsigned char up_index;     // 3 ,按下“up（向上）”键时需要跳转到的索引号； 
		unsigned char enter_index;  // 4 ,按下“enter（进入）”键时需要跳转的索引号； 
		unsigned char esc_index;    // 5 ,当前索引号下需要执行 的函数的入口地址。 
		void (*operate)(void);  				// 6 ,函数指针
}Menu_index_struct;


///*一级菜单枚举*/
//typedef enum
//{
//    INIT,           //初始化界面
//    MAIN,           //时间主页面
//    REMOTE_MONITOR, //监控器页面
//    GYRO_MONITOR,   //陀螺仪监视页面
//    MENU,           //菜单
//    TWO_LEVEL_MENU, //二级菜单（测试，后面换成具体功能菜单）

//} MENU_STATE;


//typedef struct
//{
//    unsigned int menu_state[4,];        //按键状态（1/0）
//    int menu_flag;             //菜单数
//    int oled_switch;           //切换页面时刷新屏幕标志位

//    int option_flag;           //当前选项数
//    int last_option_flag;      //上次上下翻的数据，选项上下翻直接减

//} OLED_DATA_TYPE_DEF;


//结构体和变量定义
//extern GUI_STATE GUI_state;       //图形用户界面枚举
extern Menu_index_struct menu_index;

void Menu_State_Switch(void);

#endif
