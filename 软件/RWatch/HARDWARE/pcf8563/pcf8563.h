#ifndef __PCF_H
#define __PCF_H


#define PCF8563_Write_Addr                            (unsigned char)0xa2  //pcf8563写地址
#define PCF8563_Read_Addr                             (unsigned char)0xa3  //pcf8563读地址

/******************************************************************************
                             参数寄存器地址宏定义
******************************************************************************/
#define PCF8563_Address_Control_Status_1         (unsigned char)0x00  //控制/状态寄存器1
#define PCF8563_Address_Control_Status_2         (unsigned char)0x01  //控制/状态寄存器2
 
#define PCF8563_Address_CLKOUT                   (unsigned char)0x0d  //CLKOUT频率寄存器
#define PCF8563_Address_Timer                    (unsigned char)0x0e  //定时器控制寄存器
#define PCF8563_Address_Timer_VAL                (unsigned char)0x0f  //定时器倒计数寄存器
 
#define PCF8563_Address_Years                    (unsigned char)0x08  //年
#define PCF8563_Address_Months                   (unsigned char)0x07  //月
#define PCF8563_Address_Days                     (unsigned char)0x05  //日
#define PCF8563_Address_WeekDays                 (unsigned char)0x06  //星期
#define PCF8563_Address_Hours                    (unsigned char)0x04  //小时
#define PCF8563_Address_Minutes                  (unsigned char)0x03  //分钟
#define PCF8563_Address_Seconds                  (unsigned char)0x02  //秒
 
#define PCF8563_Alarm_Minutes                    (unsigned char)0x09  //分钟报警
#define PCF8563_Alarm_Hours                      (unsigned char)0x0a  //小时报警
#define PCF8563_Alarm_Days                       (unsigned char)0x0b  //日报警
#define PCF8563_Alarm_WeekDays                   (unsigned char)0x0c  //星期报警

//=======================时钟结构体================================//
//时间结构体
typedef struct 
{
	  //时间
    vu8 hours;
    vu8 minutes;
    vu8 seconds;
  
    //公历日月年周
    vu16 years;
    vu8  months;
    vu8  days;
    vu8  weeks;         
}PCF8563_t;     


//=====================全局函数声明==========================//
void PCF8563_Init(void);
void PCF8563_Time_Update(void);
void PCF8563_Time_Set(unsigned char hou, unsigned char min, unsigned char sec);
void PCF8563_Date_Set(unsigned char year, unsigned char mon, unsigned char day);
PCF8563_t* Return_pcf8563_add(void);

#endif 
