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
* 【使用说明】： 
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
/******************************************************************
程序功能：	该函数通过do...while语句控制蜂鸣器鸣响次数和发光led闪烁次数来说明do...while语句的功能。
****************************************************************************/
#include <reg52.h>

sbit beep=P2^3;
sbit led=P1^0;

void delay(unsigned int i)		//一个延时程序。
{
	unsigned int j;
	char k;
	for(j=i;j>0;j--)
		for(k=200;k>0;k--);
}
void main()
{
	char cishu;
	do				 //当程序执行到此处，先无条件执行循环体一次。
	{
		beep=~beep;	  //每循环一次，beep与led所代表的管脚电平都会取反。
		led=~led;	  //由于次数最大值为9，初值为0，故共取了十次反，故蜂鸣器会响五下，led会闪五次
		cishu++;
		delay(5000);
	}
	while(cishu<10);//执行循环体的条件是cishu<10，若变量不满足该表达式，则跳过。
					//注意while括弧后面有一分号。
	while(1);
}