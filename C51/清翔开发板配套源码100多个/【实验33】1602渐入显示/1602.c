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
* 【程序功能】： 1602渐入显示			   			            			    
* 【使用说明】： 液晶1脚接1602黑色排母的1脚  如果是无背光的液晶只有14个脚 排母右边空2个位   
                 请把液晶对比度电位器顺时针调制10点钟方向，请勿多调超出极限位置会损坏电位器！


*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit lcden=P2^5;
sbit rs=P2^7;
sbit rw=P2^6;
sbit test = P3^4;
uchar table1[]="QX-MCS51  MCU";
uchar table2[]="  Thank you  ";
void delay(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=10;b>0;b--); 
}

void delay1(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=100;b>0;b--); 
}

void write_com(uchar com)
{
	P0=com;
	rs=0;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void write_date(uchar date)
{
	P0=date;
	rs=1;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void init()
{
        rw=0;
	write_com(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(20);
	write_com(0x0f);   //显示模式设置
	delay(20);
	write_com(0x06);   //显示模式设置：光标右移，字符不移
	delay(20);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay(20);	
}

void main()
{
	uchar a;
	init();
	test = 0;
	write_com(0x80+17);	 //将第一个字符写在向右偏移17个字符处，为后面的由右向左划入做准备。
	delay(20);
	for(a=0;a<13;a++)
	{
	write_date(table1[a]);
	delay(20);		
	}

	write_com(0xc0+17);
	delay(50);
	for(a=0;a<13;a++)
	{
	write_date(table2[a]);
	delay(40);		
	}

	for(a=0;a<16;a++)
	{
		write_com(0x18); //左移
		delay1(600);
	}
	while(1){
		delay(100);
		test = ~test;
	};
}
