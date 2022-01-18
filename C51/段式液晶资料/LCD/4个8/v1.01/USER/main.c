#include "reg51.h"
#include "intrins.h"
#include "ht1621.h"
//-------------------------------------------------------
typedef	signed char		s8;
typedef	signed int		s16;
typedef	signed long		s32;

typedef	unsigned char 	u8;
typedef	unsigned int    u16;
typedef	unsigned long	u32;
//-------------------------------------------------------
sbit		VCC		=	P0^3;
sbit		GND		=	P0^4;
//-------------------------------------------------------
void main()
{
	VCC=1;	//Ϊ�˷�����ԣ����������ģ���Դ�͵أ�ʵ��ʹ��ʱ�����Խ���ĵ�Դ
	GND=0;
	
	HT1621_Init();
	
	while(1)
	{
		HT1621_Display();
		HT1621_Delay_ms(2000);
		HT1621_Test();
	}
}