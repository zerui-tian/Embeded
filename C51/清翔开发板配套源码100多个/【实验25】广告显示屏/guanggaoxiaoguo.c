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
* �������ܡ��� ģ�����Ƶ�����������ʾ			   			            			    
* ��ʹ��˵������ ����������ʾ������һ��Ϊһ��������λ��ʾ�������֣�
		  һ��Ϊһ����ʾ������ͬ���֡�
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include<reg51.h>

#define uchar unsigned char

uchar a=0,b=1,c=2,d=3,e=4,f=5,g=6,h=7,temp,key,k,m,n;
unsigned long i,j;

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
						 //���һ��0x00��������ʾ��
void delay(unsigned long i)
{
	for(j=i;j>0;j--)
	for(k=125;k>0;k--);
}

void display(uchar wei,uchar shu)//������һλ��ʾ���������
{
   dula=0;
   P0=table[shu];
   dula=1;
   dula=0;
   
   wela=0;
   switch(wei)
   {
   	case 1:
			P0=0xfe;
   			wela=1;
  			wela=0;
   			delay(5);
			break;
	case 2:
			P0=0xfd;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;
   case 3:
		   P0=0xfb;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;
   case 4:	     
		   P0=0xf7;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;
   case 5:
		   P0=0xef;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;
   case 6:
		   P0=0xdf;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;

   case 7:
		   P0=0xbf;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;

   case 8:
		   P0=0x7f;
		   wela=1;
		   wela=0;
		   delay(5);
		   break;
   }
}
void display0(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f,uchar g,uchar h)
{						  //һ����ʾ�������֣���ÿλ��ʾ���ֿ��Զ���
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
  	while(1)
	{	
	    display(8,0);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,0);				 //һ��ֻ��һλ����ʾ
		delay(500);		 
		display(6,0);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(5,0);
		delay(500);
		display(4,0);
		delay(500);
		display(3,0);
		delay(500);
		display(2,0);
		delay(500);
		display(1,0);
		delay(500);
		display(8,1);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,1);				 //һ��ֻ��һλ����ʾ
		delay(500);	
		display(6,1);
		delay(500);
		display(5,1);
		delay(500);
		display(4,1);
		delay(500);
		display(3,1);
		delay(500);
		display(2,1);
        delay(500);
		display(8,2);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,2);				 //һ��ֻ��һλ����ʾ
		delay(500);	
		display(6,2);
		delay(500);
		display(5,2);
		delay(500);
		display(4,2);
		delay(500);
		display(3,2);
		delay(500);
		display(8,3);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,3);				 //һ��ֻ��һλ����ʾ
		delay(500);	
		display(6,3);
		delay(500);
		display(5,3);
		delay(500);
		display(4,3);
		delay(500);
		display(8,4);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,4);				 //һ��ֻ��һλ����ʾ
		delay(500);	
		display(6,4);
		delay(500);
		display(5,4);
		delay(500);
		display(8,5);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,5);				 //һ��ֻ��һλ����ʾ
		delay(500);	
		display(6,5);
		delay(500);	  
		display(8,6);				 //һ��ֻ��һλ����ʾ
		delay(500);
		display(7,6);				 //һ��ֻ��һλ����ʾ
		delay(500);	 
		display(8,7);				 //һ��ֻ��һλ����ʾ
		delay(500);
		for(m=50;m>0;m--)		  //����һ����ʾ��λ����ʵ��
		{
		display0(16,16,16,16,16,16,16,0);		//16��������ʾ
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(16,16,16,16,16,16,0,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(16,16,16,16,16,0,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(16,16,16,16,0,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(16,16,16,0,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(16,16,0,16,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(16,0,16,16,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,16,16,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,16,16,16,16,16,1);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,16,16,16,16,1,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,16,16,16,1,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,16,16,1,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,16,1,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,16,1,16,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,16,16,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,16,16,16,16,16,2);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,16,16,16,16,2,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,16,16,16,2,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,16,16,2,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,16,2,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,16,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,16,16,16,16,3);
		delay(2);		   
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,16,16,16,3,16);
		delay(2);		   
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,16,16,3,16,16);
		delay(2);		   
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,16,3,16,16,16);
		delay(2);		   
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,16,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,16,16,16,4);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,16,16,4,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,16,4,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,16,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,16,16,5);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,16,5,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,5,16,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,5,16,6);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,5,6,16);
		delay(2);
		}
		for(m=50;m>0;m--)
		{
		display0(0,1,2,3,4,5,6,7);
		delay(2);
		}
		for(n=4;n>0;n--)	 //��˸
		{
			for(m=100;m>0;m--)
			{
				display0(0,1,2,3,4,5,6,7);
			}
			for(m=100;m>0;m--)
			{
				display0(16,16,16,16,16,16,16,16);
			}
		}
	}
}