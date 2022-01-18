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
* �������ܡ��� ģ�⽻ͨ����ʾ��			   			            			    
* ��ʹ��˵������ ���У�������̵���ʾʱ���Ϊ50s���м�ĻƵ���ʾʱ��Ϊ15s��
		  �̵��ð�λ���������������˸������
		  ����ð�λ���������ȫ��������
		  �Ƶ��ð�λ���������ȫ��ȫ����˸������
		  �տ�ʼ��ʾ��Ϊ�̵ơ�
		  ��ʾ˳����--��--��--��--��--��--��--������������
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include<reg51.h>

#define uchar unsigned char
#define uint unsigned int

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;

unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar j,k,l=255;
uchar a1,a0;
uchar shijian,deng=2;
void delay(unsigned int i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display(unsigned char sh_c,unsigned char g_c)
{
   dula=0;
   P0=table[sh_c];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[g_c];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);
}

void main()
{
	TMOD=0x01;
	TR0=1;
	TH0=(65536-46080)/256;// ���ھ���Ϊ11.0592,�����Ǵ���ӦΪ46080����ʱ��ÿ��50000΢�뷢��һ���жϡ�
	TL0=(65536-46080)%256;//46080��������Ϊ50000*11.0592/12
	ET0=1;
	EA=1;
	while(1)
	{	
		if(shijian==0)
		{
			switch(deng)
			{
				case 0: shijian=50;		  //���
						deng=1;
						break;
				case 1: shijian=15;		  //�Ƶ�
						deng=2;
						break;
				case 2: shijian=50;		  //�̵�
						deng=3;
						break;
				case 3: shijian=15;		  //�Ƶ�
						deng=0;
						break;
			}
		}
		if(pp==20)
		{	pp=0;
			shijian--;
		}
		a0=shijian%10;
		a1=shijian/10;
		display(a1,a0);
		switch(deng)
		{
			case 1: P1=0X00;			 //���
					break;
			case 2: 			//�Ƶ�
					if(shijian%2==0)
					P1=0x00;
					if(shijian%2==1)
					P1=0xff;
					break;
			case 3: 		//�̵�
					if(shijian%2==0)
					P1=0x55;
					if(shijian%2==1)
					P1=0xaa;
					break;
			case 0:	if(shijian%2==0)	  //�Ƶ�
					P1=0x00;
					if(shijian%2==1)
					P1=0xff;
					break;	
		}
	}
}

void time0() interrupt 1
{	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
