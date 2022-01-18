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
* 【使用说明】： 需要按下独立键盘S2按键
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
/******************************************************************
程序功能：	 通过程序说明while与do...while间区别：
			 前者是先比较判断后执行，后者是先执行再比较判断。
			 故后者的循环体至少会执行一次，而前者有可能一次也不执行。
****************************************************************************/
#include <reg52.h>

sbit S2=P3^0;

void main()
{
	char m=3;
	P1=0xff;
	while(1)
	{
		if(S2)	 //S2未被按下时，将执行下面大括弧中的程序，但是，由于在刚开始进行的是判别
				 //操作，而3>4肯定为假，故不会执行P1=0xaa;操作。
				 //所以，此时，P1口相连的led不亮，P1各管脚的值还是刚开始赋的0xff。
		{
			while(m>4)
				P1=0xaa;
		}
		else   //当S2被按下后，执行该段程序，此时，虽然3>4仍然是假的，但是do...while
			  //是先执行一次，然后再判断，故在比较判断之前，已经执行了P1=0xaa；故此时
			  //P1口各led会被交替点亮。
		{
			do
			{
				P1=0x55;
			}
			while(m>4);
		}
    }
}