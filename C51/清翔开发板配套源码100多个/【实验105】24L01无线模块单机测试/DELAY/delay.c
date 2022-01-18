#include "reg52.h"/*包含STC单片机头文件*/
#include "QX51/qx51.h"/*包含QXP51开发板头文件*/
#include "delay.h"/*包含清翔电子提供延时函数头文件*/
#include "intrins.h"/*包含含有_nop_()函数的头文件*/


//12T单片机下适用延时函数

/*大约4uS延时函数
FOSC=11.0592、12、22.1184MHZ时，适用
其他晶振频率时，要调整_nop_();*/
void delay4us()
{
	#if FOSC==11059200
		_nop_();
	#elif FOSC==12000000
		_nop_();
	#elif FOSC==22118400
		_nop_() ;_nop_(); _nop_();
	#endif
}

void delay8us()
{
	#if FOSC==11059200
		_nop_();  _nop_(); _nop_() ;_nop_(); _nop_();
	#elif FOSC==12000000
		_nop_(); _nop_(); _nop_() ;_nop_(); _nop_();
	#elif FOSC==22118400
		_nop_() ; _nop_(); _nop_(); _nop_() ; _nop_(); _nop_();
		_nop_() ; _nop_(); _nop_(); _nop_() ; _nop_(); _nop_();
		_nop_() ; _nop_(); _nop_();
	#endif
}

/*大约1ms延时函数*/
/*FOSC=11.0592、12、22.1184MHZ时，适用*/
void delay_ms(uint z)
{
	uint x,y;
	#if FOSC==11059200
		for(x=z;x>0;x--)
			for(y=112;y>0;y--);
	#elif FOSC==12000000
		for(x=z;x>0;x--)
			for(y=122;y>0;y--);	
	#elif FOSC==22118400
		for(x=z;x>0;x--)
			for(y=228;y>0;y--);		
	#endif
}