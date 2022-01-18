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
* 【使用说明】：按下QX-MCS51 实验板上的S2按键时（一直按着），会发出频率为1000HZ和2000HZ交替
			的信号，通过蜂鸣器发出类似于救护车发出的报警声。 
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

sbit beep=P2^3;
sbit kaiguan=P3^0;

void delay()		   //大概0.5ms左右的延时
{
	uchar a;
	for(a=450;a>0;a--)
	{
		_nop_();
	}
}
void main()
{
	while(1)
	{
		if(!kaiguan)
		{
			uint m;
			for(m=800;m>0;m--)	  //持续时间0.5ms*800
			{
				beep=~beep;		
				delay();		 //2000HZ的信号。
			}
			for(m=500;m>0;m--)	 //持续时间0.5ms*2*500
			{
				beep=~beep;
				delay();
				delay();		//1000HZ的信号。
			}
		}
	}
}