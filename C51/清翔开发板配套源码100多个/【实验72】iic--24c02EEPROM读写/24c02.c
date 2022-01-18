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
* �������ܡ��� ��ʵ��65����			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
//�����ܣ�  ���� AT24C02 EEPROMоƬ��ʵ�ֶ���Ķ���д�����䱣���ʱ�������secֵ��
//			�����Ա�֤��ϵͳ��������¼ӵ�ʱ�����Զ����ϴε���˲���������ֵ��
//��ʾ������  ���س��������ܿ�ʼ��ʾ�Լ���ֵ��Ƭ�̺󣬹ر�ѧϰ��
//			��Դ��Ȼ��򿪣��ᷢ�����������ʾ����ֵ�Ƕϵ�˲������
//			ʾ����ֵ����Ҳ����AT24C02EEPROMоƬ���������ֵ����
/***************************************************************/
//����AT24C02EEPROMоƬ����
#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
unsigned char sec,num;        //�������ֵ��ÿ��1�룬sec��1
unsigned int times;        //��ʱ�жϴ���
bit  write=0;             //д24C02�ı�־��
sbit sda=P2^0;                          //IO�ڶ���
sbit scl=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char j,k;

void delay(unsigned char i)		//��ʱ����
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,				//����ܱ���
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void nop()
{
	_nop_();
	_nop_();
}
void display(uchar a,uchar b)			//��ʾ����
{
   dula=0;
   P0=table[a];		//��ʾ��λ
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[b];		//��ʾʮλ
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

}

/////////24C02��д��������////////////////////
void delay1(unsigned char m)
{	unsigned int n;
  	for(n=0;n<m;n++);
}



void init()  //24c02��ʼ���ӳ���
{
	scl=1;
	nop();
	sda=1;
	nop();
}

void start()        //����I2C����
{
	sda=1;
	nop();
	scl=1;
	nop();
	sda=0;
	nop();
	scl=0;
	nop();
}

void stop()         //ֹͣI2C����
{
	sda=0;
	nop();
	scl=1;
	nop();
	sda=1;
	nop();
}

void writebyte(unsigned char j)  //дһ���ֽ�
{
	unsigned char i,temp;
   	temp=j;
   	for (i=0;i<8;i++)
   {
	   temp=temp<<1;
	   scl=0;
	   nop();
	   sda=CY;		//temp����ʱ���Ƴ���ֵ������CY��
	   nop();
	   scl=1;		//��sda���ϵ������ȶ��󣬽�scl����
	   nop();
   }
   scl=0;
   nop();
   sda=1;
   nop();
}

unsigned char readbyte()   //��һ���ֽ�
{
   unsigned char i,j,k=0;
   scl=0; nop(); sda=1;
   for (i=0;i<8;i++)
   {  
		nop(); scl=1; nop();
      	if(sda==1) 
		j=1;
      	else
		j=0;
      	k=(k<<1)|j;
	  	scl=0;
	}
   	nop();
	return(k);
}

void clock()         //I2C����ʱ��
{
   unsigned char i=0;
   scl=1;
   nop();
   while((sda==1)&&(i<255))
   	  i++;
   scl=0;
   nop();
}

////////��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����/////
unsigned char read24c02(unsigned char address)
{
   unsigned char i;
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   start();
   writebyte(0xa1);
   clock();
   i=readbyte();
   stop();
   delay1(10);
   return(i);
}

//////��24c02��address��ַ��д��һ�ֽ�����info/////
void write24c02(unsigned char address,unsigned char info)
{
   EA=0;
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   writebyte(info);
   clock();
   stop();
   EA=1;
   delay1(50);
}

void main()
{
unsigned char i;
TMOD=0x01;  //��ʱ�������ڷ�ʽ1
ET0=1;
EA=1;
TH0=0x4c; //��TH0 TL0��ֵ
TL0=0x00; //ʹ��ʱ��0.05���ж�һ��
TR0=1;           //��ʼ��ʱ

init();        //��ʼ��24C02
sec=read24c02(1);//ÿ�����¿���ʱ��������������ݸ���sec

while(1)
  {
      i=20;
	  //num=read24c02(1);//������������ݸ���num
      while(i--)
      {
 	  	display(sec/10,sec%10);
      }
	  if(write==1) //�жϼ�ʱ���Ƿ��ʱһ��
       {
	 	write=0;              //����
        write24c02(1,sec);  //��24c02�ĵ�ַ1��д������sec
	   }
   } 
}

void timer0(void) interrupt 1 using 0  //��ʱ�жϷ�����
{
TH0=0x4c; //��TH0 TL0��ֵ ,ÿ��0.05s����һ���жϡ�
TL0=0x00; //��װ������ֵ
times++;        //ÿ��250ust tcnt��һ
if(times==20)  //����20�Σ�1�룩ʱ
  {
    times=0;   //�����ټ�
    sec++;
    write=1;  //1��дһ��24C08
    if(sec==60) //��ʱ100�룬�ڴ��㿪ʼ��ʱ
      {sec=0;}
  }
}
