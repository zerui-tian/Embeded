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
* �������ܡ��� ��QX-MCS51  ʵ������λ�����������ʾ����0��7			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include <reg51.h>

sbit dula=P2^6;		//��ѡ�źŵ�����������
sbit wela=P2^7;		//λѡ�źŵ�����������

unsigned char code wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
						//����ܸ�λ�����
unsigned char code duan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,};
						//0-5�����
void delay(unsigned int i)
{
	unsigned int m,n;
	for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

void main()
{
	unsigned char num;
	while(1)
	{
		for(num=0;num<8;num++)
		{
			P0=wei[num];	   
			wela=1;
			wela=0;

			P0=duan[num];	  
			dula=1;
			dula=0;
			delay(1000);
		 }
	}
}

