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
* 【程序功能】： DA控制旁边的LED灯发生亮度的变化			   			            			    
* 【使用说明】： 
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit sda=P2^0;
sbit scl=P2^1;

void delay()  //5us
{;;}
void delay1(uint z)//1ms
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);	
}

void start()
{
	scl=1;
	_nop_();
	sda=1;
	delay();
	sda=0;
	delay();
	scl=0;
	_nop_();
}
void stop()
{
	scl=1;
	_nop_();
	sda=0;
	delay();
	sda=1;
	delay();
}
void respons()
{
	scl=1;
	_nop_();
	sda=0;
	delay();
	scl=0;
	_nop_();
}

void init()
{
	scl=1;
	_nop_();
	sda=1;
	_nop_();
}
void write_byte(uchar dat)
{
	uchar i,temp;
	temp=dat;
	for(i=0;i<8;i++)
	{
		scl=0;
		_nop_();
		if((temp&0x80)==0x80)
			sda=1;	  //写1
		else
			sda=0;	  //写0
		scl=1;
		_nop_();
		temp<<=1;
	}
	scl=0;
	_nop_();
}

void DAC_write(uchar dat)
{
	start();
	write_byte(0x90);
	respons();
	write_byte(0x40);
	respons();
	write_byte(dat);
	respons();
	stop();

}

void main()
{
	uchar i=0;
	init();
	while(1)
	{
		DAC_write(i);
		i++;
		delay1(20);
	}
}