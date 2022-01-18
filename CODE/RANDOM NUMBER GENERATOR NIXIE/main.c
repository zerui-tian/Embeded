#include"reg52.h"
unsigned int interruptCounter;
unsigned char rand;

unsigned char vram[4];

void refresh(){
	unsigned char rand_temp = rand;
	int cnt;
	for(cnt = 0; cnt < 4; cnt++){
		vram[cnt] = rand_temp%10;
		rand_temp /= 10;
	}
}

unsigned char code dotTable[] = {
	0x40,//extinguish
	0x00//light up
};
unsigned char code digitSelectTable[] = {
	0x10,
	0x20,
	0x00,
	0x30
};
unsigned char code numberTable[] = {
	0x00,
	0x01,
	0x08,
	0x09,
	0x04,
	0x05,
	0x0C,
	0x0D,
	0x02,
	0x03
};

void SystemInit(void){
	TMOD=0x21;
	TH0=(65536-2587)/256; 
	TL0=(65536-2587)%256;
	IT0=1;
	IT1=0;
	TH1=0x00;
	TL1=0x00;
	ET0=1;
	TR0=1;
	TR1=1;
	EX0=1;
	EA=1;
	interruptCounter = 0;
}

int main(void){
	SystemInit();
	while(1){
	};
}

void Random() interrupt 0{
	rand = TL1;
	refresh();
}

void Timer0_IRQ(void) interrupt 1{
	TH0=(655365-2587)/256;
	TL0=(655365-2587)%256;
	interruptCounter = (interruptCounter+1)%4;
	switch(interruptCounter){
		case 0 :
			P1 = digitSelectTable[0] | numberTable[vram[3]] | dotTable[0];
			break;
		case 1 :
			P1 = digitSelectTable[1] | numberTable[vram[2]] | dotTable[0];
			break;
		case 2 :
			P1 = digitSelectTable[2] | numberTable[vram[1]] | dotTable[0];
			break;
		case 3 :
			P1 = digitSelectTable[3] | numberTable[vram[0]] | dotTable[0];
			break;
		default:
			break;
	}
}