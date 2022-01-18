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
* 【程序功能】： 让QX-MCS51 实验板的八位数码管同时显示数字0到7。			   			            			    
* 【使用说明】： 
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include <reg51.h>

sbit dula=P2^6;		//段选信号的锁存器控制
sbit wela=P2^7;		//位选信号的锁存器控制

int number[8];
bit fipfop = 1;

unsigned char code wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
						//数码管各位的码表
unsigned char code duan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
						//0-f的码表
void delay(unsigned int i)
{
	unsigned int m,n;
	for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

void main()
{
	unsigned char cnt;
	for(cnt = 0; cnt < 8; cnt++){
		number[cnt] = 0;
	}
	TCON |= 0x01;
	EX0 = 1;
	EA = 1;
	while(1)
	{
		for(cnt=0;cnt<8;cnt++)
		{
			P0 = 0x00;
			P0=wei[7-cnt];	   
			wela=1;
			wela=0;
			P0=duan[number[cnt]];	  
			dula=1;
			dula=0;
			delay(2);	 //时间间隔短，这是关键（所谓的同时显示，只是间隔较短而已，利用人眼的余辉效应，觉得每个数码管都一直在亮）。
		 }
	}
}

void counter_interrupt() interrupt 0{
	int cnt;
	fipfop = ~fipfop;
	if(fipfop == 1){
		number[0]++;
		for(cnt = 0; cnt < 7; cnt++){
			if(number[cnt] >= 10){
				number[cnt+1]++;
				number[cnt] %= 10;
			}
			else{
				break;
			}
		}
	}
}
