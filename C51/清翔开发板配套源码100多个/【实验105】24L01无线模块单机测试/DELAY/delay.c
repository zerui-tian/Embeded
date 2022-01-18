#include "reg52.h"/*����STC��Ƭ��ͷ�ļ�*/
#include "QX51/qx51.h"/*����QXP51������ͷ�ļ�*/
#include "delay.h"/*������������ṩ��ʱ����ͷ�ļ�*/
#include "intrins.h"/*��������_nop_()������ͷ�ļ�*/


//12T��Ƭ����������ʱ����

/*��Լ4uS��ʱ����
FOSC=11.0592��12��22.1184MHZʱ������
��������Ƶ��ʱ��Ҫ����_nop_();*/
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

/*��Լ1ms��ʱ����*/
/*FOSC=11.0592��12��22.1184MHZʱ������*/
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