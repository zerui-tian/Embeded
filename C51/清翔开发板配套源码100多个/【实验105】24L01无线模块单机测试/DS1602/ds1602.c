#include "reg52.h"	/*包含STC单片机头文件*/
#include "QX51/qx51.h" /*包含qx51开发板头文件*/
#include "DELAY/delay.h"/*包含delay延时函数文件*/
#include "ds1602.h"

uchar datstr[]="123456789ABCDEF";
uchar datnum[]= {10,20,0x10,0x10,0x01};
uchar str[4];//uchartostr函数转换的字符串 同时可以把16进制转成10进制

void write_com(uchar com)  //写命令
{
	P0=com;
	rs=0;
	LCDEN=0;
	delay_ms(5);
	LCDEN=1;
	delay_ms(5);
	LCDEN=0;
		
}
void write_data(uchar date)	  //写数据
{
	P0=date;
	rs=1;
	LCDEN=0;
	delay_ms(5);
	LCDEN=1;
	delay_ms(5);
	LCDEN=0;
		
}
void init_1602()
{
    rw=0;
	write_com(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay_ms(5);
	write_com(0x0c);   //显示模式设置
	delay_ms(5);
	write_com(0x06);   //显示模式设置：光标右移，字符不移
	delay_ms(5);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay_ms(5);	
}

uchar *uchartostr(uchar num)	//将一个字节的数据转换为字符串 或10进制
{
	uchar x2,x1,x0,i;
	x2=num/100;
	x1=num%100/10;
	x0=num%100%10;
	i=0;
	if(x2!=0)
	{
		str[i]=x2+48;
		i++;
	}
	if(x1!=0)
	{
		str[i]=x1+48;
		i++;
	}
	str[i]=x0+48;
	i++;
	str[i]='\0';
	return str;
}

void lcd_dis(uchar X,uchar Y,uchar *dis)	//显示数据数组
{                          
   uchar  pos;
	switch(X)
	{
		case 0:X=0x00;
		break;
		case 1:X=0x40;
		break;
		break;
		default:break;
	}

   pos =0x80+X+Y;  
   write_com(pos);     //显示地址
   while(*dis!='\0')
   {
	write_data(*dis++);		
   }
}

void lcd_distostr(uchar X,uchar Y,uchar dis)//显示字符串 数组
{                          
   uchar  pos,i;
   i=dis; //设定从数组中的第几个字符串开始显示
	switch(X)
	{
		case 0:X=0x00;
		break;
		case 1:X=0x40;
		break;
		break;
		default:break;
	}

   pos =0x80+X+Y;  
   write_com(pos);     //显示地址
	while(datstr[i] !='\0' )
	{
		write_data(datstr[i]);
		i++;
	}
}
