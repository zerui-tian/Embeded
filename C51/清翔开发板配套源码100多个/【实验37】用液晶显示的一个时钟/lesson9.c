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
* 【程序功能】： LCD1602液晶显示时钟,时间可调			   			            			    
* 【使用说明】： 按下键盘上的S1会在时分秒之前切换 S2时间加S3时间减


液晶1脚接1602黑色排母的1脚  如果是无背光的液晶只有14个脚 排母右边空2个位   
                 请把液晶对比度电位器顺时针调制10点钟方向，请勿多调超出极限位置会损坏电位器！

*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include<reg52.h>
#define uchar unsigned char 
#define uint unsigned int
sbit lcden=P3^4;
sbit rs=P3^5;
sbit rw=P3^6;	
sbit dula=P2^6;
sbit wela=P2^7;
sbit s1=P3^0;
sbit s2=P3^1;
sbit s3=P3^2;
uchar count,s1num;
char miao,shi,fen;
uchar code table[]="  2013-7-14 MON";
uchar code table1[]="    00:00:00";
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void write_com(uchar com)
{
	rs=0;
	lcden=0;
	P0=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

void write_date(uchar date)
{
	rs=1;
	lcden=0;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

void init()
{

	uchar num;
	lcden=0;
	dula=0;
	wela=0;
//	fen=59;
//	miao=53;
//	shi=23;	       
    rw=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
	for(num=0;num<15;num++)
		{
			write_date(table[num]);
			delay(5);
		}
	write_com(0x80+0x40);
	for(num=0;num<12;num++)
		{
			write_date(table1[num]);
			delay(5);
		}
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;
}

void write_sfm(uchar add,uchar date)
{
	uchar shi,ge;
	shi=date/10;
	ge=date%10;
	write_com(0x80+0x40+add);
	write_date(0x30+shi);
	write_date(0x30+ge);
}

void keyscan()
{
	if(s1==0)	
	{
		delay(5);
		if(s1==0)
		{	s1num++;
			while(!s1);
			if(s1num==1)
			{
				TR0=0;
				write_com(0x80+0x40+10);
				write_com(0x0f);
			}
	}
			if(s1num==2)
			{
				write_com(0x80+0x40+7);
			}
			if(s1num==3)
			{
				write_com(0x80+0x40+4);
			}
			if(s1num==4)
			{
				s1num=0;
				write_com(0x0c);
				TR0=1;
			}

				
		}
		if(s1num!=0)
		{
			if(s2==0)
			{
				delay(5);
				if(s2==0)
				{
					while(!s2);
					if(s1num==1)
					{
						miao++;
						if(miao==60)
							miao=0;
						write_sfm(10,miao);
						write_com(0x80+0x40+10);
						
						
					}
					if(s1num==2)
					{
						fen++;
						if(fen==60)
							fen=0;
						write_sfm(7,fen);
						write_com(0x80+0x40+7);
					}
					if(s1num==3)
					{
						shi++;
						if(shi==24)
							shi=0;
						write_sfm(4,shi);
						write_com(0x80+0x40+4);
					}
				}
			}
			if(s3==0)
			{
				delay(5);
				if(s3==0)
				{
					while(!s3);
					if(s1num==1)
					{
					/*	if(miao==0)
						{
							miao=59;
							write_sfm(10,miao);
							write_com(0x80+0x40+10);
						}*/
						miao--;
						if(miao==-1)
							miao=59;
						write_sfm(10,miao);
						write_com(0x80+0x40+10);
					}
					if(s1num==2)
					{
						fen--;
						if(fen==-1)
							fen=59;
						write_sfm(7,fen);
						write_com(0x80+0x40+7);
					}
					if(s1num==3)
					{
						shi--;
						if(shi==-1)
							shi=23;
						write_sfm(4,shi);
						write_com(0x80+0x40+4);
					}
				}
			}
		}
}
void main()
{
	init();
	while(1)
	{
		keyscan();
		
	}
//	while(1);
}

void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	count++;
if(count==18)
		{
			count=0;
			miao++;
			if(miao==60)
			{
				miao=0;
				fen++;
				if(fen==60)
				{
					fen=0;
					shi++;
					if(shi==24)
					{
						shi=0;
					}
					write_sfm(4,shi);
				}
				write_sfm(7,fen);
			}
			write_sfm(10,miao);
			
		}	
}

