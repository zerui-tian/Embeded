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
* �������ܡ��� ����24c02��ÿ���洢��ַֻ��Ӧ8��bit�Ĵ洢�ռ䣬��һ��
			�洢��Ԫ���洢���������Ϊ255�������̽�����һ�ֵ�������
			���ݴ���255����С��65536���Ĵ洢���������������洢��Ԫ��
			�洢һ�����ݡ�
			���س��������ܻ���ʾ���趨��numֵ���˴���Ϊ12345����			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
uint num,num1,num2;        //�������ֵ��ÿ��1�룬sec��1
sbit sda=P2^0;                          //IO�ڶ���
sbit scl=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
sbit s1=P3^4;//��������
sbit s2=P3^5;//���㰴��
unsigned char j,k;
char a,b,c,d,e;

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
void display(uchar a,b,c,d,e)			//��ʾ����
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
   P0=table[b];		//��ʾǧλ
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[c];		//��ʾ��λ
   dula=1;
   dula=0;

   wela=0;
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[d];		//��ʾʮλ
   dula=1;
   dula=0;

   wela=0;
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[e];		//��ʾ��λ
   dula=1;
   dula=0;

   wela=0;
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
	 
}

/////////24C02��д��������////////////////////
void delay1(unsigned int m)
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
   delay1(100);
   return(i);
}

//////��24c02��address��ַ��д��һ�ֽ�����info/////
void write24c02(unsigned char address,unsigned char info)
{
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   writebyte(info);
   clock();
   stop();
   delay1(5000); //�����ʱһ��Ҫ�㹻��������������Ϊ24c02�ڴ�sda��ȡ�����ݺ󣬻���Ҫһ��ʱ�����¼���̡�
}

void main()
{
	init();        //��ʼ��24C02
	num=12345;	   //numΪС�ڵ���65535��������
	num2=num/256;
	num1=num%256;
	write24c02(0,num1);
	write24c02(1,num2);
	num1=read24c02(0);	//�õ�ַ0��Ԫ�洢numʮ�����Ʊ�ʾʱ�ĵ���λ
	num2=read24c02(1);	//�õ�ַ0��Ԫ�洢numʮ�����Ʊ�ʾʱ�ĸ���λ
	num=num2*256+num1;
	a=num/10000;
	b=num%10000/1000;
	c=num%1000/100;
	d=num%100/10;
	e=num%10;
	while(1)
	display(a,b,c,d,e);
}

