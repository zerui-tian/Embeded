#include<reg52.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define FRAME_LENGTH 16
#define GETTIME_SIZE 8


//tables
uchar code dotTable[] = {
	0x00,//extinguish
	0x40//light up
};
uchar code digitSelectTable[] = {
	0x00,//
	0x80,//
	0x09,
	0x01,
	0x88,//
	0x08//
};
uchar code numberTable[] = {
	0x00,
	0x20,
	0x02,
	0x22,
	0x04,
	0x24,
	0x06,
	0x26,
	0x10,
	0x30
};


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

//global variables
volatile uchar time[8];
uchar buffer[FRAME_LENGTH];
int pBuffer;
bit isReceived_Frame;
uint interruptCounter;
uchar nixieScanner;
uchar state;
uchar lastSecond;
void delay(uint xms){
	uint x,y;
	for(x=xms;x>0;x--)
		for(y=110;y>0;y--);
}

uchar Bin2BCD(uchar bin) {
	uchar bcd;
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

void DS12C887_SetTime(uchar* aim){
	uchar regB;
	
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

uchar Nixie_Coder(uchar digit){
	uchar nixieCode;
	switch(state){
		case 0 :{
			switch(digit){
				case 0 :
					nixieCode = digitSelectTable[0] | numberTable[time[0]&0x0F] | dotTable[0];
					break;
				case 1 :
					nixieCode = digitSelectTable[1] | numberTable[ (time[0]>>4) &0x0F] | dotTable[0];
					break;
				case 2 :
					nixieCode = digitSelectTable[2] | numberTable[time[1]&0x0F] | dotTable[interruptCounter<512];
					break;
				case 3 :
					nixieCode = digitSelectTable[3] | numberTable[ (time[1]>>4) &0x0F] | dotTable[0];
					break;
				case 4 :
					nixieCode = digitSelectTable[4] | numberTable[time[2]&0x0F] | dotTable[interruptCounter<512];
					break;
				case 5 :
					nixieCode = digitSelectTable[5] | numberTable[ (time[2]>>4) &0x0F] | dotTable[0];
					break;
				default:
					break;
			}
			break;
		}
		case 1 :{
			switch(digit){
				case 0 :
					nixieCode = digitSelectTable[0] | numberTable[time[4]&0x0F] | dotTable[0];
					break;
				case 1 :
					nixieCode = digitSelectTable[1] | numberTable[ (time[4]>>4) &0x0F] | dotTable[0];
					break;
				case 2 :
					nixieCode = 0xFF;
					break;
				case 3 :
					nixieCode = digitSelectTable[3] | numberTable[time[5]&0x0F] | dotTable[0];
					break;
				case 4 :
					nixieCode = digitSelectTable[4] | numberTable[ (time[5]>>4) &0x0F] | dotTable[0];
					break;
				case 5 :
					nixieCode = 0xFF;
					break;
				default:
					break;
			}
			break;
		}
		case 2 :{
			switch(digit){
				case 0 :
					nixieCode = 0xFF;
					break;
				case 1 :
					nixieCode = digitSelectTable[1] | numberTable[time[6]&0x0F] | dotTable[0];
					break;
				case 2 :
					nixieCode = digitSelectTable[2] | numberTable[ (time[6]>>4) &0x0F] | dotTable[0];
					break;
				case 3 :
					nixieCode = digitSelectTable[3] | numberTable[time[7]&0x0F] | dotTable[0];
					break;
				case 4 :
					nixieCode = digitSelectTable[4] | numberTable[ (time[7]>>4) &0x0F] | dotTable[0];
					break;
				case 5 :
					nixieCode = 0xFF;
					break;
				default:
					break;
			}
			break;
		}
		default:{
			nixieCode = 0xFF;
			break;
		}
	}
	return nixieCode;
}

void initializer(){
	DS12C887_Init();
	pBuffer = 0;
	state = 0;
	interruptCounter = 0;
	isReceived_Frame = 0;
	
	SCON=0x50;           //设定串口工作方式
	PCON=0x00;           //波特率不倍增

	TMOD=0x20;           //定时器1工作于8位自动重载模式, 用于产生波特率
	TCON|=0x05;

	ES = 1;              //允许串口中断
	TL1=0xfd;
	TH1=0xfd;             //波特率9600
  TR1=1;

	EX0 = 1;
	EX1 = 1;
	
	IP = 0x11;
	
	EA = 1;
}

void main(){
	initializer();
	while(1){
	}
}

void Button_Action() interrupt 0{
	delay(10);
	if(P3^2==0){
		EA = 0;
		P1 = 0xFF;
		state = (state+1)%4;
		EA = 1;
	}
}

void Time_Updater() interrupt 2{
	DS12C887_GetTime();
	nixieScanner = (nixieScanner+1)%6;
	interruptCounter = (time[0]==lastSecond ? interruptCounter+1 : 0);
	P1 = Nixie_Coder(nixieScanner);
	P2 = 0x01<<(time[3]-1);
	lastSecond = time[0];
}

void  Byte_Receiver() interrupt 4{
	ES = 0;                //关闭串行中断
	RI = 0;                //清除串行接受标志位
	buffer[pBuffer++] = SBUF;            //从串口缓冲区取得数据
	if(pBuffer >= FRAME_LENGTH){
		Command_Analyzer();
	}
	else{
	}
	ES = 1;    //允许串口中断
}