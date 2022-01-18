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
* �������ܡ��� �˳���Ӧ��AT24C02оƬ���Ե��索������			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
/********************************************************************************
								���ܼ�
						S6---S15	���ּ�0-9
				S16---��������		S17---����������Ϻ�ȷ��
				S18---�������롢�����趨	S19---�ر�������
				��ʼ���룺000000 	����λ����6λ
				ע�⣺�������������ᶪʧ�������ϵ�ʱ������ָ�Ϊԭʼ��000000
				��P1������8λ����LED�������������򿪣�Ϩ�������������

�����ܣ�
		  1��������
		  ���س����ֱ�Ӱ�����S6������������0����8λLED���������򿪣���������ʱ��
		  ��λ�����������ʾС��ܡ�
		  2���������룺
		  ֻ�е�������LED�����󣬸ù��ܷ���ʹ�á�
		  ���Ȱ��¸��������S16��Ȼ��������Ӧ���룬��ʱ��λ����ܻ���ʾ���������Ӧ
		  �����֡������������λ�󣬰���S17ȷ��������ģ��˺������뼴��Ч��
		  3���������룺
		  ����������ʱ�����������¼�S18��������������λ���롣
		  ����������ʱ��������;��������룬Ҳ�ɰ��´˼��������á�
		  4���ر���������
		  ����S19���ɽ��򿪵��������رա�
�Ƽ�������ʾ���裺����ԭʼ����000000��6��S6������ȷ��LEDȫ������ʾ���Ѵ�---���¸�
�����밴��S16---��S6��S15��������---��S17
			ȷ���������---��S19�ر�������---�����µ������������
*******************************************************************************/
#include<reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar old1,old2,old3,old4,old5,old6; //ԭʼ����000000
uchar new1,new2,new3,new4,new5,new6;  //ÿ��MCU�ɼ�������������
uchar a=16,b=16,c=16,d=16,e=16,f=16; //�����������ʾ�ı���
uchar wei,key,temp;

bit allow,genggai,ok,wanbi,retry,close;	 //����״̬λ

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
sbit sda=P2^0;                          //IO�ڶ���
sbit scl=P2^1;

unsigned char code table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x40};

/*****************IICоƬ24C02�洢����������************************************/

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
/****************************����������ģ��********************************************************/

void delay(unsigned char i)
{
	uchar j,k;
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f)
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
}


void keyscan()
{
  {	
    P3=0xfe;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {	
        temp=P3;
        switch(temp)
        {
          case 0xee:
               key=0;
			   wei++;
               break;

          case 0xde:
               key=1;
			   wei++;
               break;

          case 0xbe:
               key=2;
			   wei++;
               break;

          case 0x7e:
               key=3;
			   wei++;
               break;
         }
         while(temp!=0xf0) 
        {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
    }
    P3=0xfd;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xed:
               key=4;
			   wei++;
               break;

          case 0xdd:
               key=5;
			   wei++;
               break;

          case 0xbd:
               key=6;
			   wei++;
               break;

          case 0x7d:
               key=7;
			   wei++;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
    P3=0xfb;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xeb:
               key=8;
			   wei++;
               break;

          case 0xdb:
               key=9;
			   wei++;
               break;
			   
          case 0xbb:
               genggai=1;
			   wei=0;
               break;

          case 0x7b:
		  	   if(allow)
               ok=1;
               break;
         }
        while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
	  P3=0xf7;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xe7:
		  	   retry=1;
               break;

          case 0xd7:
		  	   close=1;
               break;
         }
        while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
}
}

void shumima()		//�԰����ɼ��������ݽ��з���
{
	if(!wanbi)
	{
	switch(wei)
	{
		case 1:new1=key; 
			    if(!allow)	a=17;
			   else a=key;	break;
		case 2:new2=key;
				if(a==17) b=17;
				else b=key;	break;
		case 3:new3=key; 
				if(a==17) c=17;
				else c=key;	break;
		case 4:new4=key;
				if(a==17) d=17;
				else d=key;	break;
		case 5:new5=key; 
				if(a==17) e=17;
				else e=key;	break;
		case 6:new6=key; 
				if(a==17) f=17;
				else f=key;
				wanbi=1;	break;
	}
	}
}

void yanzheng()	  //��֤�����Ƿ���ȷ
{
	if(wanbi)	 //ֻ�е���λ�����������Ϻ󷽽�����֤
	{
	if((new1==old1)&(new2==old2)&(new3==old3)&(new4==old4)&(new5==old5)&(new6==old6))
		allow=1;	//�������������ȷ����õ�allowe��һ
	}
}

void main()
{
	
	init();        //��ʼ��24C02
/*********�����һС�γ���Ĺ���Ϊ��ʽ������洢����************
******��24c02����Щ�洢������������������ж�����***************
*******�������ݷ����˱仯��������������ʱ��	********************
******����ɾ����ǰ���ע���ߣ�Ȼ�����±������ء�****************
******�������뻹ԭΪ000000���뽫����ĳ�����******************
******ע�����ε������±��롢���أ���������ʹ��****************/
//	write24c02(110,0x01);
//	write24c02(111,0x02);//24c02�ĵ�110��115��ַ��Ԫ��Ϊ����洢��
//	write24c02(112,0x03);
//	write24c02(113,0x04);
//	write24c02(114,0x05);
//	write24c02(115,0x06);
/*******************************************************************/


	old1=read24c02(110);  
	old2=read24c02(111);
	old3=read24c02(112);
	old4=read24c02(113);
	old5=read24c02(114);
	old6=read24c02(115);

	while(1)
	{
		keyscan();
		shumima();
		yanzheng();
		if(allow)	 //��֤�����allowΪ1������
		{
			P1=0x00;
			if(!genggai)
				wanbi=0;
		}
		if(genggai)	  //��S16��������������£�genggai�ᱻ��һ
		{
			if(allow)	 //���Ѿ������򿪣����и��������Ȩ��
			{
				while(!wanbi)	//���µ���λ����û���趨�꣬��һֱ������ѭ��
				{
				 	keyscan();
					shumima();
					if(retry|close)	 //����̽�⵽���Լ�S18���߹ر���������S19������ʱ��������
					{	wanbi=1;
						break;
					}
					display(a,b,c,d,e,f);
				}
			}
		}
		if(ok)	  //��������ʱ����������λ�������������ʱ�����԰��´˼��������������
		{		  //����ʱ�䰴�´˼���Ч
			ok=0; wei=0;
			genggai=0;
			old1=new1;old2=new2;old3=new3; //��ʱ���ɵ����뽫������
			old4=new4;old5=new5;old6=new6;
			//������д��洢����
			write24c02(110,old1);
			write24c02(111,old2);
			write24c02(112,old3);
			write24c02(113,old4);
			write24c02(114,old5);
			write24c02(115,old6);
			a=16;b=16;c=16;d=16;e=16;f=16;
		}
		if(retry)	//�����԰���S18�����£�retry�ᱻ��λ
		{
		retry=0; wei=0;wanbi=0;
		a=16;b=16;c=16;d=16;e=16;f=16;
		new1=0;new2=0;new3=0;new4=0;new5=0;new6=0;		
		}
		if(close)  //���ر����������������£�close�ᱻ��λ
		{
			close=0;genggai=0;//���б����������㡣
			wei=0;	wanbi=0;
			allow=0;
			P1=0xff;
			a=16;b=16;c=16;d=16;e=16;f=16;
			new1=0;new2=0;new3=0;new4=0;new5=0;new6=0;
		}
		display(a,b,c,d,e,f); //ʵʱ��ʾ
	}
}