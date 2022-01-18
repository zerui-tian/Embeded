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
* �������ܡ��� ������Ϊһ�����������������˲ٿ�S13��S12����������ѡ�ֹ���λ���ֱ�ٿ�S6--S11�е�һ��������
		  �������˰�������ʼ����S13�󣬵���ʱ��ʼ����ʱ5s���˺����Ȱ��°�����ѡ�ֺ��뽫��ʾ��������ϡ�
		  �������µĽ�����ʾ��
		  �������ʱ�������ٰ��°���Ҳ������ʾ��
		  ��������û�а���ʼ��������ѡ����������Ϊ���档��ʱ�����ѡ�ֺ��뽫����ʾ��������ϣ������ʾ��λ����ѡ�֣�
		  ͬʱ�����������������������������ȫ����
		  ���������˰��������S12��һ��״̬���ָ������Կ�ʼ��һ�ֵ�����
					   �������ܼ��
	 		S13����ʼ	 S12 ����
	  		S6--S11   �ֱ�Ϊ1��6��ѡ�ְ���	 ��ʱ��ʾ�ڵ���λ�������		   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

#include<reg51.h>

#define uchar unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
uchar key,j,k,temp,daojishi=5,wei;
bit begin,end,clear,fangui;
uchar a0,b0=16;c0=16;d0=16;e0=16;f0=5;
unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void delay(unsigned char i)
{
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
               key=1;
			   wei++;
               break;

          case 0xde:
               key=2;
			   wei++;
               break;

          case 0xbe:
               key=3;
			   wei++;
               break;

          case 0x7e:
               key=4;
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
               key=5;
			   wei++;
               break;

          case 0xdd:
               key=6;
			   wei++;
               break;

          case 0xbd:
               clear=1;
			   //key=7;
               break;

          case 0x7d:
		  	   begin=1;
               //key=8;
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

void main()
{
	TMOD=0x01;
	
	TH0=(65536-46080)/256;// ���ھ���Ϊ11.0592,�����Ǵ���ӦΪ46080����ʱ��ÿ��50000΢�뷢��һ���жϡ�
	TL0=(65536-46080)%256;//46080��������Ϊ50000*11.0592/12
	ET0=1;
	EA=1;
	while(1)
	{
		keyscan();
		if(begin)
		{
			if(!end)
			{
				TR0=1;
				//if(!daojishi)	daojishi=5;
				//begin=0;
				//daojishi=5;
				if(pp==20)
				{
					pp=0;
					daojishi--;
					
				}

			f0=daojishi;
			if(key!=0)
			a0=key;
		//	a1=a0;
			
			//if(a0!=0)	end=1;
			if(!daojishi|a0)
			{
				end=1;
				TR0=0;
				//daojishi=5;
			//	begin=0;
				pp=0;
			}
			}
			
	}
	else
	{	
			if(key!=0)
		{
			switch(wei)
			{
				case 1: a0=key;break;
				case 2: b0=key;break;
				case 3: c0=key;break;
				case 4: d0=key;break;
				case 5: e0=key;break;
			}
			//if(!fangui)
				fangui=1;
				beep=0;
		}
	}
	if(clear)
	{
		a0=0;key=0;begin=0;end=0;fangui=0;beep=1;daojishi=5;f0=5;clear=0;
		b0=16;c0=16;d0=16;e0=16;wei=0; P1=0xff;
	}
	display(a0,b0,c0,d0,e0,f0);
	if(fangui)	
	{
		beep=0;
		P1=0x00;;
	}

}
}

void time0() interrupt 1
{	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
