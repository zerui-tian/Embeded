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
* 【程序功能】： 模拟简易门铃；且有关闭门铃功能。			   			            			    
* 【使用说明】： 当按下QX-MCS51上的S2键后，蜂鸣器会发出十次短暂的“嘟嘟”声作为呼叫。
		  S3用于关闭或者打开门铃。
		  当门铃处于关闭时，按下S2不会有呼叫效果。
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include<reg52.h>

#define uchar unsigned char 
uchar k,ok=1;//默认开机后，门铃开。
unsigned int j;
sbit s2=P3^0;
sbit s3=P3^1;
sbit beep=P2^3;

void delay(unsigned int i)
{
	for(j=i;j>0;j--)
	for(k=125;k>0;k--);
}

void main()
{
	while(1)
	{
		if(ok==1)
		{
			if(s3==0)
			{
				while(s3==0);
				ok=0;
			 }
		}
		else
		{
			if(s3==0)
			{
				while(s3==0);
				ok=1;
			 }
		}
		if((s2==0)&(ok==1))
		{	uchar i=5;
			for(i=5;i>0;i--)
			{
				beep=0;
				delay(800);
				beep=1;
				delay(800);
				beep=0;
				delay(1200);
				beep=1;
				delay(1300);
			}
		}
	}
}


