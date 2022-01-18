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
* �������ܡ��� 			   			            			    
* ��ʹ��˵������ �˳����֮ǰ��ʱ����������ʱ�䲻���Ե���ֻ������������
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
	// 		S12 ������ʱ	 S13 ���ö�ʱ
	 // S14 ȡ����ʱ		S1--S10   �ֱ�Ϊ���ּ�0--9
/********************************************************************
������Ϊ�������ʾ�ɶ�ʱ��ʱ�����õ�QX-MCS51�������о�����̵�������(������S16)�������ǰ����������

��Ҫ���ö�ʱʱ�䣬���Ȱ���S18�������ŵ�һ������
Ȼ�����ּ�S1--S10���ö�ʱʱ�䣨����˳�����ɸ�λ����λ��˳�򣩣�
�����궨ʱ�󣬰���S17�ɻָ���������ʱ״̬
��ʱʱ�䵽���������������һ���ӱ�ʱ��
��Ҫȡ����ʱ�����߷�������ʱ�����رգ����԰���S18���ܼ���
ע�⾧����Ϊ11.0592M��
��Ϊ������ֵ���������ı�TH0��TL0����ֵ�������ʱ���кܴ���� ��
***********************************************************************************************************************/
#include<reg51.h>

#define uchar unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
unsigned char j,k,a1,a0,b1,b0,c1,c0,s,f,m,key=10,temp,qq;
uchar shi20,shi10,fen20,fen10,miao20,miao10,ok=1,wei,dingshi,yidingshi;
uchar baoshi,baoshijieshu;
unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
void display(uchar shi2,uchar shi1,uchar fen2,uchar fen1,uchar miao2,uchar miao1)
{
   dula=0;
   P0=table[shi2];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[shi1]|0x80;
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[fen2];
   dula=1;
   dula=0;
   
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[fen1]|0x80;
   dula=1;
   dula=0;
   
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[miao2];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[miao1];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);
}

void keyscan0()
{
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
          case 0x7b:
               ok=1;
			   dingshi=0;
               break;
         }	

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
               ok=0;
			   dingshi=1;
               break;
		  case 0xd7:
               yidingshi=0;	 //ȡ����ʱ
               break;
         } 

		 }
		 }	
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

void main()
{
	TMOD=0x01;

	TH0=(65536-46080)/256;// ���ھ���Ϊ11.0592,�����Ǵ���ӦΪ46080����ʱ��ÿ��50000΢�뷢��һ���жϡ�
	TL0=(65536-46080)%256;//46080��������Ϊ50000*11.0592/12
	ET0=1;
	EA=1;
	
	while(1)
	{	keyscan0();
		
		if(ok==1)
		{	TR0=1;
			wei=0;
				display(a1,a0,b1,b0,c1,c0);
		if(pp==20)
		{	pp=0;
			m++;
			if(m==60)
			{
				m=0;
				f++;
				if(f==60)
				{
					f=0;
					s++;
					if(s==24)  //Ϊ24hһ��ѭ������Ҫ12h��ֻ���ڴ˸�Ϊ12���ɡ�
					{
						s=0;
					}
				}
			}
		}
		
		a0=s%10;
		a1=s/10;
		b0=f%10;
		b1=f/10;
		c0=m%10;
		c1=m/10;
		display(a1,a0,b1,b0,c1,c0);
	}
	else
	{
	if(dingshi==1)
	{
		TR0=0;
		display(shi20,shi10,fen20,fen10,miao20,miao10);	
		keyscan();
		if(key!=10)
		{
		switch(wei)
		{
			case 1: if(key<3)		//Сʱ���λΪ2
					shi20=key;
					else
					wei--;
					break;
			case 2: if(a1==1|a1==0)
					shi10=key;
					else
					if(key<5)
					a0=key;		   //��Сʱ���λΪ2ʱ����λ���Ϊ4
					break;
			case 3: if(key<6)		//�������λΪ5
					fen20=key;
					else
					wei--;
					break;
			case 4: fen10=key; break;
			case 5: if(key<6)		//�����λΪ5
					miao20=key; 
					else
					wei--;
					break;
			case 6: miao10=key; break;
		}
		key=10;
		}
		yidingshi=1;
		display(shi20,shi10,fen20,fen10,miao20,miao10);	
	}	
	}
	if(yidingshi==1)
	{
	if((a1==shi20)&&(a0==shi10)&&(b1==fen20)&&(b0==fen10)&&(c1==miao20)&&(c0==miao10))
		beep=0;
	}
	if(((a1==shi20)&&(a0==shi10)&&(b1==fen20)&&(b0==fen10+1)&&(c1==miao20)&&(c0==miao10))||(yidingshi==0))//һ���ӱ�ʱ��ʾ
		beep=1;
	}
}

void time0() interrupt 1
{	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
