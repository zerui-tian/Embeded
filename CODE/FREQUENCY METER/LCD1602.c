#include<LCD1602.h>
#include<reg51.h>
#include<intrins.h>

#define DATA P1

sbit LCD1602_RS = P3^7;
sbit LCD1602_RW = P3^6;
sbit LCD1602_EN = P3^5;

void delay_ms(unsigned int z){
	unsigned int i,j;
	for(i=z;i>0;i--)
	for(j=110;j>0;j--);
}

unsigned char LCD1602_ReadBusy(){
	unsigned char temp;
	LCD1602_RS=0;
	LCD1602_RW=1;
	_nop_();
	DATA=0xff;

	_nop_();
	LCD1602_EN=1;
	_nop_();
	temp=DATA;
	_nop_();
	LCD1602_EN=0;
	return (temp&0x80);
}

void LCD1602_WriteCom(unsigned char com){
	while(LCD1602_ReadBusy());
	LCD1602_RS=0;
	LCD1602_RW=0;
	_nop_();
	DATA=com;
	_nop_();
	LCD1602_EN=1;
	_nop_();
	_nop_();
	LCD1602_EN=0;
	_nop_();
	_nop_();   
}

void LCD1602_WriteData(unsigned char dat){
	while(LCD1602_ReadBusy());
	LCD1602_RS=1;
	LCD1602_RW=0;
	_nop_();
	DATA=dat;
	_nop_();
	LCD1602_EN=1;
	_nop_();
	_nop_();
	LCD1602_EN=0;
	_nop_();
	_nop_();   
}

void LCD1602_Init(){
	delay_ms(15);
	LCD1602_WriteCom(0x38);
	delay_ms(5);
	LCD1602_WriteCom(0x38);
	delay_ms(5);
	LCD1602_WriteCom(0x38);
	LCD1602_WriteCom(0x0c);
	LCD1602_WriteCom(0x01);
	LCD1602_WriteCom(0x06);
}

void LCD1602_Refresh(unsigned char stringOnLine1[], unsigned char stringOnLine2[]){
	unsigned char cnt;
	
	cnt = 0;
	LCD1602_WriteCom(0x80);
	while(stringOnLine1[cnt] != '\0' && cnt < 16) {
		LCD1602_WriteData(stringOnLine1[cnt]);
		cnt++;
	}
	while(cnt < 16){
		LCD1602_WriteData(' ');
		cnt++;
	}
	
	cnt = 0;
	LCD1602_WriteCom(0x80+0x40);
	while(stringOnLine1[cnt] != '\0' && cnt < 16) {
		LCD1602_WriteData(stringOnLine2[cnt]);
		cnt++;
	}
	while(cnt < 16){
		LCD1602_WriteData(' ');
		cnt++;
	}
}