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
* 【程序功能】： NRF结合1602单机显示
			   			            			    
* 【使用说明】： 液晶1脚接1602黑色排母的1脚  如果是无背光的液晶只有14个脚 排母右边空2个位   
                 请把液晶对比度电位器顺时针调制10点钟方向，请勿多调超出极限位置会损坏电位器！

				 NRF24L01+模块接在开发板无线模块液晶 天线指向开发板键盘一定不能接错，接错则
				 损坏模块

				 1602液晶显示模块地址，状态寄存器显示14 配置寄存器显示15 表示成功
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
#include "reg52.h"	/*包含STC单片机头文件*/
#include "QX51/qx51.h" /*包含qx51开发板头文件*/
#include "DELAY/delay.h"/*包含delay延时函数文件*/
#include "DS1602/ds1602.h" //包含1602驱动
#include "NRF/NRFreceive.h"//包涵NRF驱动

uchar buf0[5]={0,0,0,0,0};	//用于储存NRF接收地址的数组

void main()
{
 	qx51_init();//QX开发板初始化
	init_1602();//1602初始化
	nrf24l01_init(); //NRF初始化
	setRX_Mode(); //设置为接收模式

	SPI_Read_Buf(READ_REG+RX_ADDR_P0,buf0,TX_PLOAD_WIDTH); //读NRF的接收地址

	delay_ms(100);

	lcd_dis(0,0,"RX_ADDR_P0:");	//提示显示NRF接收端的地址

   /************************************************/
   //执行显示NRF的接收地址到1602上
	lcd_dis(1,0,uchartostr(buf0[0]));
	lcd_dis(1,2," ");

	lcd_dis(1,3,uchartostr(buf0[1]));
	lcd_dis(1,5," ");

	lcd_dis(1,6,uchartostr(buf0[2]));
	lcd_dis(1,8," ");

	lcd_dis(1,9,uchartostr(buf0[3]));
	lcd_dis(1,11," ");

	lcd_dis(1,12,uchartostr(buf0[4]));

	delay_ms(5000);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay_ms(5);
	/************************************************/

	/************************************************/
	//读NRF STATUS的值  STATUS上电复位后的值为14
	lcd_dis( 0,0,"NRF-STATUS:");
	lcd_dis( 0,12,uchartostr( SPI_Read(STATUS) ) );	   
	
	//读NRF CONFIG的值  CONFIG在接收模式下为15 如何把 setRX_Mode() 注解掉就为上电复位值8
	lcd_dis( 1,0,"NRF-CONFIG:");
	lcd_dis( 1,12,uchartostr( SPI_Read(CONFIG) ) );
	/************************************************/


	while(1);
}