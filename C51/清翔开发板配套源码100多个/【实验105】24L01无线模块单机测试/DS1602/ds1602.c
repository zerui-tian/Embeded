#include "reg52.h"	/*����STC��Ƭ��ͷ�ļ�*/
#include "QX51/qx51.h" /*����qx51������ͷ�ļ�*/
#include "DELAY/delay.h"/*����delay��ʱ�����ļ�*/
#include "ds1602.h"

uchar datstr[]="123456789ABCDEF";
uchar datnum[]= {10,20,0x10,0x10,0x01};
uchar str[4];//uchartostr����ת�����ַ��� ͬʱ���԰�16����ת��10����

void write_com(uchar com)  //д����
{
	P0=com;
	rs=0;
	LCDEN=0;
	delay_ms(5);
	LCDEN=1;
	delay_ms(5);
	LCDEN=0;
		
}
void write_data(uchar date)	  //д����
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
	write_com(0x38);   //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	delay_ms(5);
	write_com(0x0c);   //��ʾģʽ����
	delay_ms(5);
	write_com(0x06);   //��ʾģʽ���ã�������ƣ��ַ�����
	delay_ms(5);
	write_com(0x01);   //����Ļָ�����ǰ����ʾ�������
	delay_ms(5);	
}

uchar *uchartostr(uchar num)	//��һ���ֽڵ�����ת��Ϊ�ַ��� ��10����
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

void lcd_dis(uchar X,uchar Y,uchar *dis)	//��ʾ��������
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
   write_com(pos);     //��ʾ��ַ
   while(*dis!='\0')
   {
	write_data(*dis++);		
   }
}

void lcd_distostr(uchar X,uchar Y,uchar dis)//��ʾ�ַ��� ����
{                          
   uchar  pos,i;
   i=dis; //�趨�������еĵڼ����ַ�����ʼ��ʾ
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
   write_com(pos);     //��ʾ��ַ
	while(datstr[i] !='\0' )
	{
		write_data(datstr[i]);
		i++;
	}
}
