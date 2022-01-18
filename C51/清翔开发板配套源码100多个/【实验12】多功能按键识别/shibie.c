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
* 【程序功能】： 生活中常见一个按键随着按下次数不同对应不同功能			   			            			    
* 【使用说明】： 本程序以QX-MCS51 学习板中的S3键为例，模拟其具有2种功能，
		   一是点亮一个发光二极管，二是蜂鸣器发声
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include<reg52.h>

#define uchar unsigned char 
uchar num;

sbit beep=P2^3;
sbit led=P1^0;
sbit S3=P3^1;

void delay(uchar i)
{
	uchar j,k;
	for(j=i;j>0;j--)
	for(k=125;k>0;k--);
}


void main()
{
	while(1)
	{
		if(S3==0)
		{
			delay(20);		 //消抖
			if(S3==0)
			{
				while(!S3);	 //等待按键松开
				num++;
			}
			switch(num)
			{
				case 1: led=0;	beep=1; break;
				case 2: beep=0;	led=1;	num=0; break;
			}
		}
	}
}