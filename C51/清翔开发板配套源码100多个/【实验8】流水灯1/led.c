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
* 【程序功能】： 流水灯			   			            			    
* 【使用说明】： 
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include<reg52.h>
#include<intrins.h>

void delay(unsigned char i)
{
	unsigned char m,n;
	for(m=i;m>0;m--)
	for(n=125;n>0;n--);
}

void main()
{	unsigned char k;
while(1)
{
	P1=0xfe;
	for(k=0;k<8;k++)
	{
	delay(2000);
	P1=_crol_(P1,1);
	}
}
}