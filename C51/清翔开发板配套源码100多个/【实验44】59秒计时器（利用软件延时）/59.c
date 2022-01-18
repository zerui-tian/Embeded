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
* �������ܡ��� ����delay����������ͨ����Ƭ������ġ�����������ʱ��
			�����������ռ���˵�Ƭ������Դ����ʱ����ȷ�������ڶ�̬��ʾ
			�����ʱ��Ҳ�и�ȱ�㣬������ܵĵڶ�λ����ɨ�������󣬳���������
			���󡣣����������ֻ�����ں��ö�ʱ���ķ�����ʱ���Ƚϡ�Ҫ��
			�ܵĳ��ϣ������ᳫ����			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

uchar j,k,i,a,A1,A2,second;


sbit dula=P2^6;
sbit wela=P2^7;
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                    0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};


sbit beep=P2^3;
sbit kaiguan=P3^4;

void delay()		   //���0.5ms���ҵ���ʱ
{
	uchar a;
	for(a=450;a>0;a--)
	{
		_nop_();
	}
}

void display(uchar sh_c,uchar g_c)
{
   //dula=0;
   P0=table[sh_c];
   dula=1;
   dula=0;
   
   //wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay();
   
   P0=table[g_c];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay();
}

void main()
  {
     while(1)
       { unsigned int m;
		 for(m=0;m<2000;m++)
		 {
		 	delay();
		 }	
         second++;
         if(second==60)
         second=0;
         A1=second/10;
         A2=second%10;
         for(a=50;a>0;a--)
         { display(A1,A2);};
         }

    }
