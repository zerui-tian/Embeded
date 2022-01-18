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
程序功能：	 通过P1口的展示，对比说明逻辑与或非和按位与或非之间的差别。
			 当QX-MCS51实验板上的S2被按下时，执行的为按位与；
			 当QX-MCS51实验板上的S3被按下时，执行的为逻辑与；
			 当QX-MCS51实验板上的S4被按下时，执行的为按位或；
			 当QX-MCS51实验板上的S5被按下时，执行的为逻辑或。
****************************************************************************/
#include <reg51.h>

sbit S2=P3^0;
sbit S3=P3^1;
sbit S4=P3^2;
sbit S5=P3^3;

void main()
{
	char i=1,j=2;
	while(1)
	{
		if(S2==0)
			P1=i&j;	  //按位与，程序会将1与2均转换为二进制，对于每个位进行与运算，如果结果不为0，则为True
				  //故此时，即为0000 0001&0000 0010=0000 0000，结果为0，QX-MCS51学习板第一个led会亮	
		else
			P1=0xff;

		if(S3==0)
			P1=i&&j; //逻辑与，只要i和j均不为0，则结果为真（1），第一个led会灭。
		else
			P1=0xff;

		if(S4==0)
			P1=i|j;  //按位或其中之一为1则为1   第一个和第二个灯灭 	0000 0001&0000 0010=0000 0011
		else
			P1=0xff;

		if(S5==0)
			P1=i||j;
		else
			P1=0xff;
	}
}
