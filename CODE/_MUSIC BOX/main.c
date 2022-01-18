#include<reg51.h>
#include<intrins.h>
#include<main.h>
#include<BUZZER.h>

#define NOTE(pitch, note) pitch*12+note
#define BPM(bmp) (int)(60*20/2/bmp)
#define musical_len 42

sbit BEEP = P1^7;

uchar th0_f;
uchar tl0_f;
uchar ptr;
uchar timer0_cnt;
uchar duration_cnt;
uchar beat_speed;


const uchar code freq[36*2]={
	0xA9,0xEF,	0x93,0xF0,	0x73,0xF1,	0x49,0xF2,
  0x07,0xF3,	0xC8,0xF3,	0x73,0xF4,	0x1E,0xF5,
  0xB6,0xF5,	0x4C,0xF6,	0xD7,0xF6,	0x5A,0xF7,
	
  0xD8,0xF7, 	0x4D,0xF8,	0xBD,0xF8,	0x24,0xF9,
  0x87,0xF9,	0xE4,0xF9,	0x3D,0xFA,	0x90,0xFA,
  0xDE,0xFA,	0x29,0xFB,	0x6F,0xFB,	0xB1,0xFB,
	
	0xEF,0xFB,	0x2A,0xFC,	0x62,0xFC,	0x95,0xFC,
  0xC7,0xFC,	0xF6,0xFC,	0x22,0xFD,	0x4B,0xFD,
  0x73,0xFD,	0x98,0xFD,	0xBB,0xFD,	0xDC,0xFD,
};

uchar notes[musical_len]={
	NOTE(ALTO,DO), NOTE(ALTO,DO), NOTE(ALTO,SO), NOTE(ALTO,SO), NOTE(ALTO,LA), NOTE(ALTO,LA), NOTE(ALTO,SO),
	NOTE(ALTO,FA), NOTE(ALTO,FA), NOTE(ALTO,MI), NOTE(ALTO,MI),	NOTE(ALTO,RE), NOTE(ALTO,RE), NOTE(ALTO,DO),
	NOTE(ALTO,SO), NOTE(ALTO,SO), NOTE(ALTO,FA), NOTE(ALTO,FA), NOTE(ALTO,MI), NOTE(ALTO,MI), NOTE(ALTO,RE),
	NOTE(ALTO,SO), NOTE(ALTO,SO), NOTE(ALTO,FA), NOTE(ALTO,FA), NOTE(ALTO,MI), NOTE(ALTO,MI), NOTE(ALTO,RE),
	NOTE(ALTO,DO), NOTE(ALTO,DO), NOTE(ALTO,SO), NOTE(ALTO,SO), NOTE(ALTO,LA), NOTE(ALTO,LA), NOTE(ALTO,SO),
	NOTE(ALTO,FA), NOTE(ALTO,FA), NOTE(ALTO,MI), NOTE(ALTO,MI),	NOTE(ALTO,RE), NOTE(ALTO,RE), NOTE(ALTO,DO),
};

uchar duration[musical_len] = {
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2, 2, 2, 2,
	2, 2
};

void initializer(){
	TMOD = 0x11;
	ET0 = 1;
	ET1 = 1;
	TL0=0xAF;
	TH0=0x3C;
	timer0_cnt = 0;
	ptr = 0;
	duration_cnt = duration[ptr];
	beat_speed = BPM(72);
	EA = 1;
	TR0 = 1;
	TR1 = 1;
	
}

void main(void)
{
	initializer();
	while(1){
		tl0_f=freq[notes[ptr]*2];
		th0_f=freq[notes[ptr]*2+1];
		if(duration_cnt == 0) {
			ptr = (ptr+1)%musical_len;
			duration_cnt = duration[ptr];
		}
	}
}

void timer0() interrupt 1 {
	timer0_cnt++;
	if(timer0_cnt >= beat_speed) {
			duration_cnt--;
			timer0_cnt = 0;
	}
	TL0=0xAF;
	TH0=0x3C;
}

void timer1() interrupt 3 {
	TL1=tl0_f;
	TH1=th0_f;
	BEEP=~BEEP;
}