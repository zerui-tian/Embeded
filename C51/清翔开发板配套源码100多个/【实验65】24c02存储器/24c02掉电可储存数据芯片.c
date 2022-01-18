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
* �������ܡ��� �������������ʾ�ۼ����֣��رյ�Դ���ٴδ� �ϴμ�������ֵ����
����ǵ��索�����ݹ���			   			            			    
* ��ʹ��˵������ 
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
bit  write=0;           //д24C02�ı�־��
sbit sda=P2^0;                         
sbit scl=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
sbit RST=P2^5;   //ʱ��  ����Ҫ�����Թص�
sbit CS_DA =P2^4 ;  //HJ-3G DAC HL-1����Ҫ�����Թص�
sbit JD=P0^6;
sbit FM=P2^3;


uchar sec,tcnt; 
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void ioin()  //IO�ڼ̵�������������溯��
   {	  
	  wela=1;
     P0 = 1;  
	  JD = 0;    
     wela=0;
	  dula=1;
	  P0=0;
	  dula=0;
    }
void delay()
{ ;; }
void delay1ms(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void start()  //��ʼ�ź�
{	
	sda=1;
	delay();
	scl=1;
	delay();
	sda=0;
	delay();
}
void stop()   //ֹͣ
{
	sda=0;
	delay();
	scl=1;
	delay();
	sda=1;
	delay();
}
void respons()  //Ӧ��
{
	uchar i;
	scl=1;
	delay();
	while((sda==1)&&(i<250))i++;
	scl=0;
	delay();
}
void init()
{
   RST=0; //����ʱ�ӵ͵���
   CS_DA =0 ;
	sda=1;
	delay();
	scl=1;
	delay();
}
void write_byte(uchar date)
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
	    delay();
		sda=CY;
		delay();
		scl=1;
		delay();
	}
	scl=0;
	delay();
	sda=1;
	delay();
}
uchar read_byte()
{
	uchar i,k;
	scl=0;
	delay();
	sda=1;
	delay();
	for(i=0;i<8;i++)
	{
		scl=1;
		delay();	
		k=(k<<1)|sda;
		scl=0;
		delay();	
	}
	return k;
}
void write_add(uchar address,uchar date)
{
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	write_byte(date);
	respons();
	stop();
}
uchar read_add(uchar address)
{
	uchar date;
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	start();
	write_byte(0xa1);
	respons();
	date=read_byte();
	stop();
	return date;
}
void display(uchar bai_c,uchar sh_c)	//��ʾ����
{
   dula=0;
   P0=table[bai_c];		//��ʾ��һλ
   dula=1;
   dula=0;
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay1ms(5);
   dula=0;
   P0=table[sh_c];		//��ʾ�ڶ�λ
   dula=1;
   dula=0;
   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay1ms(5);
}
void main()
{
   ioin();
	init();
	sec=read_add(2);    //������������ݸ���sec
	if(sec>100)        //��ֹ�״ζ�ȡ����������
		sec=0;
	TMOD=0x01;  			//��ʱ�������ڷ�ʽ1
	ET0=1; 
	EA=1;
	TH0=(65536-50000)/256; //��TH0 TL0��ֵ
	TL0=(65536-50000)%256; //ʹ��ʱ��0.05���ж�һ��
	TR0=1;                 //��ʼ��ʱ
	while(1)
	{
		display(sec/10,sec%10);
		if(write==1) 		      //�жϼ�ʱ���Ƿ��ʱһ��
		{
			write=0;              //����
			write_add(2,sec);  	  //��24c02�ĵ�ַ2��д������sec
		}
	} 
}
void t0() interrupt 1 //��ʱ�жϷ�����
{
	TH0=(65536-50000)/256; //��TH0 TL0��ֵ
	TL0=(65536-50000)%256; //��װ������ֵ
	tcnt++;        //ÿ��50ms tcnt��һ
	if(tcnt==20)  //����20�Σ�1�룩ʱ
	{
	    tcnt=0;   //�����ټ�
	    sec++;
	    write=1;  //1��дһ��24C02
	    if(sec==100) //��ʱ100�룬�ٴ��㿪ʼ��ʱ
	    	sec=0;
	}
}
