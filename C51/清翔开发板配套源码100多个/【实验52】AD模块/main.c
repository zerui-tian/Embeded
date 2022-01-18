/**********************************************************************
����		��
WEB			��WWW.QXMCU.COM
QQ			��863888999
���Ե�Ƭ��	��STC89C52
�ⲿ����	��11.0592M
���Ի���	��KEIL3
����Ӳ��ƽ̨��ʹ�� ������ӿƼ� QX-MCS51��Ƭ�����������ͨ��
����		��
�ļ���		��

	//-------------------------������-------------------------//
	Ӳ��ƽ̨��QX-X3/QX-MCS51	
	 ��ȡ������ӿ�������4ͨ��ADֵ��LCD1602��16����ʵʱ��ʾ��

	//---------------------------��ע---------------------------//

	//-------------------------ʹ��˵��-------------------------//

**********************************************************************/
#include "qxmcs51_config.h"//����QX-MCS51��Ƭ�������������ļ�
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
	LCD1602_Init(); //LCD1602��ʼ��
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
