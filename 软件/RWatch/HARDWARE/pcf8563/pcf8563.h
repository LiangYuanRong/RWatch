#ifndef __PCF_H
#define __PCF_H


#define PCF8563_Write_Addr                            (unsigned char)0xa2  //pcf8563д��ַ
#define PCF8563_Read_Addr                             (unsigned char)0xa3  //pcf8563����ַ

/******************************************************************************
                             �����Ĵ�����ַ�궨��
******************************************************************************/
#define PCF8563_Address_Control_Status_1         (unsigned char)0x00  //����/״̬�Ĵ���1
#define PCF8563_Address_Control_Status_2         (unsigned char)0x01  //����/״̬�Ĵ���2
 
#define PCF8563_Address_CLKOUT                   (unsigned char)0x0d  //CLKOUTƵ�ʼĴ���
#define PCF8563_Address_Timer                    (unsigned char)0x0e  //��ʱ�����ƼĴ���
#define PCF8563_Address_Timer_VAL                (unsigned char)0x0f  //��ʱ���������Ĵ���
 
#define PCF8563_Address_Years                    (unsigned char)0x08  //��
#define PCF8563_Address_Months                   (unsigned char)0x07  //��
#define PCF8563_Address_Days                     (unsigned char)0x05  //��
#define PCF8563_Address_WeekDays                 (unsigned char)0x06  //����
#define PCF8563_Address_Hours                    (unsigned char)0x04  //Сʱ
#define PCF8563_Address_Minutes                  (unsigned char)0x03  //����
#define PCF8563_Address_Seconds                  (unsigned char)0x02  //��
 
#define PCF8563_Alarm_Minutes                    (unsigned char)0x09  //���ӱ���
#define PCF8563_Alarm_Hours                      (unsigned char)0x0a  //Сʱ����
#define PCF8563_Alarm_Days                       (unsigned char)0x0b  //�ձ���
#define PCF8563_Alarm_WeekDays                   (unsigned char)0x0c  //���ڱ���

//=======================ʱ�ӽṹ��================================//
//ʱ��ṹ��
typedef struct 
{
	  //ʱ��
    vu8 hours;
    vu8 minutes;
    vu8 seconds;
  
    //������������
    vu16 years;
    vu8  months;
    vu8  days;
    vu8  weeks;         
}PCF8563_t;     


//=====================ȫ�ֺ�������==========================//
void PCF8563_Init(void);
void PCF8563_Time_Update(void);
void PCF8563_Time_Set(unsigned char hou, unsigned char min, unsigned char sec);
void PCF8563_Date_Set(unsigned char year, unsigned char mon, unsigned char day);
PCF8563_t* Return_pcf8563_add(void);

#endif 
