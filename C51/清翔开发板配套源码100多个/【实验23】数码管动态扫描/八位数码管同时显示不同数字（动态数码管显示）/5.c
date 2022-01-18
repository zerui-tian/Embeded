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
* �������ܡ��� ��QX-MCS51 ʵ���İ�λ�����ͬʱ��ʾ����0��7��			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include <reg51.h>

sbit dula=P2^6;		//��ѡ�źŵ�����������
sbit wela=P2^7;		//λѡ�źŵ�����������

int number[8];
bit fipfop = 1;

unsigned char code wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
						//����ܸ�λ�����
unsigned char code duan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
						//0-f�����
void delay(unsigned int i)
{
	unsigned int m,n;
	for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

void main()
{
	unsigned char cnt;
	for(cnt = 0; cnt < 8; cnt++){
		number[cnt] = 0;
	}
	TCON |= 0x01;
	EX0 = 1;
	EA = 1;
	while(1)
	{
		for(cnt=0;cnt<8;cnt++)
		{
			P0 = 0x00;
			P0=wei[7-cnt];	   
			wela=1;
			wela=0;
			P0=duan[number[cnt]];	  
			dula=1;
			dula=0;
			delay(2);	 //ʱ�����̣����ǹؼ�����ν��ͬʱ��ʾ��ֻ�Ǽ���϶̶��ѣ��������۵����ЧӦ������ÿ������ܶ�һֱ��������
		 }
	}
}

void counter_interrupt() interrupt 0{
	int cnt;
	fipfop = ~fipfop;
	if(fipfop == 1){
		number[0]++;
		for(cnt = 0; cnt < 7; cnt++){
			if(number[cnt] >= 10){
				number[cnt+1]++;
				number[cnt] %= 10;
			}
			else{
				break;
			}
		}
	}
}
