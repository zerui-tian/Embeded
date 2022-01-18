#ifndef		__QXMCS51_CONFIG_H
#define		__QXMCS51_CONFIG_H

#include	<reg52.h>
#include	<intrins.h>
#include	"datatype.h"

sbit P_IR_RX	= P3^2;			//��������������
sbit LCD_EN 	= P3^4;			//LCDʹ��IO
sbit LCD_RS	 	= P3^5;			//LCDд���ݻ��������IO
sbit LCD_RW 	= P3^6;			//LCD��д����IO
sbit P_IR_TX	= P2^4;			//������ⷢ������
sbit BEEP		= P2^3;			//������IO�ڶ���
sbit SDA 		= P2^0;		 	//IIC��������
sbit SCL 		= P2^1;			//IICʱ������
sbit LED_WE = P2^6;
sbit LED_DU = P2^7;

/*********************************************************/

//#define MAIN_Fosc		24000000L	//������ʱ��
//#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��


/*********************************************************/
#define AT24C02_ADDR  0xa0	//AT24C02��ַ
#define PCF8591_ADDR  0x90	//PCF8591��ַ

#endif