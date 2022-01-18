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
* 【程序功能】： 1602显示刷屏大段字符			   			            			    
* 【使用说明】：  液晶1脚接1602黑色排母的1脚  如果是无背光的液晶只有14个脚 排母右边空2个位   
                 请把液晶对比度电位器顺时针调制10点钟方向，请勿多调超出极限位置会损坏电位器！

*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

sbit beep=P2^3;

uchar code table []="I want to bring ";
uchar code table1[]="out the secret  ";
uchar code table2[]="of nature and   ";
uchar code table3[]="apply them for  ";
uchar code table4[]="the happiness of";
uchar code table5[]="man.I don't know";
uchar code table6[]="of any better s-";
uchar code table7[]="-ervice to offer";
uchar code table8[]="for the short t-";
uchar code table9[]="-ime we are in  ";
uchar code table10[]="the world.      ";
uchar code table11[]="---Thomas Edison";

sbit lcden=P3^4;
sbit lcdrs=P3^5;
sbit rw=P3^6;	
sbit dula=P2^6;
sbit wela=P2^7;
uchar num;

void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void write_com(uchar com)
{
	lcdrs=0;
	P0=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}

void write_data(uchar date)
{
	lcdrs=1;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}

void init()
{	
        rw=0;
        dula=0;
        wela=0;
	lcden=0;
	write_com(0x38);
	write_com(0x0e);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
}
void main()
{
	init();
	for(num=0;num<16;num++)    //第一屏
	{
		beep=0;
		write_data(table[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40);
		for(num=0;num<16;num++)
	{
		beep=0;
		write_data(table1[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x01); //清屏
   write_com(0x80);	 //设置字符显示位置指针于第一行
	for(num=0;num<16;num++)    // 第二屏
	{	beep=0;
		write_data(table2[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40); //设置字符显示位置指针于第二行
		for(num=0;num<16;num++)
	{	beep=0;
		write_data(table3[num]);
		delay(100);
		beep=1;
		delay(50);
	}
   write_com(0x80);
	write_com(0x01);
	for(num=0;num<16;num++)    //第三屏
	{	beep=0;
		write_data(table4[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40);
		for(num=0;num<16;num++)
	{	beep=0;
		write_data(table5[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x01);
   write_com(0x80);
	for(num=0;num<16;num++)   //第四屏
	{	beep=0;
		write_data(table6[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40);
		for(num=0;num<16;num++)
	{	beep=0;
		write_data(table7[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x01);
   write_com(0x80);
	for(num=0;num<16;num++)   //第五屏
	{	beep=0;
		write_data(table8[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40);
		for(num=0;num<16;num++)
	{	beep=0;
		write_data(table9[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x01);
   write_com(0x80);

   for(num=0;num<16;num++)   //第六屏
	{	beep=0;
		write_data(table10[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40);
		for(num=0;num<16;num++)
	{	beep=0;
		write_data(table11[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	while(1);
}
