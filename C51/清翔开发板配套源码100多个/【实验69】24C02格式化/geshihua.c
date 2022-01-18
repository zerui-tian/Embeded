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
* �������ܡ��� ��������ַ����Ӧ�Ĵ洢��Ԫ��һ���㡣  
           ÿ��ʽ��10����ַ��Ԫ��P1�ڵ�LED�佻����˸��
		     ��ʽ����ɣ�����LEDȫ������Ϊ��ʾ��			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit sda=P2^0;                     //IO�ڶ���
sbit scl=P2^1;

void nop()
{
	_nop_();
	_nop_();
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
/*
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
 */
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

/*
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
} */


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
	unsigned int i;
	P1=0x55;
	init();        //��ʼ��24C02
	for(i=0;i<256;i++)
	{
		write24c02(i,0);//��������ַ����Ӧ�Ĵ洢��Ԫ��һ���㡣
		if(i%10==0)
			P1=~P1;	//ÿ��ʽ��10����ַ��Ԫ��P1�ڵ�LED�佻����˸��
	}
	P1=0X00;//��ʽ����ɣ�����LEDȫ������Ϊ��ʾ��
	while(1);
}

