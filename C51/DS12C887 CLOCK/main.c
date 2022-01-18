#include <reg52.h>
#include <intrins.h>
#include "LCD1602.h"

#define FRAME_LENGTH 16

//variables in ds12c887
xdata char chSecondsChannel 	_at_ 0x8000;
xdata char chMinutesChannel 	_at_ 0x8002;
xdata char chHoursChannel 		_at_ 0x8004;
xdata char chDofWChannel 			_at_ 0x8006;
xdata char chDateChannel 			_at_ 0x8007;
xdata char chMonthChannel		 	_at_ 0x8008;
xdata char chYearChannel 			_at_ 0x8009;
xdata char chRegA 						_at_ 0x800A;
xdata char chRegB							_at_ 0x800B;
xdata char chRegC 						_at_ 0x800C;
xdata char chRegD 						_at_ 0x800D;
xdata char chCenturyChannel 	_at_ 0x8032;

unsigned char time[8];
unsigned char buffer[FRAME_LENGTH];
int pBuffer =						0;
char interruptCounter =	0;
bit isReceived_Frame =	0;
bit buzzerFlag =				0;
bit dotFlag =						0;
char digit =						0;
char number[4] = 				{0,0,0,0};
char lastSecond = 			0xFF;

unsigned char Bin2BCD(unsigned char bin) {
	unsigned char bcd;
	bcd = bin % 10;
	bcd += ((bin / 10) << 4);
	return bcd;
}

void DS12C887_Init(){
	chRegA = 0x26; //UIP DV2 DV1 DV0 RS3 RS2 RS1 RS0
	chRegB = 0x0A; // SET PIE AIE UIE SQWE DM 24/12 DSE
}
void DS12C887_GetTime(){
	time[0] = chSecondsChannel;
	time[1] = chMinutesChannel;
	time[2] = chHoursChannel;
	time[3] = chDofWChannel;
	time[4] = chDateChannel;
	time[5] = chMonthChannel;
	time[6] = chYearChannel;
	time[7] = chCenturyChannel;
}

void DS12C887_SetTime(char* aim){
	unsigned char regB;
	
	regB = chRegB;
	chRegB = regB|0x80;
	
	chSecondsChannel = Bin2BCD( aim[0] );
	chMinutesChannel = Bin2BCD( aim[1] );
	chHoursChannel = Bin2BCD( aim[2] );
	chDofWChannel = ((aim[6] + (aim[6]>>2) + (aim[7]>>2) - 2*aim[7] + (26*(aim[5]+1)/10) + aim[4] - 1) % 7) + 1;
	chDateChannel = Bin2BCD( aim[4] );
	chMonthChannel = Bin2BCD( aim[5] );
	chYearChannel = Bin2BCD( aim[6] );
	chCenturyChannel = Bin2BCD( aim[7] );
	
	chRegB = regB;
}

void Command_Analyzer(){
	switch(buffer[0]){
		case 0x01:
			DS12C887_SetTime(buffer+1);
			break;
		default:
			break;
	}
	isReceived_Frame = 0;
	pBuffer = 0;
}

void initializer(){
	DS12C887_Init();
	LCD1602_Init();
	
	SCON=0x50;
	PCON=0x00;

	TMOD=0x20;
	TCON|=0x05;

	
	TL1=0xFD;
	TH1=0xFD;
  TR1=1;

	IP = 0x00;
	
	EX0 = 1;
	ES = 0;
	
	EA = 1;
}

void main(){
	initializer();
	while(1){
	}
}

void EX0_IRQ() interrupt 0{	
	
	digit=(digit+1)&0x03;
	
	if(time[0]==lastSecond){
		interruptCounter++;
	}
	else{
		DS12C887_GetTime();
		interruptCounter=0;
	}
	
	P2 = (buzzerFlag?0x80:0x00)|
			 (dotFlag		?0x40:0x00)|
			 (digit << 4)|
			 (number[digit] & 0x0F);
	
	lastSecond = time[0];
}

void  Byte_Receiver() interrupt 4{
	ES = 0;
	RI = 0;
	buffer[pBuffer++] = SBUF;
	if(pBuffer >= FRAME_LENGTH){
		Command_Analyzer();
	}
	ES = 1;
}