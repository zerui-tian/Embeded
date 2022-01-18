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
* �������ܡ��� 1602��ʾˢ������ַ�			   			            			    
* ��ʹ��˵������  Һ��1�Ž�1602��ɫ��ĸ��1��  ������ޱ����Һ��ֻ��14���� ��ĸ�ұ߿�2��λ   
                 ���Һ���Աȶȵ�λ��˳ʱ�����10���ӷ�����������������λ�û��𻵵�λ����

*  ˵������ѿ�Դ�����ṩԴ�������.
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
	for(num=0;num<16;num++)    //��һ��
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
	write_com(0x01); //����
   write_com(0x80);	 //�����ַ���ʾλ��ָ���ڵ�һ��
	for(num=0;num<16;num++)    // �ڶ���
	{	beep=0;
		write_data(table2[num]);
		delay(100);
		beep=1;
		delay(50);
	}
	write_com(0x80+0x40); //�����ַ���ʾλ��ָ���ڵڶ���
		for(num=0;num<16;num++)
	{	beep=0;
		write_data(table3[num]);
		delay(100);
		beep=1;
		delay(50);
	}
   write_com(0x80);
	write_com(0x01);
	for(num=0;num<16;num++)    //������
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
	for(num=0;num<16;num++)   //������
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
	for(num=0;num<16;num++)   //������
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

   for(num=0;num<16;num++)   //������
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
