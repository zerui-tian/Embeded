/*********************************************************************************
* 【编写时间】： 2014年3月5日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ 
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio3	
* 【程序功能】： 			   			            			    
* 【使用说明】： 液晶1脚接1602黑色排母的1脚  如果是无背光的液晶只有14个脚 排母右边空2个位   
                 请把液晶对比度电位器顺时针调制10点钟方向，请勿多调超出极限位置会损坏电位器！

*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
/*

	//-------------------------程序功能-------------------------//
	硬件平台：QX-X3/QX-MCS51	
	 读取清翔电子开发板上4通道AD值，LCD1602以16进制实时显示。

	//---------------------------备注---------------------------//

	//-------------------------使用说明-------------------------//

**********************************************************************/
#include "qxmcs51_config.h"//清翔QX-MCS51单片机开发板配置文件
#include "delay.h"		   
#include "LCD1602.h"
#include "IIC.h"
#include "PCF8591.H"


void main()
{
	INT8U ADC_Value;  
	LCD1602_Init(); //LCD1602初始化
	I2C_Init();
	LCD1602_Write_String(0, 0, "CH0:");
	LCD1602_Write_String(8, 0, "CH1:");
	LCD1602_Write_String(0, 1, "CH2:");
	LCD1602_Write_String(8, 1, "CH3:");
	while(1)
	{
		I2C_ADC_ReadData(1, &ADC_Value);
		LCD1602_Write_String(4, 0, INT8UtostrHEX(ADC_Value));  // 通道0	 V1电位器
		Delay_Ms(100);
		
		I2C_ADC_ReadData(2, &ADC_Value);							  
		LCD1602_Write_String(12, 0, INT8UtostrHEX(ADC_Value)); // 通道1	  V2电位器
		Delay_Ms(100);

		I2C_ADC_ReadData(3, &ADC_Value);
		LCD1602_Write_String(4, 1, INT8UtostrHEX(ADC_Value)); // 通道2	  J6  光敏热敏接口
		Delay_Ms(100);

		I2C_ADC_ReadData(0, &ADC_Value);
		LCD1602_Write_String(12, 1, INT8UtostrHEX(ADC_Value));// 通道3	   J7 接口
		Delay_Ms(100); 
	}	

}
