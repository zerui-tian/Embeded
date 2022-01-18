/*********************************************************************************
* ����дʱ�䡿�� 2014��3��5��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ 
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio3	
* �������ܡ��� DA�����Աߵ�LED�Ʒ������ȵı仯			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
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
			sda=1;	  //д1
		else
			sda=0;	  //д0
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