#include "main.h"
#include "pcf8563.h"
#include "iic.h"
 
static PCF8563_t pcf8563;
 
//====================================================文件内部函数=========================================//
//十进制转BCD码
static unsigned char DecToBCD(unsigned char BINValue)
{
	unsigned char bcdhigh = 0;
	
	while (BINValue >= 10)
	{
		bcdhigh++;
		BINValue -= 10;
	}
	
	return ((unsigned char)(bcdhigh << 4) | BINValue);
}

//BCD码转十进制
static unsigned char BCDToDec(unsigned char BCDValue)
{
	unsigned char tmp = 0;
	
	tmp = ((unsigned char)(BCDValue & (unsigned char)0xF0) >> (unsigned char)0x04) * 10;
	return (tmp + (BCDValue & (unsigned char)0x0F));
}


//pcf8563读取单个寄存器数据
static unsigned char PCF8563_Read_Byte(unsigned char REG_ADD)
{
	u8 ReData=0;
	IIC_Start();
	IIC_WriteBit(PCF8563_Write_Addr);	//发送写命令并检查应答位
	while(IIC_WaitAck());
	IIC_WriteBit(REG_ADD);	//确定要操作的寄存器
	IIC_WaitAck();
	IIC_Start();	//重启总线
	IIC_WriteBit(PCF8563_Read_Addr);	//发送读取命令
	IIC_WaitAck();
	ReData = IIC_Read_Byte(0);	//读取数据,加发送非应答
	IIC_Stop();
	return ReData;
}

//pcf8563写入单个寄存器数据
static void PCF8563_Write_Byte(unsigned char REG_ADD, unsigned char dat)
{
	IIC_Start();
	IIC_WriteBit(PCF8563_Write_Addr);//发送写命令并检查应答位
	while(IIC_WaitAck());
	IIC_WriteBit(REG_ADD);
	IIC_WaitAck();
	IIC_WriteBit(dat);	//发送数据	
	IIC_WaitAck();
	IIC_Stop();
} 


//======================================全局函数=========================================================//
//初始化pcf8563配置
void PCF8563_Init(void)
{ 
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, 0x00); //控制状态寄存器1写入0
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_2, 0x00); //控制状态寄存器2写入0
	//定时器默认设置
	PCF8563_Write_Byte(PCF8563_Address_CLKOUT, 0x81);           //CLKOUT频率寄存器，使能CLK输出，1024Hz
	PCF8563_Time_Set(15,49,0);
	PCF8563_Date_Set(1,2,26);
}


//时间更新  
void PCF8563_Time_Update(void)
{
	pcf8563.seconds= PCF8563_Read_Byte(PCF8563_Address_Seconds);
	pcf8563.minutes= PCF8563_Read_Byte(PCF8563_Address_Minutes);
	pcf8563.hours	 = PCF8563_Read_Byte(PCF8563_Address_Hours);
	pcf8563.days   = PCF8563_Read_Byte(PCF8563_Address_Days);	
	pcf8563.weeks	 = PCF8563_Read_Byte(PCF8563_Address_WeekDays);	
	pcf8563.months = PCF8563_Read_Byte(PCF8563_Address_Months);
	pcf8563.years  = PCF8563_Read_Byte(PCF8563_Address_Years);
	
	//将时间从BCD编码变成十进制
	pcf8563.seconds = BCDToDec(pcf8563.seconds);
	pcf8563.minutes = BCDToDec(pcf8563.minutes);
	pcf8563.hours 	= BCDToDec(pcf8563.hours);
	pcf8563.weeks 	= BCDToDec(pcf8563.weeks);	
	pcf8563.days 		= BCDToDec(pcf8563.days);
	pcf8563.months 	= BCDToDec(pcf8563.months);
	pcf8563.years 	= BCDToDec(pcf8563.years);
}


//时间设定
void PCF8563_Time_Set(unsigned char hou, unsigned char min, unsigned char sec)
{
	PCF8563_Write_Byte(PCF8563_Address_Hours, DecToBCD(hou));
	PCF8563_Write_Byte(PCF8563_Address_Minutes, DecToBCD(min));
	PCF8563_Write_Byte(PCF8563_Address_Seconds, DecToBCD(sec));
}
//日期设定
void PCF8563_Date_Set(unsigned char year, unsigned char mon, unsigned char day)
{
	PCF8563_Write_Byte(PCF8563_Address_Years, DecToBCD(year));
	PCF8563_Write_Byte(PCF8563_Address_Months, DecToBCD(mon));
	PCF8563_Write_Byte(PCF8563_Address_Days, DecToBCD(day));	
}

/*
*函数功能：返回时钟结构体地址
*/
PCF8563_t* Return_pcf8563_add(void)
{
    return &pcf8563;
}
