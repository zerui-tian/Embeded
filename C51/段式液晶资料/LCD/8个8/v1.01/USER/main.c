#include "reg51.h"
#include "intrins.h"
#include "VKL144.h"
//-------------------------------------------------------
typedef	signed char		s8;
typedef	signed int		s16;
typedef	signed long		s32;

typedef	unsigned char 	u8;
typedef	unsigned int    u16;
typedef	unsigned long	u32;
//-------------------------------------------------------
sbit		LED		=	P0^0; //±³¹â
//-------------------------------------------------------
void main()
{
	LED=1;
	VKL144_Init();
	DispJUHUI();
	VKL144_Delay_ms(5000);
	
	while(1)
	{
		VKL144_Display();
		VKL144_Delay_ms(2000);
		VKL144_Test();
	}
}