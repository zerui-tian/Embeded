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
* �������ܡ��� �������ʾ����9λ��������			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include"stc.h"

#define uchar unsigned char 
unsigned long num;
uchar j,k;
uchar a0,b0,c0,d0,e0,f0,g0,h0;
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void delay(uchar i)
{
	for(j=i;j>0;j--)
	for(k=125;k>0;k--);
}

void display(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f,uchar g,uchar h)
{
   dula=0;
   P0=table[a];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[b];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[c];
   dula=1;
   dula=0;
   
   P0=0xfb;
   wela=1;

   wela=0;
   delay(5);
   
   P0=table[d];
   dula=1;
   dula=0;
   
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[e];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[f];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);

   P0=table[g];
   dula=1;
   dula=0;
   
   P0=0xbf;
   wela=1;
   wela=0;
   delay(5);

   P0=table[h];
   dula=1;
   dula=0;
   
   P0=0x7f;
   wela=1;
   wela=0;
   delay(5);
}

void main()
{
	num=94556225;//����Ǳ���ʾ��������ֻҪ��������λ��������ʾ������
			//ͬѧ�ǿ�������������num������ֵ�����¡�
	while(1)
	{
	
	if(num<10)
	{
		a0=num;b0=16;c0=16;d0=16;e0=16;f0=16;g0=16;h0=16;
	}
	else
	{
		if(num<100)
		{a0=num/10;b0=num%10;c0=16;d0=16;e0=16;f0=16;g0=16;h0=16;}
		else
		{
			if(num<1000)
			{a0=num/100;b0=num%100/10;c0=num%10;d0=16;e0=16;f0=16;g0=16;h0=16;}
			else
			{
				if(num<10000)
				{a0=num/1000;b0=num%1000/100;c0=num%100/10;d0=num%10;e0=16;f0=16;g0=16;h0=16;}
				else
				{
					if(num<100000)
					{a0=num/10000;b0=num%10000/1000;c0=num%1000/100;d0=num%100/10;e0=num%10;f0=16;g0=16;h0=16;}
					else
					{
						if(num<1000000)
						{a0=num/100000;b0=num%100000/10000;c0=num%10000/1000;d0=num%1000/100;e0=num%100/10;f0=num%10;g0=16;h0=16;}
						else
						{
						    if(num<10000000)
						    {a0=num/1000000;b0=num%1000000/100000;c0=num%100000/10000;d0=num%10000/1000;e0=num%1000/100;f0=num%100/10;g0=num%10;h0=16;}  
							else
							{
							    if(num<100000000)
					        	{a0=num/10000000;b0=num%10000000/1000000;c0=num%1000000/100000;d0=num%100000/10000;e0=num%10000/1000;f0=num%1000/100;g0=num%100/10;h0=num%10;}
							} 
						}
					}
				}
			}
		}
	}
	display(a0,b0,c0,d0,e0,f0,g0,h0);
	}
}