#include<reg52.h>
#define uchar unsigned char
sbit K1=P3^0;  	//	按下独立键盘S2正转
sbit K2=P3^1;	//	按下独立键盘S3反转
sbit K3=P3^2;	//	按下独立键盘S4停转
uchar code table[]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9};
uchar code table1[]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1};
void delay(uchar z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);
}
void main()
{
 	uchar k,i;
	P1=table[0];
	while(1)
	{
		if(!K1)
		delay(5);
		if(!K1)
		k=1;

		if(!K2)
		delay(5);
		if(!K2)
		k=2;

		if(!K3)
		delay(5);
		if(!K3)
		k=3;

		if(k==1||k==2)
		{
			for(i=0;i<8;i++)
			{
				if(k==1) P1=table[i];
				else P1=table1[i];
				delay(20);
			}
		}
	}
}