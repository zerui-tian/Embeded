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
* �������ܡ��� ��IIC��������24c02EEPROM�е�ַΪ0�Ĵ洢��Ԫ��д������0xaa
			���������������P1�ڣ��ɹ۲쵽P1��������LED���汻������
			��ҪΪ����ϰIIC���ߵ����������Լ�24C02�Ĳ���������			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
//����AT24C02EEPROMоƬ����

#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit sda=P2^0;                          //IO�ڶ���
sbit scl=P2^1;

//��Ϊ��д��24c02�����ݡ�Ϊ�˱�����֤��������������Ϊ�����ظ��ġ�
char code music[] = {
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0
};	 //�������Ҫ����Щ���ݶ����͵�P1�ڣ��ʿɷ���P1��������led�й��ɵ���˸15������

uchar data buffer[100]; //���ڻ����24c02�ж�ȡ�����ݡ�
				 
delay(unsigned int m)
{
	unsigned int n,p;
	for(n=m;n>0;n--)
		for(p=125;p>0;p--);
}
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
	uchar add,i,j,m;
	init();        //��ʼ��24C02
	P1=0X55;
	while(add!=sizeof(music))
	{
		write24c02(0x00+add,music[add]);
		add++;
		if(add%4==0)
		P1=~P1;
	}
	P1=0X00;
//����Ϊֹ����24C02��д�����ݵĹ��̾��ѽ���������ĳ���Ϊ���ӵġ�
//����д��������ٶ������͵�P1�ڣ�ͨ���۲�led������������д��������Ƿ���ȷ��
	while(m!=add)
	{
		buffer[i]=read24c02(m);
		i++;
		m++;
	}
	
	while(j!=add)
	{
		P1=buffer[j];
		j++;
		delay(1000);
	}
	while(1);
}

