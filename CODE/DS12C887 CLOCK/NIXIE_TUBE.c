#include<NIXIE_TUBE.h>
#include<reg52.h>
#include<intrins.h>

uchar Nixie_Action(bit buzzerFlag, bit dotFlag, uchar digit, uchar number){
	
	uchar ret_val = 0x00;
	ret_val = buzzerFlag!=0 ? ret_val|0x80 : ret_val;
	ret_val = dotFlag!=0 ? ret_val : ret_val|0x40;
	ret_val = ret_val|(digit<<4);
	ret_val = ret_val|number;
	
	return ret_val;
}