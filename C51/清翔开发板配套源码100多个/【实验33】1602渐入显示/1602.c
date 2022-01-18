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
* �������ܡ��� 1602������ʾ			   			            			    
* ��ʹ��˵������ Һ��1�Ž�1602��ɫ��ĸ��1��  ������ޱ����Һ��ֻ��14���� ��ĸ�ұ߿�2��λ   
                 ���Һ���Աȶȵ�λ��˳ʱ�����10���ӷ�����������������λ�û��𻵵�λ����


*  ˵������ѿ�Դ�����ṩԴ�������.
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
	write_com(0x38);   //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	delay(20);
	write_com(0x0f);   //��ʾģʽ����
	delay(20);
	write_com(0x06);   //��ʾģʽ���ã�������ƣ��ַ�����
	delay(20);
	write_com(0x01);   //����Ļָ�����ǰ����ʾ�������
	delay(20);	
}

void main()
{
	uchar a;
	init();
	test = 0;
	write_com(0x80+17);	 //����һ���ַ�д������ƫ��17���ַ�����Ϊ�����������������׼����
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
		write_com(0x18); //����
		delay1(600);
	}
	while(1){
		delay(100);
		test = ~test;
	};
}
