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
sbit lcden=P3^4;
sbit rs=P3^5;
sbit rw=P3^6;	
sbit dula=P2^6;
sbit wela=P2^7;
uchar table1[]="HELLO QX-MCS51";
uchar table2[]=" QQ:863888999 ";
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
	dula=0;
	wela=0;
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

	write_com(0x80);	 //����һ���ַ�д������ƫ��17���ַ�����Ϊ�����������������׼����
	delay(20);
	for(a=0;a<14;a++)
	{
	write_date(table1[a]);
	delay(20);		
	}

	write_com(0xc0);
	delay(50);
	for(a=0;a<14;a++)
	{
	write_date(table2[a]);
	delay(40);		
	}
	while(1);
}
