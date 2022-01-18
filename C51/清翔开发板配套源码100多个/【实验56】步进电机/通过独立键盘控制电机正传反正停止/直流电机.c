#include<reg52.h>
sbit s=P1^4;
void delay(unsigned char z)
{
	while(z!=0) z--;
}
void main()
{
	while(1)
	{
		s=0;
		delay(9);
		s=1;
		delay(1);
	}
}