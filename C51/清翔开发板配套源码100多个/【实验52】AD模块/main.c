/**********************************************************************
功能		：
WEB			：WWW.QXMCU.COM
QQ			：863888999
测试单片机	：STC89C52
外部晶振	：11.0592M
调试环境	：KEIL3
调试硬件平台：使用 清翔电子科技 QX-MCS51单片机开发板测试通过
日期		：
文件名		：

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

sbit LED_WE = P2^6;
sbit LED_DU = P2^7;

void main()
{
	INT8U ADC_Value;
	LED_WE = 0;
	LED_DU = 0;
	I2C_Init();
	LCD1602_Init(); //LCD1602初始化
	LCD1602_Write_String(0, 0, "CH0:");
	LCD1602_Write_String(8, 0, "CH1:");
	LCD1602_Write_String(0, 1, "CH2:");
	LCD1602_Write_String(8, 1, "CH3:");
	while(1)
	{
		I2C_ADC_ReadData(0, &ADC_Value);
		LCD1602_Write_String(4, 0, INT8UtostrHEX(ADC_Value));
		Delay_Ms(100);
		
		I2C_ADC_ReadData(1, &ADC_Value);							  
		LCD1602_Write_String(12, 0, INT8UtostrHEX(ADC_Value));
		Delay_Ms(100);

		I2C_ADC_ReadData(2, &ADC_Value);
		LCD1602_Write_String(4, 1, INT8UtostrHEX(ADC_Value));
		Delay_Ms(100);

		I2C_ADC_ReadData(3, &ADC_Value);
		LCD1602_Write_String(12, 1, INT8UtostrHEX(ADC_Value));
		Delay_Ms(100); 
	}	

}
