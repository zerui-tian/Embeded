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
* 【程序功能】： 数码管显示DS1302芯片走时    			    
* 【使用说明】： 使用前是用杜邦线分别把DS1302旁边3个脚的排针与P1口的0~2接相接
接线方式是 请看开饭背面有丝印
					TSCLK  -  P10
					TIO    -  P11
					TRST   -  P12
					程序时间被设定为15.10.30
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit sclk = P1^0;
sbit dio = P1^1;
sbit ce = P1^2;
sbit wela = P2^7;
sbit dula = P2^6;
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);
}
uchar code table[]={ 
 
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"
                0x5C,  //"o"
                0x40,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                         };
void display(uchar hour10,hour,minute10,minute,second10,second)
{
	dula=1;
	P0=table[hour10];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfe;
	wela=0;
	delay(1);

	dula=1;
	P0=table[hour]|0x80;
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfd;
	wela=0;
	delay(1);

	dula=1;
	P0=table[minute10];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfb;
	wela=0;
	delay(1);

	dula=1;
	P0=table[minute]|0x80;
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xf7;
	wela=0;
	delay(1);

	dula=1;
	P0=table[second10];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xef;
	wela=0;
	delay(1);

	dula=1;
	P0=table[second];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xdf;
	wela=0;
	delay(1);
}
void write_1302(uchar add,dat)
{
	uchar i,temp;
	temp=add;
	ce=0;
	_nop_();
	sclk=0;
	_nop_();
	ce=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		sclk=0;
		_nop_();
		if((temp&0x01)==0x01)
			dio=1;
			else
			dio=0;
		temp>>=1;
		sclk=1;
		_nop_();
	}
	temp=dat;
	for(i=0;i<8;i++)
	{
		sclk=0;
		_nop_();
		if((temp&0x01)==0x01)
			dio=1;
			else
			dio=0;
		temp>>=1;
		sclk=1;
		_nop_();
	}
	ce=0;
	sclk=0;
}
uchar read_1302(uchar add)
{
	uchar dat,dat1,i,temp;
	temp=add;
	ce=0;
	_nop_();
	sclk=0;
	_nop_();
	ce=1;
	for(i=0;i<8;i++)
	{
		sclk=0;
		_nop_();
		if((temp&0x01)==0x01)
			dio=1;
			else
			dio=0;
		temp>>=1;
		sclk=1;
		_nop_();
	}
	for(i=0;i<8;i++)
	{
		sclk=0;
		_nop_();
		if(dio)
		    dat|=0x80;
		if(i<7)
			dat>>=1;
		sclk=1;
	}
	dat1=dat/16;
	dat=dat%16;
	dat=dat1*10+dat;  
	ce=0;
	sclk=0;
	return dat;
}
void main()
{
	uchar hour,minute,second;	
	write_1302(0x8e,0x00);

	write_1302(0xc0,0xfe);
	write_1302(0xc2,0xff);
	
	write_1302(0x80,0x30);

	write_1302(0x82,0x10);
	
	write_1302(0x84,0x15); 
	
	write_1302(0x8e,0x80);	
	while(1)
	{
		write_1302(0x8e,0x00);
		hour=read_1302(0x85);
		
		minute=read_1302(0x83);
		
		second=read_1302(0x81);	

		//r=read_1302(0xc1);
	//	r1=read_1302(0xc3);
		write_1302(0x8e,0x80);
		display(hour/10,hour%10,minute/10,minute%10,second/10,second%10);
		//display(r/100,r%100/10,r%100%10,r1/100,r1%100/10,r1%100%10);		
	}
}